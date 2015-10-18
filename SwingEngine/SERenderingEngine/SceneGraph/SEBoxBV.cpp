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

#include "SERenderingEnginePCH.h"
#include "SEBoxBV.h"
#include "SEContBox3.h"
#include "SEIntrRay3Box3.h"
#include "SEIntrBox3Box3.h"

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, SEBoxBV, SEBoundingVolume);
SE_IMPLEMENT_STREAM(SEBoxBV);
SE_IMPLEMENT_DEFAULT_NAME_ID(SEBoxBV, SEBoundingVolume);

//----------------------------------------------------------------------------
SEBoxBV::SEBoxBV()
    :
    m_Box(SEVector3f::ZERO, SEVector3f::UNIT_X, SEVector3f::UNIT_Y, 
        SEVector3f::UNIT_Z, 1.0f, 1.0f, 1.0f)
{
}
//----------------------------------------------------------------------------
SEBoxBV::SEBoxBV(const SEBox3f& rBox)
    :
    m_Box(rBox)
{
}
//----------------------------------------------------------------------------
SEBoxBV::~SEBoxBV()
{
}
//----------------------------------------------------------------------------
void SEBoxBV::SetRadius(float fRadius)
{
    m_Box.Extent[0] = fRadius;
    m_Box.Extent[1] = fRadius;
    m_Box.Extent[2] = fRadius;
}
//----------------------------------------------------------------------------
float SEBoxBV::GetRadius() const
{
    float fRadius = m_Box.Extent[0];
    if( fRadius < m_Box.Extent[1] )
    {
        fRadius = m_Box.Extent[1];
    }
    if( fRadius < m_Box.Extent[2] )
    {
        fRadius = m_Box.Extent[2];
    }

    return fRadius;
}
//----------------------------------------------------------------------------
void SEBoxBV::ComputeFromData(const SEVector3fArray* pVertices)
{
    if( pVertices )
    {
        int iVCount = pVertices->GetCount();
        const SEVector3f* aVertex = pVertices->GetData();
        m_Box = ContOBBf(iVCount, aVertex);
    }
}
//----------------------------------------------------------------------------
void SEBoxBV::TransformBy(const SETransformation& rTransform,
    SEBoundingVolume* pResult)
{
    SEBox3f& rTarget = ((SEBoxBV*)pResult)->m_Box;
    rTransform.ApplyForwardToPoint(m_Box.Center, rTarget.Center);
    for( int i = 0; i < 3; ++i )
    {
        rTarget.Axis[i] = m_Box.Axis[i] * rTransform.GetRotate();
        rTarget.Extent[i] = rTransform.GetNorm() * m_Box.Extent[i];
    }
}
//----------------------------------------------------------------------------
int SEBoxBV::OnWhichSide(const SEPlane3f& rPlane) const
{
    float fProjCenter = rPlane.Normal.Dot(m_Box.Center) - rPlane.Constant;
    float fAbs0 = SEMathf::FAbs(rPlane.Normal.Dot(m_Box.Axis[0]));
    float fAbs1 = SEMathf::FAbs(rPlane.Normal.Dot(m_Box.Axis[1]));
    float fAbs2 = SEMathf::FAbs(rPlane.Normal.Dot(m_Box.Axis[2]));
    float fProjRadius = m_Box.Extent[0]*fAbs0 + m_Box.Extent[1]*fAbs1 +
        m_Box.Extent[2]*fAbs2;

    if( fProjCenter - fProjRadius >= 0.0f )
    {
        return +1;
    }

    if( fProjCenter + fProjRadius <= 0.0f )
    {
        return -1;
    }

    return 0;
}
//----------------------------------------------------------------------------
bool SEBoxBV::TestIntersection(const SERay3f& rRay) const
{
    return SEIntrRay3Box3f(rRay, m_Box).Test();
}
//----------------------------------------------------------------------------
bool SEBoxBV::TestIntersection(const SEBoundingVolume* pInput) const
{
    return SEIntrBox3Box3f(m_Box, ((SEBoxBV*)pInput)->m_Box).Test();
}
//----------------------------------------------------------------------------
void SEBoxBV::CopyFrom(const SEBoundingVolume* pInput)
{
    m_Box = ((SEBoxBV*)pInput)->m_Box;
}
//----------------------------------------------------------------------------
void SEBoxBV::GrowToContain(const SEBoundingVolume* pInput)
{
    m_Box = MergeBoxesf(m_Box, ((SEBoxBV*)pInput)->m_Box);
}
//----------------------------------------------------------------------------
bool SEBoxBV::Contains(const SEVector3f& rPoint) const
{
    return IsInBoxf(rPoint, m_Box);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// streaming
//----------------------------------------------------------------------------
void SEBoxBV::Load(SEStream& rStream, SEStream::SELink* pLink)
{
    SE_BEGIN_DEBUG_STREAM_LOAD;

    SEBoundingVolume::Load(rStream, pLink);

    // native data
    rStream.Read(m_Box.Center);
    rStream.Read(m_Box.Axis[0]);
    rStream.Read(m_Box.Axis[1]);
    rStream.Read(m_Box.Axis[2]);
    rStream.Read(m_Box.Extent[0]);
    rStream.Read(m_Box.Extent[1]);
    rStream.Read(m_Box.Extent[2]);

    SE_END_DEBUG_STREAM_LOAD(SEBoxBV);
}
//----------------------------------------------------------------------------
void SEBoxBV::Link(SEStream& rStream, SEStream::SELink* pLink)
{
    SEBoundingVolume::Link(rStream, pLink);
}
//----------------------------------------------------------------------------
bool SEBoxBV::Register(SEStream& rStream) const
{
    return SEBoundingVolume::Register(rStream);
}
//----------------------------------------------------------------------------
void SEBoxBV::Save(SEStream& rStream) const
{
    SE_BEGIN_DEBUG_STREAM_SAVE;

    SEBoundingVolume::Save(rStream);

    // native data
    rStream.Write(m_Box.Center);
    rStream.Write(m_Box.Axis[0]);
    rStream.Write(m_Box.Axis[1]);
    rStream.Write(m_Box.Axis[2]);
    rStream.Write(m_Box.Extent[0]);
    rStream.Write(m_Box.Extent[1]);
    rStream.Write(m_Box.Extent[2]);

    SE_END_DEBUG_STREAM_SAVE(SEBoxBV);
}
//----------------------------------------------------------------------------
int SEBoxBV::GetDiskUsed(const SEStreamVersion& rVersion) const
{
    return SEBoundingVolume::GetDiskUsed(rVersion) +
        sizeof(m_Box);
}
//----------------------------------------------------------------------------