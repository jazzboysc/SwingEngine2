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
#include "SEDistLine3Segment3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEDistLine3Segment3f::SEDistLine3Segment3f(const SELine3f& rLine,
    const SESegment3f& rSegment)
    :
    m_pLine(&rLine),
    m_pSegment(&rSegment)
{
}
//----------------------------------------------------------------------------
const SELine3f& SEDistLine3Segment3f::GetLine() const
{
    return *m_pLine;
}
//----------------------------------------------------------------------------
const SESegment3f& SEDistLine3Segment3f::GetSegment() const
{
    return *m_pSegment;
}
//----------------------------------------------------------------------------
float SEDistLine3Segment3f::Get()
{
    float fSqrDist = GetSquared();

    return SEMath<float>::Sqrt(fSqrDist);
}
//----------------------------------------------------------------------------
float SEDistLine3Segment3f::GetSquared()
{
    SEVector3f vec3fDiff = m_pLine->Origin - m_pSegment->Origin;
    float fA01 = -m_pLine->Direction.Dot(m_pSegment->Direction);
    float fB0 = vec3fDiff.Dot(m_pLine->Direction);
    float fC = vec3fDiff.GetSquaredLength();
    float fDet = SEMath<float>::FAbs(1.0f - fA01*fA01);
    float fB1, fS0, fS1, fSqrDist, fExtDet;

    if( fDet >= SEMath<float>::ZERO_TOLERANCE )
    {
        // 直线和线段不平行.
        fB1 = -vec3fDiff.Dot(m_pSegment->Direction);
        fS1 = fA01*fB0 - fB1;
        fExtDet = m_pSegment->Extent*fDet;

        if( fS1 >= -fExtDet )
        {
            if( fS1 <= fExtDet )
            {
                // 线上点是最近点,一个在直线上一个在线段上.
                float fInvDet = 1.0f / fDet;
                fS0 = (fA01*fB1 - fB0)*fInvDet;
                fS1 *= fInvDet;
                fSqrDist = fS0*(fS0 + fA01*fS1 + 2.0f*fB0) +
                    fS1*(fA01*fS0+fS1 + 2.0f*fB1) + fC;
            }
            else
            {
                // 线段端点e1和直线线上点是最近点.
                fS1 = m_pSegment->Extent;
                fS0 = -(fA01*fS1 + fB0);
                fSqrDist = -fS0*fS0 + fS1*(fS1 + 2.0f*fB1) + fC;
            }
        }
        else
        {
            // 线段端点e0和直线线上点是最近点.
            fS1 = -m_pSegment->Extent;
            fS0 = -(fA01*fS1 + fB0);
            fSqrDist = -fS0*fS0 + fS1*(fS1 + 2.0f*fB1) + fC;
        }
    }
    else
    {
        // 直线和线段平行,选择线段原点所在的最近点对.
        fS1 = 0.0f;
        fS0 = -fB0;
        fSqrDist = fB0*fS0 + fC;
    }

    m_ClosestPoint0 = m_pLine->Origin + fS0*m_pLine->Direction;
    m_ClosestPoint1 = m_pSegment->Origin + fS1*m_pSegment->Direction;
    m_fLineParameter = fS0;
    m_fSegmentParameter = fS1;

    return SEMath<float>::FAbs(fSqrDist);
}
//----------------------------------------------------------------------------
float SEDistLine3Segment3f::Get(float fT, const SEVector3f& rVelocity0,
    const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMOrigin0 = m_pLine->Origin + fT*rVelocity0;
    SEVector3f vec3fMOrigin1 = m_pSegment->Origin + fT*rVelocity1;
    SELine3f tempMLine(vec3fMOrigin0, m_pLine->Direction);
    SESegment3f tempMSegment(vec3fMOrigin1, m_pSegment->Direction,
        m_pSegment->Extent);

    return SEDistLine3Segment3f(tempMLine, tempMSegment).Get();
}
//----------------------------------------------------------------------------
float SEDistLine3Segment3f::GetSquared(float fT, const SEVector3f& rVelocity0, 
    const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMOrigin0 = m_pLine->Origin + fT*rVelocity0;
    SEVector3f vec3fMOrigin1 = m_pSegment->Origin + fT*rVelocity1;
    SELine3f tempMLine(vec3fMOrigin0, m_pLine->Direction);
    SESegment3f tempMSegment(vec3fMOrigin1, m_pSegment->Direction,
        m_pSegment->Extent);

    return SEDistLine3Segment3f(tempMLine, tempMSegment).GetSquared();
}
//----------------------------------------------------------------------------
float SEDistLine3Segment3f::GetLineParameter() const
{
    return m_fLineParameter;
}
//----------------------------------------------------------------------------
float SEDistLine3Segment3f::GetSegmentParameter() const
{
    return m_fSegmentParameter;
}
//----------------------------------------------------------------------------
