// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SEVRayRTDevicePCH.h"
#include "SEVRayRTDevice.h"
#include "SERayTracingDeviceImage.h"
#include "SERayTracingDeviceBitmap.h"

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
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(CreateRTImage, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(DeleteRTImage, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(CreateRTBitmap, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(DeleteRTBitmap, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(GetImageSize, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(SetImageSize, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(Render, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(GetImage, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(RTBitmapGetPixels, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(RTBitmapGetInfoHeader, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(RTImageSaveToBmpFile, SEVRayRTDevice);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
SEVRayRTDevice::SEVRayRTDevice()
    :
    mVRayInit(nullptr),
    mVRayRenderer(nullptr),
    mImageOpMutex(nullptr)
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

    mImageOpMutex = SEMutex::Create();

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

    if( mVRayRenderer )
    {
        delete mVRayRenderer;
        mVRayRenderer = nullptr;
    }

    if( mImageOpMutex )
    {
        SEMutex::Destroy(mImageOpMutex);
        mImageOpMutex = nullptr;
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
SERTImageHandle* SEVRayRTDevice::__CreateRTImage(SERayTracingDeviceImage*)
{
    SERTImageHandle* imageHandle = SE_NEW SEVRayRTImageHandle();
    imageHandle->RTDevice = this;
    return imageHandle;
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__DeleteRTImage(SERayTracingDeviceImage* img)
{
    if( img )
    {
        SEVRayRTImageHandle* imageHandle = (SEVRayRTImageHandle*)img->GetImageHandle();
        delete imageHandle->mImage;
        imageHandle->mImage = nullptr;
    }
}
//----------------------------------------------------------------------------
SERTBitmapHandle* SEVRayRTDevice::__CreateRTBitmap(SERayTracingDeviceBitmap*, SERayTracingDeviceImage* img, int width, int height)
{
    SEMutexLock lock(*mImageOpMutex);

    SEVRayRTBitmapHandle* bitmapHandle = SE_NEW SEVRayRTBitmapHandle();
    bitmapHandle->RTDevice = this;

    SEVRayRTImageHandle* imageHandle = (SEVRayRTImageHandle*)img->GetImageHandle();
    if( imageHandle && imageHandle->mImage )
    {
        VRay::VRayImage* image = imageHandle->mImage->downscale(width, height);
        VRay::LocalVRayImage tempImage(image);
        bitmapHandle->mBitmap = tempImage->getBmp();
    }
    return bitmapHandle;
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__DeleteRTBitmap(SERayTracingDeviceBitmap* bmp)
{
    if( bmp )
    {
        SEVRayRTBitmapHandle* bitmapHandle = (SEVRayRTBitmapHandle*)bmp->GetBitmapHandle();
        delete bitmapHandle->mBitmap;
        bitmapHandle->mBitmap = nullptr;
    }
}
//----------------------------------------------------------------------------
bool SEVRayRTDevice::__GetImageSize(int& width, int& height)
{
    if( mVRayRenderer )
    {
        return mVRayRenderer->getImageSize(width, height);
    }

    return false;
}
//----------------------------------------------------------------------------
bool SEVRayRTDevice::__SetImageSize(int width, int height)
{
    if( mVRayRenderer )
    {
        return mVRayRenderer->setImageSize(width, height);
    }

    return false;
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__Render()
{
    if( mVRayRenderer )
    {
        mVRayRenderer->start();
    }
}
//----------------------------------------------------------------------------
SERayTracingDeviceImage* SEVRayRTDevice::__GetImage()
{
    SERayTracingDeviceImage* res = nullptr;
    if( mVRayRenderer )
    {
        VRay::VRayImage* img = mVRayRenderer->getImage();
        if( img )
        {
            res = SE_NEW SERayTracingDeviceImage();
            SEVRayRTImageHandle* imageHandle = (SEVRayRTImageHandle*)this->CreateRTImage(res);
            imageHandle->mImage = img;
            res->SetImageHandle(imageHandle);
            img->getSize(res->Width, res->Height);
        }

    }

    return res;
}
//----------------------------------------------------------------------------
void* SEVRayRTDevice::__RTBitmapGetPixels(SERayTracingDeviceBitmap* bmp)
{
    void* res = nullptr;
    if( bmp )
    {
        SEVRayRTBitmapHandle* bitmapHandle = (SEVRayRTBitmapHandle*)bmp->GetBitmapHandle();
        if( bitmapHandle && bitmapHandle->mBitmap )
        {
            res = bitmapHandle->mBitmap->getPixels();
        }
    }

    return res;
}
//----------------------------------------------------------------------------
void* SEVRayRTDevice::__RTBitmapGetInfoHeader(SERayTracingDeviceBitmap* bmp)
{
    void* res = nullptr;
    if( bmp )
    {
        SEVRayRTBitmapHandle* bitmapHandle = (SEVRayRTBitmapHandle*)bmp->GetBitmapHandle();
        if( bitmapHandle )
        {
            res = reinterpret_cast<void*>(static_cast<VRay::BitmapInfoHeader*>(bitmapHandle->mBitmap));
        }
    }

    return res;
}
//----------------------------------------------------------------------------
bool SEVRayRTDevice::__RTImageSaveToBmpFile(SERayTracingDeviceImage* img, const std::string& fileName, bool preserveAlpha, bool invertChannels)
{
    bool res = false;
    if( img )
    {
        SEVRayRTImageHandle* imageHandle = (SEVRayRTImageHandle*)img->GetImageHandle();
        if( imageHandle )
        {
            res = imageHandle->mImage->saveToBmpFile(fileName, preserveAlpha, invertChannels);
        }
    }

    return res;
}
//----------------------------------------------------------------------------



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
        SEVRayRTImageHandle* imageHandle = (SEVRayRTImageHandle*)this->CreateRTImage(rtImage);
        imageHandle->mImage = img;
        rtImage->SetImageHandle(imageHandle);
        img->getSize(rtImage->Width, rtImage->Height);

		SERayTracingDevice::RTImageUpdatedCallback(this, rtImage);
	}
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__OnDeviceClose(VRay::VRayRenderer&, void*)
{
    SERayTracingDevice::DeviceCloseCallback(this);
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__OnDumpMessage(VRay::VRayRenderer&, const char* msg, int level, void*)
{
    SERayTracingDevice::DumpMessageCallback(this, msg, level);
}
//----------------------------------------------------------------------------