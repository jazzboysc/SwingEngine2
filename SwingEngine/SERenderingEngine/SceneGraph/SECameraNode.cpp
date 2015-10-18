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
#include "SECameraNode.h"

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, SECameraNode, SENode);
SE_IMPLEMENT_STREAM(SECameraNode);

//----------------------------------------------------------------------------
SECameraNode::SECameraNode(SECamera* pCamera)
    :
    m_spCamera(pCamera)
{
    if( m_spCamera )
    {
        Local.SetTranslate(m_spCamera->GetLocation());
        Local.SetRotate(SEMatrix3f(m_spCamera->GetRVector(),
            m_spCamera->GetUVector(), m_spCamera->GetDVector()));
    }
}
//----------------------------------------------------------------------------
SECameraNode::~SECameraNode()
{
}
//----------------------------------------------------------------------------
void SECameraNode::SetCamera(SECamera* pCamera)
{
    m_spCamera = pCamera;

    if( m_spCamera )
    {
        Local.SetTranslate(m_spCamera->GetLocation());
        Local.SetRotate(SEMatrix3f(m_spCamera->GetRVector(),
            m_spCamera->GetUVector(), m_spCamera->GetDVector()));

        UpdateGS();
    }
}
//----------------------------------------------------------------------------
void SECameraNode::UpdateWorldData(double dAppTime)
{
    SENode::UpdateWorldData(dAppTime);

    if( m_spCamera )
    {
        SEVector3f vec3fR, vec3fU, vec3fD;
        World.GetRotate().GetRow(0, vec3fR);
        World.GetRotate().GetRow(1, vec3fU);
        World.GetRotate().GetRow(2, vec3fD);
        m_spCamera->SetFrame(World.GetTranslate(), vec3fR, vec3fU, vec3fD);
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// name and unique id
//----------------------------------------------------------------------------
SEObject* SECameraNode::GetObjectByName(const std::string& rName)
{
    SEObject* pFound = SENode::GetObjectByName(rName);
    if( pFound )
    {
        return pFound;
    }

    if( m_spCamera )
    {
        pFound = m_spCamera->GetObjectByName(rName);
        if( pFound )
        {
            return pFound;
        }
    }

    return 0;
}
//----------------------------------------------------------------------------
void SECameraNode::GetAllObjectsByName(const std::string& rName,
    std::vector<SEObject*>& rObjects)
{
    SENode::GetAllObjectsByName(rName, rObjects);

    if( m_spCamera )
    {
        m_spCamera->GetAllObjectsByName(rName, rObjects);
    }
}
//----------------------------------------------------------------------------
SEObject* SECameraNode::GetObjectByID(unsigned int uiID)
{
    SEObject* pFound = SENode::GetObjectByID(uiID);
    if( pFound )
    {
        return pFound;
    }

    if( m_spCamera )
    {
        pFound = m_spCamera->GetObjectByID(uiID);
        if( pFound )
        {
            return pFound;
        }
    }

    return 0;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// streaming
//----------------------------------------------------------------------------
void SECameraNode::Load(SEStream& rStream, SEStream::SELink* pLink)
{
    SE_BEGIN_DEBUG_STREAM_LOAD;

    SENode::Load(rStream, pLink);

    // link data
    SEObject* pObject;
    rStream.Read(pObject);  // m_spCamera
    pLink->Add(pObject);

    SE_END_DEBUG_STREAM_LOAD(SECameraNode);
}
//----------------------------------------------------------------------------
void SECameraNode::Link(SEStream& rStream, SEStream::SELink* pLink)
{
    SENode::Link(rStream, pLink);

    SEObject* pLinkID = pLink->GetLinkID();
    m_spCamera = (SECamera*)rStream.GetFromMap(pLinkID);
}
//----------------------------------------------------------------------------
bool SECameraNode::Register(SEStream& rStream) const
{
    if( !SENode::Register(rStream) )
    {
        return false;
    }

    if( m_spCamera )
    {
        m_spCamera->Register(rStream);
    }

    return true;
}
//----------------------------------------------------------------------------
void SECameraNode::Save(SEStream& rStream) const
{
    SE_BEGIN_DEBUG_STREAM_SAVE;

    SENode::Save(rStream);

    // link data
    rStream.Write(m_spCamera);

    SE_END_DEBUG_STREAM_SAVE(SECameraNode);
}
//----------------------------------------------------------------------------
int SECameraNode::GetDiskUsed(const SEStreamVersion& rVersion) const
{
    return SENode::GetDiskUsed(rVersion) + SE_STREAM_PTRSIZE(m_spCamera);
}
//----------------------------------------------------------------------------