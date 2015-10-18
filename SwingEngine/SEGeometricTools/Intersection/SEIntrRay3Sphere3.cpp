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
#include "SEIntrRay3Sphere3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEIntrRay3Sphere3f::SEIntrRay3Sphere3f(const SERay3f& rRay, const SESphere3f& 
    rSphere)
    :
    m_pRay(&rRay),
    m_pSphere(&rSphere)
{
    m_iCount = 0;
}
//----------------------------------------------------------------------------
const SERay3f& SEIntrRay3Sphere3f::GetRay() const
{
    return *m_pRay;
}
//----------------------------------------------------------------------------
const SESphere3f& SEIntrRay3Sphere3f::GetSphere() const
{
    return *m_pSphere;
}
//----------------------------------------------------------------------------
bool SEIntrRay3Sphere3f::Test()
{
    SEVector3f vec3fDiff = m_pRay->Origin - m_pSphere->Center;
    float fA0 = vec3fDiff.Dot(vec3fDiff) - m_pSphere->Radius*m_pSphere->Radius;
    if( fA0 <= 0.0f )
    {
        // P点在球内.
        return true;
    }
    // else: P点在球外.

    float fA1 = m_pRay->Direction.Dot(vec3fDiff);
    if( fA1 >= 0.0f )
    {
        return false;
    }

    // 如果判别式非负,则二次曲线有实根.
    return fA1*fA1 >= fA0;
}
//----------------------------------------------------------------------------
bool SEIntrRay3Sphere3f::Find()
{
    SEVector3f vec3fDiff = m_pRay->Origin - m_pSphere->Center;
    float fA0 = vec3fDiff.Dot(vec3fDiff) - m_pSphere->Radius*m_pSphere->Radius;
    float fA1, fDiscr, fRoot;
    if( fA0 <= 0.0f )
    {
        // P点在球内.
        m_iCount = 1;
        fA1 = m_pRay->Direction.Dot(vec3fDiff);
        fDiscr = fA1*fA1 - fA0;
        fRoot = SEMathf::Sqrt(fDiscr);
        m_afRayT[0] = -fA1 + fRoot;
        m_aPoint[0] = m_pRay->Origin + m_afRayT[0]*m_pRay->Direction;

        return true;
    }
    // else: P点在球外.

    fA1 = m_pRay->Direction.Dot(vec3fDiff);
    if( fA1 >= 0.0f )
    {
        m_iCount = 0;

        return false;
    }

    fDiscr = fA1*fA1 - fA0;
    if( fDiscr < 0.0f )
    {
        m_iCount = 0;
    }
    else if( fDiscr >= SEMathf::ZERO_TOLERANCE )
    {
        fRoot = SEMathf::Sqrt(fDiscr);
        m_afRayT[0] = -fA1 - fRoot;
        m_afRayT[1] = -fA1 + fRoot;
        m_aPoint[0] = m_pRay->Origin + m_afRayT[0]*m_pRay->Direction;
        m_aPoint[1] = m_pRay->Origin + m_afRayT[1]*m_pRay->Direction;
        m_iCount = 2;
    }
    else
    {
        m_afRayT[0] = -fA1;
        m_aPoint[0] = m_pRay->Origin + m_afRayT[0]*m_pRay->Direction;
        m_iCount = 1;
    }

    return m_iCount > 0;
}
//----------------------------------------------------------------------------
int SEIntrRay3Sphere3f::GetCount() const
{
    return m_iCount;
}
//----------------------------------------------------------------------------
const SEVector3f& SEIntrRay3Sphere3f::GetPoint(int i) const
{
    SE_ASSERT( 0 <= i && i < m_iCount );

    return m_aPoint[i];
}
//----------------------------------------------------------------------------
float SEIntrRay3Sphere3f::GetRayT(int i) const
{
    SE_ASSERT( 0 <= i && i < m_iCount );

    return m_afRayT[i];
}
//----------------------------------------------------------------------------