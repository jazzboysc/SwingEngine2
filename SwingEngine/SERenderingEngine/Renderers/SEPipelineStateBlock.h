// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_PipelineStateBlock_H
#define Swing_PipelineStateBlock_H

#include "SERenderingEngineLIB.h"
#include "SEReferencable.h"
#include "SEVector4.h"
#include "SEFrameBuffer.h"

namespace Swing
{

enum SEPipelineBit
{
    PB_Rasterizer = 1,
    PB_OutputMerger = 2
};

enum SEPolygonFillMode
{
    PFM_Wireframe = 1,
    PFM_Solid = 2
};

enum SEPolygonCullMode
{
    PCM_Cull_None = 1,
    PCM_Cull_Front = 2,
    PCM_Cull_Back = 3,
};

enum SERasterizerBit
{
    RB_Viewport = 1,
    RB_FillMode = 2,
    RB_CullMode = 4,
    RB_FrontFace = 8
};

enum SEOutputMergerBit
{
    OMB_Clear = 1,
    OMB_ClearColor = 2,
    OMB_DepthStencil = 4,
    OMB_Blend = 8
};

enum SEOutputMergerClearBit
{
    OMCB_Color_Buffer = 1,
    OMCB_Depth_Buffer = 2,
    OMCB_Stencil_Buffer = 4
};

struct SE_RENDERING_ENGINE_API SEInternalStateHandle
{
    SEInternalStateHandle();

    void* Handle;
};

struct SE_RENDERING_ENGINE_API SEViewportState
{
    SEViewportState();

    int X;
    int Y;
    int Width;
    int Height;
};

struct SE_RENDERING_ENGINE_API SERasterizerState
{
    SERasterizerState();

    unsigned int RasterizerOpFlag;
    SEPolygonFillMode FillMode;
    SEPolygonCullMode CullMode;
    bool FrontFaceClockwise;
    SEViewportState Viewport;
};

struct SE_RENDERING_ENGINE_API SEDepthStencilState : public SEInternalStateHandle
{
    SEDepthStencilState();

    bool DepthEnable;
    bool StencilEnable;
};

enum SEColorMaskBit
{
    CMB_Red = 1,
    CMB_Green = 2,
    CMB_Blue = 4,
    CMB_Alpha = 8,
};

struct SE_RENDERING_ENGINE_API SEFrameBufferTargetBlendState
{
    SEFrameBufferTargetBlendState();

    SE_UInt8 ColorMask; // default: 0x00 enable writes to R,G,B,A.
};

enum SEBlendStateBit
{
    BSB_ColorMask = 1
};

struct SE_RENDERING_ENGINE_API SEBlendState : public SEInternalStateHandle
{
    SEBlendState();

    unsigned int BlendStateOpFlag;

    // TODO:
    // Only support target 0 for now.
    SEFrameBufferTargetBlendState FrameBufferTarget[SEFrameBuffer::FBO_MAX_COLOR_TARGETS];
};

struct SE_RENDERING_ENGINE_API SEOutputMergerState
{
    SEOutputMergerState();

    unsigned int OutputMergerOpFlag;
    unsigned int ClearMask;
    SEVector4f ClearColor;
    SEDepthStencilState DepthStencil;
    SEBlendState Blend;
};

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 12/05/2014
//----------------------------------------------------------------------------
struct SE_RENDERING_ENGINE_API SEPipelineStateBlock : public SEGPUDeviceResident
{
    SEPipelineStateBlock();
    ~SEPipelineStateBlock();

    unsigned int PipelineStageFlag;
    SERasterizerState Rasterizer;
    SEOutputMergerState OutputMerger;

    void CreateDeviceResource(SEGPUDevice* device);
};

typedef SESmartPointer<SEPipelineStateBlock> SEPipelineStateBlockPtr;

}

#endif