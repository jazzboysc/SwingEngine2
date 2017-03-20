// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERenderingEnginePCH.h"
#include "SESubRenderer.h"
#include "SEStructuredBuffer.h"
#include "SEAtomicCounterBuffer.h"
#include "SEUniformBuffer.h"
#include "SEDispatchIndirectBuffer.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEBufferBase* CreateStructuredBuffer(SEGPUDevice* device, int size, 
    SEBufferUsage usage)
{
    SEStructuredBuffer* buffer = SE_NEW SEStructuredBuffer();
    buffer->ReserveMutableDeviceResource(device, (size_t)size, usage);
    return (SEBufferBase*)buffer;
}
//----------------------------------------------------------------------------
SEBufferBase* CreateAtomicCounterBuffer(SEGPUDevice* device, int size, 
    SEBufferUsage usage)
{
    SEAtomicCounterBuffer* buffer = SE_NEW SEAtomicCounterBuffer();
    buffer->ReserveMutableDeviceResource(device, (size_t)size, usage);
    return (SEBufferBase*)buffer;
}
//----------------------------------------------------------------------------
SEBufferBase* CreateUniformBuffer(SEGPUDevice* device, int size, 
    SEBufferUsage usage)
{
    SEUniformBuffer* buffer = SE_NEW SEUniformBuffer();
    buffer->ReserveMutableDeviceResource(device, (size_t)size, usage);
    return (SEBufferBase*)buffer;
}
//----------------------------------------------------------------------------
SEBufferBase* CreateDispatchIndirectBuffer(SEGPUDevice* device, int size, 
    SEBufferUsage usage)
{
    SEDispatchIndirectBuffer* buffer = SE_NEW SEDispatchIndirectBuffer();
    buffer->ReserveMutableDeviceResource(device, (size_t)size, usage);
    return (SEBufferBase*)buffer;
}
//----------------------------------------------------------------------------
SEBufferBase* CreateDrawIndirectBuffer(SEGPUDevice*, int, SEBufferUsage)
{
    // TODO:
    SE_ASSERT(false);
    return 0;
}
//----------------------------------------------------------------------------
SEBufferBase* CreatePixelBuffer(SEGPUDevice* device, int size, SEBufferUsage usage)
{
    SEPixelBuffer* buffer = SE_NEW SEPixelBuffer();
    buffer->ReserveMutableDeviceResource(device, (size_t)size, usage);
    return (SEBufferBase*)buffer;
}
//----------------------------------------------------------------------------
SEBufferBase* CreateTextureBuffer(SEGPUDevice* device, int size, 
    SEBufferUsage usage)
{
    SETextureBuffer* buffer = SE_NEW SETextureBuffer();
    buffer->ReserveMutableDeviceResource(device, (size_t)size, usage);
    return (SEBufferBase*)buffer;
}
//----------------------------------------------------------------------------

SubRendererCreateRendererData SESubRenderer::msFactoryFunctions[7] =
{
    CreateAtomicCounterBuffer,
    CreateDispatchIndirectBuffer,
    CreateDrawIndirectBuffer,
    CreatePixelBuffer,
    CreateStructuredBuffer,
    CreateTextureBuffer,
    CreateUniformBuffer
};

