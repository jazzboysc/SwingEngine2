// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_TessellationControlShader_H
#define Swing_TessellationControlShader_H

#include "SERenderingEngineLIB.h"
#include "SEShader.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 09/05/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SETessellationControlShader : public SEShader
{
public:
    SETessellationControlShader(const std::string& shaderFileName);
    virtual ~SETessellationControlShader();
};

typedef SESmartPointer<SETessellationControlShader> SETessellationControlShaderPtr;

}

#endif