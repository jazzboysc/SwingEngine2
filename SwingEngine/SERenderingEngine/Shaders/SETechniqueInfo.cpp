// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERenderingEnginePCH.h"
#include "SETechniqueInfo.h"

using namespace Swing;

//----------------------------------------------------------------------------
SETechniqueInfo::SETechniqueInfo()
{
}
//----------------------------------------------------------------------------
SETechniqueInfo::~SETechniqueInfo()
{
}
//----------------------------------------------------------------------------
void SETechniqueInfo::CreateRenderPassInfo(SEGPUDevice* device, 
    SETechnique* technique, SEGeometryAttributes* geometryAttr)
{
	unsigned int pcount = technique->GetPassCount();
	mPassInfoArray.reserve(pcount);
	for( unsigned int i = 0; i < pcount; ++i )
	{
		SERenderPass* pass = (SERenderPass*)technique->GetPass(i);
		SEShaderProgram* program = pass->GetShaderProgram();
        SEPipelineStateBlock* psb = pass->GetPipelineStateBlock();
        SERenderPassTargetsInfo* targetsInfo = pass->GetRenderPassTargetsInfo();

		SERenderPassInfo* renderPassInfo = SE_NEW SERenderPassInfo();
		renderPassInfo->CreateDeviceChild(device, program, geometryAttr, psb, 
            0, targetsInfo);
		mPassInfoArray.push_back(renderPassInfo);
	}
}
//----------------------------------------------------------------------------
SERenderPassInfo* SETechniqueInfo::GetPassInfo(int i) const
{
	SE_ASSERT( i >= 0 && i < (int)mPassInfoArray.size());
	return mPassInfoArray[i];
}
//----------------------------------------------------------------------------