// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SEVRayRTDevicePCH.h"
#include "SEVRayRTDevice.h"
#include "SERayTracingDeviceImage.h"

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
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(LoadNativeScene, SEVRayRTDevice);
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

    // Create VRay renderer.
    puts("Initializing Renderer...\n");
    mVRayRenderer = new VRayRenderer(options);
    mVRayRenderer->setRTImageUpdateDifference(2);
    mVRayRenderer->setRTImageUpdateTimeout(5000);
    mVRayRenderer->setKeepRTframesInCallback(true);

    // Setup renderer settings.
    Plugin settingsRTEngine = mVRayRenderer->getInstanceOrCreate("settingsRTEngine", "SettingsRTEngine");
    settingsRTEngine.setValue("undersampling", false);
    settingsRTEngine.setValue("gi_depth", 1);
    settingsRTEngine.setValue("gpu_bundle_size", 128);
    settingsRTEngine.setValue("gpu_samples_per_pixel", 1);

    // Setup callback functions.
    mVRayRenderer->setOnRenderStart<SEVRayRTDevice, &SEVRayRTDevice::__OnRenderStart>(*this, nullptr);
    mVRayRenderer->setOnImageReady<SEVRayRTDevice, &SEVRayRTDevice::__OnImageReady>(*this, nullptr);
    mVRayRenderer->setOnRTImageUpdated<SEVRayRTDevice, &SEVRayRTDevice::__OnRTimageUpdated>(*this, nullptr);
    mVRayRenderer->setOnRendererClose<SEVRayRTDevice, &SEVRayRTDevice::__OnDeviceClose>(*this, nullptr);
    mVRayRenderer->setOnDumpMessage<SEVRayRTDevice, &SEVRayRTDevice::__OnDumpMessage>(*this, nullptr);
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
bool SEVRayRTDevice::__LoadNativeScene(const char* fileName)
{
    if( mVRayRenderer )
    {
        return mVRayRenderer->load(fileName) == 0 ? true : false;
    }

    return false;
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__OnRenderStart(VRay::VRayRenderer&, void*)
{
    SERayTracingDevice::RenderStartCallback(this);
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__OnImageReady(VRay::VRayRenderer&, void*)
{
    SERayTracingDevice::ImageReadyCallback(this);
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__OnRTimageUpdated(VRay::VRayRenderer&, VRay::VRayImage* img, void*)
{
	if( img )
	{
		SERayTracingDeviceImage* rtImage = SE_NEW SERayTracingDeviceImage();
		SERayTracingDevice::RTImageUpdatedCallback(this, rtImage);
	}
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__OnDeviceClose(VRay::VRayRenderer&, void*)
{
    SERayTracingDevice::DeviceCloseCallback(this);
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__OnDumpMessage(VRay::VRayRenderer&, const char*, int, void*)
{
}
//----------------------------------------------------------------------------