// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_D3D12GPUDeviceChild_H
#define Swing_D3D12GPUDeviceChild_H

#include "SED3D12DeviceLIB.h"
#include "SED3D12Common.h"
#include "SEGPUDeviceBaseChild.h"

namespace Swing
{

struct SE_D3D12_DEVICE_API SED3D12CommandQueueHandle : public SECommandQueueHandle
{
    ComPtr<ID3D12CommandQueue> mCommandQueue;
};

struct SE_D3D12_DEVICE_API SED3D12CommandAllocatorHandle : public SECommandAllocatorHandle
{
    ComPtr<ID3D12CommandAllocator> mCommandAllocator;
};

struct SE_D3D12_DEVICE_API SED3D12CommandListHandle : public SECommandListHandle
{
    ComPtr<ID3D12CommandList> mCommandList;
};

struct SE_D3D12_DEVICE_API SED3D12RootSignatureHandle : public SERootSignatureHandle
{
    ComPtr<ID3D12RootSignature> mRootSignature;
};

struct SE_D3D12_DEVICE_API SED3D12RenderPassInfoHandle : public SERenderPassInfoHandle
{
    ComPtr<ID3D12PipelineState> mPipelineState;
};

}

#endif