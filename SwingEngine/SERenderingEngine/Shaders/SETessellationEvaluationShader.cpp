// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SETessellationEvaluationShader.h"

using namespace Swing;

//----------------------------------------------------------------------------
SETessellationEvaluationShader::SETessellationEvaluationShader(
    const std::string& shaderFileName)
	: 
    SEShader(ST_TessellationEvaluation, shaderFileName)
{
}
//----------------------------------------------------------------------------
SETessellationEvaluationShader::~SETessellationEvaluationShader()
{
}
//----------------------------------------------------------------------------