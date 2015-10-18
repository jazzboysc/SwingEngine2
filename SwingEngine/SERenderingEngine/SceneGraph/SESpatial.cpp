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
#include "SESpatial.h"
#include "SECamera.h"
#include "SECuller.h"

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, SESpatial, SEAdvancedObject);
SE_IMPLEMENT_ABSTRACT_STREAM(SESpatial);

const char* SESpatial::ms_aacCullingMode[MAX_CULLING_MODE] =
{
    "CULL_DYNAMIC",
    "CULL_ALWAYS",
    "CULL_NEVER"
};

//----------------------------------------------------------------------------
SESpatial::SESpatial()
    :
    WorldBound(SEBoundingVolume::Create())
{
    Culling = CULL_DYNAMIC;
    WorldIsCurrent = false;
    WorldBoundIsCurrent = false;
    m_pParent = 0;
}
//----------------------------------------------------------------------------
SESpatial::~SESpatial()
{
}
//----------------------------------------------------------------------------
void SESpatial::UpdateGS(double dAppTime, bool bInitiator)
{
    // 更新controller,计算世界变换,
    // 此外,派生类SENode通过此虚函数实现AB递归遍历子树,
    // SEGeometry没有重载此函数.
    UpdateWorldData(dAppTime);
    // 从以上函数返回后,才能进行世界空间BV更新.
    UpdateWorldBound();

    if( bInitiator )
    {
        // 初始调用者有责任向上传递世界空间BV更新.
        PropagateBoundToRoot();
    }
}
//----------------------------------------------------------------------------
void SESpatial::UpdateBS()
{
    UpdateWorldBound();
    PropagateBoundToRoot();
}
//----------------------------------------------------------------------------
void SESpatial::UpdateWorldData(double dAppTime)
{
    // 更新所有当前对象的controller
    UpdateControllers(dAppTime);

    // 更新世界变换
    if( !WorldIsCurrent )
    {
        if( m_pParent )
        {
            World.Product(Local, m_pParent->World);
        }
        else
        {
            World = Local;
        }
    }
}
//----------------------------------------------------------------------------
void SESpatial::PropagateBoundToRoot()
{
    if( m_pParent )
    {
        m_pParent->UpdateWorldBound();
        m_pParent->PropagateBoundToRoot();
    }
}
//----------------------------------------------------------------------------
void SESpatial::OnGetUnculledSet(SECuller& rCuller, bool bNoCull)
{
    if( Culling == CULL_ALWAYS )
    {
        return;
    }

    if( Culling == CULL_NEVER )
    {
        bNoCull = true;
    }

    unsigned int uiSavePlaneState = rCuller.GetPlaneState();
    if( bNoCull || rCuller.IsInFrustum(WorldBound) )
    {
        // 派生类SENode实现AB递归,并将剔除过程传播给子树,
        // 派生类SEGeometry实现未被剔除的可渲染对象收集.
        GetUnculledSet(rCuller, bNoCull);
    }
    rCuller.SetPlaneState(uiSavePlaneState);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// name and unique id
//----------------------------------------------------------------------------
SEObject* SESpatial::GetObjectByName(const std::string& rName)
{
    SEObject* pFound = SEAdvancedObject::GetObjectByName(rName);
    if( pFound )
    {
        return pFound;
    }

    if( WorldBound )
    {
        pFound = WorldBound->GetObjectByName(rName);
        if( pFound )
        {
            return pFound;
        }
    }

    return 0;
}
//----------------------------------------------------------------------------
void SESpatial::GetAllObjectsByName(const std::string& rName,
    std::vector<SEObject*>& rObjects)
{
    SEAdvancedObject::GetAllObjectsByName(rName, rObjects);

    if( WorldBound )
    {
        WorldBound->GetAllObjectsByName(rName, rObjects);
    }
}
//----------------------------------------------------------------------------
SEObject* SESpatial::GetObjectByID(unsigned int uiID)
{
    SEObject* pFound = SEAdvancedObject::GetObjectByID(uiID);
    if( pFound )
    {
        return pFound;
    }

    if( WorldBound )
    {
        pFound = WorldBound->GetObjectByID(uiID);
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
void SESpatial::Load(SEStream& rStream, SEStream::SELink* pLink)
{
    SE_BEGIN_DEBUG_STREAM_LOAD;

    SEAdvancedObject::Load(rStream, pLink);

    // native data
    rStream.Read(Local);
    rStream.Read(World);
    int iTemp;
    rStream.Read(iTemp);
    Culling = (CullingMode)iTemp;

    rStream.Read(WorldIsCurrent);
    rStream.Read(WorldBoundIsCurrent);

    // link data
    SEObject* pObject;
    rStream.Read(pObject);  // WorldBound
    pLink->Add(pObject);

    SE_END_DEBUG_STREAM_LOAD(SESpatial);
}
//----------------------------------------------------------------------------
void SESpatial::Link(SEStream& rStream, SEStream::SELink* pLink)
{
    SEAdvancedObject::Link(rStream, pLink);

    SEObject* pLinkID = pLink->GetLinkID();
    WorldBound = (SEBoundingVolume*)rStream.GetFromMap(pLinkID);
}
//----------------------------------------------------------------------------
bool SESpatial::Register(SEStream& rStream) const
{
    if( !SEAdvancedObject::Register(rStream) )
    {
        return false;
    }

    if( WorldBound )
    {
        WorldBound->Register(rStream);
    }

    // m_pParent不需要在这里register,显然根据场景视图的遍历顺序,
    // 其已被遍历到并自身调用了Register函数并继续传播其全部子节点,
    // 当前节点就是其子节点中的一个.
    return true;
}
//----------------------------------------------------------------------------
void SESpatial::Save(SEStream& rStream) const
{
    SE_BEGIN_DEBUG_STREAM_SAVE;

    SEAdvancedObject::Save(rStream);

    // native data
    rStream.Write(Local);
    rStream.Write(World);
    rStream.Write((int)Culling);
    rStream.Write(WorldIsCurrent);
    rStream.Write(WorldBoundIsCurrent);

    // link data
    rStream.Write(WorldBound);

    // m_pParent不需要在这里save到文件,
    // 在load阶段,会被动态赋予.

    SE_END_DEBUG_STREAM_SAVE(SESpatial);
}
//----------------------------------------------------------------------------
int SESpatial::GetDiskUsed(const SEStreamVersion& rVersion) const
{
    int iSize = SEAdvancedObject::GetDiskUsed(rVersion) +
        SETransformation::DISK_USED +  // Local
        SETransformation::DISK_USED +  // World
        sizeof(char) + // WorldIsCurrent
        sizeof(char) + // WorldBoundIsCurrent
        SE_STREAM_PTRSIZE(WorldBound);

    iSize += sizeof(int); // Culling

    return iSize;
}
//----------------------------------------------------------------------------