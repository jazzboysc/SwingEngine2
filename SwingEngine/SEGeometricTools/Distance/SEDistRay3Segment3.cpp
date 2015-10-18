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
#include "SEDistRay3Segment3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEDistRay3Segment3f::SEDistRay3Segment3f(const SERay3f& rRay,
    const SESegment3f& rSegment)
    :
    m_pRay(&rRay),
    m_pSegment(&rSegment)
{
}
//----------------------------------------------------------------------------
const SERay3f& SEDistRay3Segment3f::GetRay() const
{
    return *m_pRay;
}
//----------------------------------------------------------------------------
const SESegment3f& SEDistRay3Segment3f::GetSegment() const
{
    return *m_pSegment;
}
//----------------------------------------------------------------------------
float SEDistRay3Segment3f::Get()
{
    float fSqrDist = GetSquared();

    return SEMath<float>::Sqrt(fSqrDist);
}
//----------------------------------------------------------------------------
float SEDistRay3Segment3f::GetSquared()
{
    SEVector3f vec3fDiff = m_pRay->Origin - m_pSegment->Origin;
    float fA01 = -m_pRay->Direction.Dot(m_pSegment->Direction);
    float fB0 = vec3fDiff.Dot(m_pRay->Direction);
    float fB1 = -vec3fDiff.Dot(m_pSegment->Direction);
    float fC = vec3fDiff.GetSquaredLength();
    float fDet = SEMath<float>::FAbs(1.0f - fA01*fA01);
    float fS0, fS1, fSqrDist, fExtDet;

    if( fDet >= SEMath<float>::ZERO_TOLERANCE )
    {
        // 射线和线段不平行.
        fS0 = fA01*fB1 - fB0;
        fS1 = fA01*fB0 - fB1;
        fExtDet = m_pSegment->Extent*fDet;

        if( fS0 >= 0.0f )
        {
            if( fS1 >= -fExtDet )
            {
                if( fS1 <= fExtDet )  // region 0
                {
                    // 最近点是直线和射线线上点.
                    float fInvDet = 1.0f / fDet;
                    fS0 *= fInvDet;
                    fS1 *= fInvDet;
                    fSqrDist = fS0*(fS0 + fA01*fS1 + 2.0f*fB0) +
                        fS1*(fA01*fS0 + fS1 + 2.0f*fB1) + fC;
                }
                else  // region 1
                {
                    fS1 = m_pSegment->Extent;
                    fS0 = -(fA01*fS1 + fB0);
                    if( fS0 > 0.0f )
                    {
                        fSqrDist = -fS0*fS0 + fS1*(fS1 + 2.0f*fB1) + fC;
                    }
                    else
                    {
                        fS0 = 0.0f;
                        fSqrDist = fS1*(fS1 + 2.0f*fB1) + fC;
                    }
                }
            }
            else  // region 5
            {
                fS1 = -m_pSegment->Extent;
                fS0 = -(fA01*fS1 + fB0);
                if( fS0 > 0.0f )
                {
                    fSqrDist = -fS0*fS0 + fS1*(fS1 + 2.0f*fB1) + fC;
                }
                else
                {
                    fS0 = 0.0f;
                    fSqrDist = fS1*(fS1 + 2.0f*fB1) + fC;
                }
            }
        }
        else
        {
            if( fS1 <= -fExtDet )  // region 4
            {
                fS0 = -(-fA01*m_pSegment->Extent + fB0);
                if( fS0 > 0.0f )
                {
                    fS1 = -m_pSegment->Extent;
                    fSqrDist = -fS0*fS0 + fS1*(fS1 + 2.0f*fB1) + fC;
                }
                else
                {
                    fS0 = 0.0f;
                    fS1 = -fB1;
                    if( fS1 < -m_pSegment->Extent )
                    {
                        fS1 = -m_pSegment->Extent;
                    }
                    else if( fS1 > m_pSegment->Extent )
                    {
                        fS1 = m_pSegment->Extent;
                    }
                    fSqrDist = fS1*(fS1 + 2.0f*fB1) + fC;
                }
            }
            else if( fS1 <= fExtDet )  // region 3
            {
                fS0 = 0.0f;
                fS1 = -fB1;
                if( fS1 < -m_pSegment->Extent )
                {
                    fS1 = -m_pSegment->Extent;
                }
                else if( fS1 > m_pSegment->Extent )
                {
                    fS1 = m_pSegment->Extent;
                }
                fSqrDist = fS1*(fS1 + 2.0f*fB1) + fC;
            }
            else  // region 2
            {
                fS0 = -(fA01*m_pSegment->Extent + fB0);
                if( fS0 > 0.0f )
                {
                    fS1 = m_pSegment->Extent;
                    fSqrDist = -fS0*fS0 + fS1*(fS1 + 2.0f*fB1) + fC;
                }
                else
                {
                    fS0 = 0.0f;
                    fS1 = -fB1;
                    if( fS1 < -m_pSegment->Extent )
                    {
                        fS1 = -m_pSegment->Extent;
                    }
                    else if( fS1 > m_pSegment->Extent )
                    {
                        fS1 = m_pSegment->Extent;
                    }
                    fSqrDist = fS1*(fS1 + 2.0f*fB1) + fC;
                }
            }
        }
    }
    else
    {
        // 射线和线段平行.
        if( fA01 > 0.0f )
        {
            // 方向向量的方向相反.
            fS1 = -m_pSegment->Extent;
        }
        else
        {
            // 方向向量的方向相同.
            fS1 = m_pSegment->Extent;
        }

        fS0 = -(fA01*fS1 + fB0);
        if( fS0 > 0.0f )
        {
            fSqrDist = -fS0*fS0 + fS1*(fS1 + 2.0f*fB1) + fC;
        }
        else
        {
            fS0 = 0.0f;
            fSqrDist = fS1*(fS1 + 2.0f*fB1) + fC;
        }
    }

    m_ClosestPoint0 = m_pRay->Origin + fS0*m_pRay->Direction;
    m_ClosestPoint1 = m_pSegment->Origin + fS1*m_pSegment->Direction;
    m_fRayParameter = fS0;
    m_fSegmentParameter = fS1;

    return SEMath<float>::FAbs(fSqrDist);
}
//----------------------------------------------------------------------------
float SEDistRay3Segment3f::Get(float fT, const SEVector3f& rVelocity0,
    const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMOrigin0 = m_pRay->Origin + fT*rVelocity0;
    SEVector3f vec3fMOrigin1 = m_pSegment->Origin + fT*rVelocity1;
    SERay3f tempMRay(vec3fMOrigin0, m_pRay->Direction);
    SESegment3f tempMSegment(vec3fMOrigin1, m_pSegment->Direction,
        m_pSegment->Extent);

    return SEDistRay3Segment3f(tempMRay, tempMSegment).Get();
}
//----------------------------------------------------------------------------
float SEDistRay3Segment3f::GetSquared(float fT, const SEVector3f& rVelocity0, 
    const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMOrigin0 = m_pRay->Origin + fT*rVelocity0;
    SEVector3f vec3fMOrigin1 = m_pSegment->Origin + fT*rVelocity1;
    SERay3f tempMRay(vec3fMOrigin0, m_pRay->Direction);
    SESegment3f tempMSegment(vec3fMOrigin1, m_pSegment->Direction,
        m_pSegment->Extent);

    return SEDistRay3Segment3f(tempMRay, tempMSegment).GetSquared();
}
//----------------------------------------------------------------------------
float SEDistRay3Segment3f::GetRayParameter() const
{
    return m_fRayParameter;
}
//----------------------------------------------------------------------------
float SEDistRay3Segment3f::GetSegmentParameter() const
{
    return m_fSegmentParameter;
}
//----------------------------------------------------------------------------
