// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_VRayRTDevice_H
#define Swing_VRayRTDevice_H

#include "SEVRayRTDeviceLIB.h"
#include "SERayTracingDevice.h"

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
};

typedef SESmartPointer<SEVRayRTDevice> SEVRayRTDevicePtr;

}

#endif