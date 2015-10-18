// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SEPass.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEPass::SEPass(const SEShaderProgramInfo& programInfo)
    :
    SEPassBase(programInfo)
{
}
//----------------------------------------------------------------------------
SEPass::SEPass(SEShaderProgram* shaderProgram)
    :
    SEPassBase(shaderProgram)
{
}
//----------------------------------------------------------------------------
SEPass::~SEPass()
{
}
//----------------------------------------------------------------------------
void SEPass::Enable()
{
	mShaderProgram->Enable();
}
//----------------------------------------------------------------------------
void SEPass::Disable()
{
	mShaderProgram->Disable();
}
//----------------------------------------------------------------------------
bool SEPass::IsTessellationEnabled() const
{
    return mShaderProgram->IsTessellationEnabled();
}
//----------------------------------------------------------------------------