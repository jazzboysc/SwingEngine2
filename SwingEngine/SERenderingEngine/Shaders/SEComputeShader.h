// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_ComputeShader_H
#define Swing_ComputeShader_H

#ifndef __APPLE__

#include "SERenderingEngineLIB.h"
#include "SEShader.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 07/16/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEComputeShader : public SEShader
{
public:
	SEComputeShader(const std::string& shaderFileName);
	virtual ~SEComputeShader();
};

typedef SESmartPointer<SEComputeShader> SEComputeShaderPtr;

}

#endif

#endif