// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_VRayRTDeviceResource_H
#define Swing_VRayRTDeviceResource_H

#include "SEVRayRTDeviceLIB.h"
#include "SERayTracingDeviceResource.h"

#include "SEVRayTypes.h"

namespace Swing
{

//----------------------------------------------------------------------------
struct SE_VRAY_RT_DEVICE_API SEVRayRTImageHandle : public SERTImageHandle
{
	VRay::VRayImage* mImage;

	SEVRayRTImageHandle();
	~SEVRayRTImageHandle();
};
//----------------------------------------------------------------------------
struct SE_VRAY_RT_DEVICE_API SEVRayRTBitmapHandle : public SERTBitmapHandle
{
    VRay::Bmp* mBitmap;

    SEVRayRTBitmapHandle();
    ~SEVRayRTBitmapHandle();
};
//----------------------------------------------------------------------------
struct SE_VRAY_RT_DEVICE_API SEVRayRTDeviceCameraHandle : public SERTDeviceCameraHandle
{
    VRay::Plugins::RenderView* mRenderView;

    SEVRayRTDeviceCameraHandle();
    ~SEVRayRTDeviceCameraHandle();
};
//----------------------------------------------------------------------------
struct SE_VRAY_RT_DEVICE_API SEVRayRTDeviceLightRectangleHandle : public SERTDeviceLightRectangleHandle
{
    VRay::Plugins::LightRectangle* mLightRectangle;

    SEVRayRTDeviceLightRectangleHandle();
    ~SEVRayRTDeviceLightRectangleHandle();
};
//----------------------------------------------------------------------------
struct SE_VRAY_RT_DEVICE_API SEVRayRTDeviceSkyLightHandle : public SERTDeviceSkyLightHandle
{
    VRay::Plugins::LightDome* mLightDome;

    SEVRayRTDeviceSkyLightHandle();
    ~SEVRayRTDeviceSkyLightHandle();
};
//----------------------------------------------------------------------------
struct SE_VRAY_RT_DEVICE_API SEVRayRTDeviceStaticMeshHandle : public SERTDeviceStaticMeshHandle
{
    VRay::Plugins::GeomStaticMesh* mStaticMesh;

    SEVRayRTDeviceStaticMeshHandle();
    ~SEVRayRTDeviceStaticMeshHandle();
};
//----------------------------------------------------------------------------
struct SE_VRAY_RT_DEVICE_API SEVRayRTDeviceSceneNodeHandle : public SERTDeviceSceneNodeHandle
{
    VRay::Plugins::Node* mNode;

    SEVRayRTDeviceSceneNodeHandle();
    ~SEVRayRTDeviceSceneNodeHandle();
};
//----------------------------------------------------------------------------
struct SE_VRAY_RT_DEVICE_API SEVRayRTDeviceMaterialHandle : public SERTDeviceMaterialHandle
{
    VRay::Plugins::BRDFDiffuse* mBRDF;
    VRay::Plugins::MtlSingleBRDF* mMaterial;

    SEVRayRTDeviceMaterialHandle();
    ~SEVRayRTDeviceMaterialHandle();
};
//----------------------------------------------------------------------------
struct SE_VRAY_RT_DEVICE_API SEVRayRTDeviceBakeViewHandle : public SERTDeviceBakeViewHandle
{
    VRay::Plugins::BakeView* mBakeView;
    VRay::Plugins::UVWGenChannel* mUVWGenChannel;

    SEVRayRTDeviceBakeViewHandle();
    ~SEVRayRTDeviceBakeViewHandle();
};

}

#endif