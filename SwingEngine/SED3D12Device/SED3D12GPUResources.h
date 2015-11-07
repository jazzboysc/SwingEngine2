// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_D3D12GPUResources_H
#define Swing_D3D12GPUResources_H

#include "SED3D12DeviceLIB.h"
#include "SEGPUResource.h"

namespace Swing
{

class SE_D3D12_DEVICE_API SED3D12ShaderHandle : public SEShaderHandle
{
public:
    SED3D12ShaderHandle();
    ~SED3D12ShaderHandle();

    void* ShaderInterface;
};

}

#endif