// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SED3D12DevicePCH.h"
#include "SED3D12Device.h"

using namespace Swing;

//----------------------------------------------------------------------------
void SED3D12Device::InsertGPUDeviceBaseFunctions()
{
    SE_INSERT_GPU_DEVICE_BASE_FUNC(Initialize, SED3D12Device);
    SE_INSERT_GPU_DEVICE_BASE_FUNC(Terminate, SED3D12Device);
    SE_INSERT_GPU_DEVICE_BASE_FUNC(GetMaxAnisFilterLevel, SED3D12Device);
    SE_INSERT_GPU_DEVICE_BASE_FUNC(SetAnisFilterLevel, SED3D12Device);

    SE_INSERT_GPU_DEVICE_BASE_FUNC(CreateShader, SED3D12Device);
    SE_INSERT_GPU_DEVICE_BASE_FUNC(DeleteShader, SED3D12Device);

    SE_INSERT_GPU_DEVICE_BASE_FUNC(CreateRenderPassInfo, SED3D12Device);
    SE_INSERT_GPU_DEVICE_BASE_FUNC(DeleteRenderPassInfo, SED3D12Device);
    SE_INSERT_GPU_DEVICE_BASE_FUNC(EnableRenderPassInfo, SED3D12Device);
    SE_INSERT_GPU_DEVICE_BASE_FUNC(DisableRenderPassInfo, SED3D12Device);

    SE_INSERT_GPU_DEVICE_BASE_FUNC(CreateCommandQueue, SED3D12Device);
    SE_INSERT_GPU_DEVICE_BASE_FUNC(DeleteCommandQueue, SED3D12Device);

    SE_INSERT_GPU_DEVICE_BASE_FUNC(CreateCommandAllocator, SED3D12Device);
    SE_INSERT_GPU_DEVICE_BASE_FUNC(DeleteCommandAllocator, SED3D12Device);

    SE_INSERT_GPU_DEVICE_BASE_FUNC(CreateCommandList, SED3D12Device);
    SE_INSERT_GPU_DEVICE_BASE_FUNC(DeleteCommandList, SED3D12Device);
    SE_INSERT_GPU_DEVICE_BASE_FUNC(ResetRenderCommandList, SED3D12Device);
    SE_INSERT_GPU_DEVICE_BASE_FUNC(CloseRenderCommandList, SED3D12Device);
    SE_INSERT_GPU_DEVICE_BASE_FUNC(RenderCommandListSetRootSignature, SED3D12Device);
    SE_INSERT_GPU_DEVICE_BASE_FUNC(RenderCommandListSetViewport, SED3D12Device);

    SE_INSERT_GPU_DEVICE_BASE_FUNC(CreateRootSignature, SED3D12Device);
    SE_INSERT_GPU_DEVICE_BASE_FUNC(DeleteRootSignature, SED3D12Device);
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

// Create default render command allocator and render command list.
mDefaultRenderCommandAllocator = SE_NEW SERenderCommandAllocator();
mDefaultRenderCommandList = SE_NEW SERenderCommandList();
mDefaultRenderCommandAllocator->CreateDeviceChild(this, mDefaultRenderCommandList);
mDefaultRenderCommandList->CreateDeviceChild(this, mDefaultRenderCommandAllocator);

// Create default empty root signature.
SERootSignatureInfo defaultRootSignatureInfo;
defaultRootSignatureInfo.Flags = RSF_Allow_Input_Assembler_Input_Layout;
mDefaultRootSignature = SE_NEW SERootSignature(defaultRootSignatureInfo);
mDefaultRootSignature->CreateDeviceChild(this);
}
//----------------------------------------------------------------------------
void SED3D12Device::__Terminate()
{
    SE_DELETE mDefaultRenderCommandQueue;
    mDefaultRenderCommandQueue = 0;

    SE_DELETE mDefaultRenderCommandAllocator;
    mDefaultRenderCommandAllocator = 0;

    SE_DELETE mDefaultRenderCommandList;
    mDefaultRenderCommandList = 0;

    SE_DELETE mDefaultRootSignature;
    mDefaultRootSignature = 0;
}
//----------------------------------------------------------------------------
SEShaderHandle* SED3D12Device::__CreateShader(SEShader* shader)
{
    const std::string& shaderSource = shader->GetSource();
    const std::string& shaderSourceName = shader->GetShaderFileName();
    SEShaderType type = shader->GetType();

    const std::string& target = gsShaderProfileTarget[(int)type];

    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(_DEBUG)
    flags |= D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* codeBuffer;
    ID3DBlob* errorBuffer;

    HRESULT hr = D3DCompile((LPCVOID)shaderSource.c_str(),
        shaderSource.length(), shaderSourceName.c_str(), nullptr, nullptr, "main",
        target.c_str(), flags, 0, &codeBuffer, &errorBuffer);
    if( FAILED(hr) )
    {
        std::string error = std::string((char*)errorBuffer->GetBufferPointer());
        printf("Failed compiling %s\n%s\n", shaderSourceName.c_str(),
            error.c_str());
        errorBuffer->Release();

        return 0;
    }

    SED3D12ShaderHandle* shaderHandle = SE_NEW SED3D12ShaderHandle();
    shaderHandle->Device = this;
    shaderHandle->mShaderHandle = codeBuffer;

    return shaderHandle;
}
//----------------------------------------------------------------------------
void SED3D12Device::__DeleteShader(SEShader* shader)
{
    SED3D12ShaderHandle* shaderHandle =
        (SED3D12ShaderHandle*)shader->GetShaderHandle();
    if( shaderHandle )
    {
        shaderHandle->mShaderHandle = nullptr;
    }
}
//----------------------------------------------------------------------------
SERenderPassInfoHandle* SED3D12Device::__CreateRenderPassInfo(
    SERenderPassInfo* renderPassInfo, SEShaderProgram* program,
    SEGeometryAttributes* geometryAttr, SEPipelineStateBlock* psb,
    SERootSignature* rootSignature, SERenderPassTargetsInfo* targetsInfo)
{
    SE_ASSERT(renderPassInfo && geometryAttr && targetsInfo);

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};

    // Setup input layout.
    // TODO:
    // Remove this naive input layout specification system.
    std::vector<D3D12_INPUT_ELEMENT_DESC> inputElements;
    if( !geometryAttr->HasNormal && !geometryAttr->HasTCoord )
    {
        D3D12_INPUT_ELEMENT_DESC positionDesc = { "vPosition", 0,
            DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

        inputElements.push_back(positionDesc);
    }
    else if( geometryAttr->HasNormal && !geometryAttr->HasTCoord )
    {
        D3D12_INPUT_ELEMENT_DESC positionDesc = { "vPosition", 0,
            DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

        D3D12_INPUT_ELEMENT_DESC normalDesc = { "vNormal", 0,
            DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

        inputElements.push_back(positionDesc);
        inputElements.push_back(normalDesc);
    }
    else if( geometryAttr->HasNormal && geometryAttr->HasTCoord )
    {
        D3D12_INPUT_ELEMENT_DESC positionDesc = { "vPosition", 0,
            DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

        D3D12_INPUT_ELEMENT_DESC tcoordDesc = { "vTCoord", 0,
            DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

        D3D12_INPUT_ELEMENT_DESC normalDesc = { "vNormal", 0,
            DXGI_FORMAT_R32G32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

        inputElements.push_back(positionDesc);
        inputElements.push_back(tcoordDesc);
        inputElements.push_back(normalDesc);
    }
    else
    {
        D3D12_INPUT_ELEMENT_DESC positionDesc = { "vPosition", 0,
            DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

        D3D12_INPUT_ELEMENT_DESC tcoordDesc = { "vTCoord", 0,
            DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

        inputElements.push_back(positionDesc);
        inputElements.push_back(tcoordDesc);
    }

    psoDesc.InputLayout = { (const D3D12_INPUT_ELEMENT_DESC*)inputElements.data(),
        UINT(inputElements.size()) };

    // Setup root signature.
    if( !rootSignature )
    {
        rootSignature = mDefaultRootSignature;
    }
    SED3D12RootSignatureHandle* rootSignatureHandle =
        (SED3D12RootSignatureHandle*)rootSignature->GetRootSignatureHandle();
    SE_ASSERT(rootSignatureHandle);
    psoDesc.pRootSignature = rootSignatureHandle->mRootSignature.Get();

    // Setup shaders.
    if( program )
    {
        SEVertexShader* vShader = program->GetVertexShader();
        if( vShader )
        {
            SED3D12ShaderHandle* vShaderHandle =
                (SED3D12ShaderHandle*)vShader->GetShaderHandle();
            SE_ASSERT(vShaderHandle);

            psoDesc.VS = { reinterpret_cast<UINT8*>(
                vShaderHandle->mShaderHandle->GetBufferPointer()),
                vShaderHandle->mShaderHandle->GetBufferSize() };
        }

        SETessellationControlShader* tcShader = program->GetTessellationControlShader();
        if( tcShader )
        {
            SED3D12ShaderHandle* tcShaderHandle =
                (SED3D12ShaderHandle*)tcShader->GetShaderHandle();
            SE_ASSERT(tcShaderHandle);

            psoDesc.HS = { reinterpret_cast<UINT8*>(
                tcShaderHandle->mShaderHandle->GetBufferPointer()),
                tcShaderHandle->mShaderHandle->GetBufferSize() };
        }

        SETessellationEvaluationShader* teShader = program->GetTessellationEvaluationShader();
        if( teShader )
        {
            SED3D12ShaderHandle* teShaderHandle =
                (SED3D12ShaderHandle*)teShader->GetShaderHandle();
            SE_ASSERT(teShaderHandle);

            psoDesc.DS = { reinterpret_cast<UINT8*>(
                teShaderHandle->mShaderHandle->GetBufferPointer()),
                teShaderHandle->mShaderHandle->GetBufferSize() };
        }

        SEGeometryShader* gShader = program->GetGeometryShader();
        if( gShader )
        {
            SED3D12ShaderHandle* gShaderHandle =
                (SED3D12ShaderHandle*)gShader->GetShaderHandle();
            SE_ASSERT(gShaderHandle);

            psoDesc.GS = { reinterpret_cast<UINT8*>(
                gShaderHandle->mShaderHandle->GetBufferPointer()),
                gShaderHandle->mShaderHandle->GetBufferSize() };
        }

        SEFragmentShader* fShader = program->GetFragmentShader();
        if( fShader )
        {
            SED3D12ShaderHandle* fShaderHandle =
                (SED3D12ShaderHandle*)fShader->GetShaderHandle();
            SE_ASSERT(fShaderHandle);

            psoDesc.PS = { reinterpret_cast<UINT8*>(
                fShaderHandle->mShaderHandle->GetBufferPointer()),
                fShaderHandle->mShaderHandle->GetBufferSize() };
        }
    }

    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

    if( psb )
    {
        if( psb->PipelineStageFlag & PB_Rasterizer )
        {
            SetupRasterizerState(psb->Rasterizer, psoDesc.RasterizerState);
        }

        if( psb->PipelineStageFlag & PB_OutputMerger )
        {
            if( psb->OutputMerger.OutputMergerOpFlag & OMB_Blend )
            {
                SetupBlendState(psb->OutputMerger.Blend, psoDesc.BlendState);
            }

            if( psb->OutputMerger.OutputMergerOpFlag & OMB_DepthStencil )
            {
                SetupDepthStencilState(psb->OutputMerger.DepthStencil,
                    psoDesc.DepthStencilState);
            }
        }
    }

    psoDesc.SampleMask = UINT_MAX;

    // Specify primitive type.
    psoDesc.PrimitiveTopologyType = gsPrimitiveTopologyType[
        (int)geometryAttr->Prim->Type];

    // Specify render targets.
    psoDesc.NumRenderTargets = targetsInfo->ColorTargetCount;
    for( unsigned int i = 0; i < targetsInfo->ColorTargetCount; ++i )
    {
        SEBufferFormat format = targetsInfo->ColorTargetFormats[i];
        psoDesc.RTVFormats[i] = gsBufferFormat[(int)format];
    }
    psoDesc.DSVFormat = gsBufferFormat[(int)targetsInfo->DepthFormat];

    // TODO:
    // Implement multisampling setup.
    psoDesc.SampleDesc.Count = 1;

    SED3D12RenderPassInfoHandle* renderPassInfoHandle = SE_NEW SED3D12RenderPassInfoHandle();
    renderPassInfoHandle->DeviceBase = this;

    HRESULT hr = mD3DDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&
        renderPassInfoHandle->mPipelineState));
    SE_ASSERT(hr == S_OK);

    return renderPassInfoHandle;
}
//----------------------------------------------------------------------------
void SED3D12Device::__DeleteRenderPassInfo(SERenderPassInfo* renderPassInfo)
{
    SED3D12RenderPassInfoHandle* renderPassInfoHandle =
        (SED3D12RenderPassInfoHandle*)renderPassInfo->GetPassInfoHandle();
    if( renderPassInfoHandle )
    {
        renderPassInfoHandle->mPipelineState = nullptr;
    }
}
//----------------------------------------------------------------------------
void SED3D12Device::__EnableRenderPassInfo(SERenderPassInfo*)
{
    // TODO:
}
//----------------------------------------------------------------------------
void SED3D12Device::__DisableRenderPassInfo(SERenderPassInfo*)
{
    // TODO:
}
//----------------------------------------------------------------------------
SECommandQueueHandle* SED3D12Device::__CreateCommandQueue(
    SECommandQueue* commandQueue)
{
    SED3D12CommandQueueHandle* commandQueueHandle
        = SE_NEW SED3D12CommandQueueHandle();
    commandQueueHandle->DeviceBase = this;

    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = gsCommandQueueType[(int)commandQueue->GetType()];

    HRESULT hr = mD3DDevice->CreateCommandQueue(&queueDesc,
        IID_PPV_ARGS(&commandQueueHandle->mCommandQueue));
    SE_ASSERT(hr == S_OK);

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
    SECommandAllocator* commandAllocator, SECommandList*)
{
    SED3D12CommandAllocatorHandle* commandAllocatorHandle
        = SE_NEW SED3D12CommandAllocatorHandle();
    commandAllocatorHandle->DeviceBase = this;

    HRESULT hr = mD3DDevice->CreateCommandAllocator(
        gsCommandAllocatorType[(int)commandAllocator->GetType()],
        IID_PPV_ARGS(&commandAllocatorHandle->mCommandAllocator));
    SE_ASSERT(hr == S_OK);

    return commandAllocatorHandle;
}
//----------------------------------------------------------------------------
void SED3D12Device::__DeleteCommandAllocator(
    SECommandAllocator* commandAllocator, SECommandList*)
{
    SED3D12CommandAllocatorHandle* commandAllocatorHandle =
        (SED3D12CommandAllocatorHandle*)commandAllocator->GetCommandAllocatorHandle();
    if( commandAllocatorHandle )
    {
        commandAllocatorHandle->mCommandAllocator = nullptr;
    }
}
//----------------------------------------------------------------------------
SECommandListHandle* SED3D12Device::__CreateCommandList(
    SECommandList* commandList, SECommandAllocator* commandAllocator)
{
    SED3D12CommandListHandle* commandListHandle = SE_NEW SED3D12CommandListHandle();
    commandListHandle->DeviceBase = this;

    SED3D12CommandAllocatorHandle* commandAllocatorHandle =
        (SED3D12CommandAllocatorHandle*)commandAllocator->GetCommandAllocatorHandle();
    SE_ASSERT(commandAllocatorHandle);

    HRESULT hr = mD3DDevice->CreateCommandList(0,
        gsCommandListType[(int)commandList->GetType()],
        commandAllocatorHandle->mCommandAllocator.Get(), nullptr,
        IID_PPV_ARGS(&commandListHandle->mCommandList));
    SE_ASSERT(hr == S_OK);

    return commandListHandle;
}
//----------------------------------------------------------------------------
void SED3D12Device::__DeleteCommandList(SECommandList* commandList,
    SECommandAllocator*)
{
    SED3D12CommandListHandle* commandListHandle =
        (SED3D12CommandListHandle*)commandList->GetCommandListHandle();
    if( commandListHandle )
    {
        commandListHandle->mCommandList = nullptr;
    }
}
//----------------------------------------------------------------------------
void SED3D12Device::__ResetRenderCommandList(
    SERenderCommandList* renderCommandList, SERenderPassInfo* renderPassInfo)
{
    SED3D12CommandListHandle* commandListHandle =
        (SED3D12CommandListHandle*)renderCommandList->GetCommandListHandle();
    SE_ASSERT(commandListHandle);

    SED3D12RenderPassInfoHandle* renderPassInfoHandle =
        (SED3D12RenderPassInfoHandle*)renderPassInfo->GetPassInfoHandle();
    SE_ASSERT(renderPassInfoHandle);

    SECommandAllocator* commandAllocator = renderCommandList->GetCommandAllocator();
    SED3D12CommandAllocatorHandle* commandAllocatorHandle =
        (SED3D12CommandAllocatorHandle*)commandAllocator->GetCommandAllocatorHandle();
    SE_ASSERT(commandAllocatorHandle);

    HRESULT hr = ((ID3D12GraphicsCommandList*)commandListHandle->mCommandList.Get()
        )->Reset(commandAllocatorHandle->mCommandAllocator.Get(),
            renderPassInfoHandle->mPipelineState.Get());
    SE_ASSERT(hr == S_OK);
}
//----------------------------------------------------------------------------
void SED3D12Device::__CloseRenderCommandList(
    SERenderCommandList* renderCommandList)
{
    SED3D12CommandListHandle* commandListHandle =
        (SED3D12CommandListHandle*)renderCommandList->GetCommandListHandle();
    SE_ASSERT(commandListHandle);

    HRESULT hr = ((ID3D12GraphicsCommandList*)commandListHandle->mCommandList.Get())->Close();
    SE_ASSERT(hr == S_OK);
}
//----------------------------------------------------------------------------
void SED3D12Device::__RenderCommandListSetRootSignature(
    SERenderCommandList* renderCommandList, SERootSignature* rootSignature)
{
    SED3D12CommandListHandle* commandListHandle =
        (SED3D12CommandListHandle*)renderCommandList->GetCommandListHandle();
    SE_ASSERT(commandListHandle);

    SED3D12RootSignatureHandle* rootSignatureHandle =
        (SED3D12RootSignatureHandle*)rootSignature->GetRootSignatureHandle();
    SE_ASSERT(rootSignatureHandle);

    ((ID3D12GraphicsCommandList*)
        commandListHandle->mCommandList.Get())->SetGraphicsRootSignature(
            rootSignatureHandle->mRootSignature.Get());
}
//----------------------------------------------------------------------------
void SED3D12Device::__RenderCommandListSetViewport(
    SERenderCommandList* renderCommandList, SEViewportState* srcViewport)
{
    // TODO:
}
//----------------------------------------------------------------------------
SERootSignatureHandle* SED3D12Device::__CreateRootSignature(
    SERootSignature* rootSignature)
{
    const SERootSignatureInfo& info = rootSignature->GetRootSignatureInfo();

    D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;
    if( info.Flags & RSF_Allow_Input_Assembler_Input_Layout )
    {
        flags |= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    }
    if( info.Flags & RSF_Allow_Stream_Output )
    {
        flags |= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_STREAM_OUTPUT;
    }
    if( info.Flags & RSF_Deny_VS_Root_Access )
    {
        flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS;
    }
    if( info.Flags & RSF_Deny_TCS_Root_Access )
    {
        flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
    }
    if( info.Flags & RSF_Deny_TES_Root_Access )
    {
        flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
    }
    if( info.Flags & RSF_Deny_GS_Root_Access )
    {
        flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
    }
    if( info.Flags & RSF_Deny_FS_Root_Access )
    {
        flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;
    }

    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
    rootSignatureDesc.Init(0, nullptr, 0, nullptr, flags);

    ComPtr<ID3DBlob> signature;
    ComPtr<ID3DBlob> error;
    HRESULT hr = D3D12SerializeRootSignature(&rootSignatureDesc,
        D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
    SE_ASSERT(hr == S_OK);

    SED3D12RootSignatureHandle* rootSignatureHandle = SE_NEW SED3D12RootSignatureHandle();
    rootSignatureHandle->DeviceBase = this;

    hr = mD3DDevice->CreateRootSignature(0, signature->GetBufferPointer(),
        signature->GetBufferSize(), IID_PPV_ARGS(&rootSignatureHandle->mRootSignature));
    SE_ASSERT(hr == S_OK);

    return rootSignatureHandle;
}
//----------------------------------------------------------------------------
void SED3D12Device::__DeleteRootSignature(SERootSignature* rootSignature)
{
    SED3D12RootSignatureHandle* rootSignatureHandle =
        (SED3D12RootSignatureHandle*)rootSignature->GetRootSignatureHandle();
    if( rootSignatureHandle )
    {
        rootSignatureHandle->mRootSignature = nullptr;
    }
}
//----------------------------------------------------------------------------