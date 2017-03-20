// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERenderingEnginePCH.h"
#include "SEGeometryShader.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEGeometryShader::SEGeometryShader(const std::string& shaderFileName)
	: 
	SEShader(ST_Geometry, shaderFileName)
{
}
//----------------------------------------------------------------------------
SEGeometryShader::~SEGeometryShader()
{
}
//----------------------------------------------------------------------------