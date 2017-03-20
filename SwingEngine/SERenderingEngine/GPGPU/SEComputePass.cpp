// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERenderingEnginePCH.h"
#include "SEComputePass.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEComputePass::SEComputePass(const SEShaderProgramInfo& programInfo)
    :
    SEPassBase(programInfo)
{
    if( (programInfo.ShaderStageFlag & SEShaderStage::SS_Compute) != 0 )
    {
        mIsVertexPass = false;
    }
    else if( (programInfo.ShaderStageFlag & SEShaderStage::SS_Vertex) != 0 )
    {
        mIsVertexPass = true;
    }
    else
    {
        SE_ASSERT(false);
    }
}
//----------------------------------------------------------------------------
SEComputePass::SEComputePass(SEShaderProgram* computeProgram)
    :
    SEPassBase(computeProgram)
{
    if( computeProgram->IsComputeEnabled() )
    {
        mIsVertexPass = false;
    }
    else if( computeProgram->IsVertexEnabled() )
    {
        mIsVertexPass = true;
    }
    else
    {
        SE_ASSERT(false);
    }
}
//----------------------------------------------------------------------------
SEComputePass::SEComputePass()
    :
    SEPassBase(0)
{
}
//----------------------------------------------------------------------------
SEComputePass::~SEComputePass()
{
}
//----------------------------------------------------------------------------