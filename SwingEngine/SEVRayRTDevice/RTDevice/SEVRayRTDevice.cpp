// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SEVRayRTDevicePCH.h"
#include "SEVRayRTDevice.h"

#define VRAY_RUNTIME_LOAD_PRIMARY
#include "vraysdk.hpp"

using namespace Swing;
using namespace VRay;

//----------------------------------------------------------------------------
void SEVRayRTDevice::InsertRayTracingDeviceFunctions()
{
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(Initialize, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(Terminate, SEVRayRTDevice);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
SEVRayRTDevice::SEVRayRTDevice()
    :
    mVRayInit(nullptr),
    mVRayRenderer(nullptr)
{
    InsertRayTracingDeviceFunctions();
}
//----------------------------------------------------------------------------
SEVRayRTDevice::~SEVRayRTDevice()
{
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__Initialize(SERayTracingDeviceDescription* deviceDesc)
{
    if( mVRayInit )
    {
        return;
    }

    puts("Initializing VRay...");
    // Initializing without VRay framebuffer support.
    mVRayInit = new VRayInit("VRaySDKLibrary", false);
    int stopHere = 0;
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__Terminate()
{
    if( mVRayInit )
    {
        delete mVRayInit;
        mVRayInit = nullptr;
    }
}
//----------------------------------------------------------------------------