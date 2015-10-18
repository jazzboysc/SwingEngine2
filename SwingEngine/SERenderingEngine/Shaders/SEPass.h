// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_Pass_H
#define Swing_Pass_H

#include "SERenderingEngineLIB.h"
#include "SEPassBase.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 05/18/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEPass : public SEPassBase
{
public:
    SEPass(const SEShaderProgramInfo& programInfo);
	SEPass(SEShaderProgram* shaderProgram);
	~SEPass();

	void Enable();
	void Disable();

    bool IsTessellationEnabled() const;
};

typedef SESmartPointer<SEPass> SEPassPtr;

}

#endif