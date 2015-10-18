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
#include "SEIntrRay3Box3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEIntrRay3Box3f::SEIntrRay3Box3f(const SERay3f& rRay, const SEBox3f& rBox)
    :
    m_pRay(&rRay),
    m_pBox(&rBox)
{
}
//----------------------------------------------------------------------------
const SERay3f& SEIntrRay3Box3f::GetRay() const
{
    return *m_pRay;
}
//----------------------------------------------------------------------------
const SEBox3f& SEIntrRay3Box3f::GetBox() const
{
    return *m_pBox;
}
//----------------------------------------------------------------------------
bool SEIntrRay3Box3f::Test()
{
    float afWdU[3], afAWdU[3], afDdU[3], afADdU[3], afAWxDdU[3], fRhs;

    SEVector3f vec3fDiff = m_pRay->Origin - m_pBox->Center;

    afWdU[0] = m_pRay->Direction.Dot(m_pBox->Axis[0]);
    afAWdU[0] = SEMath<float>::FAbs(afWdU[0]);
    afDdU[0] = vec3fDiff.Dot(m_pBox->Axis[0]);
    afADdU[0] = SEMath<float>::FAbs(afDdU[0]);
    if( afADdU[0] > m_pBox->Extent[0] && afDdU[0]*afWdU[0] >= 0.0f )
    {
        return false;
    }

    afWdU[1] = m_pRay->Direction.Dot(m_pBox->Axis[1]);
    afAWdU[1] = SEMath<float>::FAbs(afWdU[1]);
    afDdU[1] = vec3fDiff.Dot(m_pBox->Axis[1]);
    afADdU[1] = SEMath<float>::FAbs(afDdU[1]);
    if( afADdU[1] > m_pBox->Extent[1] && afDdU[1]*afWdU[1] >= 0.0f )
    {
        return false;
    }

    afWdU[2] = m_pRay->Direction.Dot(m_pBox->Axis[2]);
    afAWdU[2] = SEMath<float>::FAbs(afWdU[2]);
    afDdU[2] = vec3fDiff.Dot(m_pBox->Axis[2]);
    afADdU[2] = SEMath<float>::FAbs(afDdU[2]);
    if( afADdU[2] > m_pBox->Extent[2] && afDdU[2]*afWdU[2] >= 0.0f )
    {
        return false;
    }

    SEVector3f vec3fWxD = m_pRay->Direction.Cross(vec3fDiff);

    afAWxDdU[0] = SEMath<float>::FAbs(vec3fWxD.Dot(m_pBox->Axis[0]));
    fRhs = m_pBox->Extent[1]*afAWdU[2] + m_pBox->Extent[2]*afAWdU[1];
    if( afAWxDdU[0] > fRhs)
    {
        return false;
    }

    afAWxDdU[1] = SEMath<float>::FAbs(vec3fWxD.Dot(m_pBox->Axis[1]));
    fRhs = m_pBox->Extent[0]*afAWdU[2] + m_pBox->Extent[2]*afAWdU[0];
    if( afAWxDdU[1] > fRhs )
    {
        return false;
    }

    afAWxDdU[2] = SEMath<float>::FAbs(vec3fWxD.Dot(m_pBox->Axis[2]));
    fRhs = m_pBox->Extent[0]*afAWdU[1] + m_pBox->Extent[1]*afAWdU[0];
    if( afAWxDdU[2] > fRhs )
    {
        return false;
    }

    return true;
}
//----------------------------------------------------------------------------
bool SEIntrRay3Box3f::Find()
{
    float fT0 = 0.0f, fT1 = SEMath<float>::MAX_REAL;

    return SEIntrLine3Box3f::DoClipping(fT0, fT1, m_pRay->Origin,
        m_pRay->Direction, *m_pBox, true, m_iCount, m_aPoint,
        m_iIntersectionType);
}
//----------------------------------------------------------------------------
int SEIntrRay3Box3f::GetCount() const
{
    return m_iCount;
}
//----------------------------------------------------------------------------
const SEVector3f& SEIntrRay3Box3f::GetPoint(int i) const
{
    SE_ASSERT( 0 <= i && i < m_iCount );

    return m_aPoint[i];
}
//----------------------------------------------------------------------------
