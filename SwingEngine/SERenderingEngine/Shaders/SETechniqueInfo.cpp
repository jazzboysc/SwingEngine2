// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

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
void SETechniqueInfo::CreatePassInfo(SEGPUDevice* device, SETechnique* technique,
	SEGeometryAttributes* geometryAttr)
{
	unsigned int pcount = technique->GetPassCount();
	mPassInfoArray.reserve(pcount);
	for( unsigned int i = 0; i < pcount; ++i )
	{
		SEPass* pass = (SEPass*)technique->GetPass(i);
		SEShaderProgram* program = pass->GetShaderProgram();

		SEPassInfo* passInfo = SE_NEW SEPassInfo();
		passInfo->Create(device, program, geometryAttr->Prim, 
            geometryAttr->HasNormal, geometryAttr->HasTCoord, 
            geometryAttr->VertexComponentCount);
		mPassInfoArray.push_back(passInfo);
	}
}
//----------------------------------------------------------------------------
SEPassInfo* SETechniqueInfo::GetPassInfo(int i) const
{
	SE_ASSERT( i >= 0 && i < (int)mPassInfoArray.size());
	return mPassInfoArray[i];
}
//----------------------------------------------------------------------------