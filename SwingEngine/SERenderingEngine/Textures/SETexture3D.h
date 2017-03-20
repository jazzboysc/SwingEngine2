// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_Texture3D_H
#define Swing_Texture3D_H

#include "SERenderingEngineLIB.h"
#include "SETexture.h"
#include "SEPixelBuffer.h"
#include "SETextureBuffer.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 08/05/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SETexture3D : public SETexture
{
public:
	SETexture3D();
	~SETexture3D();

    bool LoadFromSystemMemory(SEGPUDevice* device, 
        SEBufferInternalFormat internalFormat, int width, int height, int depth, 
        SEBufferFormat format, SEBufferComponentType type, void* pixels);

    void CreateRenderTarget(SEGPUDevice* device, int width, int height, 
        int depth, SEBufferFormat format);

		// Update a render target from pixel buffer.
	void UpdateFromPixelBuffer(SEPixelBuffer* pixelBuffer);

	int Width, Height, Depth;
};

typedef SESmartPointer<SETexture3D> SETexture3DPtr;

}

#endif