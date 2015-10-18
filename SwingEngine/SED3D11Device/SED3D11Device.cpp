// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SED3D11DevicePCH.h"
#include "SED3D11Device.h"
#include "SED3D11GPUResources.h"
#include "SwingEngineCore.h"
#include "SwingEngineRenderingEngine.h"

using namespace Swing;

//----------------------------------------------------------------------------
void SED3D11Device::__Initialize(SEGPUDeviceDescription* deviceDesc)
{
    UINT createDeviceFlags = 0;
#if defined(_DEBUG)  
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevel;
    HRESULT hr = D3D11CreateDevice(
        0,                 // default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        0,                 // no software device
        createDeviceFlags,
        0, 0,              // default feature level array
        D3D11_SDK_VERSION,
        &md3dDevice,
        &featureLevel,
        &md3dImmediateContext);

    if( FAILED(hr) )
    {
        MessageBox(0, "D3D11CreateDevice Failed.", 0, 0);
        return;
    }

    if( featureLevel != D3D_FEATURE_LEVEL_11_0 )
    {
        MessageBox(0, "Direct3D Feature Level 11 unsupported.", 0, 0);
        return;
    }

    // Check 4X MSAA quality support for our back buffer format.
    // All Direct3D 11 capable devices support 4X MSAA for all render 
    // target formats, so we only need to check quality support.

    hr = md3dDevice->CheckMultisampleQualityLevels(
        DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality);
    if( FAILED(hr) )
    {
        SE_ASSERT( false );
    }
    SE_ASSERT( m4xMsaaQuality > 0 );

    // Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.

    DXGI_SWAP_CHAIN_DESC sd;
    sd.BufferDesc.Width = deviceDesc->FramebufferWidth;
    sd.BufferDesc.Height = deviceDesc->FramebufferHeight;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    mEnable4xMsaa = deviceDesc->EnableMSAA;
    if( mEnable4xMsaa )
    {
        sd.SampleDesc.Count = 4;
        sd.SampleDesc.Quality = m4xMsaaQuality - 1;
    }
    else
    {
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
    }

    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 1;
    sd.OutputWindow = mMainWindow;
    sd.Windowed = true;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;

    // To correctly create the swap chain, we must use the IDXGIFactory that was
    // used to create the device.  If we tried to use a different IDXGIFactory instance
    // (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
    // This function is being called with a device from a different IDXGIFactory."

    IDXGIDevice* dxgiDevice = 0;
    md3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

    IDXGIAdapter* dxgiAdapter = 0;
    dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

    IDXGIFactory* dxgiFactory = 0;
    dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

    dxgiFactory->CreateSwapChain(md3dDevice, &sd, &mSwapChain);

    SE_RELEASE_COM(dxgiDevice);
    SE_RELEASE_COM(dxgiAdapter);
    SE_RELEASE_COM(dxgiFactory);

    __OnResize(deviceDesc->FramebufferWidth, deviceDesc->FramebufferHeight);
}
//----------------------------------------------------------------------------
void SED3D11Device::__Terminate()
{
    SE_RELEASE_COM(mRenderTargetView);
    SE_RELEASE_COM(mDepthStencilView);
    SE_RELEASE_COM(mSwapChain);
    SE_RELEASE_COM(mDepthStencilBuffer);

    // Restore all default settings.
    if( md3dImmediateContext )
    {
        md3dImmediateContext->ClearState();
    }

    SE_RELEASE_COM(md3dImmediateContext);
    SE_RELEASE_COM(md3dDevice);
}
//----------------------------------------------------------------------------
void SED3D11Device::__OnResize(unsigned int width, unsigned int height)
{
    SE_ASSERT( md3dImmediateContext );
    SE_ASSERT( md3dDevice );
    SE_ASSERT( mSwapChain );

    // Release the old views, as they hold references to the buffers we
    // will be destroying.  Also release the old depth/stencil buffer.
    SE_RELEASE_COM(mRenderTargetView);
    SE_RELEASE_COM(mDepthStencilView);
    SE_RELEASE_COM(mDepthStencilBuffer);

    // Resize the swap chain and recreate the render target view.
    mSwapChain->ResizeBuffers(1, width, height, 
        DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    ID3D11Texture2D* backBuffer;
    mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), 
        reinterpret_cast<void**>(&backBuffer));
    md3dDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView);
    SE_RELEASE_COM(backBuffer);

    // Create the depth/stencil buffer and view.
    D3D11_TEXTURE2D_DESC depthStencilDesc;
    depthStencilDesc.Width = width;
    depthStencilDesc.Height = height;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

    if( mEnable4xMsaa )
    {
        depthStencilDesc.SampleDesc.Count = 4;
        depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
    }
    else
    {
        depthStencilDesc.SampleDesc.Count = 1;
        depthStencilDesc.SampleDesc.Quality = 0;
    }

    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    md3dDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer);
    md3dDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, 
        &mDepthStencilView);

    // Bind the render target view and depth/stencil view to the pipeline.
    md3dImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, 
        mDepthStencilView);

    // Set the viewport transform.
    mScreenViewport.TopLeftX = 0;
    mScreenViewport.TopLeftY = 0;
    mScreenViewport.Width = static_cast<float>(width);
    mScreenViewport.Height = static_cast<float>(height);
    mScreenViewport.MinDepth = 0.0f;
    mScreenViewport.MaxDepth = 1.0f;
    md3dImmediateContext->RSSetViewports(1, &mScreenViewport);
}
//----------------------------------------------------------------------------
SEShaderHandle* SED3D11Device::__CreateShader(SEShader* shader)
{
    const std::string& shaderSource = shader->GetShaderSource();
    const std::string& shaderSourceName = shader->GetShaderFileName();
    SEShader::SEShaderStage type = shader->GetShaderType();

    std::string target;
    switch( type )
    {
    case SEShader::SS_Vertex:
        target = "vs_5_0";
        break;

    case SEShader::SS_Fragment:
        target = "ps_5_0";
        break;

    case SEShader::SS_Geometry:
        target = "gs_5_0";
        break;

    case SEShader::SS_Compute:
        target = "cs_5_0";
        break;

    case SEShader::SS_TessellationControl:
        target = "hs_5_0";
        break;

    case SEShader::SS_TessellationEvaluation:
        target = "ds_5_0";
        break;

    default:
        SE_ASSERT(false);
        break;
    }
    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(_DEBUG)
    flags |= D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* codeBuffer;
    ID3DBlob* errorBuffer;

    HRESULT hr = D3DCompile((LPCVOID)shaderSource.c_str(), 
        shaderSource.length(), shaderSourceName.c_str(), NULL, NULL, "main", 
        target.c_str(), flags, 0, &codeBuffer, &errorBuffer);
    if( FAILED(hr) )
    {
        std::string error = std::string((char*)errorBuffer->GetBufferPointer());
        printf("Failed compiling %s\n%s\n", shaderSourceName.c_str(), 
            error.c_str());
        errorBuffer->Release();

        return 0;
    }

    SED3D11ShaderHandle* handle = SE_NEW SED3D11ShaderHandle();
    switch( type )
    {
    case SEShader::SS_Vertex:
        hr = md3dDevice->CreateVertexShader(codeBuffer->GetBufferPointer(),
            codeBuffer->GetBufferSize(), NULL,
            (ID3D11VertexShader**)&handle->ShaderInterface);
        break;

    case SEShader::SS_Fragment:
        hr = md3dDevice->CreatePixelShader(codeBuffer->GetBufferPointer(),
            codeBuffer->GetBufferSize(), NULL,
            (ID3D11PixelShader**)&handle->ShaderInterface);
        break;

    case SEShader::SS_Geometry:
        hr = md3dDevice->CreateGeometryShader(codeBuffer->GetBufferPointer(),
            codeBuffer->GetBufferSize(), NULL,
            (ID3D11GeometryShader**)&handle->ShaderInterface);
        break;

    case SEShader::SS_Compute:
        hr = md3dDevice->CreateComputeShader(codeBuffer->GetBufferPointer(),
            codeBuffer->GetBufferSize(), NULL,
            (ID3D11ComputeShader**)&handle->ShaderInterface);
        break;

    case SEShader::SS_TessellationControl:
        hr = md3dDevice->CreateHullShader(codeBuffer->GetBufferPointer(),
            codeBuffer->GetBufferSize(), NULL,
            (ID3D11HullShader**)&handle->ShaderInterface);
        break;

    case SEShader::SS_TessellationEvaluation:
        hr = md3dDevice->CreateDomainShader(codeBuffer->GetBufferPointer(),
            codeBuffer->GetBufferSize(), NULL,
            (ID3D11DomainShader**)&handle->ShaderInterface);
        break;

    default:
        SE_ASSERT( false );
        break;
    }

    if( FAILED(hr) )
    {
        SE_DELETE handle;
        SE_ASSERT(false);
        return 0;
    }

    return handle;
}

//----------------------------------------------------------------------------
SED3D11Device::SED3D11Device(HWND mainWindow)
{
    Initialize = (SEGPUDeviceInitialize)&SED3D11Device::__Initialize;
    Terminate = (SEGPUDeviceTerminate)&SED3D11Device::__Terminate;
    CreateShader = (SEGPUDeviceCreateShader)&SED3D11Device::__CreateShader;

    mMainWindow = mainWindow;
    mEnable4xMsaa = false;
    m4xMsaaQuality = 0;

    md3dDevice = 0;
    md3dImmediateContext = 0;
    mSwapChain = 0;
    mDepthStencilBuffer = 0;
    mRenderTargetView = 0;
    mDepthStencilView = 0;
}
//----------------------------------------------------------------------------
SED3D11Device::~SED3D11Device()
{
}
//----------------------------------------------------------------------------