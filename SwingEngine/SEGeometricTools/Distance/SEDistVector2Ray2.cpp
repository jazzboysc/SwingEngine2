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
#include "SEDistVector2Ray2.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEDistVector2Ray2f::SEDistVector2Ray2f(const SEVector2f& rVector, const 
    SERay2f& rRay)
    :
    m_pVector(&rVector),
    m_pRay(&rRay)
{
}
//----------------------------------------------------------------------------
const SEVector2f& SEDistVector2Ray2f::GetVector() const
{
    return *m_pVector;
}
//----------------------------------------------------------------------------
const SERay2f& SEDistVector2Ray2f::GetRay() const
{
    return *m_pRay;
}
//----------------------------------------------------------------------------
float SEDistVector2Ray2f::Get()
{
    float fSqrDist = GetSquared();

    return SEMath<float>::Sqrt(fSqrDist);
}
//----------------------------------------------------------------------------
float SEDistVector2Ray2f::GetSquared()
{
    SEVector2f vec2fDiff = *m_pVector - m_pRay->Origin;
    float fParam = m_pRay->Direction.Dot(vec2fDiff);
    if( fParam > 0.0f )
    {
        m_ClosestPoint1 = m_pRay->Origin + fParam*m_pRay->Direction;
    }
    else
    {
        m_ClosestPoint1 = m_pRay->Origin;
    }

    m_ClosestPoint0 = *m_pVector;
    vec2fDiff = m_ClosestPoint1 - m_ClosestPoint0;

    return vec2fDiff.GetSquaredLength();
}
//----------------------------------------------------------------------------
float SEDistVector2Ray2f::Get(float fT, const SEVector2f& rVelocity0,
    const SEVector2f& rVelocity1)
{
    SEVector2f vec2fMVector = *m_pVector + fT*rVelocity0;
    SEVector2f vec2fMOrigin = m_pRay->Origin + fT*rVelocity1;
    SERay2f tempMRay(vec2fMOrigin, m_pRay->Direction);

    return SEDistVector2Ray2f(vec2fMVector, tempMRay).Get();
}
//----------------------------------------------------------------------------
float SEDistVector2Ray2f::GetSquared(float fT, const SEVector2f& rVelocity0, 
    const SEVector2f& rVelocity1)
{
    SEVector2f vec2fMVector = *m_pVector + fT*rVelocity0;
    SEVector2f vec2fMOrigin = m_pRay->Origin + fT*rVelocity1;
    SERay2f tempMRay(vec2fMOrigin, m_pRay->Direction);

    return SEDistVector2Ray2f(vec2fMVector, tempMRay).GetSquared();
}
//----------------------------------------------------------------------------
