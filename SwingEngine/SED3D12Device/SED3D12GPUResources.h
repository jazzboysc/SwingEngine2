// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_D3D12GPUResources_H
#define Swing_D3D12GPUResources_H

#include "SED3D12DeviceLIB.h"
#include "SED3D12Common.h"
#include "SEGPUResource.h"

namespace Swing
{

struct SE_D3D12_DEVICE_API SED3D12ShaderHandle : public SEShaderHandle
{
    ComPtr<ID3DBlob> mShaderHandle;
};

}

#endif