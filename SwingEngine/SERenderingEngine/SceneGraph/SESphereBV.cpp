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
#include "SESphereBV.h"
#include "SEContSphere3.h"
#include "SEIntrRay3Sphere3.h"
#include "SEIntrSphere3Sphere3.h"

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, SESphereBV, SEBoundingVolume);
SE_IMPLEMENT_STREAM(SESphereBV);
SE_IMPLEMENT_DEFAULT_NAME_ID(SESphereBV, SEBoundingVolume);

//----------------------------------------------------------------------------
// SEBoundingVolume的工厂函数默认实现为创建SESphereBV对象,
// 可以删除当前函数实现,使用另一个SEBoundingVolume派生类型.
//----------------------------------------------------------------------------
SEBoundingVolume* SEBoundingVolume::Create()
{
    return SE_NEW SESphereBV;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
SESphereBV::SESphereBV()
    :
    m_Sphere(SEVector3f::ZERO, 0.0f)
{
}
//----------------------------------------------------------------------------
SESphereBV::SESphereBV(const SESphere3f& rSphere)
    :
    m_Sphere(rSphere)
{
}
//----------------------------------------------------------------------------
void SESphereBV::ComputeFromData(const SEVector3fArray* pVertices)
{
    if( pVertices )
    {
        int iVCount = pVertices->GetCount();
        const SEVector3f* aVertex = pVertices->GetData();
        m_Sphere = ContSphereAveragef(iVCount, aVertex);
    }
}
//----------------------------------------------------------------------------
void SESphereBV::TransformBy(const SETransformation& rTransform,
    SEBoundingVolume* pResult)
{
    SESphere3f& rTarget = ((SESphereBV*)pResult)->m_Sphere;
    rTransform.ApplyForwardToPoint(m_Sphere.Center, rTarget.Center);
    rTarget.Radius = rTransform.GetNorm() * m_Sphere.Radius;
}
//----------------------------------------------------------------------------
int SESphereBV::OnWhichSide(const SEPlane3f& rPlane) const
{
    float fDistance = rPlane.GetDistance(m_Sphere.Center);

    if( fDistance <= -m_Sphere.Radius )
    {
        return -1;
    }

    if( fDistance >= m_Sphere.Radius )
    {
        return +1;
    }

    return 0;
}
//----------------------------------------------------------------------------
bool SESphereBV::TestIntersection(const SERay3f& rRay) const
{
    return SEIntrRay3Sphere3f(rRay, m_Sphere).Test();
}
//----------------------------------------------------------------------------
bool SESphereBV::TestIntersection(const SEBoundingVolume* pInput) const
{
    return SEIntrSphere3Sphere3f(m_Sphere,
        ((SESphereBV*)pInput)->m_Sphere).Test();
}
//----------------------------------------------------------------------------
void SESphereBV::CopyFrom(const SEBoundingVolume* pInput)
{
    m_Sphere = ((SESphereBV*)pInput)->m_Sphere;
}
//----------------------------------------------------------------------------
void SESphereBV::GrowToContain(const SEBoundingVolume* pInput)
{
    m_Sphere = MergeSpheresf(m_Sphere, ((SESphereBV*)pInput)->m_Sphere);
}
//----------------------------------------------------------------------------
bool SESphereBV::Contains(const SEVector3f& rPoint) const
{
    return IsInSpheref(rPoint, m_Sphere);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// streaming
//----------------------------------------------------------------------------
void SESphereBV::Load(SEStream& rStream, SEStream::SELink* pLink)
{
    SE_BEGIN_DEBUG_STREAM_LOAD;

    SEBoundingVolume::Load(rStream, pLink);

    // native data
    rStream.Read(m_Sphere.Center);
    rStream.Read(m_Sphere.Radius);

    SE_END_DEBUG_STREAM_LOAD(SESphereBV);
}
//----------------------------------------------------------------------------
void SESphereBV::Link(SEStream& rStream, SEStream::SELink* pLink)
{
    SEBoundingVolume::Link(rStream, pLink);
}
//----------------------------------------------------------------------------
bool SESphereBV::Register(SEStream& rStream) const
{
    return SEBoundingVolume::Register(rStream);
}
//----------------------------------------------------------------------------
void SESphereBV::Save(SEStream& rStream) const
{
    SE_BEGIN_DEBUG_STREAM_SAVE;

    SEBoundingVolume::Save(rStream);

    // native data
    rStream.Write(m_Sphere.Center);
    rStream.Write(m_Sphere.Radius);

    SE_END_DEBUG_STREAM_SAVE(SESphereBV);
}
//----------------------------------------------------------------------------
int SESphereBV::GetDiskUsed(const SEStreamVersion& rVersion) const
{
    return SEBoundingVolume::GetDiskUsed(rVersion) +
        sizeof(m_Sphere);
}
//----------------------------------------------------------------------------