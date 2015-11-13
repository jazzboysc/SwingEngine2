// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SERenderPass.h"

using namespace Swing;

//----------------------------------------------------------------------------
SERenderPass::SERenderPass(const SEShaderProgramInfo& programInfo, 
    const SERenderPassTargetsInfo& targetsInfo)
    :
    SEPassBase(programInfo)
{
    mTargetsInfo = targetsInfo;
}
//----------------------------------------------------------------------------
SERenderPass::SERenderPass(SEShaderProgram* shaderProgram)
    :
    SEPassBase(shaderProgram)
{
}
//----------------------------------------------------------------------------
SERenderPass::~SERenderPass()
{
}
//----------------------------------------------------------------------------
void SERenderPass::Enable()
{
	mShaderProgram->Enable();
}
//----------------------------------------------------------------------------
void SERenderPass::Disable()
{
	mShaderProgram->Disable();
}
//----------------------------------------------------------------------------
bool SERenderPass::IsTessellationEnabled() const
{
    return mShaderProgram->IsTessellationEnabled();
}
//----------------------------------------------------------------------------