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
#include "SEDistVector3Plane3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEDistVector3Plane3f::SEDistVector3Plane3f(const SEVector3f& rVector,
    const SEPlane3f& rPlane)
    :
    m_pVector(&rVector),
    m_pPlane(&rPlane)
{
}
//----------------------------------------------------------------------------
const SEVector3f& SEDistVector3Plane3f::GetVector() const
{
    return *m_pVector;
}
//----------------------------------------------------------------------------
const SEPlane3f& SEDistVector3Plane3f::GetPlane() const
{
    return *m_pPlane;
}
//----------------------------------------------------------------------------
float SEDistVector3Plane3f::Get()
{
    float fSigned = m_pPlane->Normal.Dot(*m_pVector) - m_pPlane->Constant;
    m_ClosestPoint0 = *m_pVector;
    m_ClosestPoint1 = *m_pVector - fSigned*m_pPlane->Normal;

    return SEMath<float>::FAbs(fSigned);
}
//----------------------------------------------------------------------------
float SEDistVector3Plane3f::GetSquared()
{
    float fSigned = m_pPlane->Normal.Dot(*m_pVector) - m_pPlane->Constant;
    m_ClosestPoint0 = *m_pVector;
    m_ClosestPoint1 = *m_pVector - fSigned*m_pPlane->Normal;

    return fSigned*fSigned;
}
//----------------------------------------------------------------------------
float SEDistVector3Plane3f::Get(float fT, const SEVector3f& rVelocity0,
    const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMVector = *m_pVector + fT*rVelocity0;
    float fMConstant = m_pPlane->Constant +
        fT*m_pPlane->Normal.Dot(rVelocity1);
    SEPlane3f tempMPlane(m_pPlane->Normal, fMConstant);

    return SEDistVector3Plane3f(vec3fMVector, tempMPlane).Get();
}
//----------------------------------------------------------------------------
float SEDistVector3Plane3f::GetSquared(float fT, const SEVector3f& rVelocity0, 
    const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMVector = *m_pVector + fT*rVelocity0;
    float fMConstant = m_pPlane->Constant +
        fT*m_pPlane->Normal.Dot(rVelocity1);
    SEPlane3f tempMPlane(m_pPlane->Normal, fMConstant);

    return SEDistVector3Plane3f(vec3fMVector, tempMPlane).GetSquared();
}
//----------------------------------------------------------------------------
