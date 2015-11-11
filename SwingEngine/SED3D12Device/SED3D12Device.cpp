// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SED3D12DevicePCH.h"
#include "SED3D12Device.h"
#include "SED3D12GPUResources.h"
#include "SED3D12ThinGPUDeviceChild.h"
#include "SECommandQueue.h"
#include "SERenderCommandQueue.h"
#include "SEComputeCommandQueue.h"
#include "SECommandAllocator.h"
#include "SERenderCommandAllocator.h"
#include "SEComputeCommandAllocator.h"
#include "SECommandList.h"
#include "SERenderCommandList.h"
#include "SEComputeCommandList.h"

#include "d3dx12.h"

using namespace Swing;

D3D12_COMMAND_LIST_TYPE gsCommandQueueType[CommandQueueType_Max] =
{
    D3D12_COMMAND_LIST_TYPE_DIRECT,
    D3D12_COMMAND_LIST_TYPE_COMPUTE
};

D3D12_COMMAND_LIST_TYPE gsCommandAllocatorType[CommandAllocatorType_Max] =
{
    D3D12_COMMAND_LIST_TYPE_DIRECT,
    D3D12_COMMAND_LIST_TYPE_COMPUTE
};

//----------------------------------------------------------------------------
void SED3D12Device::GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter)
{
    ComPtr<IDXGIAdapter1> adapter;
    *ppAdapter = nullptr;

    for( UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex )
    {
        DXGI_ADAPTER_DESC1 desc;
        adapter->GetDesc1(&desc);

        if( desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE )
        {
            // Don't select the Basic Render Driver adapter.
            // If you want a software adapter, pass in "/warp" on the command line.
            continue;
        }

        // Check to see if the adapter supports Direct3D 12, but don't create the
        // actual device yet.
        if( SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)) )
        {
            break;
        }
    }

    *ppAdapter = adapter.Detach();
}
//----------------------------------------------------------------------------
void SED3D12Device::SetMainWindow(HWND mainWindow)
{
    mMainWindow = mainWindow;
}
//----------------------------------------------------------------------------
void SED3D12Device::__Initialize(SEGPUDeviceDescription*)
{
#if defined(_DEBUG)
    // Enable the D3D12 debug layer.
    {

        ComPtr<ID3D12Debug> debugController;
        if( SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))) )
        {
            debugController->EnableDebugLayer();
        }
    }
