// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_GeometryShader_H
#define Swing_GeometryShader_H

#include "SERenderingEngineLIB.h"
#include "SEShader.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 09/05/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEGeometryShader : public SEShader
{
public:
    SEGeometryShader(const std::string& shaderFileName);
    virtual ~SEGeometryShader();
};

typedef SESmartPointer<SEGeometryShader> SEGeometryShaderPtr;

}

#endif