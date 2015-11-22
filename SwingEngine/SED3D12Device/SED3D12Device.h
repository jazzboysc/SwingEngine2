// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_D3D12Device_H
#define Swing_D3D12Device_H

#include "SED3D12DeviceLIB.h"
#include "SED3D12Common.h"
#include "SEThinGPUDevice.h"
#include "SEGPUDeviceBaseChild.h"

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

    // Command queue stuff.
    SECommandQueueHandle* __CreateCommandQueue(SECommandQueue* commandQueue);
    void __DeleteCommandQueue(SECommandQueue* commandQueue);

    // Command allocator stuff.
    SECommandAllocatorHandle* __CreateCommandAllocator(
        SECommandAllocator* commandAllocator, SECommandList* commandList);
    void __DeleteCommandAllocator(
        SECommandAllocator* commandAllocator, SECommandList* commandList);

    // Shader stuff.
    SEShaderHandle* __CreateShader(SEShader* shader);
    void __DeleteShader(SEShader* shader);

    void __OnResize(unsigned int width, unsigned int height);

private:
    void GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter);

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