#endif

    ComPtr<IDXGIFactory4> factory;
    ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&factory)));

    // Get the first GPU.
    ComPtr<IDXGIAdapter1> hardwareAdapter;
    GetHardwareAdapter(factory.Get(), &hardwareAdapter);

    // Create device.
    ThrowIfFailed(D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0,
        IID_PPV_ARGS(&mD3DDevice)));

    // Create default render command queue.
    mDefaultRenderCommandQueue = SE_NEW SERenderCommandQueue();
    mDefaultRenderCommandQueue->CreateDeviceChild(this);
    SED3D12CommandQueueHandle* commandQueueHandle = 
        (SED3D12CommandQueueHandle*)mDefaultRenderCommandQueue->GetCommandQueueHandle();

    // Describe and create the swap chain.
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = BackBufferCount;
    swapChainDesc.BufferDesc.Width = mDeviceDesc.FramebufferWidth;
    swapChainDesc.BufferDesc.Height = mDeviceDesc.FramebufferHeight;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.OutputWindow = mMainWindow;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.Windowed = TRUE;

    ComPtr<IDXGISwapChain> swapChain;
    ThrowIfFailed(factory->CreateSwapChain(commandQueueHandle->mCommandQueue.Get(),
        &swapChainDesc, &swapChain));

    ThrowIfFailed(swapChain.As(&mSwapChain));

    // This sample does not support fullscreen transitions.
    ThrowIfFailed(factory->MakeWindowAssociation(mMainWindow, DXGI_MWA_NO_ALT_ENTER));

    mBackBufferIndex = mSwapChain->GetCurrentBackBufferIndex();

    // Create default descriptor heap for backbuffers.
    {
        // Describe and create a render target view (RTV) descriptor heap.
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
        rtvHeapDesc.NumDescriptors = BackBufferCount;
        rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        ThrowIfFailed(mD3DDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&mRtvHeap)));

        mRtvDescriptorSize = mD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }

    // Create backbuffer resources.
    {
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(mRtvHeap->GetCPUDescriptorHandleForHeapStart());

        // Create a RTV for each backbuffer.
        for( UINT n = 0; n < BackBufferCount; n++ )
        {
            ThrowIfFailed(mSwapChain->GetBuffer(n, IID_PPV_ARGS(&mRenderTargets[n])));
            mD3DDevice->CreateRenderTargetView(mRenderTargets[n].Get(), nullptr, rtvHandle);
            rtvHandle.Offset(1, mRtvDescriptorSize);
        }
    }

    // Create default render command allocator.
    mDefaultRenderCommandAllocator = SE_NEW SERenderCommandAllocator();
    mDefaultRenderCommandAllocator->CreateDeviceChild(this);
}
//----------------------------------------------------------------------------
void SED3D12Device::__Terminate()
{
    SE_DELETE mDefaultRenderCommandQueue;
    mDefaultRenderCommandQueue = 0;
}
//----------------------------------------------------------------------------
SECommandQueueHandle* SED3D12Device::__CreateCommandQueue(
    SECommandQueue* commandQueue)
{
    SED3D12CommandQueueHandle* commandQueueHandle 
        = SE_NEW SED3D12CommandQueueHandle();
    commandQueueHandle->ThinDevice = this;

    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = gsCommandQueueType[(int)commandQueue->GetType()];
    
    HRESULT hr = mD3DDevice->CreateCommandQueue(&queueDesc, 
        IID_PPV_ARGS(&commandQueueHandle->mCommandQueue));
    SE_ASSERT( hr == S_OK );

    return commandQueueHandle;
}
//----------------------------------------------------------------------------
void SED3D12Device::__DeleteCommandQueue(SECommandQueue* commandQueue)
{
    SED3D12CommandQueueHandle* commandQueueHandle = 
        (SED3D12CommandQueueHandle*)commandQueue->GetCommandQueueHandle();
    if( commandQueueHandle )
    {
        commandQueueHandle->mCommandQueue = nullptr;
    }
}
//----------------------------------------------------------------------------
SECommandAllocatorHandle* SED3D12Device::__CreateCommandAllocator(
    SECommandAllocator* commandAllocator)
{
    SED3D12CommandAllocatorHandle* commandAllocatorHandle 
        = SE_NEW SED3D12CommandAllocatorHandle();
    commandAllocatorHandle->ThinDevice = this;

    HRESULT hr = mD3DDevice->CreateCommandAllocator(
        gsCommandAllocatorType[(int)commandAllocator->GetType()],
        IID_PPV_ARGS(&commandAllocatorHandle->mCommandAllocator));
    SE_ASSERT( hr == S_OK );

    return commandAllocatorHandle;
}
//----------------------------------------------------------------------------
void SED3D12Device::__DeleteCommandAllocator(
    SECommandAllocator* commandAllocator)
{
    SED3D12CommandAllocatorHandle* commandAllocatorHandle = 
        (SED3D12CommandAllocatorHandle*)commandAllocator->GetCommandAllocatorHandle();
    if( commandAllocatorHandle )
    {
        commandAllocatorHandle->mCommandAllocator = nullptr;
    }
}
//----------------------------------------------------------------------------
void SED3D12Device::__GetMaxAnisFilterLevel(int*)
{
    // TODO:
}
//----------------------------------------------------------------------------
void SED3D12Device::__SetAnisFilterLevel(int)
{
    // TODO:
}
//----------------------------------------------------------------------------
void SED3D12Device::__OnResize(unsigned int, unsigned int)
{
    // TODO:
}
//----------------------------------------------------------------------------
SEShaderHandle* SED3D12Device::__CreateShader(SEShader*)
{
    // TODO:
    return 0;
}

//----------------------------------------------------------------------------
SED3D12Device::SED3D12Device(HWND mainWindow)
{
    SE_INSERT_GPU_DEVICE_BASE_FUNC(Initialize, SED3D12Device);
    SE_INSERT_GPU_DEVICE_BASE_FUNC(Terminate, SED3D12Device);
    SE_INSERT_GPU_DEVICE_BASE_FUNC(GetMaxAnisFilterLevel, SED3D12Device);
    SE_INSERT_GPU_DEVICE_BASE_FUNC(SetAnisFilterLevel, SED3D12Device);

    SE_INSERT_THIN_GPU_DEVICE_FUNC(CreateCommandQueue, SED3D12Device);
    SE_INSERT_THIN_GPU_DEVICE_FUNC(DeleteCommandQueue, SED3D12Device);
    SE_INSERT_THIN_GPU_DEVICE_FUNC(CreateCommandAllocator, SED3D12Device);
    SE_INSERT_THIN_GPU_DEVICE_FUNC(DeleteCommandAllocator, SED3D12Device);

    mMainWindow = mainWindow;
    mMsaaQuality = 0;
    mBackBufferIndex = 0;
}
//----------------------------------------------------------------------------
SED3D12Device::~SED3D12Device()
{
}
//----------------------------------------------------------------------------