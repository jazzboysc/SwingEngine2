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
#include "SEDistVector2Segment2.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEDistVector2Segment2f::SEDistVector2Segment2f(const SEVector2f& rVector,
    const SESegment2f& rSegment)
    :
    m_pVector(&rVector),
    m_pSegment(&rSegment)
{
}
//----------------------------------------------------------------------------
const SEVector2f& SEDistVector2Segment2f::GetVector() const
{
    return *m_pVector;
}
//----------------------------------------------------------------------------
const SESegment2f& SEDistVector2Segment2f::GetSegment() const
{
    return *m_pSegment;
}
//----------------------------------------------------------------------------
float SEDistVector2Segment2f::Get()
{
    float fSqrDist = GetSquared();

    return SEMath<float>::Sqrt(fSqrDist);
}
//----------------------------------------------------------------------------
float SEDistVector2Segment2f::GetSquared()
{
    SEVector2f vec2fDiff = *m_pVector - m_pSegment->Origin;
    float fParam = m_pSegment->Direction.Dot(vec2fDiff);

    if( -m_pSegment->Extent < fParam )
    {
        if( fParam < m_pSegment->Extent )
        {
            m_ClosestPoint1 = m_pSegment->Origin +
                fParam*m_pSegment->Direction;
        }
        else
        {
            m_ClosestPoint1 = m_pSegment->Origin +
                m_pSegment->Extent*m_pSegment->Direction;
        }
    }
    else
    {
        m_ClosestPoint1 = m_pSegment->Origin -
            m_pSegment->Extent*m_pSegment->Direction;
    }

    m_ClosestPoint0 = *m_pVector;
    vec2fDiff = m_ClosestPoint1 - m_ClosestPoint0;

    return vec2fDiff.GetSquaredLength();
}
//----------------------------------------------------------------------------
float SEDistVector2Segment2f::Get(float fT, const SEVector2f& rVelocity0, 
    const SEVector2f& rVelocity1)
{
    SEVector2f vec2fMVector = *m_pVector + fT*rVelocity0;
    SEVector2f vec2fMOrigin = m_pSegment->Origin + fT*rVelocity1;
    SESegment2f tempMSegment(vec2fMOrigin, m_pSegment->Direction,
        m_pSegment->Extent);

    return SEDistVector2Segment2f(vec2fMVector, tempMSegment).Get();
}
//----------------------------------------------------------------------------
float SEDistVector2Segment2f::GetSquared(float fT, const SEVector2f& 
    rVelocity0, const SEVector2f& rVelocity1)
{
    SEVector2f vec2fMVector = *m_pVector + fT*rVelocity0;
    SEVector2f vec2fMOrigin = m_pSegment->Origin + fT*rVelocity1;
    SESegment2f tempMSegment(vec2fMOrigin, m_pSegment->Direction,
        m_pSegment->Extent);

    return SEDistVector2Segment2f(vec2fMVector, tempMSegment).GetSquared();
}
//----------------------------------------------------------------------------
