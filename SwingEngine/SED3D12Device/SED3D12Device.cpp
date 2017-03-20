// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SED3D12DevicePCH.h"
#include "SED3D12Device.h"

using namespace Swing;

const D3D12_COMMAND_LIST_TYPE SED3D12Device::gsCommandQueueType[CommandQueueType_Max] =
{
    D3D12_COMMAND_LIST_TYPE_DIRECT,
    D3D12_COMMAND_LIST_TYPE_COMPUTE
};

const D3D12_COMMAND_LIST_TYPE SED3D12Device::gsCommandAllocatorType[CommandAllocatorType_Max] =
{
    D3D12_COMMAND_LIST_TYPE_DIRECT,
    D3D12_COMMAND_LIST_TYPE_COMPUTE
};

const D3D12_COMMAND_LIST_TYPE SED3D12Device::gsCommandListType[CommandListType_Max] =
{
    D3D12_COMMAND_LIST_TYPE_DIRECT,
    D3D12_COMMAND_LIST_TYPE_COMPUTE
};

const std::string SED3D12Device::gsShaderProfileTarget[ShaderType_Max] =
{
    "vs_5_0",
    "ps_5_0",
    "gs_5_0",
    "cs_5_0",
    "hs_5_0",
    "ds_5_0"
};

const D3D12_PRIMITIVE_TOPOLOGY_TYPE SED3D12Device::gsPrimitiveTopologyType[PrimitiveType_Max] =
{
    D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT,
    D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE,
    D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
    D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED,  // D3D12 doesn't have quad type.
    D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH
};

const DXGI_FORMAT SED3D12Device::gsBufferFormat[BufferFormat_Max] =
{
    DXGI_FORMAT_R8_UNORM,
    DXGI_FORMAT_R8G8_UNORM,
    DXGI_FORMAT_R8G8B8A8_UNORM,
    DXGI_FORMAT_R8G8B8A8_UNORM,
    DXGI_FORMAT_R32G32B32_FLOAT,
    DXGI_FORMAT_R32G32B32A32_FLOAT,
    DXGI_FORMAT_R16G16B16A16_FLOAT,
    DXGI_FORMAT_R32_UINT,
    DXGI_FORMAT_R32_FLOAT,
    DXGI_FORMAT_D24_UNORM_S8_UINT
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
void SED3D12Device::SetupRasterizerState(const SERasterizerState& srcState, 
    D3D12_RASTERIZER_DESC& dstState)
{
    // Polygon fill mode.
    if( srcState.RasterizerOpFlag & RB_FillMode )
    {
        if( srcState.FillMode == PFM_Solid )
        {
            dstState.FillMode = D3D12_FILL_MODE_SOLID;
        }
        else
        {
            dstState.FillMode = D3D12_FILL_MODE_WIREFRAME;
        }
    }

    // Polygon cull mode.
    if( srcState.RasterizerOpFlag & RB_CullMode )
    {
        if( srcState.CullMode == PCM_Cull_None )
        {
            dstState.CullMode = D3D12_CULL_MODE_NONE;
        }
        else if( srcState.CullMode == PCM_Cull_Front )
        {
            dstState.CullMode = D3D12_CULL_MODE_FRONT;
        }
        else if( srcState.CullMode == PCM_Cull_Back )
        {
            dstState.CullMode = D3D12_CULL_MODE_BACK;
        }
    }

    // Polygon front face.
    if( srcState.RasterizerOpFlag & RB_FrontFace )
    {
        dstState.FrontCounterClockwise = !srcState.FrontFaceClockwise;
    }
}
//----------------------------------------------------------------------------
void SED3D12Device::SetupBlendState(const SEBlendState& srcState, 
    D3D12_BLEND_DESC& dstState)
{
    if( srcState.BlendStateOpFlag & BSB_ColorMask )
    {
        // TODO:
        // Only support target 0 for now.
        const SE_UInt8& colorMask = srcState.FrameBufferTarget[0].ColorMask;

        dstState.RenderTarget[0].RenderTargetWriteMask = 0;
        if( !(colorMask & CMB_Red) )
        {
            dstState.RenderTarget[0].RenderTargetWriteMask |= D3D12_COLOR_WRITE_ENABLE_RED;
        }
        if( !(colorMask & CMB_Green) )
        {
            dstState.RenderTarget[0].RenderTargetWriteMask |= D3D12_COLOR_WRITE_ENABLE_GREEN;
        }
        if( !(colorMask & CMB_Blue) )
        {
            dstState.RenderTarget[0].RenderTargetWriteMask |= D3D12_COLOR_WRITE_ENABLE_BLUE;
        }
        if( !(colorMask & CMB_Alpha) )
        {
            dstState.RenderTarget[0].RenderTargetWriteMask |= D3D12_COLOR_WRITE_ENABLE_ALPHA;
        }
    }
}
//----------------------------------------------------------------------------
void SED3D12Device::SetupDepthStencilState(
    const SEDepthStencilState& srcState, D3D12_DEPTH_STENCIL_DESC& dstState)
{
    dstState.DepthEnable = srcState.DepthEnable;
    dstState.StencilEnable = srcState.StencilEnable;
}
//----------------------------------------------------------------------------

void SED3D12Device::SetMainWindow(HWND mainWindow)
{
    mMainWindow = mainWindow;
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

//----------------------------------------------------------------------------
SED3D12Device::SED3D12Device(HWND mainWindow)
{
    InsertGPUDeviceBaseFunctions();

    mMainWindow = mainWindow;
    mMsaaQuality = 0;
    mBackBufferIndex = 0;
}
//----------------------------------------------------------------------------
SED3D12Device::~SED3D12Device()
{
}
//----------------------------------------------------------------------------