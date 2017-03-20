// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_RenderPass_H
#define Swing_RenderPass_H

#include "SERenderingEngineLIB.h"
#include "SEPassBase.h"
#include "SEFrameBuffer.h"

namespace Swing
{

struct SE_RENDERING_ENGINE_API SERenderPassTargetsInfo
{
    unsigned int ColorTargetCount;
    SEBufferFormat ColorTargetFormats[SEFrameBuffer::FBO_MAX_COLOR_TARGETS];
    SEBufferFormat DepthFormat;
    SEBufferFormat StencilFormat;

    SERenderPassTargetsInfo()
        :
        DepthFormat(BF_Depth),
        StencilFormat(BF_Unknown)
    {
    }
};

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 05/18/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SERenderPass : public SEPassBase
{
public:
    SERenderPass(const SEShaderProgramInfo& programInfo, 
        const SERenderPassTargetsInfo& targetsInfo);
	SERenderPass(SEShaderProgram* shaderProgram);
	~SERenderPass();

	void Enable();
	void Disable();

    SERenderPassTargetsInfo* GetRenderPassTargetsInfo();

    bool IsTessellationEnabled() const;

protected:
    SERenderPassTargetsInfo mTargetsInfo;
};

typedef SESmartPointer<SERenderPass> SEPassPtr;

}

#endif