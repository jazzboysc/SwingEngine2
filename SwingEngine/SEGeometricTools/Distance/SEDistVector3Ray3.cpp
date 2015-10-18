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
#include "SEDistVector3Ray3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEDistVector3Ray3f::SEDistVector3Ray3f(const SEVector3f& rVector, const 
    SERay3f& rRay)
    :
    m_pVector(&rVector),
    m_pRay(&rRay)
{
}
//----------------------------------------------------------------------------
const SEVector3f& SEDistVector3Ray3f::GetVector() const
{
    return *m_pVector;
}
//----------------------------------------------------------------------------
const SERay3f& SEDistVector3Ray3f::GetRay() const
{
    return *m_pRay;
}
//----------------------------------------------------------------------------
float SEDistVector3Ray3f::Get()
{
    float fSqrDist = GetSquared();

    return SEMath<float>::Sqrt(fSqrDist);
}
//----------------------------------------------------------------------------
float SEDistVector3Ray3f::GetSquared()
{
    SEVector3f vec3fDiff = *m_pVector - m_pRay->Origin;
    m_fRayParameter = m_pRay->Direction.Dot(vec3fDiff);
    if( m_fRayParameter > 0.0f )
    {
        m_ClosestPoint1 = m_pRay->Origin + m_fRayParameter*m_pRay->Direction;
    }
    else
    {
        m_ClosestPoint1 = m_pRay->Origin;
        m_fRayParameter = 0.0f;
    }

    m_ClosestPoint0 = *m_pVector;
    vec3fDiff = m_ClosestPoint1 - m_ClosestPoint0;

    return vec3fDiff.GetSquaredLength();
}
//----------------------------------------------------------------------------
float SEDistVector3Ray3f::Get(float fT, const SEVector3f& rVelocity0,
    const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMVector = *m_pVector + fT*rVelocity0;
    SEVector3f vec3fMOrigin = m_pRay->Origin + fT*rVelocity1;
    SERay3f tempMRay(vec3fMOrigin, m_pRay->Direction);

    return SEDistVector3Ray3f(vec3fMVector, tempMRay).Get();
}
//----------------------------------------------------------------------------
float SEDistVector3Ray3f::GetSquared(float fT, const SEVector3f& rVelocity0, 
    const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMVector = *m_pVector + fT*rVelocity0;
    SEVector3f vec3fMOrigin = m_pRay->Origin + fT*rVelocity1;
    SERay3f tempMRay(vec3fMOrigin, m_pRay->Direction);

    return SEDistVector3Ray3f(vec3fMVector, tempMRay).GetSquared();
}
//----------------------------------------------------------------------------
float SEDistVector3Ray3f::GetRayParameter() const
{
    return m_fRayParameter;
}
//----------------------------------------------------------------------------
