// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_VRayRTDevice_H
#define Swing_VRayRTDevice_H

#include "SEVRayRTDeviceLIB.h"
#include "SERayTracingDevice.h"
#include "SEMutex.h"

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
    // Internal interface implementation.
    void InsertRayTracingDeviceFunctions();

    void __Initialize(SERayTracingDeviceDescription* deviceDesc);
    void __Terminate();

    bool __LoadNativeScene(const char* fileName);

    SERTImageHandle* __CreateRTImage(SERayTracingDeviceImage* img);
    void __DeleteRTImage(SERayTracingDeviceImage* img);

    SERTBitmapHandle* __CreateRTBitmap(SERayTracingDeviceBitmap* bmp, SERayTracingDeviceImage* img, int width, int height);
    void __DeleteRTBitmap(SERayTracingDeviceBitmap* bmp);

    bool __GetImageSize(int& width, int& height);
    bool __SetImageSize(int width, int height);

    void __Render();

    SERayTracingDeviceImage* __GetImage();

    void* __RTBitmapGetPixels(SERayTracingDeviceBitmap* bmp);
    void* __RTBitmapGetInfoHeader(SERayTracingDeviceBitmap* bmp);

    bool __RTImageSaveToBmpFile(SERayTracingDeviceImage* img, const std::string& fileName, bool preserveAlpha, bool invertChannels);

private:
    // Renderer callback functions.
    void __OnRenderStart(VRay::VRayRenderer& r, void*);
    void __OnImageReady(VRay::VRayRenderer& r, void*);
    void __OnRTimageUpdated(VRay::VRayRenderer& r, VRay::VRayImage* img, void*);
    void __OnDeviceClose(VRay::VRayRenderer& r, void*);
    void __OnDumpMessage(VRay::VRayRenderer& r, const char* msg, int level, void*);

private:
    VRay::VRayInit* mVRayInit;
    VRay::VRayRenderer* mVRayRenderer;

    SEMutex* mImageOpMutex;

    static const SE_Int32 gsRenderMode[RTDRM_MAX - 1];
};

typedef SESmartPointer<SEVRayRTDevice> SEVRayRTDevicePtr;

}

#endif