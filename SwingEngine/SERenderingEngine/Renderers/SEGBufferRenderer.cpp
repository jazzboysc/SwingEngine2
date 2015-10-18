// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SEGBufferRenderer.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEGBufferRenderer::SEGBufferRenderer(SEGPUDevice* device, SERenderSet* renderSet)
    :
    SESubRenderer(device, renderSet)
{
    mPSB = SE_NEW SEPipelineStateBlock();
    mPSB->PipelineStageFlag |= PB_OutputMerger;
    mPSB->OutputMerger.OutputMergerOpFlag |= OMB_Clear;
    mPSB->OutputMerger.ClearMask = OMCB_Color_Buffer | OMCB_Depth_Buffer;
}
//----------------------------------------------------------------------------
SEGBufferRenderer::~SEGBufferRenderer()
{
    mPSB = 0;
}
//----------------------------------------------------------------------------
void SEGBufferRenderer::Initialize(SEGBufferDesc* desc)
{
    SE_ASSERT(desc);
    mGBufferDesc = *desc;

    AddFrameBufferTarget(RTGI_GBuffer_Position_Name, desc->Width, 
        desc->Height, 0, TT_Texture2D, desc->PositionFormat, 
        desc->PositionMipmap);
    AddFrameBufferTarget(RTGI_GBuffer_Normal_Name, desc->Width, desc->Height, 
        0, TT_Texture2D, desc->NormalFormat, desc->NormalMipmap);
    AddFrameBufferTarget(RTGI_GBuffer_Albedo_Name, desc->Width, desc->Height, 
        0, TT_Texture2D, desc->AlbedoFormat, desc->AlbedoMipmap);
    if( desc->RPCFormat != BF_Unknown )
    {
        AddFrameBufferTarget(RTGI_GBuffer_RPC_Name, desc->Width, desc->Height,
            0, TT_Texture2D, desc->RPCFormat, desc->RPCMipmap);
    }

    CreateFrameBuffer(desc->Width, desc->Height, 0, TT_Texture2D);
}
//----------------------------------------------------------------------------
void SEGBufferRenderer::Render(int technique, int pass, SERTGICamera* camera)
{
    SESubRenderer::Render(technique, pass, SRO_FrameBuffer, mPSB, camera);
}
//----------------------------------------------------------------------------
const SEGBufferDesc& SEGBufferRenderer::GetGBufferDesc() const
{
    return mGBufferDesc;
}
//----------------------------------------------------------------------------