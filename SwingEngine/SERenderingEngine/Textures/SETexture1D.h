// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_Texture1D_H
#define Swing_Texture1D_H

#include "SERenderingEngineLIB.h"
#include "SETexture.h"
#include "SEPixelBuffer.h"
#include "SETextureBuffer.h"
#include "SEGPUDevice.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 08/10/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SETexture1D : public SETexture
{
public:
	SETexture1D();
	~SETexture1D();

    bool LoadFromSystemMemory(SEGPUDevice* device, 
        SEBufferInternalFormat internalFormat, int width, SEBufferFormat format, 
        SEBufferComponentType type, void* pixels);

    // Update a render target from pixel buffer.
    void UpdateFromPixelBuffer(SEPixelBuffer* pixelBuffer);

    void CreateUniformRandomTexture(SEGPUDevice* device, int sampleCount, 
        int channelCount);

    void CreateHaltonRandomTexture(SEGPUDevice* device, int sampleCount,
        int channelCount);

    void GetDataFromGPUMemory(void* dstData);

	int Width;

protected:
    void SetupRandomTextureFormat(int sampleCount, int channelCount);
};

typedef SESmartPointer<SETexture1D> SETexture1DPtr;

}

#endif