//----------------------------------------------------------------------------
SESubRenderer::SESubRenderer()
{
}
//----------------------------------------------------------------------------
SESubRenderer::SESubRenderer(SEGPUDevice* device, SERenderSet* renderSet)
    :
    mDevice(device)
{
    mFrameBuffer = SE_NEW SEFrameBuffer(device);
    mRenderSet = renderSet;
}
//----------------------------------------------------------------------------
SESubRenderer::~SESubRenderer()
{
    for( int i = 0; i < (int)mInputs.size(); ++i )
    {
        mInputs[i] = 0;
    }

    for( int i = 0; i < (int)mFrameBufferTargets.size(); ++i )
    {
        mFrameBufferTargets[i] = 0;
    }
    mDepthTarget = 0;
    mFrameBuffer = 0;
    mRenderSet = 0;
    mTimer = 0;
}
//----------------------------------------------------------------------------
void SESubRenderer::SetRenderSet(SERenderSet* renderSet)
{
    mRenderSet = renderSet;
}
//----------------------------------------------------------------------------
SERenderSet* SESubRenderer::GetRenderSet() const
{
    return mRenderSet;
}
//----------------------------------------------------------------------------
void SESubRenderer::AddFrameBufferTarget(const std::string& name, int width,
    int height, int depth, SETextureType type, SEBufferFormat format, 
    bool generateMipmap)
{
    SE_ASSERT(width > 0 && format != BF_Unknown);
    SE_ASSERT(GetFrameBufferTargetByName(name) == 0);
    SE_ASSERT(GetGenericBufferTargetByName(name) == 0);

    SEBufferBase* texture = 0;
    switch( type )
    {
    case TT_Texture2D:
        texture = SE_NEW SETexture2D();
        ((SETexture2D*)texture)->CreateRenderTarget(mDevice, width, height, 
            format, generateMipmap);
        break;
    case TT_Texture2DArray:
        texture = SE_NEW SETexture2DArray();
        ((SETexture2DArray*)texture)->CreateRenderTarget(mDevice, width, height, 
            depth, format, generateMipmap);
        break;
    default:
        SE_ASSERT(false);
        break;
    }

    SERendererOutput* ro = SE_NEW SERendererOutput(name, texture);
    mFrameBufferTargets.push_back(ro);

    if( generateMipmap )
    {
        mFrameBufferTargetsMipmapped.push_back((SETexture*)texture);
    }
}
//----------------------------------------------------------------------------
int SESubRenderer::GetFrameBufferTargetCount() const
{
    return (int)mFrameBufferTargets.size();
}
//----------------------------------------------------------------------------
SERendererOutput* SESubRenderer::GetFrameBufferTarget(int i) const
{
    SE_ASSERT(i >= 0 && i < (int)mFrameBufferTargets.size());
    return mFrameBufferTargets[i];
}
//----------------------------------------------------------------------------
SERendererOutput* SESubRenderer::GetFrameBufferTargetByName(
    const std::string& name) const
{
    for( int i = 0; i < (int)mFrameBufferTargets.size(); ++i )
    {
        if( mFrameBufferTargets[i]->Name == name )
        {
            return mFrameBufferTargets[i];
        }
    }

    return 0;
}
//----------------------------------------------------------------------------
SERendererOutput* SESubRenderer::GetDepthTarget() const
{
    return mDepthTarget;
}
//----------------------------------------------------------------------------
SETexture* SESubRenderer::GetFrameBufferTexture(int i) const
{
    SERendererOutput* output = GetFrameBufferTarget(i);
    if( output )
    {
        return (SETexture*)(SEBufferBase*)output->OutputBuffer;
    }

    return 0;
}
//----------------------------------------------------------------------------
SETexture* SESubRenderer::GetFrameBufferTextureByName(const std::string& name) const
{
    SERendererOutput* output = GetFrameBufferTargetByName(name);
    if( output )
    {
        return (SETexture*)(SEBufferBase*)output->OutputBuffer;
    }

    return 0;
}
//----------------------------------------------------------------------------
SETexture* SESubRenderer::GetDepthTexture() const
{
    return (SETexture*)(SEBufferBase*)GetDepthTarget()->OutputBuffer;
}
//----------------------------------------------------------------------------
void SESubRenderer::AddInputDependency(SESubRenderer* producer, 
    const std::string& srcName, SERendererInputDataView* view)
{
    SE_ASSERT(view->BindingType >= BF_Bindless && 
        view->BindingType < BindingFlag_Max);
    SE_ASSERT(mInputs.size() < MAX_INPUT_DEPENDENCY_COUNT);
    SERendererOutput* producerOutput = 
        producer->GetFrameBufferTargetByName(srcName);
    if( !producerOutput )
    {
        producerOutput = producer->GetGenericBufferTargetByName(srcName);
    }
    SE_ASSERT(producerOutput);

    SERendererInput* consumerInput = SE_NEW SERendererInput(producerOutput->Name,
        producerOutput->OutputBuffer, view);
    if( consumerInput->InputBufferView )
    {
        consumerInput->InputBufferView->CreateDeviceResource(mDevice, 
            consumerInput->InputBuffer);
    }
    mInputs.push_back(consumerInput);
}
//----------------------------------------------------------------------------
SERendererInput* SESubRenderer::GetInputDependency(int i) const
{
    SE_ASSERT(i >= 0 && i < (int)mInputs.size());
    return mInputs[i];
}
//----------------------------------------------------------------------------
void SESubRenderer::ClearInputDependency()
{
    for( int i = 0; i < (int)mInputs.size(); ++i )
    {
        mInputs[i] = 0;
    }
    mInputs.clear();
}
//----------------------------------------------------------------------------
void SESubRenderer::CreateFrameBuffer(int depthWidth, int depthHeight, 
    int depthCount, SETextureType depthType)
{
    int rtCount = (int)mFrameBufferTargets.size();
    SE_ASSERT(rtCount > 0);
    SETexture** renderTargets = SE_NEW SETexture*[rtCount];
    for( int i = 0; i < rtCount; ++i )
    {
        renderTargets[i] = 
            (SETexture*)(SEBufferBase*)mFrameBufferTargets[i]->OutputBuffer;
    }

    SEBufferBase* depthTexture = 0;
    switch( depthType )
    {
    case TT_Texture2D:
        depthTexture = SE_NEW SETexture2D();
        ((SETexture2D*)depthTexture)->CreateRenderTarget(mDevice, depthWidth, 
            depthHeight, BF_Depth);
        break;

    case TT_Texture2DArray:
        depthTexture = SE_NEW SETexture2DArray();
        ((SETexture2DArray*)depthTexture)->CreateRenderTarget(mDevice, 
            depthWidth, depthHeight, depthCount, BF_Depth);
        break;

    default:
        SE_ASSERT(false);
        break;
    }
    mDepthTarget = SE_NEW SERendererOutput("Depth", depthTexture);

    mFrameBuffer->SetRenderTargets(rtCount, renderTargets, 
        (SETexture*)(SEBufferBase*)mDepthTarget->OutputBuffer);
    SE_DELETE[] renderTargets;
}
//----------------------------------------------------------------------------
void SESubRenderer::AddGenericBufferTarget(const std::string& name, 
    SERendererDataType bufferType, int size, SEBufferUsage usage, 
    SEBindingFlag flag, unsigned int binding, bool reset, int resetValue)
{
    SE_ASSERT(GetFrameBufferTargetByName(name) == 0);
    SE_ASSERT(GetGenericBufferTargetByName(name) == 0);

    SEBufferBase* genericBufferTarget = msFactoryFunctions[(int)bufferType](
        mDevice, size, usage);

    SERendererOutput* ro = SE_NEW SERendererOutput(name, genericBufferTarget, 
        ROT_Buffer, flag, (SEBufferType)bufferType, binding, reset, resetValue);
    if( ro->OutputBufferView )
    {
        ro->OutputBufferView->CreateDeviceResource(mDevice, ro->OutputBuffer);
    }
    mGenericBufferTargets.push_back(ro);
}
//----------------------------------------------------------------------------
int SESubRenderer::GetGenericBufferTargetCount() const
{
    return (int)mGenericBufferTargets.size();
}
//----------------------------------------------------------------------------
SERendererOutput* SESubRenderer::GetGenericBufferTarget(int i) const
{
    SE_ASSERT(i >= 0 && i < (int)mGenericBufferTargets.size());
    return mGenericBufferTargets[i];
}
//----------------------------------------------------------------------------
SERendererOutput* SESubRenderer::GetGenericBufferTargetByName(
    const std::string& name) const
{
    for( int i = 0; i < (int)mGenericBufferTargets.size(); ++i )
    {
        if( mGenericBufferTargets[i]->Name == name )
        {
            return mGenericBufferTargets[i];
        }
    }

    return 0;
}
//----------------------------------------------------------------------------
SEBuffer* SESubRenderer::GetGenericBuffer(int i) const
{
    SERendererOutput* output = GetGenericBufferTarget(i);
    if( output )
    {
        return (SEBuffer*)(SEBufferBase*)output->OutputBuffer;
    }

    return 0;
}
//----------------------------------------------------------------------------
SEBuffer* SESubRenderer::GetGenericBufferByName(const std::string& name) const
{
    SERendererOutput* output = GetGenericBufferTargetByName(name);
    if( output )
    {
        return (SEBuffer*)(SEBufferBase*)output->OutputBuffer;
    }

    return 0;
}
//----------------------------------------------------------------------------
void SESubRenderer::Render(int technique, int pass, 
    unsigned int outputFlag, SEPipelineStateBlock* psb, SERTGICamera* camera)
{
    PreRender(outputFlag, psb);

    // Render scene inputs.
    OnRender(technique, pass, camera);

    PostRender(outputFlag, psb);
}
//----------------------------------------------------------------------------
void SESubRenderer::RenderSingle(SERenderObject* object, int technique, int pass,
    SESubRendererOutput outputFlag, SEPipelineStateBlock* psb,
    SERTGICamera* camera)
{
    SE_ASSERT(object);
    PreRender(outputFlag, psb);

    // Render single object.
    object->SetCamera(camera);
    object->Render(technique, pass, this);

    PostRender(outputFlag, psb);
}
//----------------------------------------------------------------------------
void SESubRenderer::OnRender(int technique, int pass, SERTGICamera* camera)
{
    SE_ASSERT(mRenderSet);
    int renderObjectCount = mRenderSet->GetRenderObjectCount();
    if( camera )
    {
        // Assign viewing camera. 
        for( int i = 0; i < renderObjectCount; ++i )
        {
            SERenderObject* renderObject = mRenderSet->GetRenderObject(i);
            renderObject->SetCamera(camera);
        }
    }
    for( int i = 0; i < renderObjectCount; ++i )
    {
        SERenderObject* renderObject = mRenderSet->GetRenderObject(i);
        renderObject->Render(technique, pass, this);
    }
}
//----------------------------------------------------------------------------
void SESubRenderer::SetTimer(SEGPUTimer* timer)
{
    mTimer = timer;
}
//----------------------------------------------------------------------------
SEGPUTimer* SESubRenderer::GetTimer() const
{
    return mTimer;
}
//----------------------------------------------------------------------------
double SESubRenderer::GetTimeElapsed() const
{
    if( mTimer )
    {
        return mTimer->GetTimeElapsed();
    }

    return 0.0;
}
//----------------------------------------------------------------------------
void SESubRenderer::PreRender(unsigned int outputFlag, 
    SEPipelineStateBlock* psb)
{
    SE_ASSERT(mTimer);
    if( mTimer )
    {
        mTimer->Start();
    }

    // Enable renderer inputs.
    for( int i = 0; i < (int)mInputs.size(); ++i )
    {
        mInputs[i]->Enable();
    }

    // Enable renderer framebuffer outputs.
    if( outputFlag & SRO_FrameBuffer )
    {
        SE_ASSERT(mFrameBuffer);
        mFrameBuffer->Enable();
    }

    // Enable renderer generic buffer outputs.
    if( outputFlag & SRO_GenericBuffer )
    {
        for( int i = 0; i < (int)mGenericBufferTargets.size(); ++i )
        {
            mGenericBufferTargets[i]->Enable();
        }
    }

    // Apply PSB.
    if( psb )
    {
        mDevice->ApplyPipelineStateBlock(psb);
    }
}
//----------------------------------------------------------------------------
void SESubRenderer::PostRender(unsigned int outputFlag, 
    SEPipelineStateBlock*)
{
    // Disable renderer inputs.
    for( int i = 0; i < (int)mInputs.size(); ++i )
    {
        mInputs[i]->Disable();
    }

    // Disable renderer framebuffer outputs and possibly generate mipmaps.
    if( outputFlag & SRO_FrameBuffer )
    {
        mFrameBuffer->Disable();

        for( int i = 0; i < (int)mFrameBufferTargetsMipmapped.size(); ++i )
        {
            mFrameBufferTargetsMipmapped[i]->GenerateMipmap();
        }
    }

    // Disable renderer generic buffer outputs.
    if( outputFlag & SRO_GenericBuffer )
    {
        for( int i = 0; i < (int)mGenericBufferTargets.size(); ++i )
        {
            mGenericBufferTargets[i]->Disable();
        }
    }

    if( mTimer )
    {
        mTimer->Stop();
    }
}
//----------------------------------------------------------------------------