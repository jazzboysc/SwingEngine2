// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SED3D12DevicePCH.h"
#include "SED3D12Device.h"
#include "SED3D12GPUResources.h"
#include "SwingEngineCore.h"
#include "SwingEngineRenderingEngine.h"

using namespace Swing;

//----------------------------------------------------------------------------
void SED3D12Device::__Initialize(SEGPUDeviceDescription*)
{
    // TODO:
}
//----------------------------------------------------------------------------
void SED3D12Device::__Terminate()
{
    // TODO:
}
//----------------------------------------------------------------------------
void SED3D12Device::__OnResize(unsigned int, unsigned int)
{
    // TODO:
}
//----------------------------------------------------------------------------
SEShaderHandle* SED3D12Device::__CreateShader(SEShader*)
{
    // TODO:
    return 0;
}

//----------------------------------------------------------------------------
SED3D12Device::SED3D12Device(HWND mainWindow)
{
    SE_INSERT_GPU_DEVICE_BASE_FUNC(Initialize, SED3D12Device);
    SE_INSERT_GPU_DEVICE_BASE_FUNC(Terminate, SED3D12Device);

    mMainWindow = mainWindow;
    mMsaaQuality = 0;
}
//----------------------------------------------------------------------------
SED3D12Device::~SED3D12Device()
{
}
//----------------------------------------------------------------------------