// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_PassInfo_H
#define Swing_PassInfo_H

#include "SERenderingEngineLIB.h"
#include "SEGPUDevice.h"
#include "SEPrimitive.h"
#include "SEShaderProgram.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 08/09/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEPassInfo
{
public:
	SEPassInfo();
	~SEPassInfo();

	void Create(SEGPUDevice* device, SEShaderProgram* program, 
        SEPrimitive* primitive, bool hasNormal, bool hasTCoord, 
        int vertexComponentCount);

	void Enable();
	void Disable();

    SEPassInfoHandle* GetPassInfoHandle() const;

private:
    SEPassInfoHandle* mPassInfoHandle;
};

}

#endif