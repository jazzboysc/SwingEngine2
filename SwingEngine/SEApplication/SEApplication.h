// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_Application_H
#define Swing_Application_H

#include "SEApplicationLIB.h"
#include "SEGPUDeviceBase.h"
#include "SERayTracingDevice.h"
#include <string>

namespace Swing
{

enum SEApplicationGraphicsFeature
{
    AGF_Unknown    = 0x00000000,
    AGF_Rasterizer = 0x00000001,
    AGF_RayTracer  = 0x00000002,
    AGF_Both       = AGF_Rasterizer | AGF_RayTracer
};

struct SE_APPLICATION_API SEApplicationDescription
{
    SEApplicationGraphicsFeature GraphicsFeature;
    SEGPUDeviceBase* GPUDevice;
    SERayTracingDevice* RayTracingDevice;
};

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 09/14/2013
//----------------------------------------------------------------------------
class SERTGICamera;
class SE_APPLICATION_API SEApplication
{
public:
	SEApplication();
	virtual ~SEApplication();

    // Basic interfaces.
	virtual void Initialize(SEApplicationDescription* ApplicationDesc) = 0;
	virtual void Run() = 0;
	virtual void Terminate() = 0;
	virtual void ProcessInput() = 0;
	virtual void FrameFunc() = 0;
    virtual void UpdateMainCamera() = 0;
	virtual void ProcessInput(int key, int scancode, int action, int mods) = 0;

    // Window message callbacks.
    virtual void OnSizing(int newWidth, int newHeight);
    virtual void OnSize(int left, int top, int right, int bottom);
    virtual void OnWindowClose();
    virtual void OnPaintGetBitmapData(void*& bitmapPixels, void*& bitmapInfoHeader);

    // Client region updating.
    virtual void Refresh(bool updateWindowSize = true);

    // Open file dialog helper.
    virtual const char* OpenFileDialog(const char* filter, const char* title);

    // Singleton access.
	static SEApplication* GetInstance();

	int Width = 1024, Height = 768;
	std::string Title = "Default Application";
    unsigned int FrameCounter;

protected:
    SEApplicationGraphicsFeature mGraphicsFeature;
    SEGPUDeviceBasePtr mGPUDevice;
    SERayTracingDevicePtr mRayTracingDevice;

	SERTGICamera* mMainCamera;
	float mMainCameraSpeed;

	bool mInitialized = false;

	static SEApplication* mInstance;
};

}

#endif