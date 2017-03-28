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