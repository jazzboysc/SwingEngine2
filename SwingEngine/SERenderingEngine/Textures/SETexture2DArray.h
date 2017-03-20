// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_Texture2DArray_H
#define Swing_Texture2DArray_H

#include "SERenderingEngineLIB.h"
#include "SETexture.h"
#include "SEGPUDevice.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 09/22/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SETexture2DArray : public SETexture
{
public:
    SETexture2DArray();
    ~SETexture2DArray();

	void CreateRenderTarget(SEGPUDevice* device, int width, int height, 
		int depth, SEBufferFormat format, bool generateMipMap = false);

	int Width, Height, Depth;
	bool IsRenderTarget;
};

typedef SESmartPointer<SETexture2DArray> SETexture2DArrayPtr;

}

#endif