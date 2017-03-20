// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_VertexShader_H
#define Swing_VertexShader_H

#include "SERenderingEngineLIB.h"
#include "SEShader.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 09/14/2013
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEVertexShader : public SEShader
{
public:
	SEVertexShader(const std::string& shaderFileName);
	virtual ~SEVertexShader();
};

typedef SESmartPointer<SEVertexShader> SEVertexShaderPtr;

}

#endif