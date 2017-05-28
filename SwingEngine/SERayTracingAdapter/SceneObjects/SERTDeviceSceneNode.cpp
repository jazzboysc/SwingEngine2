// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERayTracingAdapterPCH.h"
#include "SERTDeviceSceneNode.h"

using namespace Swing;

//----------------------------------------------------------------------------
SERTDeviceSceneNode::SERTDeviceSceneNode()
    :
    mSceneNodeHandle(nullptr)
{
}
//----------------------------------------------------------------------------
SERTDeviceSceneNode::~SERTDeviceSceneNode()
{
    if( mSceneNodeHandle )
    {
        mSceneNodeHandle->RTDevice->DeleteSceneNode(this);
        SE_DELETE mSceneNodeHandle;
        mSceneNodeHandle = nullptr;
    }
}
//----------------------------------------------------------------------------
void SERTDeviceSceneNode::CreateDeviceResource(SERayTracingDevice& device, SEISpatialInfo* spatialInfo)
{
    if( !mSceneNodeHandle )
    {
        mSceneNodeHandle = device.CreateSceneNode(this, spatialInfo);
    }
}
//----------------------------------------------------------------------------
SERTDeviceSceneNodeHandle* SERTDeviceSceneNode::GetSceneNodeHandle()
{
    return mSceneNodeHandle;
}
//----------------------------------------------------------------------------
void SERTDeviceSceneNode::SetTransform(SEMatrix3f* srcRotation, SEVector3f* srcLocation)
{
    if( !mSceneNodeHandle )
    {
        mSceneNodeHandle->RTDevice->SceneNodeSetTransform(this, srcRotation, srcLocation);
    }
}
//----------------------------------------------------------------------------
void SERTDeviceSceneNode::SetGeometry(SERTDeviceGeometry* geometry)
{
    if( !mSceneNodeHandle )
    {
        mSceneNodeHandle->RTDevice->SceneNodeSetGeometry(this, geometry);
    }
}
//----------------------------------------------------------------------------