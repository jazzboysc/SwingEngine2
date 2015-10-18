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
#include "SEDistVector3Line3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEDistVector3Line3f::SEDistVector3Line3f(const SEVector3f& rVector,
    const SELine3f& rLine)
    :
    m_pVector(&rVector),
    m_pLine(&rLine)
{
}
//----------------------------------------------------------------------------
const SEVector3f& SEDistVector3Line3f::GetVector() const
{
    return *m_pVector;
}
//----------------------------------------------------------------------------
const SELine3f& SEDistVector3Line3f::GetLine() const
{
    return *m_pLine;
}
//----------------------------------------------------------------------------
float SEDistVector3Line3f::Get()
{
    float fSqrDist = GetSquared();

    return SEMath<float>::Sqrt(fSqrDist);
}
//----------------------------------------------------------------------------
float SEDistVector3Line3f::GetSquared()
{
    SEVector3f vec3fDiff = *m_pVector - m_pLine->Origin;
    m_fLineParameter = m_pLine->Direction.Dot(vec3fDiff);
    m_ClosestPoint0 = *m_pVector;
    m_ClosestPoint1 = m_pLine->Origin + m_fLineParameter*m_pLine->Direction;
    vec3fDiff = m_ClosestPoint1 - m_ClosestPoint0;

    return vec3fDiff.GetSquaredLength();
}
//----------------------------------------------------------------------------
float SEDistVector3Line3f::Get(float fT, const SEVector3f& rVelocity0,
    const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMVector = *m_pVector + fT*rVelocity0;
    SEVector3f vec3fMOrigin = m_pLine->Origin + fT*rVelocity1;
    SELine3f tempMLine(vec3fMOrigin, m_pLine->Direction);

    return SEDistVector3Line3f(vec3fMVector, tempMLine).Get();
}
//----------------------------------------------------------------------------
float SEDistVector3Line3f::GetSquared(float fT, const SEVector3f& rVelocity0, 
    const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMVector = *m_pVector + fT*rVelocity0;
    SEVector3f vec3fMOrigin = m_pLine->Origin + fT*rVelocity1;
    SELine3f tempMLine(vec3fMOrigin, m_pLine->Direction);

    return SEDistVector3Line3f(vec3fMVector, tempMLine).GetSquared();
}
//----------------------------------------------------------------------------
float SEDistVector3Line3f::GetLineParameter() const
{
    return m_fLineParameter;
}
//----------------------------------------------------------------------------
