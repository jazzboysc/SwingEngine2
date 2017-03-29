// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SEVRayRTDevicePCH.h"
#include "SEVRayRTDevice.h"

#ifdef _WIN32
#pragma warning(disable:4189)
#endif

#define VRAY_RUNTIME_LOAD_PRIMARY
#include "vraysdk.hpp"

using namespace Swing;
using namespace VRay;

const SE_UInt32 SEVRayRTDevice::gsRenderMode[RTDRM_RT_MAX - 1] =
{
    RendererOptions::RENDER_MODE_RT_CPU,
    RendererOptions::RENDER_MODE_RT_GPU_CUDA,
    RendererOptions::RENDER_MODE_RT_GPU_OPENCL
};

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
    puts("Done.");

    if (deviceDesc->RenderMode == RTDRM_Unknown)
    {
        puts("Unknown Render Mode.");
        return;
    }

    RendererOptions options;
    options.imageWidth = deviceDesc->ImageWidth;
    options.imageHeight = deviceDesc->ImageHeight;
    options.rtNoiseThreshold = 0;
    options.renderMode = (RendererOptions::RenderMode)gsRenderMode[(int)deviceDesc->RenderMode - 1];

    puts("Initializing Renderer...\n");
    mVRayRenderer = new VRayRenderer(options);
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__Terminate()
{
    if( mVRayInit )
    {
        puts("Terminating VRay...");
        delete mVRayInit;
        mVRayInit = nullptr;
        puts("Done.");
    }
}
//----------------------------------------------------------------------------