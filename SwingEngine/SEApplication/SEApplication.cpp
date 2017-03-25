// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SEApplicationPCH.h"
#include "SEApplication.h"
#include "SERTGICamera.h"

using namespace Swing;

SEApplication* SEApplication::mInstance = nullptr;

//----------------------------------------------------------------------------
SEApplication::SEApplication()
	:
    FrameCounter(0),
	mMainCameraSpeed(5.0f),
    mGraphicsFeature(AGF_Unknown),
    mGPUDevice(nullptr),
    mRayTracingDevice(nullptr),
    mMainCamera(nullptr)
{
    SE_ASSERT( mInstance == nullptr );
    mInstance = this;
}
//----------------------------------------------------------------------------
SEApplication::~SEApplication()
{
    mGPUDevice = nullptr;
    mRayTracingDevice = nullptr;
}
//----------------------------------------------------------------------------
SEApplication* SEApplication::GetInstance()
{
    return mInstance;
}
//----------------------------------------------------------------------------