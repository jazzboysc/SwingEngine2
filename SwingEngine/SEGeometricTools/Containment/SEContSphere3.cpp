// Swing Engine Version 2 Source Code 
// Most of techniques in the engine are mainly based on David Eberly's
// Wild Magic 4 and 5 open-source game engine. The author of Swing Engine 
// learned a lot from Eberly's experience of architecture and algorithm.
// Several subsystems are totally new, and others are reorganized or
// reimplimented based on Wild Magic's subsystems.
// Copyright (c) 2007-2011
//
// David Eberly's permission:
// Geometric Tools, LLC
// Copyright (c) 1998-2010
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt

#include "SEGeometricToolsPCH.h"
#include "SEContSphere3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SESphere3f Swing::ContSphereOfAABBf(int iCount, const SEVector3f* aPoint)
{
    SEVector3f vec3fMinPoint, vec3fMaxPoint;
    SEVector3f::ComputeExtremes(iCount, aPoint, vec3fMinPoint, vec3fMaxPoint);

    SESphere3f tempSphere;
    tempSphere.Center = 0.5f * (vec3fMaxPoint + vec3fMinPoint);
    SEVector3f vec3fHalfDiagonal = 0.5f * (vec3fMaxPoint - vec3fMinPoint);
    tempSphere.Radius = vec3fHalfDiagonal.GetLength();
    
    return tempSphere;
}
//----------------------------------------------------------------------------
SESphere3f Swing::ContSphereAveragef(int iCount, const SEVector3f* aPoint)
{
    SESphere3f tempSphere;

    tempSphere.Center = aPoint[0];
	tempSphere.Radius = 0.0f;

    int i;
    for( i = 1; i < iCount; i++ )
    {
        tempSphere.Center += aPoint[i];
    }
    tempSphere.Center /= (float)iCount;

    for( i = 0; i < iCount; i++ )
    {
        SEVector3f vec3fDiff = aPoint[i] - tempSphere.Center;
        float fRadiusSqr = vec3fDiff.GetSquaredLength();

        if( fRadiusSqr > tempSphere.Radius )
        {
            tempSphere.Radius = fRadiusSqr;
        }
    }

    tempSphere.Radius = SEMath<float>::Sqrt(tempSphere.Radius);

    return tempSphere;
}
//----------------------------------------------------------------------------
bool Swing::IsInSpheref(const SEVector3f& rPoint, const SESphere3f& rSphere)
{
    SEVector3f vec3fDiff = rPoint - rSphere.Center;

    return vec3fDiff.GetSquaredLength() <= rSphere.Radius * rSphere.Radius;
}
//----------------------------------------------------------------------------
SESphere3f Swing::MergeSpheresf(const SESphere3f& rSphere0, 
    const SESphere3f& rSphere1)
{
    SEVector3f vec3fCenterDiff = rSphere1.Center - rSphere0.Center;
    float fLSqr = vec3fCenterDiff.GetSquaredLength();
    float fRadiusDiff = rSphere1.Radius - rSphere0.Radius;
    float fRadiusDiffSqr = fRadiusDiff * fRadiusDiff;

    if( fRadiusDiffSqr >= fLSqr )
    {
        return ( fRadiusDiff >= 0.0f ? rSphere1 : rSphere0 );
    }

    float fLength = SEMath<float>::Sqrt(fLSqr);
    SESphere3f tempSphere;

    if( fLength > SEMath<float>::ZERO_TOLERANCE )
    {
        float fCoeff = (fLength + fRadiusDiff) / (2.0f * fLength);
        tempSphere.Center = rSphere0.Center + fCoeff*vec3fCenterDiff;
    }
    else
    {
        tempSphere.Center = rSphere0.Center;
    }

    tempSphere.Radius = 0.5f * (fLength + rSphere0.Radius + rSphere1.Radius);

    return tempSphere;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// SEMinSphere3f
//
// 所有最小球的内部计算函数都在SESphere3f对象中暂存平方半径,只有最后输出时才
// 开方.
//----------------------------------------------------------------------------
SEMinSphere3f::SEMinSphere3f(int iCount, const SEVector3f* aPoint, 
    SESphere3f& rMinimal, float fEpsilon)
{
    m_fEpsilon = fEpsilon;
    m_aoUpdate[0] = 0;
    m_aoUpdate[1] = &SEMinSphere3f::UpdateSupport1;
    m_aoUpdate[2] = &SEMinSphere3f::UpdateSupport2;
    m_aoUpdate[3] = &SEMinSphere3f::UpdateSupport3;
    m_aoUpdate[4] = &SEMinSphere3f::UpdateSupport4;

    SESupport tempSupp;
    float fDistDiff;

    if( iCount >= 1 )
    {
        // 创建等同于输入顶点的新顶点集指针数组.
        SEVector3f** apPermute = new SEVector3f*[iCount];
        int i;
        for( i = 0; i < iCount; i++ )
        {
            apPermute[i] = (SEVector3f*)&aPoint[i];
        }

        // 随机重新排列顶点顺序.
        for( i = iCount - 1; i > 0; i-- )
        {
            int j = rand() % (i + 1);
            if( j != i )
            {
                SEVector3f* pTemp = apPermute[i];
                apPermute[i] = apPermute[j];
                apPermute[j] = pTemp;
            }
        }

        rMinimal = ExactSphere1(*apPermute[0]);
		tempSupp.Count = 1;
        tempSupp.Index[0] = 0;
        i = 1;
        while( i < iCount )
        {
            if( !tempSupp.Contains(i, apPermute, m_fEpsilon) )
            {
                // 如果当前support集合不包含当前顶点i.
 
                if( !Contains(*apPermute[i], rMinimal, fDistDiff) )
                {
                    // 如果当前最小球不包含当前顶点i.

					UpdateFunction oUpdate = m_aoUpdate[tempSupp.Count];
                    // 计算新的当前最小球,
                    // 使其能够同时包含当前support集合和当前顶点i.
                    SESphere3f tempSphere =(this->*oUpdate)(i, apPermute, 
                        tempSupp);
                    if( tempSphere.Radius > rMinimal.Radius )
                    {
                        rMinimal = tempSphere;
                        i = 0;

                        continue;
                    }
                }
            }
            i++;
        }

        delete[] apPermute;
    }
    else
    {
        SE_ASSERT( false );
    }

    rMinimal.Radius = SEMath<float>::Sqrt(rMinimal.Radius);
}
//----------------------------------------------------------------------------
bool SEMinSphere3f::Contains(const SEVector3f& rPoint, const SESphere3f& 
    rSphere, float& rfDistDiff)
{
    SEVector3f vec3fDiff = rPoint - rSphere.Center;
    float fTest = vec3fDiff.GetSquaredLength();

    // 这里的sphere半径是平方半径.
    rfDistDiff = fTest - rSphere.Radius;

    return rfDistDiff <= 0.0f;
}
//----------------------------------------------------------------------------
SESphere3f SEMinSphere3f::ExactSphere1(const SEVector3f& rPoint)
{
    SESphere3f tempMinimal;
    tempMinimal.Center = rPoint;
    tempMinimal.Radius = 0.0f;
    
    return tempMinimal;
}
//----------------------------------------------------------------------------
SESphere3f SEMinSphere3f::ExactSphere2(const SEVector3f& rPoint0, 
    const SEVector3f& rPoint1)
{
    SESphere3f tempMinimal;
    tempMinimal.Center = 0.5f * (rPoint0 + rPoint1);
    SEVector3f vec3fDiff = rPoint1 - rPoint0;
    tempMinimal.Radius = 0.25f * vec3fDiff.GetSquaredLength();
    
    return tempMinimal;
}
//----------------------------------------------------------------------------
SESphere3f SEMinSphere3f::ExactSphere3(const SEVector3f& rPoint0, 
    const SEVector3f& rPoint1, const SEVector3f& rPoint2)
{
    // 计算经过三个顶点p0,p1,p2的球,球心C是三个顶点的重心,
    // 且K = u0*p0 + u1*p1 + u2*p2,其中u0 + u1 + u2 = 1,
    // 且|K - p0| = |K - p1| = |K - p2| = 半径r
    // 由此得到:
    //   K - p0 = (u0*A + u1*B) - A
    //   K - p1 = (u0*A + u1*B) - B
    //   K - p2 = (u0*A + u1*B)
    // 其中 A = p0 - p2, B = p1 - p2,
    // 由此得到:
    //   r^2 = |u0*A + u1*B|^2 - 2*Dot(A, u0*A + u1*B) + |A|^2
    //   r^2 = |u0*A + u1*B|^2 - 2*Dot(B, u0*A + u1*B) + |B|^2
    //   r^2 = |u0*A + u1*B|^2
    // 把前两式减去最后式,得到线性方程组:
    // +-                 -++   -+       +-        -+
    // | Dot(A,A) Dot(A,B) || u0 | = 0.5 | Dot(A,A) |
    // | Dot(B,A) Dot(B,B) || u1 |       | Dot(B,B) |
    // +-                 -++   -+       +-        -+
    //
    // 以下代码解该线性方程组,得到u0,u1,然后得到r,
    // 注意线性方程组无解的情况.

    SEVector3f vec3fA = rPoint0 - rPoint2;
    SEVector3f vec3fB = rPoint1 - rPoint2;
    float fAdA = vec3fA.Dot(vec3fA);
    float fAdB = vec3fA.Dot(vec3fB);
    float fBdB = vec3fB.Dot(vec3fB);
    float fDet = fAdA*fBdB - fAdB*fAdB;

    SESphere3f tempMinimal;

    if( SEMath<float>::FAbs(fDet) > m_fEpsilon )
    {
        float fHalfInvDet = 0.5f / fDet;
        float fU0 = fHalfInvDet * fBdB * (fAdA - fAdB);
        float fU1 = fHalfInvDet * fAdA * (fBdB - fAdB);
        float fU2 = 1.0f - fU0 - fU1;
        
        tempMinimal.Center = fU0*rPoint0 + fU1*rPoint1 + fU2*rPoint2;
        SEVector3f vec3fTemp = fU0*vec3fA + fU1*vec3fB;
        tempMinimal.Radius = vec3fTemp.GetSquaredLength();
    }
    else
    {
        tempMinimal.Center = SEVector3f::ZERO;
        tempMinimal.Radius = SEMath<float>::MAX_REAL;
    }

    return tempMinimal;
}
//----------------------------------------------------------------------------
SESphere3f SEMinSphere3f::ExactSphere4(const SEVector3f& rPoint0, 
    const SEVector3f& rPoint1, const SEVector3f& rPoint2, const SEVector3f& 
    rPoint3)
{
    // 计算经过四个顶点p0,p1,p2,p3的球,球心C是四个顶点的重心,
    // 且K = u0*p0 + u1*p1 + u2*p2 + u3*p3,其中u0 + u1 + u2 + u3 = 1,
    // 且|K - p0| = |K - p1| = |K - p2| = |K - p3| = 半径r
    // 由此得到:
    //   K - p0 = u0*A + u1*B + u2*C - A
    //   K - p1 = u0*A + u1*B + u2*C - B
    //   K - p2 = u0*A + u1*B + u2*C - C
    //   K - p3 = u0*A + u1*B + u2*C
    // 其中 A = p0 - p3, B = p1 - p3, C = p2 - p3
    // 由此得到:
    //   r^2 = |u0*A + u1*B+u2*C|^2 - 2*Dot(A, u0*A + u1*B+u2*C) + |A|^2
    //   r^2 = |u0*A + u1*B+u2*C|^2 - 2*Dot(B, u0*A + u1*B+u2*C) + |B|^2
    //   r^2 = |u0*A + u1*B+u2*C|^2 - 2*Dot(C, u0*A + u1*B+u2*C) + |C|^2
    //   r^2 = |u0*A + u1*B+u2*C|^2
    // 把前三式减去最后式,得到线性方程组
    //
    // +-                          -++   -+       +-        -+
    // | Dot(A,A) Dot(A,B) Dot(A,C) || u0 | = 0.5 | Dot(A,A) |
    // | Dot(B,A) Dot(B,B) Dot(B,C) || u1 |       | Dot(B,B) |
    // | Dot(C,A) Dot(C,B) Dot(C,C) || u2 |       | Dot(C,C) |
    // +-                          -++   -+       +-        -+
    //
    // 以下代码解该线性方程组,得到u0,u1,u2然后得到r,
    // 注意线性方程组无解的情况.

    SEVector3f vec3fE10 = rPoint0 - rPoint3;
    SEVector3f vec3fE20 = rPoint1 - rPoint3;
    SEVector3f vec3fE30 = rPoint2 - rPoint3;

    float aafA[3][3];
    aafA[0][0] = vec3fE10.Dot(vec3fE10);
    aafA[0][1] = vec3fE10.Dot(vec3fE20);
    aafA[0][2] = vec3fE10.Dot(vec3fE30);
    aafA[1][0] = aafA[0][1];
    aafA[1][1] = vec3fE20.Dot(vec3fE20);
    aafA[1][2] = vec3fE20.Dot(vec3fE30);
    aafA[2][0] = aafA[0][2];
    aafA[2][1] = aafA[1][2];
    aafA[2][2] = vec3fE30.Dot(vec3fE30);

    float afB[3];
    afB[0] = 0.5f * aafA[0][0];
    afB[1] = 0.5f * aafA[1][1];
    afB[2] = 0.5f * aafA[2][2];

    float aafAInv[3][3];
    aafAInv[0][0] = aafA[1][1]*aafA[2][2] - aafA[1][2]*aafA[2][1];
    aafAInv[0][1] = aafA[0][2]*aafA[2][1] - aafA[0][1]*aafA[2][2];
    aafAInv[0][2] = aafA[0][1]*aafA[1][2] - aafA[0][2]*aafA[1][1];
    aafAInv[1][0] = aafA[1][2]*aafA[2][0] - aafA[1][0]*aafA[2][2];
    aafAInv[1][1] = aafA[0][0]*aafA[2][2] - aafA[0][2]*aafA[2][0];
    aafAInv[1][2] = aafA[0][2]*aafA[1][0] - aafA[0][0]*aafA[1][2];
    aafAInv[2][0] = aafA[1][0]*aafA[2][1] - aafA[1][1]*aafA[2][0];
    aafAInv[2][1] = aafA[0][1]*aafA[2][0] - aafA[0][0]*aafA[2][1];
    aafAInv[2][2] = aafA[0][0]*aafA[1][1] - aafA[0][1]*aafA[1][0];
    float fDet = aafA[0][0]*aafAInv[0][0] + aafA[0][1]*aafAInv[1][0] + 
        aafA[0][2]*aafAInv[2][0];

    SESphere3f tempMinimal;

    if( SEMath<float>::FAbs(fDet) > m_fEpsilon )
    {
        float fInvDet = 1.0f / fDet;
        int iRow, iCol;
        for( iRow = 0; iRow < 3; iRow++ )
        {
            for( iCol = 0; iCol < 3; iCol++ )
            {
                aafAInv[iRow][iCol] *= fInvDet;
            }
        }
        
        float afU[4];
        for( iRow = 0; iRow < 3; iRow++ )
        {
            afU[iRow] = 0.0f;
            for( iCol = 0; iCol < 3; iCol++ )
            {
                afU[iRow] += aafAInv[iRow][iCol] * afB[iCol];
            }
        }
        afU[3] = 1.0f - afU[0] - afU[1] - afU[2];
        
        tempMinimal.Center = afU[0]*rPoint0 + afU[1]*rPoint1 + afU[2]*rPoint2 + 
            afU[3]*rPoint3;
        SEVector3f vec3fTemp = afU[0]*vec3fE10 + afU[1]*vec3fE20 + 
            afU[2]*vec3fE30;
        tempMinimal.Radius = vec3fTemp.GetSquaredLength();
    }
    else
    {
        tempMinimal.Center = SEVector3f::ZERO;
        tempMinimal.Radius = SEMath<float>::MAX_REAL;
    }

    return tempMinimal;
}
//----------------------------------------------------------------------------
SESphere3f SEMinSphere3f::UpdateSupport1(int i, SEVector3f** apPermute, 
    SESupport& rSupport)
{
    const SEVector3f& rPoint0 = *apPermute[rSupport.Index[0]];
    const SEVector3f& rPoint1 = *apPermute[i];

    SESphere3f tempMinimal = ExactSphere2(rPoint0, rPoint1);
	rSupport.Count = 2;
    rSupport.Index[1] = i;

    return tempMinimal;
}
//----------------------------------------------------------------------------
SESphere3f SEMinSphere3f::UpdateSupport2(int i, SEVector3f** apPermute, 
    SESupport& rSupport)
{
    const SEVector3f& rPoint0 = *apPermute[rSupport.Index[0]];
    const SEVector3f& rPoint1 = *apPermute[rSupport.Index[1]];
    const SEVector3f& rPoint2 = *apPermute[i];

    SESphere3f tempSpheres[3];
    float fMinRSqr = SEMath<float>::MAX_REAL;
    float fDistDiff;
    int iIndex = -1;

    tempSpheres[0] = ExactSphere2(rPoint0, rPoint2);
    if( Contains(rPoint1, tempSpheres[0], fDistDiff) )
    {
        fMinRSqr = tempSpheres[0].Radius;
        iIndex = 0;
    }

    tempSpheres[1] = ExactSphere2(rPoint1, rPoint2);
    if( tempSpheres[1].Radius < fMinRSqr )
    {
        if( Contains(rPoint0, tempSpheres[1], fDistDiff) )
        {
            fMinRSqr = tempSpheres[1].Radius;
            iIndex = 1;
        }
    }

    SESphere3f tempMinimal;

    if( iIndex != -1 )
    {
        tempMinimal = tempSpheres[iIndex];
        rSupport.Index[1 - iIndex] = i;
    }
    else
    {
        tempMinimal = ExactSphere3(rPoint0, rPoint1, rPoint2);
        SE_ASSERT( tempMinimal.Radius <= fMinRSqr );
		rSupport.Count = 3;
        rSupport.Index[2] = i;
    }

    return tempMinimal;
}
//----------------------------------------------------------------------------
SESphere3f SEMinSphere3f::UpdateSupport3(int i, SEVector3f** apPermute, 
    SESupport& rSupport)
{
    const SEVector3f& rPoint0 = *apPermute[rSupport.Index[0]];
    const SEVector3f& rPoint1 = *apPermute[rSupport.Index[1]];
    const SEVector3f& rPoint2 = *apPermute[rSupport.Index[2]];
    const SEVector3f& rPoint3 = *apPermute[i];

    SESphere3f tempSpheres[6];
    float fMinRSqr = SEMath<float>::MAX_REAL;
    float fDistDiff;
    int iIndex = -1;

    tempSpheres[0] = ExactSphere2(rPoint0, rPoint3);
    if( Contains(rPoint1, tempSpheres[0], fDistDiff) && 
        Contains(rPoint2, tempSpheres[0], fDistDiff) )
    {
        fMinRSqr = tempSpheres[0].Radius;
        iIndex = 0;
    }

    tempSpheres[1] = ExactSphere2(rPoint1, rPoint3);
    if( tempSpheres[1].Radius < fMinRSqr && 
        Contains(rPoint0, tempSpheres[1], fDistDiff) && 
        Contains(rPoint2, tempSpheres[1], fDistDiff) )
    {
        fMinRSqr = tempSpheres[1].Radius;
        iIndex = 1;
    }

    tempSpheres[2] = ExactSphere2(rPoint2, rPoint3);
    if( tempSpheres[2].Radius < fMinRSqr && 
        Contains(rPoint0, tempSpheres[2], fDistDiff) && 
        Contains(rPoint1, tempSpheres[2], fDistDiff) )
    {
        fMinRSqr = tempSpheres[2].Radius;
        iIndex = 2;
    }

    tempSpheres[3] = ExactSphere3(rPoint0, rPoint1, rPoint3);
    if( tempSpheres[3].Radius < fMinRSqr && 
        Contains(rPoint2, tempSpheres[3], fDistDiff) )
    {
        fMinRSqr = tempSpheres[3].Radius;
        iIndex = 3;
    }

    tempSpheres[4] = ExactSphere3(rPoint0, rPoint2, rPoint3);
    if( tempSpheres[4].Radius < fMinRSqr && 
        Contains(rPoint1, tempSpheres[4], fDistDiff) )
    {
        fMinRSqr = tempSpheres[4].Radius;
        iIndex = 4;
    }

    tempSpheres[5] = ExactSphere3(rPoint1, rPoint2, rPoint3);
    if( tempSpheres[5].Radius < fMinRSqr && 
        Contains(rPoint0, tempSpheres[5], fDistDiff) )
    {
        fMinRSqr = tempSpheres[5].Radius;
        iIndex = 5;
    }

    SESphere3f tempMinimal;

    switch( iIndex )
    {
    case 0:
        tempMinimal = tempSpheres[0];
		rSupport.Count = 2;
        rSupport.Index[1] = i;
        break;
    case 1:
        tempMinimal = tempSpheres[1];
        rSupport.Count = 2;
        rSupport.Index[0] = i;
        break;
    case 2:
        tempMinimal = tempSpheres[2];
        rSupport.Count = 2;
        rSupport.Index[0] = rSupport.Index[2];
        rSupport.Index[1] = i;
        break;
    case 3:
        tempMinimal = tempSpheres[3];
        rSupport.Index[2] = i;
        break;
    case 4:
        tempMinimal = tempSpheres[4];
        rSupport.Index[1] = i;
        break;
    case 5:
        tempMinimal = tempSpheres[5];
        rSupport.Index[0] = i;
        break;
    default:
        tempMinimal = ExactSphere4(rPoint0, rPoint1, rPoint2, rPoint3);
        SE_ASSERT( tempMinimal.Radius <= fMinRSqr );
        rSupport.Count = 4;
        rSupport.Index[3] = i;
        break;
    }

    return tempMinimal;
}
//----------------------------------------------------------------------------
SESphere3f SEMinSphere3f::UpdateSupport4(int i, SEVector3f** apPermute, 
    SESupport& rSupport)
{
    const SEVector3f* aPoint[4] =
    {
        apPermute[rSupport.Index[0]],
        apPermute[rSupport.Index[1]],
        apPermute[rSupport.Index[2]],
        apPermute[rSupport.Index[3]]
    };

    const SEVector3f& rPoint4 = *apPermute[i];

    // 排列类型1.
    int aiT1[4][4] =
    {
        {0, /*4*/ 1,2,3},
        {1, /*4*/ 0,2,3},
        {2, /*4*/ 0,1,3},
        {3, /*4*/ 0,1,2}
    };

    // 排列类型2.
    int aiT2[6][4] =
    {
        {0,1, /*4*/ 2,3},
        {0,2, /*4*/ 1,3},
        {0,3, /*4*/ 1,2},
        {1,2, /*4*/ 0,3},
        {1,3, /*4*/ 0,2},
        {2,3, /*4*/ 0,1}
    };

    // 排列类型3.
    int aiT3[4][4] =
    {
        {0,1,2, /*4*/ 3},
        {0,1,3, /*4*/ 2},
        {0,2,3, /*4*/ 1},
        {1,2,3, /*4*/ 0}
    };

    SESphere3f tempSpheres[14];
    float fMinRSqr = SEMath<float>::MAX_REAL;
    int iIndex = -1;
    float fDistDiff, fMinDistDiff = SEMath<float>::MAX_REAL;
    int iMinIndex = -1;
    int k = 0;  // 球的索引

    // 排列类型1.
    int j;
    for( j = 0; j < 4; j++, k++ )
    {
        tempSpheres[k] = ExactSphere2(*aPoint[aiT1[j][0]], rPoint4);
        if( tempSpheres[k].Radius < fMinRSqr )
        {
            if( Contains(*aPoint[aiT1[j][1]], tempSpheres[k], fDistDiff) &&
                Contains(*aPoint[aiT1[j][2]], tempSpheres[k], fDistDiff) &&
                Contains(*aPoint[aiT1[j][3]], tempSpheres[k], fDistDiff) )
            {
                fMinRSqr = tempSpheres[k].Radius;
                iIndex = k;
            }
            else if( fDistDiff < fMinDistDiff )
            {
                fMinDistDiff = fDistDiff;
                iMinIndex = k;
            }
        }
    }

    // 排列类型2.
    for( j = 0; j < 6; j++, k++ )
    {
        tempSpheres[k] = ExactSphere3(*aPoint[aiT2[j][0]], *aPoint[aiT2[j][1]], 
            rPoint4);
        if( tempSpheres[k].Radius < fMinRSqr )
        {
            if( Contains(*aPoint[aiT2[j][2]], tempSpheres[k], fDistDiff) &&  
                Contains(*aPoint[aiT2[j][3]], tempSpheres[k], fDistDiff) )
            {
                fMinRSqr = tempSpheres[k].Radius;
                iIndex = k;
            }
            else if( fDistDiff < fMinDistDiff )
            {
                fMinDistDiff = fDistDiff;
                iMinIndex = k;
            }
        }
    }

    // 排列类型3.
    for( j = 0; j < 4; j++, k++ )
    {
        tempSpheres[k] = ExactSphere4(*aPoint[aiT3[j][0]], *aPoint[aiT3[j][1]], 
            *aPoint[aiT3[j][2]], rPoint4);
        if( tempSpheres[k].Radius < fMinRSqr )
        {
            if( Contains(*aPoint[aiT3[j][3]], tempSpheres[k], fDistDiff) )
            {
                fMinRSqr = tempSpheres[k].Radius;
                iIndex = k;
            }
            else if( fDistDiff < fMinDistDiff )
            {
                fMinDistDiff = fDistDiff;
                iMinIndex = k;
            }
        }
    }

    // 理论上来说,iIndex >= 0应该会发生,但由于浮点运算舍入问题引起误差,
    // iIndex == -1也有可能发生,
    // 此时选择具备最小外围顶点误差的球.
    if( iIndex == -1 )
    {
        iIndex = iMinIndex;
    }

    SESphere3f tempMinimal = tempSpheres[iIndex];

    switch( iIndex )
    {
    case 0:
        rSupport.Count = 2;
        rSupport.Index[1] = i;
        break;
    case 1:
        rSupport.Count = 2;
        rSupport.Index[0] = i;
        break;
    case 2:
        rSupport.Count = 2;
        rSupport.Index[0] = rSupport.Index[2];
        rSupport.Index[1] = i;
        break;
    case 3:
        rSupport.Count = 2;
        rSupport.Index[0] = rSupport.Index[3];
        rSupport.Index[1] = i;
        break;
    case 4:
        rSupport.Count = 3;
        rSupport.Index[2] = i;
        break;
    case 5:
        rSupport.Count = 3;
        rSupport.Index[1] = i;
        break;
    case 6:
        rSupport.Count = 3;
        rSupport.Index[1] = rSupport.Index[3];
        rSupport.Index[2] = i;
        break;
    case 7:
        rSupport.Count = 3;
        rSupport.Index[0] = i;
        break;
    case 8:
        rSupport.Count = 3;
        rSupport.Index[0] = rSupport.Index[3];
        rSupport.Index[2] = i;
        break;
    case 9:
        rSupport.Count = 3;
        rSupport.Index[0] = rSupport.Index[3];
        rSupport.Index[1] = i;
        break;
    case 10:
        rSupport.Index[3] = i;
        break;
    case 11:
        rSupport.Index[2] = i;
        break;
    case 12:
        rSupport.Index[1] = i;
        break;
    case 13:
        rSupport.Index[0] = i;
        break;
    }

    return tempMinimal;
}
//----------------------------------------------------------------------------