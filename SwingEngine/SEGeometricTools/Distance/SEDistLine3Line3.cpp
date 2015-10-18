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
#include "SEDistLine3Line3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEDistLine3Line3f::SEDistLine3Line3f(const SELine3f& rLine0, const SELine3f& 
    rLine1)
    :
    m_pLine0(&rLine0),
    m_pLine1(&rLine1)
{
}
//----------------------------------------------------------------------------
const SELine3f& SEDistLine3Line3f::GetLine0() const
{
    return *m_pLine0;
}
//----------------------------------------------------------------------------
const SELine3f& SEDistLine3Line3f::GetLine1() const
{
    return *m_pLine1;
}
//----------------------------------------------------------------------------
float SEDistLine3Line3f::Get()
{
    float fSqrDist = GetSquared();

    return SEMath<float>::Sqrt(fSqrDist);
}
//----------------------------------------------------------------------------
float SEDistLine3Line3f::GetSquared()
{
    SEVector3f vec3fDiff = m_pLine0->Origin - m_pLine1->Origin;
    float fA01 = -m_pLine0->Direction.Dot(m_pLine1->Direction);
    float fB0 = vec3fDiff.Dot(m_pLine0->Direction);
    float fC = vec3fDiff.GetSquaredLength();
    float fDet = SEMath<float>::FAbs(1.0f - fA01*fA01);
    float fB1, fS0, fS1, fSqrDist;

    if( fDet >= SEMath<float>::ZERO_TOLERANCE )
    {
        // 直线不平行.
        fB1 = -vec3fDiff.Dot(m_pLine1->Direction);
        float fInvDet = 1.0f / fDet;
        fS0 = (fA01*fB1 - fB0)*fInvDet;
        fS1 = (fA01*fB0 - fB1)*fInvDet;
        fSqrDist = fS0*(fS0 + fA01*fS1 + 2.0f*fB0) +
            fS1*(fA01*fS0 + fS1 + 2.0f*fB1) + fC;
    }
    else
    {
        // 直线平行,选择任意一对最近点.
        fS0 = -fB0;
        fS1 = 0.0f;
        fSqrDist = fB0*fS0 + fC;
    }

    m_ClosestPoint0 = m_pLine0->Origin + fS0*m_pLine0->Direction;
    m_ClosestPoint1 = m_pLine1->Origin + fS1*m_pLine1->Direction;
    m_fLine0Parameter = fS0;
    m_fLine1Parameter = fS1;

    return SEMath<float>::FAbs(fSqrDist);
}
//----------------------------------------------------------------------------
float SEDistLine3Line3f::Get(float fT, const SEVector3f& rVelocity0,
    const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMOrigin0 = m_pLine0->Origin + fT*rVelocity0;
    SEVector3f vec3fMOrigin1 = m_pLine1->Origin + fT*rVelocity1;
    SELine3f tempMLine0(vec3fMOrigin0, m_pLine0->Direction);
    SELine3f tempMLine1(vec3fMOrigin1, m_pLine1->Direction);

    return SEDistLine3Line3f(tempMLine0, tempMLine1).Get();
}
//----------------------------------------------------------------------------
float SEDistLine3Line3f::GetSquared(float fT, const SEVector3f& rVelocity0, 
    const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMOrigin0 = m_pLine0->Origin + fT*rVelocity0;
    SEVector3f vec3fMOrigin1 = m_pLine1->Origin + fT*rVelocity1;
    SELine3f tempMLine0(vec3fMOrigin0, m_pLine0->Direction);
    SELine3f tempMLine1(vec3fMOrigin1, m_pLine1->Direction);

    return SEDistLine3Line3f(tempMLine0, tempMLine1).GetSquared();
}
//----------------------------------------------------------------------------
float SEDistLine3Line3f::GetLine0Parameter() const
{
    return m_fLine0Parameter;
}
//----------------------------------------------------------------------------
float SEDistLine3Line3f::GetLine1Parameter() const
{
    return m_fLine1Parameter;
}
//----------------------------------------------------------------------------
