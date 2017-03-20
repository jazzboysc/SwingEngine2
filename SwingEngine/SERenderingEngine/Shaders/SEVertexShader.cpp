// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERenderingEnginePCH.h"
#include "SEVertexShader.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEVertexShader::SEVertexShader(const std::string& shaderFileName)
	: 
	SEShader(ST_Vertex, shaderFileName)
{
}
//----------------------------------------------------------------------------
SEVertexShader::~SEVertexShader()
{
}
//----------------------------------------------------------------------------