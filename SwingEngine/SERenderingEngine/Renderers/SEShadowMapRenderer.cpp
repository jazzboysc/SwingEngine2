// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SEShadowMapRenderer.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEShadowMapRenderer::SEShadowMapRenderer(SEGPUDevice* device, SERenderSet* renderSet)
    :
    SESubRenderer(device, renderSet)
{
    mPSB = SE_NEW SEPipelineStateBlock();
    mPSB->PipelineStageFlag |= PB_OutputMerger;
    mPSB->OutputMerger.OutputMergerOpFlag |= OMB_Clear;
    mPSB->OutputMerger.ClearMask = OMCB_Color_Buffer | OMCB_Depth_Buffer;
}
//----------------------------------------------------------------------------
SEShadowMapRenderer::~SEShadowMapRenderer()
{
    mPSB = 0;
}
//----------------------------------------------------------------------------
void SEShadowMapRenderer::CreateShadowMap(int width, int height,
    SEBufferFormat format)
{
    AddFrameBufferTarget(RTGI_ShadowMapRenderer_ShadowMap_Name, width, height,
        0, TT_Texture2D, format);
    CreateFrameBuffer(width, height, 0, TT_Texture2D);
}
//----------------------------------------------------------------------------
void SEShadowMapRenderer::Render(int technique, int pass, SERTGICamera* camera)
{
    SESubRenderer::Render(technique, pass, SRO_FrameBuffer, mPSB, camera);
}
//----------------------------------------------------------------------------