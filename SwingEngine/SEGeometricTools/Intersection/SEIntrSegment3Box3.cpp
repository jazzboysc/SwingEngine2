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
#include "SEIntrSegment3Box3.h"
#include "SEIntrLine3Box3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEIntrSegment3Box3f::SEIntrSegment3Box3f(const SESegment3f& rSegment,
    const SEBox3f& rBox, bool bSolid)
    :
    m_pSegment(&rSegment),
    m_pBox(&rBox)
{
    m_bSolid = bSolid;
}
//----------------------------------------------------------------------------
const SESegment3f& SEIntrSegment3Box3f::GetSegment() const
{
    return *m_pSegment;
}
//----------------------------------------------------------------------------
const SEBox3f& SEIntrSegment3Box3f::GetBox() const
{
    return *m_pBox;
}
//----------------------------------------------------------------------------
bool SEIntrSegment3Box3f::Test()
{
    float afAWdU[3], afADdU[3], afAWxDdU[3], fRhs;

    SEVector3f vec3fDiff = m_pSegment->Origin - m_pBox->Center;

    afAWdU[0] = SEMath<float>::FAbs(m_pSegment->Direction.Dot(
        m_pBox->Axis[0]));
    afADdU[0] = SEMath<float>::FAbs(vec3fDiff.Dot(m_pBox->Axis[0]));
    fRhs = m_pBox->Extent[0] + m_pSegment->Extent*afAWdU[0];
    if( afADdU[0] > fRhs )
    {
        return false;
    }

    afAWdU[1] = SEMath<float>::FAbs(m_pSegment->Direction.Dot(
        m_pBox->Axis[1]));
    afADdU[1] = SEMath<float>::FAbs(vec3fDiff.Dot(m_pBox->Axis[1]));
    fRhs = m_pBox->Extent[1] + m_pSegment->Extent*afAWdU[1];
    if( afADdU[1] > fRhs )
    {
        return false;
    }

    afAWdU[2] = SEMath<float>::FAbs(m_pSegment->Direction.Dot(
        m_pBox->Axis[2]));
    afADdU[2] = SEMath<float>::FAbs(vec3fDiff.Dot(m_pBox->Axis[2]));
    fRhs = m_pBox->Extent[2] + m_pSegment->Extent*afAWdU[2];
    if( afADdU[2] > fRhs )
    {
        return false;
    }

    SEVector3f vec3fWxD = m_pSegment->Direction.Cross(vec3fDiff);

    afAWxDdU[0] = SEMath<float>::FAbs(vec3fWxD.Dot(m_pBox->Axis[0]));
    fRhs = m_pBox->Extent[1]*afAWdU[2] + m_pBox->Extent[2]*afAWdU[1];
    if( afAWxDdU[0] > fRhs )
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
bool SEIntrSegment3Box3f::Find()
{
    float fT0 = -m_pSegment->Extent, fT1 = m_pSegment->Extent;

    return SEIntrLine3Box3f::DoClipping(fT0, fT1, m_pSegment->Origin,
        m_pSegment->Direction, *m_pBox, m_bSolid, m_iCount, m_aPoint,
        m_iIntersectionType);
}
//----------------------------------------------------------------------------
int SEIntrSegment3Box3f::GetCount() const
{
    return m_iCount;
}
//----------------------------------------------------------------------------
const SEVector3f& SEIntrSegment3Box3f::GetPoint(int i) const
{
    SE_ASSERT( 0 <= i && i < m_iCount );

    return m_aPoint[i];
}
//----------------------------------------------------------------------------
