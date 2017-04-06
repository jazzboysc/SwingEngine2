// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_VRayRTDevice_H
#define Swing_VRayRTDevice_H

#include "SEVRayRTDeviceLIB.h"
#include "SERayTracingDevice.h"

#include "SEVRayTypes.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 03/25/2017
//----------------------------------------------------------------------------
class SE_VRAY_RT_DEVICE_API SEVRayRTDevice : public SERayTracingDevice
{
public:
    SEVRayRTDevice();
    ~SEVRayRTDevice();

private:
    void InsertRayTracingDeviceFunctions();

    void __Initialize(SERayTracingDeviceDescription* deviceDesc);
    void __Terminate();

    bool __LoadNativeScene(const char* fileName);

    SERTImageHandle* __CreateRTImage(SERayTracingDeviceImage* img);
    void __DeleteRTImage(SERayTracingDeviceImage* img);

    void __OnRenderStart(VRay::VRayRenderer& r, void*);
    void __OnImageReady(VRay::VRayRenderer& r, void*);
    void __OnRTimageUpdated(VRay::VRayRenderer& r, VRay::VRayImage* img, void*);
    void __OnDeviceClose(VRay::VRayRenderer& r, void*);
    void __OnDumpMessage(VRay::VRayRenderer& r, const char* msg, int level, void*);

private:
    VRay::VRayInit* mVRayInit;
    VRay::VRayRenderer* mVRayRenderer;

    static const SE_UInt32 gsRenderMode[RTDRM_RT_MAX - 1];
};

typedef SESmartPointer<SEVRayRTDevice> SEVRayRTDevicePtr;

}

#endif