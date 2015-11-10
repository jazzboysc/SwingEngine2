// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015
//
// This part of Swing Engine belongs to an abstraction layer of the next 
// generation GPU graphics and compute API. Currently being constructed to 
// support Microsoft's DirectX 12 and Apple's Metal.

#ifndef Swing_ThinGPUDevice_H
#define Swing_ThinGPUDevice_H

#include "SERenderingEngineLIB.h"
#include "SEGPUDeviceBase.h"

namespace Swing
{

//----------------------------------------------------------------------------
#define SE_INSERT_THIN_GPU_DEVICE_FUNC(function, device) \
    _##function = (ThinGPUDevice##function)&##device::__##function
//----------------------------------------------------------------------------

class SEThinGPUDevice;
class SECommandAllocator;
class SECommandList;
class SECommandQueue;
class SEDescriptor;
class SEDescriptorHeap;
class SEThinGPUDeviceFence;

struct SECommandQueueHandle;
struct SECommandAllocatorHandle;
struct SECommandListHandle;
struct SEDescriptorHandle;
struct SEDescriptorHeapHandle;

typedef SECommandQueueHandle* (SEThinGPUDevice::*ThinGPUDeviceCreateCommandQueue)(SECommandQueue* commandQueue);
typedef void (SEThinGPUDevice::*ThinGPUDeviceDeleteCommandQueue)(SECommandQueue* commandQueue);

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 10/17/2015
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEThinGPUDevice : public SEGPUDeviceBase
{
public:
    ~SEThinGPUDevice();

protected:
    // Abstract base class.
    SEThinGPUDevice();

public:
    // Command queue stuff.
    inline 	SECommandQueueHandle* CreateCommandQueue(SECommandQueue* commandQueue);
    inline  void DeleteCommandQueue(SECommandQueue* commandQueue);

protected:
    ThinGPUDeviceCreateCommandQueue    _CreateCommandQueue;
    ThinGPUDeviceDeleteCommandQueue    _DeleteCommandQueue;
};

typedef SESmartPointer<SEThinGPUDevice> SEThinGPUDevicePtr;

#include "SEThinGPUDevice.inl"

}

#endif