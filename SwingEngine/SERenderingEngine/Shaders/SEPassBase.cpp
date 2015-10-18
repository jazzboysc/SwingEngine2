// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SEPassBase.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEPassBase::SEPassBase(const SEShaderProgramInfo& programInfo)
{
    mShaderProgram = SE_NEW SEShaderProgram(programInfo);
}
//----------------------------------------------------------------------------
SEPassBase::SEPassBase(SEShaderProgram* shaderProgram)
{
    mShaderProgram = shaderProgram;
}
//----------------------------------------------------------------------------
SEPassBase::~SEPassBase()
{
    mShaderProgram = 0;
    mPSB = 0;
}
//----------------------------------------------------------------------------
SEShaderProgram* SEPassBase::GetShaderProgram()
{
    return mShaderProgram;
}
//----------------------------------------------------------------------------
void SEPassBase::CreateDeviceResource(SEGPUDevice* device)
{
    mShaderProgram->CreateDeviceResource(device);
}
//----------------------------------------------------------------------------
void SEPassBase::SetPipelineStateBlock(SEPipelineStateBlock* psb)
{
    mPSB = psb;
}
//----------------------------------------------------------------------------
SEPipelineStateBlock* SEPassBase::GetPipelineStateBlock() const
{
    return mPSB;
}
//----------------------------------------------------------------------------