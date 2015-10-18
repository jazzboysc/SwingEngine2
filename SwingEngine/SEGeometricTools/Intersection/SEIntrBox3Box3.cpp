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
#include "SEIntrBox3Box3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEIntrBox3Box3f::SEIntrBox3Box3f(const SEBox3f& rBox0, const SEBox3f& rBox1)
    :
    m_pBox0(&rBox0), 
    m_pBox1(&rBox1)
{
}
//----------------------------------------------------------------------------
const SEBox3f& SEIntrBox3Box3f::GetBox0() const
{
    return *m_pBox0;
}
//----------------------------------------------------------------------------
const SEBox3f& SEIntrBox3Box3f::GetBox1() const
{
    return *m_pBox1;
}
//----------------------------------------------------------------------------
bool SEIntrBox3Box3f::Test()
{
    // 两box轴之间夹角的cos值的阀值.当大于此阀值时,则认为两向量平行.
    // 用于判断两box所有轴中,是否至少存在一对平行向量.
    // 当此情况发生时,则没有必要测试那些Cross(A[i], B[j])构成的潜在分离轴了.
    const float fCutoff = 1.0f - SEMath<float>::ZERO_TOLERANCE;
    bool bExistsParallelPair = false;
    int i;

    // 辅助变量.
    const SEVector3f* aA = m_pBox0->Axis;
    const SEVector3f* aB = m_pBox1->Axis;
    const float* afEA = m_pBox0->Extent;
    const float* afEB = m_pBox1->Extent;

    // 计算两box中心点差向量,D = C1 - C0.
    SEVector3f vec3fD = m_pBox1->Center - m_pBox0->Center;

    float aafC[3][3];     // matrix C = A^T B, c_{ij} = Dot(A_i, B_j)
    float aafAbsC[3][3];  // |c_{ij}|
    float afAD[3];        // Dot(A_i, D)
    float fR0, fR1, fR;   // 投影区间半径和两区间中点距离
    float fR01;           // = R0 + R1

    // axis C0 + t*A0
    for( i = 0; i < 3; i++ )
    {
        aafC[0][i] = aA[0].Dot(aB[i]);
        aafAbsC[0][i] = SEMath<float>::FAbs(aafC[0][i]);
        if( aafAbsC[0][i] > fCutoff )
        {
            bExistsParallelPair = true;
        }
    }
    afAD[0] = aA[0].Dot(vec3fD);
    fR = SEMath<float>::FAbs(afAD[0]);
    fR1 = afEB[0]*aafAbsC[0][0] + afEB[1]*aafAbsC[0][1] + afEB[2]*aafAbsC[0][2];
    fR01 = afEA[0] + fR1;
    if( fR > fR01 )
    {
        return false;
    }

    // axis C0 + t*A1
    for( i = 0; i < 3; i++ )
    {
        aafC[1][i] = aA[1].Dot(aB[i]);
        aafAbsC[1][i] = SEMath<float>::FAbs(aafC[1][i]);
        if( aafAbsC[1][i] > fCutoff )
        {
            bExistsParallelPair = true;
        }
    }
    afAD[1] = aA[1].Dot(vec3fD);
    fR = SEMath<float>::FAbs(afAD[1]);
    fR1 = afEB[0]*aafAbsC[1][0] + afEB[1]*aafAbsC[1][1] + afEB[2]*aafAbsC[1][2];
    fR01 = afEA[1] + fR1;
    if( fR > fR01 )
    {
        return false;
    }

    // axis C0 + t*A2
    for( i = 0; i < 3; i++ )
    {
        aafC[2][i] = aA[2].Dot(aB[i]);
        aafAbsC[2][i] = SEMath<float>::FAbs(aafC[2][i]);
        if( aafAbsC[2][i] > fCutoff )
        {
            bExistsParallelPair = true;
        }
    }
    afAD[2] = aA[2].Dot(vec3fD);
    fR = SEMath<float>::FAbs(afAD[2]);
    fR1 = afEB[0]*aafAbsC[2][0] + afEB[1]*aafAbsC[2][1] + afEB[2]*aafAbsC[2][2];
    fR01 = afEA[2] + fR1;
    if( fR > fR01 )
    {
        return false;
    }

    // axis C0 + t*B0
    fR = SEMath<float>::FAbs(aB[0].Dot(vec3fD));
    fR0 = afEA[0]*aafAbsC[0][0] + afEA[1]*aafAbsC[1][0] + afEA[2]*aafAbsC[2][0];
    fR01 = fR0 + afEB[0];
    if( fR > fR01 )
    {
        return false;
    }

    // axis C0 + t*B1
    fR = SEMath<float>::FAbs(aB[1].Dot(vec3fD));
    fR0 = afEA[0]*aafAbsC[0][1] + afEA[1]*aafAbsC[1][1] + afEA[2]*aafAbsC[2][1];
    fR01 = fR0 + afEB[1];
    if( fR > fR01 )
    {
        return false;
    }

    // axis C0 + t*B2
    fR = SEMath<float>::FAbs(aB[2].Dot(vec3fD));
    fR0 = afEA[0]*aafAbsC[0][2] + afEA[1]*aafAbsC[1][2] + afEA[2]*aafAbsC[2][2];
    fR01 = fR0 + afEB[2];
    if( fR > fR01 )
    {
        return false;
    }

    // 当至少存在一对两box平行轴时,
    // 潜在分离轴检查将退化为2D问题,因此检查所有box"边"法线向量就够了.
    if( bExistsParallelPair )
    {
        return true;
    }

    // axis C0 + t*A0xB0
    fR = SEMath<float>::FAbs(afAD[2]*aafC[1][0] - afAD[1]*aafC[2][0]);
    fR0 = afEA[1]*aafAbsC[2][0] + afEA[2]*aafAbsC[1][0];
    fR1 = afEB[1]*aafAbsC[0][2] + afEB[2]*aafAbsC[0][1];
    fR01 = fR0 + fR1;
    if( fR > fR01 )
    {
        return false;
    }

    // axis C0 + t*A0xB1
    fR = SEMath<float>::FAbs(afAD[2]*aafC[1][1] - afAD[1]*aafC[2][1]);
    fR0 = afEA[1]*aafAbsC[2][1] + afEA[2]*aafAbsC[1][1];
    fR1 = afEB[0]*aafAbsC[0][2] + afEB[2]*aafAbsC[0][0];
    fR01 = fR0 + fR1;
    if( fR > fR01 )
    {
        return false;
    }

    // axis C0 + t*A0xB2
    fR = SEMath<float>::FAbs(afAD[2]*aafC[1][2] - afAD[1]*aafC[2][2]);
    fR0 = afEA[1]*aafAbsC[2][2] + afEA[2]*aafAbsC[1][2];
    fR1 = afEB[0]*aafAbsC[0][1] + afEB[1]*aafAbsC[0][0];
    fR01 = fR0 + fR1;
    if( fR > fR01 )
    {
        return false;
    }

    // axis C0 + t*A1xB0
    fR = SEMath<float>::FAbs(afAD[0]*aafC[2][0] - afAD[2]*aafC[0][0]);
    fR0 = afEA[0]*aafAbsC[2][0] + afEA[2]*aafAbsC[0][0];
    fR1 = afEB[1]*aafAbsC[1][2] + afEB[2]*aafAbsC[1][1];
    fR01 = fR0 + fR1;
    if( fR > fR01 )
    {
        return false;
    }

    // axis C0 + t*A1xB1
    fR = SEMath<float>::FAbs(afAD[0]*aafC[2][1] - afAD[2]*aafC[0][1]);
    fR0 = afEA[0]*aafAbsC[2][1] + afEA[2]*aafAbsC[0][1];
    fR1 = afEB[0]*aafAbsC[1][2] + afEB[2]*aafAbsC[1][0];
    fR01 = fR0 + fR1;
    if( fR > fR01 )
    {
        return false;
    }

    // axis C0 + t*A1xB2
    fR = SEMath<float>::FAbs(afAD[0]*aafC[2][2] - afAD[2]*aafC[0][2]);
    fR0 = afEA[0]*aafAbsC[2][2] + afEA[2]*aafAbsC[0][2];
    fR1 = afEB[0]*aafAbsC[1][1] + afEB[1]*aafAbsC[1][0];
    fR01 = fR0 + fR1;
    if( fR > fR01 )
    {
        return false;
    }

    // axis C0 + t*A2xB0
    fR = SEMath<float>::FAbs(afAD[1]*aafC[0][0] - afAD[0]*aafC[1][0]);
    fR0 = afEA[0]*aafAbsC[1][0] + afEA[1]*aafAbsC[0][0];
    fR1 = afEB[1]*aafAbsC[2][2] + afEB[2]*aafAbsC[2][1];
    fR01 = fR0 + fR1;
    if( fR > fR01 )
    {
        return false;
    }

    // axis C0 + t*A2xB1
    fR = SEMath<float>::FAbs(afAD[1]*aafC[0][1] - afAD[0]*aafC[1][1]);
    fR0 = afEA[0]*aafAbsC[1][1] + afEA[1]*aafAbsC[0][1];
    fR1 = afEB[0]*aafAbsC[2][2] + afEB[2]*aafAbsC[2][0];
    fR01 = fR0 + fR1;
    if( fR > fR01 )
    {
        return false;
    }

    // axis C0 + t*A2xB2
    fR = SEMath<float>::FAbs(afAD[1]*aafC[0][2] - afAD[0]*aafC[1][2]);
    fR0 = afEA[0]*aafAbsC[1][2] + afEA[1]*aafAbsC[0][2];
    fR1 = afEB[0]*aafAbsC[2][1] + afEB[1]*aafAbsC[2][0];
    fR01 = fR0 + fR1;
    if( fR > fR01 )
    {
        return false;
    }

    return true;
}
//----------------------------------------------------------------------------
bool SEIntrBox3Box3f::Test(float fTMax, const SEVector3f& rVelocity0, 
    const SEVector3f& rVelocity1)
{
    if( rVelocity0 == rVelocity1 )
    {
        if( Test() )
        {
            m_fContactTime = 0.0f;

            return true;
        }

        return false;
    }

    // 两box轴之间夹角的cos值的阀值.当大于此阀值时,则认为两向量平行.
    // 用于判断两box所有轴中,是否至少存在一对平行向量.
    // 当此情况发生时,则没有必要测试那些Cross(A[i], B[j])构成的潜在分离轴了.
    const float fCutoff = 1.0f - SEMath<float>::ZERO_TOLERANCE;
    bool bExistsParallelPair = false;

    // 辅助变量.
    const SEVector3f* aA = m_pBox0->Axis;
    const SEVector3f* aB = m_pBox1->Axis;
    const float* afEA = m_pBox0->Extent;
    const float* afEB = m_pBox1->Extent;
    SEVector3f vec3fD = m_pBox1->Center - m_pBox0->Center;
    SEVector3f vec3fW = rVelocity1 - rVelocity0;
    float aafC[3][3];     // matrix C = A^T B, c_{ij} = Dot(A_i, B_j)
    float aafAbsC[3][3];  // |c_{ij}|
    float afAD[3];        // Dot(A_i, D)
    float afAW[3];        // Dot(A_i, W)
    float fMin0, fMax0, fMin1, fMax1, fCenter, fRadius, fSpeed;
    int i, j;

    m_fContactTime = 0.0f;
    float fTLast = SEMath<float>::MAX_REAL;

    // axes C0 + t*A[i]
    for( i = 0; i < 3; i++ )
    {
        for( j = 0; j < 3; j++ )
        {
            aafC[i][j] = aA[i].Dot(aB[j]);
            aafAbsC[i][j] = SEMath<float>::FAbs(aafC[i][j]);
            if( aafAbsC[i][j] > fCutoff )
            {
                bExistsParallelPair = true;
            }
        }
        afAD[i] = aA[i].Dot(vec3fD);
        afAW[i] = aA[i].Dot(vec3fW);
        fMin0 = -afEA[i];
        fMax0 = +afEA[i];
        fRadius = afEB[0]*aafAbsC[i][0] + afEB[1]*aafAbsC[i][1] + 
            afEB[2]*aafAbsC[i][2];
        fMin1 = afAD[i] - fRadius;
        fMax1 = afAD[i] + fRadius;
        fSpeed = afAW[i];
        if( IsSeparated(fMin0, fMax0, fMin1, fMax1, fSpeed, fTMax, m_fContactTime, 
            fTLast) )
        {
            return false;
        }
    }

    // axes C0 + t*B[i]
    for( i = 0; i < 3; i++ )
    {
        fRadius = afEA[0]*aafAbsC[0][i] + afEA[1]*aafAbsC[1][i] +
            afEA[2]*aafAbsC[2][i];
        fMin0 = -fRadius;
        fMax0 = +fRadius;
        fCenter = aB[i].Dot(vec3fD);
        fMin1 = fCenter - afEB[i];
        fMax1 = fCenter + afEB[i];
        fSpeed = vec3fW.Dot(aB[i]);
        if( IsSeparated(fMin0, fMax0, fMin1, fMax1, fSpeed, fTMax, m_fContactTime, 
            fTLast) )
        {
            return false;
        }
    }

    // 当至少存在一对两box平行轴时,
    // 潜在分离轴检查将退化为2D问题,因此检查所有box"边"法线向量就够了.
    if( bExistsParallelPair )
    {
        return true;
    }

    // axis C0 + t*A0xB0
    fRadius = afEA[1]*aafAbsC[2][0] + afEA[2]*aafAbsC[1][0];
    fMin0 = -fRadius;
    fMax0 = +fRadius;
    fCenter = afAD[2]*aafC[1][0] - afAD[1]*aafC[2][0];
    fRadius = afEB[1]*aafAbsC[0][2] + afEB[2]*aafAbsC[0][1];
    fMin1 = fCenter - fRadius;
    fMax1 = fCenter + fRadius;
    fSpeed = afAW[2]*aafC[1][0] - afAW[1]*aafC[2][0];
    if( IsSeparated(fMin0, fMax0, fMin1, fMax1, fSpeed, fTMax, m_fContactTime, 
        fTLast) )
    {
        return false;
    }

    // axis C0 + t*A0xB1
    fRadius = afEA[1]*aafAbsC[2][1] + afEA[2]*aafAbsC[1][1];
    fMin0 = -fRadius;
    fMax0 = +fRadius;
    fCenter = afAD[2]*aafC[1][1] - afAD[1]*aafC[2][1];
    fRadius = afEB[0]*aafAbsC[0][2] + afEB[2]*aafAbsC[0][0];
    fMin1 = fCenter - fRadius;
    fMax1 = fCenter + fRadius;
    fSpeed = afAW[2]*aafC[1][1] - afAW[1]*aafC[2][1];
    if( IsSeparated(fMin0, fMax0, fMin1, fMax1, fSpeed, fTMax, m_fContactTime, 
        fTLast) )
    {
        return false;
    }

    // axis C0 + t*A0xB2
    fRadius = afEA[1]*aafAbsC[2][2] + afEA[2]*aafAbsC[1][2];
    fMin0 = -fRadius;
    fMax0 = +fRadius;
    fCenter = afAD[2]*aafC[1][2] - afAD[1]*aafC[2][2];
    fRadius = afEB[0]*aafAbsC[0][1] + afEB[1]*aafAbsC[0][0];
    fMin1 = fCenter - fRadius;
    fMax1 = fCenter + fRadius;
    fSpeed = afAW[2]*aafC[1][2] - afAW[1]*aafC[2][2];
    if( IsSeparated(fMin0, fMax0, fMin1, fMax1, fSpeed, fTMax, m_fContactTime, 
        fTLast) )
    {
        return false;
    }

    // axis C0 + t*A1xB0
    fRadius = afEA[0]*aafAbsC[2][0] + afEA[2]*aafAbsC[0][0];
    fMin0 = -fRadius;
    fMax0 = +fRadius;
    fCenter = afAD[0]*aafC[2][0] - afAD[2]*aafC[0][0];
    fRadius = afEB[1]*aafAbsC[1][2] + afEB[2]*aafAbsC[1][1];
    fMin1 = fCenter - fRadius;
    fMax1 = fCenter + fRadius;
    fSpeed = afAW[0]*aafC[2][0] - afAW[2]*aafC[0][0];
    if( IsSeparated(fMin0, fMax0, fMin1, fMax1, fSpeed, fTMax, m_fContactTime, 
        fTLast) )
    {
        return false;
    }

    // axis C0 + t*A1xB1
    fRadius = afEA[0]*aafAbsC[2][1] + afEA[2]*aafAbsC[0][1];
    fMin0 = -fRadius;
    fMax0 = +fRadius;
    fCenter = afAD[0]*aafC[2][1] - afAD[2]*aafC[0][1];
    fRadius = afEB[0]*aafAbsC[1][2] + afEB[2]*aafAbsC[1][0];
    fMin1 = fCenter - fRadius;
    fMax1 = fCenter + fRadius;
    fSpeed = afAW[0]*aafC[2][1] - afAW[2]*aafC[0][1];
    if( IsSeparated(fMin0, fMax0, fMin1, fMax1, fSpeed, fTMax, m_fContactTime, 
        fTLast) )
    {
        return false;
    }

    // axis C0 + t*A1xB2
    fRadius = afEA[0]*aafAbsC[2][2] + afEA[2]*aafAbsC[0][2];
    fMin0 = -fRadius;
    fMax0 = +fRadius;
    fCenter = afAD[0]*aafC[2][2] - afAD[2]*aafC[0][2];
    fRadius = afEB[0]*aafAbsC[1][1] + afEB[1]*aafAbsC[1][0];
    fMin1 = fCenter - fRadius;
    fMax1 = fCenter + fRadius;
    fSpeed = afAW[0]*aafC[2][2] - afAW[2]*aafC[0][2];
    if( IsSeparated(fMin0, fMax0, fMin1, fMax1, fSpeed, fTMax, m_fContactTime, 
        fTLast) )
    {
        return false;
    }

    // axis C0 + t*A2xB0
    fRadius = afEA[0]*aafAbsC[1][0] + afEA[1]*aafAbsC[0][0];
    fMin0 = -fRadius;
    fMax0 = +fRadius;
    fCenter = afAD[1]*aafC[0][0] - afAD[0]*aafC[1][0];
    fRadius = afEB[1]*aafAbsC[2][2] + afEB[2]*aafAbsC[2][1];
    fMin1 = fCenter - fRadius;
    fMax1 = fCenter + fRadius;
    fSpeed = afAW[1]*aafC[0][0] - afAW[0]*aafC[1][0];
    if( IsSeparated(fMin0, fMax0, fMin1, fMax1, fSpeed, fTMax, m_fContactTime, 
        fTLast) )
    {
        return false;
    }

    // axis C0 + t*A2xB1
    fRadius = afEA[0]*aafAbsC[1][1] + afEA[1]*aafAbsC[0][1];
    fMin0 = -fRadius;
    fMax0 = +fRadius;
    fCenter = afAD[1]*aafC[0][1] - afAD[0]*aafC[1][1];
    fRadius = afEB[0]*aafAbsC[2][2] + afEB[2]*aafAbsC[2][0];
    fMin1 = fCenter - fRadius;
    fMax1 = fCenter + fRadius;
    fSpeed = afAW[1]*aafC[0][1] - afAW[0]*aafC[1][1];
    if( IsSeparated(fMin0, fMax0, fMin1, fMax1, fSpeed, fTMax, m_fContactTime, 
        fTLast) )
    {
        return false;
    }

    // axis C0 + t*A2xB2
    fRadius = afEA[0]*aafAbsC[1][2] + afEA[1]*aafAbsC[0][2];
    fMin0 = -fRadius;
    fMax0 = +fRadius;
    fCenter = afAD[1]*aafC[0][2] - afAD[0]*aafC[1][2];
    fRadius = afEB[0]*aafAbsC[2][1] + afEB[1]*aafAbsC[2][0];
    fMin1 = fCenter - fRadius;
    fMax1 = fCenter + fRadius;
    fSpeed = afAW[1]*aafC[0][2] - afAW[0]*aafC[1][2];
    if( IsSeparated(fMin0, fMax0, fMin1, fMax1, fSpeed, fTMax, m_fContactTime, 
        fTLast) )
    {
        return false;
    }

    return true;
}
//----------------------------------------------------------------------------
bool SEIntrBox3Box3f::IsSeparated(float fMin0, float fMax0, float fMin1, 
    float fMax1, float fSpeed, float fTMax, float& rfTFirst, float& rfTLast)
{
    float fInvSpeed, fT;

    if( fMax1 < fMin0 ) // box1初始在box0左边
    {
        if( fSpeed <= 0.0f )
        {
            // 两投影区间正在逐渐远离.
            return true;
        }
        fInvSpeed = 1.0f / fSpeed;

        fT = (fMin0 - fMax1)*fInvSpeed;
        if( fT > rfTFirst )
        {
            rfTFirst = fT;
        }

        if( rfTFirst > fTMax )
        {
            // 在指定时间范围内两投影区间不相交.
            return true;
        }

        fT = (fMax0 - fMin1)*fInvSpeed;
        if( fT < rfTLast )
        {
            rfTLast = fT;
        }

        if( rfTFirst > rfTLast )
        {
            // Physically inconsistent times--对象无法相交.
            return true;
        }
    }
    else if( fMax0 < fMin1 ) // box1初始在box0右边
    {
        if( fSpeed >= 0.0f )
        {
            // 两投影区间正在逐渐远离.
            return true;
        }
        fInvSpeed = 1.0f / fSpeed;

        fT = (fMax0 - fMin1)*fInvSpeed;
        if( fT > rfTFirst )
        {
            rfTFirst = fT;
        }

        if( rfTFirst > fTMax )
        {
            // 在指定时间范围内两投影区间不相交.
            return true;
        }

        fT = (fMin0 - fMax1)*fInvSpeed;
        if( fT < rfTLast )
        {
            rfTLast = fT;
        }

        if( rfTFirst > rfTLast )
        {
            // Physically inconsistent times--对象无法相交.
            return true;
        }
    }
    else // box0和box1的投影区间初始迭交
    {
        if( fSpeed > 0.0f )
        {
            fT = (fMax0 - fMin1)/fSpeed;
            if( fT < rfTLast )
            {
                rfTLast = fT;
            }

            if( rfTFirst > rfTLast )
            {
                // Physically inconsistent times--对象无法相交.
                return true;
            }
        }
        else if( fSpeed < 0.0f )
        {
            fT = (fMin0 - fMax1)/fSpeed;
            if( fT < rfTLast )
            {
                rfTLast = fT;
            }

            if( rfTFirst > rfTLast )
            {
                // Physically inconsistent times--对象无法相交.
                return true;
            }
        }
    }

    return false;
}
//----------------------------------------------------------------------------
