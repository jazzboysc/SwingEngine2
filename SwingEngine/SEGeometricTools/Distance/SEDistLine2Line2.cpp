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
#include "SEDistLine2Line2.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEDistLine2Line2f::SEDistLine2Line2f(const SELine2f& rLine0, const SELine2f& 
    rLine1)
    :
    m_pLine0(&rLine0),
    m_pLine1(&rLine1)
{
}
//----------------------------------------------------------------------------
const SELine2f& SEDistLine2Line2f::GetLine0() const
{
    return *m_pLine0;
}
//----------------------------------------------------------------------------
const SELine2f& SEDistLine2Line2f::GetLine1() const
{
    return *m_pLine1;
}
//----------------------------------------------------------------------------
float SEDistLine2Line2f::Get()
{
    float fSqrDist = GetSquared();

    return SEMath<float>::Sqrt(fSqrDist);
}
//----------------------------------------------------------------------------
float SEDistLine2Line2f::GetSquared()
{
    SEVector2f vec2fDiff = m_pLine0->Origin - m_pLine1->Origin;
    float fA01 = -m_pLine0->Direction.Dot(m_pLine1->Direction);
    float fB0 = vec2fDiff.Dot(m_pLine0->Direction);
    float fC = vec2fDiff.GetSquaredLength();
    float fDet = SEMath<float>::FAbs(1.0f - fA01*fA01);
    float fB1, fS0, fS1, fSqrDist;

    if( fDet >= SEMath<float>::ZERO_TOLERANCE )
    {
        // 直线不平行.
        fB1 = -vec2fDiff.Dot(m_pLine1->Direction);
        float fInvDet = 1.0f / fDet;
        fS0 = (fA01*fB1 - fB0)*fInvDet;
        fS1 = (fA01*fB0 - fB1)*fInvDet;
        fSqrDist = 0.0f;
    }
    else
    {
        // 直线平行,选择任意一对最近顶点.
        fS0 = -fB0;
        fS1 = 0.0f;
        fSqrDist = fB0*fS0 + fC;
    }

    m_ClosestPoint0 = m_pLine0->Origin + fS0*m_pLine0->Direction;
    m_ClosestPoint1 = m_pLine1->Origin + fS1*m_pLine1->Direction;

    return SEMath<float>::FAbs(fSqrDist);
}
//----------------------------------------------------------------------------
float SEDistLine2Line2f::Get(float fT, const SEVector2f& rVelocity0,
    const SEVector2f& rVelocity1)
{
    SEVector2f vec2fMOrigin0 = m_pLine0->Origin + fT*rVelocity0;
    SEVector2f vec2fMOrigin1 = m_pLine1->Origin + fT*rVelocity1;
    SELine2f tempMLine0(vec2fMOrigin0, m_pLine0->Direction);
    SELine2f tempMLine1(vec2fMOrigin1, m_pLine1->Direction);

    return SEDistLine2Line2f(tempMLine0, tempMLine1).Get();
}
//----------------------------------------------------------------------------
float SEDistLine2Line2f::GetSquared(float fT,
    const SEVector2f& rVelocity0, const SEVector2f& rVelocity1)
{
    SEVector2f vec2fMOrigin0 = m_pLine0->Origin + fT*rVelocity0;
    SEVector2f vec2fMOrigin1 = m_pLine1->Origin + fT*rVelocity1;
    SELine2f tempMLine0(vec2fMOrigin0, m_pLine0->Direction);
    SELine2f tempMLine1(vec2fMOrigin1, m_pLine1->Direction);

    return SEDistLine2Line2f(tempMLine0, tempMLine1).GetSquared();
}
//----------------------------------------------------------------------------
