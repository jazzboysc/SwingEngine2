// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SETessellationControlShader.h"

using namespace Swing;

//----------------------------------------------------------------------------
SETessellationControlShader::SETessellationControlShader(
    const std::string& shaderFileName)
	: 
    SEShader(ST_TessellationControl, shaderFileName)
{
}
//----------------------------------------------------------------------------
SETessellationControlShader::~SETessellationControlShader()
{
}
//----------------------------------------------------------------------------