// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_TechniqueInfo_H
#define Swing_TechniqueInfo_H

#include "SERenderingEngineLIB.h"
#include "SEGPUDevice.h"
#include "SETechnique.h"
#include "SEPassInfo.h"
#include "SEGeometryAttributes.h"
#include <vector>

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 08/09/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SETechniqueInfo
{
public:
	SETechniqueInfo();
	~SETechniqueInfo();

	void CreatePassInfo(SEGPUDevice* device, SETechnique* technique, 
        SEGeometryAttributes* geometryAttr);

	SEPassInfo* GetPassInfo(int i) const;

private:
	std::vector<SEPassInfo*> mPassInfoArray;
};

}

#endif