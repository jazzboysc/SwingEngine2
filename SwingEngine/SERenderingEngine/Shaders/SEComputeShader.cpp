// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERenderingEnginePCH.h"
#include "SEComputeShader.h"

#ifndef __APPLE__

using namespace Swing;

//----------------------------------------------------------------------------
SEComputeShader::SEComputeShader(const std::string& shaderFileName)
	: 
	SEShader(ST_Compute, shaderFileName)
{
}
//----------------------------------------------------------------------------
SEComputeShader::~SEComputeShader()
{
}
//----------------------------------------------------------------------------

#endif