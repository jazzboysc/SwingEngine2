// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017
//
// This part of Swing Engine belongs to an abstraction layer of the next 
// generation GPU graphics and compute API. Currently being constructed to 
// support Microsoft's DirectX 12 and Apple's Metal.

#ifndef Swing_ThinGPUDeviceChild_H
#define Swing_ThinGPUDeviceChild_H

#include "SERenderingEngineLIB.h"
#include "SEReferencable.h"

namespace Swing
{

class SEGPUDeviceBase;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/09/2015
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEGPUDeviceBaseChild : public SEReferencable
{
public:
    SEGPUDeviceBaseChild();
    virtual ~SEGPUDeviceBaseChild();

    SEGPUDeviceBase* DeviceBase;
};

typedef SESmartPointer<SEGPUDeviceBaseChild> SEThinGPUDeviceChildPtr;

struct SE_RENDERING_ENGINE_API SECommandQueueHandle : public SEGPUDeviceBaseChild
{
};

struct SE_RENDERING_ENGINE_API SECommandAllocatorHandle : public SEGPUDeviceBaseChild
{
};

struct SE_RENDERING_ENGINE_API SECommandListHandle : public SEGPUDeviceBaseChild
{
};

struct SE_RENDERING_ENGINE_API SEDescriptorHandle : public SEGPUDeviceBaseChild
{
};

struct SE_RENDERING_ENGINE_API SEDescriptorHeapHandle : public SEGPUDeviceBaseChild
{
};

struct SE_RENDERING_ENGINE_API SERootSignatureHandle : public SEGPUDeviceBaseChild
{
};

struct SE_RENDERING_ENGINE_API SERenderPassInfoHandle : public SEGPUDeviceBaseChild
{
};

}

#endif