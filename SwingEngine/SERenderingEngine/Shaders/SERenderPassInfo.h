// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_RenderPassInfo_H
#define Swing_RenderPassInfo_H

#include "SERenderingEngineLIB.h"
#include "SEGPUDeviceBase.h"
#include "SEGPUDeviceBaseChild.h"
#include "SEPrimitive.h"
#include "SEShaderProgram.h"
#include "SEGeometryAttributes.h"
#include "SEPipelineStateBlock.h"
#include "SERootSignature.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 08/09/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SERenderPassInfo
{
public:
	SERenderPassInfo();
	~SERenderPassInfo();

	void CreateDeviceChild(SEGPUDeviceBase* device, SEShaderProgram* program,
        SEGeometryAttributes* geometryAttr, SEPipelineStateBlock* psb, 
        SERootSignature* rootSignature, SERenderPassTargetsInfo* targetsInfo);

	void Enable();
	void Disable();

    SERenderPassInfoHandle* GetPassInfoHandle() const;

private:
    SERenderPassInfoHandle* mPassInfoHandle;
};

}

#endif