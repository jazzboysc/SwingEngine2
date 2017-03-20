// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_D3D11GPUResources_H
#define Swing_D3D11GPUResources_H

#include "SED3D11DeviceLIB.h"
#include "SEGPUResourceHandle.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/15/2014
//----------------------------------------------------------------------------
class SE_D3D11_DEVICE_API SED3D11ShaderHandle : public SEShaderHandle
{
public:
    SED3D11ShaderHandle();
    ~SED3D11ShaderHandle();

    void* ShaderInterface;
};

}

#endif