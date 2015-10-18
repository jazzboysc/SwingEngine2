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
#include "SEDistVector3Triangle3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEDistVector3Triangle3f::SEDistVector3Triangle3f(const SEVector3f& rVector, 
    const SETriangle3f& rTriangle)
    :
    m_pVector(&rVector),
    m_pTriangle(&rTriangle)
{
}
//----------------------------------------------------------------------------
const SEVector3f& SEDistVector3Triangle3f::GetVector() const
{
    return *m_pVector;
}
//----------------------------------------------------------------------------
const SETriangle3f& SEDistVector3Triangle3f::GetTriangle() const
{
    return *m_pTriangle;
}
//----------------------------------------------------------------------------
float SEDistVector3Triangle3f::Get()
{
    float fSqrDist = GetSquared();

    return SEMath<float>::Sqrt(fSqrDist);
}
//----------------------------------------------------------------------------
float SEDistVector3Triangle3f::GetSquared()
{
    SEVector3f vec3fDiff = m_pTriangle->V[0] - *m_pVector;
    SEVector3f vec3fEdge0 = m_pTriangle->V[1] - m_pTriangle->V[0];
    SEVector3f vec3fEdge1 = m_pTriangle->V[2] - m_pTriangle->V[0];
    float fA00 = vec3fEdge0.GetSquaredLength();
    float fA01 = vec3fEdge0.Dot(vec3fEdge1);
    float fA11 = vec3fEdge1.GetSquaredLength();
    float fB0 = vec3fDiff.Dot(vec3fEdge0);
    float fB1 = vec3fDiff.Dot(vec3fEdge1);
    float fC = vec3fDiff.GetSquaredLength();
    float fDet = SEMath<float>::FAbs(fA00*fA11 - fA01*fA01);
    float fS = fA01*fB1 - fA11*fB0;
    float fT = fA01*fB0 - fA00*fB1;
    float fSqrDistance;

    if( fS + fT <= fDet )
    {
        if( fS < 0.0f )
        {
            if( fT < 0.0f )  // region 4
            {
                if( fB0 < 0.0f )
                {
                    fT = 0.0f;
                    if( -fB0 >= fA00 )
                    {
                        fS = 1.0f;
                        fSqrDistance = fA00 + 2.0f*fB0 + fC;
                    }
                    else
                    {
                        fS = -fB0/fA00;
                        fSqrDistance = fB0*fS + fC;
                    }
                }
                else
                {
                    fS = 0.0f;
                    if( fB1 >= 0.0f )
                    {
                        fT = 0.0f;
                        fSqrDistance = fC;
                    }
                    else if( -fB1 >= fA11 )
                    {
                        fT = 1.0f;
                        fSqrDistance = fA11 + 2.0f*fB1 + fC;
                    }
                    else
                    {
                        fT = -fB1/fA11;
                        fSqrDistance = fB1*fT + fC;
                    }
                }
            }
            else  // region 3
            {
                fS = 0.0f;
                if( fB1 >= 0.0f )
                {
                    fT = 0.0f;
                    fSqrDistance = fC;
                }
                else if( -fB1 >= fA11 )
                {
                    fT = 1.0f;
                    fSqrDistance = fA11 + 2.0f*fB1 + fC;
                }
                else
                {
                    fT = -fB1/fA11;
                    fSqrDistance = fB1*fT + fC;
                }
            }
        }
        else if( fT < 0.0f )  // region 5
        {
            fT = 0.0f;
            if( fB0 >= 0.0f )
            {
                fS = 0.0f;
                fSqrDistance = fC;
            }
            else if( -fB0 >= fA00 )
            {
                fS = 1.0f;
                fSqrDistance = fA00 + 2.0f*fB0 + fC;
            }
            else
            {
                fS = -fB0/fA00;
                fSqrDistance = fB0*fS + fC;
            }
        }
        else  // region 0
        {
            // minimum at interior point
            float fInvDet = 1.0f / fDet;
            fS *= fInvDet;
            fT *= fInvDet;
            fSqrDistance = fS*(fA00*fS + fA01*fT + 2.0f*fB0) +
                fT*(fA01*fS + fA11*fT + 2.0f*fB1) + fC;
        }
    }
    else
    {
        float fTmp0, fTmp1, fNumer, fDenom;

        if( fS < 0.0f )  // region 2
        {
            fTmp0 = fA01 + fB0;
            fTmp1 = fA11 + fB1;
            if( fTmp1 > fTmp0 )
            {
                fNumer = fTmp1 - fTmp0;
                fDenom = fA00 - 2.0f*fA01 + fA11;
                if( fNumer >= fDenom )
                {
                    fS = 1.0f;
                    fT = 0.0f;
                    fSqrDistance = fA00 + 2.0f*fB0 + fC;
                }
                else
                {
                    fS = fNumer/fDenom;
                    fT = 1.0f - fS;
                    fSqrDistance = fS*(fA00*fS + fA01*fT + 2.0f*fB0) +
                        fT*(fA01*fS + fA11*fT + 2.0f*fB1) + fC;
                }
            }
            else
            {
                fS = 0.0f;
                if( fTmp1 <= 0.0f )
                {
                    fT = 1.0f;
                    fSqrDistance = fA11 + 2.0f*fB1 + fC;
                }
                else if( fB1 >= 0.0f )
                {
                    fT = 0.0f;
                    fSqrDistance = fC;
                }
                else
                {
                    fT = -fB1/fA11;
                    fSqrDistance = fB1*fT + fC;
                }
            }
        }
        else if( fT < 0.0f )  // region 6
        {
            fTmp0 = fA01 + fB1;
            fTmp1 = fA00 + fB0;
            if( fTmp1 > fTmp0 )
            {
                fNumer = fTmp1 - fTmp0;
                fDenom = fA00 - 2.0f*fA01 + fA11;
                if( fNumer >= fDenom )
                {
                    fT = 1.0f;
                    fS = 0.0f;
                    fSqrDistance = fA11 + 2.0f*fB1 + fC;
                }
                else
                {
                    fT = fNumer/fDenom;
                    fS = 1.0f - fT;
                    fSqrDistance = fS*(fA00*fS + fA01*fT + 2.0f*fB0) +
                        fT*(fA01*fS + fA11*fT + 2.0f*fB1) + fC;
                }
            }
            else
            {
                fT = 0.0f;
                if( fTmp1 <= 0.0f )
                {
                    fS = 1.0f;
                    fSqrDistance = fA00 + 2.0f*fB0 + fC;
                }
                else if( fB0 >= 0.0f )
                {
                    fS = 0.0f;
                    fSqrDistance = fC;
                }
                else
                {
                    fS = -fB0/fA00;
                    fSqrDistance = fB0*fS + fC;
                }
            }
        }
        else  // region 1
        {
            fNumer = fA11 + fB1 - fA01 - fB0;
            if( fNumer <= 0.0f )
            {
                fS = 0.0f;
                fT = 1.0f;
                fSqrDistance = fA11 + 2.0f*fB1 + fC;
            }
            else
            {
                fDenom = fA00 - 2.0f*fA01 + fA11;
                if( fNumer >= fDenom )
                {
                    fS = 1.0f;
                    fT = 0.0f;
                    fSqrDistance = fA00 + 2.0f*fB0 + fC;
                }
                else
                {
                    fS = fNumer/fDenom;
                    fT = 1.0f - fS;
                    fSqrDistance = fS*(fA00*fS + fA01*fT + 2.0f*fB0) +
                        fT*(fA01*fS + fA11*fT + 2.0f*fB1) + fC;
                }
            }
        }
    }

    // account for numerical round-off error
    if( fSqrDistance < 0.0f )
    {
        fSqrDistance = 0.0f;
    }

    m_ClosestPoint0 = *m_pVector;
    m_ClosestPoint1 = m_pTriangle->V[0] + fS*vec3fEdge0 + fT*vec3fEdge1;
    m_afTriangleBary[1] = fS;
    m_afTriangleBary[2] = fT;
    m_afTriangleBary[0] = 1.0f - fS - fT;

    return fSqrDistance;
}
//----------------------------------------------------------------------------
float SEDistVector3Triangle3f::Get(float fT, const SEVector3f& rVelocity0, 
    const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMVector = *m_pVector + fT*rVelocity0;
    SEVector3f vec3fMV0 = m_pTriangle->V[0] + fT*rVelocity1;
    SEVector3f vec3fMV1 = m_pTriangle->V[1] + fT*rVelocity1;
    SEVector3f vec3fMV2 = m_pTriangle->V[2] + fT*rVelocity1;
    SETriangle3f tempMTriangle(vec3fMV0, vec3fMV1, vec3fMV2);

    return SEDistVector3Triangle3f(vec3fMVector, tempMTriangle).Get();
}
//----------------------------------------------------------------------------
float SEDistVector3Triangle3f::GetSquared(float fT, const SEVector3f& 
    rVelocity0, const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMVector = *m_pVector + fT*rVelocity0;
    SEVector3f vec3fMV0 = m_pTriangle->V[0] + fT*rVelocity1;
    SEVector3f vec3fMV1 = m_pTriangle->V[1] + fT*rVelocity1;
    SEVector3f vec3fMV2 = m_pTriangle->V[2] + fT*rVelocity1;
    SETriangle3f tempMTriangle(vec3fMV0, vec3fMV1, vec3fMV2);

    return SEDistVector3Triangle3f(vec3fMVector, tempMTriangle).GetSquared();
}
//----------------------------------------------------------------------------
float SEDistVector3Triangle3f::GetTriangleBary(int i) const
{
    SE_ASSERT( 0 <= i && i < 3 );

    return m_afTriangleBary[i];
}
//----------------------------------------------------------------------------
