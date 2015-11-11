// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_D3D12ThinGPUDeviceChild_H
#define Swing_D3D12ThinGPUDeviceChild_H

#include "SED3D12DeviceLIB.h"
#include "SED3D12Common.h"
#include "SEThinGPUDeviceChild.h"

namespace Swing
{

struct SE_D3D12_DEVICE_API SED3D12CommandQueueHandle : public SECommandQueueHandle
{
    ComPtr<ID3D12CommandQueue> mCommandQueue;
};

}

#endif