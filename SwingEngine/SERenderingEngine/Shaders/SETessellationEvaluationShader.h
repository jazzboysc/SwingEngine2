// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_TessellationEvaluationShader_H
#define Swing_TessellationEvaluationShader_H

#include "SERenderingEngineLIB.h"
#include "SEShader.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 09/05/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SETessellationEvaluationShader : public SEShader
{
public:
    SETessellationEvaluationShader(const std::string& shaderFileName);
    virtual ~SETessellationEvaluationShader();
};

typedef SESmartPointer<SETessellationEvaluationShader> SETessellationEvaluationShaderPtr;

}

#endif