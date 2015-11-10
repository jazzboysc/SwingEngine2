// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015
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

class SEThinGPUDevice;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/09/2015
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEThinGPUDeviceChild : public SEReferencable
{
public:
    SEThinGPUDeviceChild();
    virtual ~SEThinGPUDeviceChild();

    SEThinGPUDevice* ThinDevice;
};

typedef SESmartPointer<SEThinGPUDeviceChild> SEThinGPUDeviceChildPtr;

struct SE_RENDERING_ENGINE_API SECommandQueueHandle : public SEThinGPUDeviceChild
{
};

struct SE_RENDERING_ENGINE_API SECommandAllocatorHandle : public SEThinGPUDeviceChild
{
};

struct SE_RENDERING_ENGINE_API SECommandListHandle : public SEThinGPUDeviceChild
{
};

struct SE_RENDERING_ENGINE_API SEDescriptorHandle : public SEThinGPUDeviceChild
{
};

struct SE_RENDERING_ENGINE_API SEDescriptorHeapHandle : public SEThinGPUDeviceChild
{
};

}

#endif