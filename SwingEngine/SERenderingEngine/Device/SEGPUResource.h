// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_GPUResource_H
#define Swing_GPUResource_H

#include "SERenderingEngineLIB.h"
#include "SEReferencable.h"

namespace Swing
{

class SEGPUDeviceBase;
class SEGPUDevice;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/12/2015
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEGPUResourceBase : public SEReferencable
{
public:
    SEGPUResourceBase();
    virtual ~SEGPUResourceBase();

    SEGPUDeviceBase* Device;
};

struct SE_RENDERING_ENGINE_API SEShaderHandle : public SEGPUResourceBase
{
};

struct SE_RENDERING_ENGINE_API SEPassInfoHandle : public SEGPUResourceBase
{
};

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/29/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEGPUResource : public SEReferencable
{
public:
    SEGPUResource();
    virtual ~SEGPUResource();

    SEGPUDevice* Device;
};

typedef SESmartPointer<SEGPUResource> SEGPUResourcePtr;

struct SE_RENDERING_ENGINE_API SEShaderProgramHandle : public SEGPUResource
{
};

struct SE_RENDERING_ENGINE_API SEShaderUniformHandle : public SEGPUResource
{
};

struct SE_RENDERING_ENGINE_API SETextureHandle : public SEGPUResource
{
};

struct SE_RENDERING_ENGINE_API SEFBOHandle : public SEGPUResource
{
};

struct SE_RENDERING_ENGINE_API SEBufferHandle : public SEGPUResource
{
};

struct SE_RENDERING_ENGINE_API SEBufferViewHandle : public SEGPUResource
{
};

struct SE_RENDERING_ENGINE_API SEGPUTimerHandle : public SEGPUResource
{
};

}

#endif