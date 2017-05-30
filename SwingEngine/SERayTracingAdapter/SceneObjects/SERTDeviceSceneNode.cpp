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
void SERTDeviceSceneNode::SetTransform(SEMatrix3f* srcMatrix, SEVector3f* srcOffset)
{
    if( mSceneNodeHandle )
    {
        mSceneNodeHandle->RTDevice->SceneNodeSetTransform(this, srcMatrix, srcOffset);
    }
}
//----------------------------------------------------------------------------
void SERTDeviceSceneNode::SetGeometry(SERTDeviceGeometry* geometry)
{
    if( mSceneNodeHandle )
    {
        mSceneNodeHandle->RTDevice->SceneNodeSetGeometry(this, geometry);
    }
}
//----------------------------------------------------------------------------
void SERTDeviceSceneNode::SetMaterial(SERTDeviceMaterial* material)
{
    if( mSceneNodeHandle )
    {
        mSceneNodeHandle->RTDevice->SceneNodeSetMaterial(this, material);
    }
}
//----------------------------------------------------------------------------