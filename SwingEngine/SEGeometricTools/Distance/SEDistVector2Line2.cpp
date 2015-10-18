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
#include "SEDistVector2Line2.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEDistVector2Line2f::SEDistVector2Line2f(const SEVector2f& rVector,
    const SELine2f& rLine)
    :
    m_pVector(&rVector),
    m_pLine(&rLine)
{
}
//----------------------------------------------------------------------------
const SEVector2f& SEDistVector2Line2f::GetVector() const
{
    return *m_pVector;
}
//----------------------------------------------------------------------------
const SELine2f& SEDistVector2Line2f::GetLine() const
{
    return *m_pLine;
}
//----------------------------------------------------------------------------
float SEDistVector2Line2f::Get()
{
    float fSqrDist = GetSquared();

    return SEMath<float>::Sqrt(fSqrDist);
}
//----------------------------------------------------------------------------
float SEDistVector2Line2f::GetSquared()
{
    SEVector2f vec2fDiff = *m_pVector - m_pLine->Origin;
    float fParam = m_pLine->Direction.Dot(vec2fDiff);
    m_ClosestPoint0 = *m_pVector;
    m_ClosestPoint1 = m_pLine->Origin + fParam*m_pLine->Direction;
    vec2fDiff = m_ClosestPoint1 - m_ClosestPoint0;

    return vec2fDiff.GetSquaredLength();
}
//----------------------------------------------------------------------------
float SEDistVector2Line2f::Get(float fT, const SEVector2f& rVelocity0,
    const SEVector2f& rVelocity1)
{
    SEVector2f vec2fMVector = *m_pVector + fT*rVelocity0;
    SEVector2f vec2fMOrigin = m_pLine->Origin + fT*rVelocity1;
    SELine2f tempMLine(vec2fMOrigin, m_pLine->Direction);

    return SEDistVector2Line2f(vec2fMVector, tempMLine).Get();
}
//----------------------------------------------------------------------------
float SEDistVector2Line2f::GetSquared(float fT, const SEVector2f& rVelocity0, 
    const SEVector2f& rVelocity1)
{
    SEVector2f vec2fMVector = *m_pVector + fT*rVelocity0;
    SEVector2f vec2fMOrigin = m_pLine->Origin + fT*rVelocity1;
    SELine2f tempMLine(vec2fMOrigin, m_pLine->Direction);

    return SEDistVector2Line2f(vec2fMVector, tempMLine).GetSquared();
}
//----------------------------------------------------------------------------
