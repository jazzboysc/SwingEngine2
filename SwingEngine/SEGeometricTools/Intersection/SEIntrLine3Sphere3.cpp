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
#include "SEIntrLine3Sphere3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEIntrLine3Sphere3f::SEIntrLine3Sphere3f(const SELine3f& rLine,
    const SESphere3f& rSphere)
    :
    m_pLine(&rLine),
    m_pSphere(&rSphere)
{
    m_iCount = 0;
}
//----------------------------------------------------------------------------
const SELine3f& SEIntrLine3Sphere3f::GetLine() const
{
    return *m_pLine;
}
//----------------------------------------------------------------------------
const SESphere3f& SEIntrLine3Sphere3f::GetSphere() const
{
    return *m_pSphere;
}
//----------------------------------------------------------------------------
bool SEIntrLine3Sphere3f::Test()
{
    SEVector3f vec3fDiff = m_pLine->Origin - m_pSphere->Center;
    float fA0 = vec3fDiff.Dot(vec3fDiff) - m_pSphere->Radius*m_pSphere->Radius;
    float fA1 = m_pLine->Direction.Dot(vec3fDiff);
    float fDiscr = fA1*fA1 - fA0;

    return fDiscr >= 0.0f;
}
//----------------------------------------------------------------------------
bool SEIntrLine3Sphere3f::Find ()
{
    SEVector3f vec3fDiff = m_pLine->Origin - m_pSphere->Center;
    float fA0 = vec3fDiff.Dot(vec3fDiff) - m_pSphere->Radius*m_pSphere->Radius;
    float fA1 = m_pLine->Direction.Dot(vec3fDiff);
    float fDiscr = fA1*fA1 - fA0;

    if( fDiscr < 0.0f )
    {
        m_iCount = 0;
    }
    else if( fDiscr >= SEMath<float>::ZERO_TOLERANCE )
    {
        float fRoot = SEMath<float>::Sqrt(fDiscr);
        m_afLineT[0] = -fA1 - fRoot;
        m_afLineT[1] = -fA1 + fRoot;
        m_aPoint[0] = m_pLine->Origin + m_afLineT[0]*m_pLine->Direction;
        m_aPoint[1] = m_pLine->Origin + m_afLineT[1]*m_pLine->Direction;
        m_iCount = 2;
    }
    else
    {
        m_afLineT[0] = -fA1;
        m_aPoint[0] = m_pLine->Origin + m_afLineT[0]*m_pLine->Direction;
        m_iCount = 1;
    }

    return m_iCount > 0;
}
//----------------------------------------------------------------------------
int SEIntrLine3Sphere3f::GetCount() const
{
    return m_iCount;
}
//----------------------------------------------------------------------------
const SEVector3f& SEIntrLine3Sphere3f::GetPoint(int i) const
{
    SE_ASSERT( 0 <= i && i < m_iCount );

    return m_aPoint[i];
}
//----------------------------------------------------------------------------
float SEIntrLine3Sphere3f::GetLineT(int i) const
{
    SE_ASSERT( 0 <= i && i < m_iCount );

    return m_afLineT[i];
}
//----------------------------------------------------------------------------
