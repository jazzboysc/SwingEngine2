// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_FragmentShader_H
#define Swing_FragmentShader_H

#include "SERenderingEngineLIB.h"
#include "SEShader.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 09/14/2013
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEFragmentShader : public SEShader
{
public:
	SEFragmentShader(const std::string& shaderFileName);
	virtual ~SEFragmentShader();
};

typedef SESmartPointer<SEFragmentShader> SEFragmentShaderPtr;

}

#endif