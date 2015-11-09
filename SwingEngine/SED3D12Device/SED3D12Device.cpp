// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SED3D12DevicePCH.h"
#include "SED3D12Device.h"
#include "SED3D12GPUResources.h"
#include "SwingEngineCore.h"
#include "SwingEngineRenderingEngine.h"

using namespace Swing;

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

    ComPtr<IDXGIAdapter1> hardwareAdapter;
    GetHardwareAdapter(factory.Get(), &hardwareAdapter);

    ThrowIfFailed(D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0,
        IID_PPV_ARGS(&mD3DDevice)));

//    // Describe and create the command queue.
//    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
//    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
//    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
//
//    ThrowIfFailed(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));
//
//    // Describe and create the swap chain.
//    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
//    swapChainDesc.BufferCount = FrameCount;
//    swapChainDesc.BufferDesc.Width = m_width;
//    swapChainDesc.BufferDesc.Height = m_height;
//    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
//    swapChainDesc.OutputWindow = Win32Application::GetHwnd();
//    swapChainDesc.SampleDesc.Count = 1;
//    swapChainDesc.Windowed = TRUE;
//
//    ComPtr<IDXGISwapChain> swapChain;
//    ThrowIfFailed(factory->CreateSwapChain(
//        m_commandQueue.Get(),        // Swap chain needs the queue so that it can force a flush on it.
//        &swapChainDesc,
//        &swapChain
//        ));
//
//    ThrowIfFailed(swapChain.As(&m_swapChain));
//
//    // This sample does not support fullscreen transitions.
//    ThrowIfFailed(factory->MakeWindowAssociation(Win32Application::GetHwnd(), DXGI_MWA_NO_ALT_ENTER));
//
//    m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
//
//    // Create descriptor heaps.
//    {
//        // Describe and create a render target view (RTV) descriptor heap.
//        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
//        rtvHeapDesc.NumDescriptors = FrameCount;
//        rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
//        rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
//        ThrowIfFailed(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));
//
//        m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
//    }
//
//    // Create frame resources.
//    {
//        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
//
//        // Create a RTV for each frame.
//        for( UINT n = 0; n < FrameCount; n++ )
//        {
//            ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
//            m_device->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
//            rtvHandle.Offset(1, m_rtvDescriptorSize);
//        }
//    }
//
//    ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAll
}
//----------------------------------------------------------------------------
void SED3D12Device::__Terminate()
{
    // TODO:
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

    mMainWindow = mainWindow;
    mMsaaQuality = 0;
}
//----------------------------------------------------------------------------
SED3D12Device::~SED3D12Device()
{
}
//----------------------------------------------------------------------------