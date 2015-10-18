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
#include "SEDistLine2Segment2.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEDistLine2Segment2f::SEDistLine2Segment2f(const SELine2f& rLine,
    const SESegment2f& rSegment)
    :
    m_pLine(&rLine),
    m_pSegment(&rSegment)
{
}
//----------------------------------------------------------------------------
const SELine2f& SEDistLine2Segment2f::GetLine() const
{
    return *m_pLine;
}
//----------------------------------------------------------------------------
const SESegment2f& SEDistLine2Segment2f::GetSegment() const
{
    return *m_pSegment;
}
//----------------------------------------------------------------------------
float SEDistLine2Segment2f::Get()
{
    float fSqrDist = GetSquared();

    return SEMath<float>::Sqrt(fSqrDist);
}
//----------------------------------------------------------------------------
float SEDistLine2Segment2f::GetSquared()
{
    SEVector2f vec2fDiff = m_pLine->Origin - m_pSegment->Origin;
    float fA01 = -m_pLine->Direction.Dot(m_pSegment->Direction);
    float fB0 = vec2fDiff.Dot(m_pLine->Direction);
    float fC = vec2fDiff.GetSquaredLength();
    float fDet = SEMath<float>::FAbs(1.0f - fA01*fA01);
    float fB1, fS0, fS1, fSqrDist, fExtDet;

    if( fDet >= SEMath<float>::ZERO_TOLERANCE )
    {
        // 直线和线段不平行.
        fB1 = -vec2fDiff.Dot(m_pSegment->Direction);
        fS1 = fA01*fB0 - fB1;
        fExtDet = m_pSegment->Extent * fDet;

        if( fS1 >= -fExtDet )
        {
            if( fS1 <= fExtDet )
            {
                // 两个线上点是最近点,一个在直线上一个在线段上.
                float fInvDet = 1.0f / fDet;
                fS0 = (fA01*fB1 - fB0)*fInvDet;
                fS1 *= fInvDet;
                fSqrDist = 0.0f;
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

    return SEMath<float>::FAbs(fSqrDist);
}
//----------------------------------------------------------------------------
float SEDistLine2Segment2f::Get(float fT, const SEVector2f& rVelocity0,
    const SEVector2f& rVelocity1)
{
    SEVector2f vec2fMOrigin0 = m_pLine->Origin + fT*rVelocity0;
    SEVector2f vec2fMOrigin1 = m_pSegment->Origin + fT*rVelocity1;
    SELine2f tempMLine(vec2fMOrigin0, m_pLine->Direction);
    SESegment2f tempMSegment(vec2fMOrigin1, m_pSegment->Direction,
        m_pSegment->Extent);

    return SEDistLine2Segment2f(tempMLine, tempMSegment).Get();
}
//----------------------------------------------------------------------------
float SEDistLine2Segment2f::GetSquared(float fT,
    const SEVector2f& rVelocity0, const SEVector2f& rVelocity1)
{
    SEVector2f vec2fMOrigin0 = m_pLine->Origin + fT*rVelocity0;
    SEVector2f vec2fMOrigin1 = m_pSegment->Origin + fT*rVelocity1;
    SELine2f tempMLine(vec2fMOrigin0, m_pLine->Direction);
    SESegment2f tempMSegment(vec2fMOrigin1, m_pSegment->Direction,
        m_pSegment->Extent);

    return SEDistLine2Segment2f(tempMLine, tempMSegment).GetSquared();
}
//----------------------------------------------------------------------------
