// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERenderingEnginePCH.h"
#include "SEPipelineStateBlock.h"

#include <cstring>

using namespace Swing;

//----------------------------------------------------------------------------
SEViewportState::SEViewportState()
{
    memset(this, 0, sizeof(SEViewportState));
}
//----------------------------------------------------------------------------
SERasterizerState::SERasterizerState()
{
    RasterizerOpFlag = 0;
    FillMode = PFM_Solid;
    CullMode = PCM_Cull_Back;
    FrontFaceClockwise = true;
}
//----------------------------------------------------------------------------
SEOutputMergerState::SEOutputMergerState()
{
    OutputMergerOpFlag = 0;
    ClearMask = 0;
}
//----------------------------------------------------------------------------
SEPipelineStateBlock::SEPipelineStateBlock()
{
    PipelineStageFlag = 0;
}
//----------------------------------------------------------------------------
SEPipelineStateBlock::~SEPipelineStateBlock()
{
}
//----------------------------------------------------------------------------
void SEPipelineStateBlock::CreateDeviceResource(SEGPUDevice*)
{
    // TODO:
}
//----------------------------------------------------------------------------
SEInternalStateHandle::SEInternalStateHandle()
{
    Handle = 0;
}
//----------------------------------------------------------------------------
SEDepthStencilState::SEDepthStencilState()
{
    DepthEnable = true;
    StencilEnable = false;
}
//----------------------------------------------------------------------------
SEFrameBufferTargetBlendState::SEFrameBufferTargetBlendState()
{
    ColorMask = 0;
}
//----------------------------------------------------------------------------
SEBlendState::SEBlendState()
{
    BlendStateOpFlag = 0;
}
//----------------------------------------------------------------------------