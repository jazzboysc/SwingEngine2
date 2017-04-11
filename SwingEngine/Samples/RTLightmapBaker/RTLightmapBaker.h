#pragma once

#include "SEVRayWin32Application.h"
#include "SERayTracingDeviceImage.h"
#include "SERayTracingDeviceBitmap.h"
#include "SEMutex.h"

namespace Swing
{

class RTLightmapBaker : public SEVRayWin32Application
{
public:
    RTLightmapBaker(int width = 1024, int height = 768);
    ~RTLightmapBaker();

    // Override SEVRayWin32Application interface.
	void Initialize(SEApplicationDescription* ApplicationDesc) override;
	void ProcessInput() override;
	void FrameFunc() override;
	void Terminate() override;

    // Override SEApplication interface.
    void OnSizing(int newWidth, int newHeight) override;
    void OnSize(int left, int top, int right, int bottom) override;
    void OnWindowClose() override;
    void OnPaintGetBitmapData(void*& bitmapPixels, void*& bitmapInfoHeader) override;

    // Implement device delegate.
    void OnRenderStart(SERayTracingDevice& rtDevice, void* userObj);
    void OnImageReady(SERayTracingDevice& rtDevice, void* userObj);
    void OnDeviceClose(SERayTracingDevice& rtDevice, void* userObj);
    void OnRTImageUpdated(SERayTracingDevice& rtDevice, SERayTracingDeviceImage* img, void* userObj);
    void OnDumpMessage(SERayTracingDevice& rtDevice, const char* msg, int level, void* userObj);

private:
    void UpdateImage(SERayTracingDeviceImage* image);

    unsigned int mFrameNumber;
    bool mFinished;
    bool mStop;
    SEMutex* mImgMutex;
    SEMutex* mBmpMutex;
    SERayTracingDeviceImage* mImg;
    SERayTracingDeviceBitmap* mBmp;
};

}