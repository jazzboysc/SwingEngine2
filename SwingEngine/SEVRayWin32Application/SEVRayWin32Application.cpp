// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SEVRayWin32ApplicationPCH.h"
#include "SEVRayWin32Application.h"
#include "SERTGICamera.h"
#include "SETextureManager.h"
#include "SEVector2.h"
#include "SEVector3.h"

#pragma warning(disable:4996)

using namespace Swing;

//----------------------------------------------------------------------------
inline int roundInt(float number) {
    return _mm_cvtss_si32(_mm_set_ss(number));
}
//----------------------------------------------------------------------------
inline int roundInt(double number) {
    return _mm_cvtsd_si32(_mm_set_sd(number));
}
//----------------------------------------------------------------------------

const UINT_PTR TIMER2_ID = 2;
const UINT TIMEOUT = 100;

//----------------------------------------------------------------------------
class TimeInterval {
    unsigned long oldTime;
    unsigned long newTime;
    LARGE_INTEGER frequency;
    LARGE_INTEGER counter0;

public:
    TimeInterval() : oldTime(), newTime() {
        QueryPerformanceFrequency(&frequency);
        reset();
    }

    // Resets the internal initial time
    void reset() {
        QueryPerformanceCounter(&counter0);
        newTime = 0;
    }

    void update() {
        LARGE_INTEGER counter1;
        QueryPerformanceCounter(&counter1);
        oldTime = newTime;
        newTime = roundInt(((counter1.QuadPart - counter0.QuadPart)*1000.0) / frequency.QuadPart);
    }

    // Returns the elapsed time between last call to update() and object initialization or last call to reset()  (in miliseconds)
    inline unsigned long interval0() const {
        return newTime;
    }

    // Returns the elapsed time between last and previos call to update()  (in miliseconds)
    inline unsigned long interval1() const {
        return newTime - oldTime;
    }
};
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
SEVRayWin32Application::SEVRayWin32Application()
{
}
//----------------------------------------------------------------------------
SEVRayWin32Application::~SEVRayWin32Application()
{
}
//----------------------------------------------------------------------------
void SEVRayWin32Application::Initialize(SEApplicationDescription* ApplicationDesc)
{
    if (mInitialized)
    {
        return;
    }

    // Set working directory to resource folder.
    //chdir("..\\..\\Bin\\");

    mGraphicsFeature = AGF_RayTracer;
    mGPUDevice = nullptr;
    mRayTracingDevice = ApplicationDesc->RayTracingDevice;
    
    SERayTracingDeviceDescription deviceDesc;
    deviceDesc.RenderMode = RTDRM_RT_GPU_CUDA;
    deviceDesc.ImageWidth = Width;
    deviceDesc.ImageHeight = Height;
    mRayTracingDevice->Initialize(&deviceDesc);

	mMainCamera = SE_NEW SERTGICamera;

	// Call child class initialize
	this->Initialize(ApplicationDesc);
	this->mInitialized = true;
}
//----------------------------------------------------------------------------
void SEVRayWin32Application::UpdateMainCamera()
{
}
//----------------------------------------------------------------------------
void SEVRayWin32Application::ProcessInput()
{
	this->ProcessInput();
}
//----------------------------------------------------------------------------
void SEVRayWin32Application::Run()
{
}
//----------------------------------------------------------------------------
void SEVRayWin32Application::FrameFunc()
{
    // Update frame status.
    ++FrameCounter;

	this->FrameFunc();
}
//----------------------------------------------------------------------------
void SEVRayWin32Application::Terminate()
{
    mRayTracingDevice = nullptr;

	this->Terminate();
	SE_DELETE mMainCamera;
}
//----------------------------------------------------------------------------
void SEVRayWin32Application::ProcessInput(int, int, int, int)
{
}
//----------------------------------------------------------------------------