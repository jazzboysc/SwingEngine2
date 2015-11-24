// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_D3D12Device_H
#define Swing_D3D12Device_H

#include "SED3D12DeviceLIB.h"
#include "SED3D12Common.h"
#include "SEThinGPUDevice.h"
#include "SEGPUDeviceBaseChild.h"
#include "SEPipelineStateBlock.h"

namespace Swing
{

class SE_D3D12_DEVICE_API SED3D12Device : public SEThinGPUDevice
{
public:
    SED3D12Device(HWND mainWindow);
    ~SED3D12Device();

    void SetMainWindow(HWND mainWindow);

private:
    void __Initialize(SEGPUDeviceDescription* deviceDesc);
    void __Terminate();

    void __GetMaxAnisFilterLevel(int* maxAnisFilterLevel);
    void __SetAnisFilterLevel(int anisFilterLevel);

    void __OnResize(unsigned int width, unsigned int height);

    // Shader stuff.
    SEShaderHandle* __CreateShader(SEShader* shader);
    void __DeleteShader(SEShader* shader);

    // Render pass info stuff.
    SERenderPassInfoHandle* __CreateRenderPassInfo(SERenderPassInfo* renderPassInfo, 
        SEShaderProgram* program, SEGeometryAttributes* geometryAttr, 
        SEPipelineStateBlock* psb, SERootSignature* rootSignature, 
        SERenderPassTargetsInfo* targetsInfo);
    void __DeleteRenderPassInfo(SERenderPassInfo* renderPassInfo);
    void __EnableRenderPassInfo(SERenderPassInfo* renderPassInfo);
    void __DisableRenderPassInfo(SERenderPassInfo* renderPassInfo);

    // Command queue stuff.
    SECommandQueueHandle* __CreateCommandQueue(SECommandQueue* commandQueue);
    void __DeleteCommandQueue(SECommandQueue* commandQueue);

    // Command allocator stuff.
    SECommandAllocatorHandle* __CreateCommandAllocator(
        SECommandAllocator* commandAllocator, SECommandList* commandList);
    void __DeleteCommandAllocator(
        SECommandAllocator* commandAllocator, SECommandList* commandList);

    // Command list stuff.
    SECommandListHandle* __CreateCommandList(SECommandList* commandList,
        SECommandAllocator* commandAllocator);
    void __DeleteCommandList(SECommandList* commandList,
        SECommandAllocator* commandAllocator);

    // Root signature stuff.
    SERootSignatureHandle* __CreateRootSignature(SERootSignature* rootSignature);
    void __DeleteRootSignature(SERootSignature* rootSignature);

private:
    void GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter);

    // Render state helpers.
    void SetupRasterizerState(const SERasterizerState& srcState, D3D12_RASTERIZER_DESC& dstState);
    void SetupBlendState(const SEBlendState& srcState, D3D12_BLEND_DESC& dstState);
    void SetupDepthStencilState(const SEDepthStencilState& srcState, D3D12_DEPTH_STENCIL_DESC& dstState);

    HWND mMainWindow;
    UINT mMsaaQuality;

    enum { BackBufferCount = 2 };

    D3D12_VIEWPORT mScreenViewport;
    D3D12_RECT mScissorRect;
    ComPtr<IDXGISwapChain3> mSwapChain;
    ComPtr<ID3D12Device> mD3DDevice;
    ComPtr<ID3D12Resource> mRenderTargets[BackBufferCount];
    ComPtr<ID3D12RootSignature> mRootSignature;
    ComPtr<ID3D12DescriptorHeap> mRtvHeap;
    ComPtr<ID3D12PipelineState> mPipelineState;
    ComPtr<ID3D12GraphicsCommandList> mCommandList;
    UINT mRtvDescriptorSize;

    // Synchronization objects.
    UINT mBackBufferIndex;
};

typedef SESmartPointer<SED3D12Device> SED3D12DevicePtr;

}

#endif