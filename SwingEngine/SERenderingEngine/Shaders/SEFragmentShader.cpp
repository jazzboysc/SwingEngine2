// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERenderingEnginePCH.h"
#include "SEFragmentShader.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEFragmentShader::SEFragmentShader(const std::string& shaderFileName)
	: 
	SEShader(ST_Fragment, shaderFileName)
{
}
//----------------------------------------------------------------------------
SEFragmentShader::~SEFragmentShader()
{
}
//----------------------------------------------------------------------------