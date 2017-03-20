// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_Texture2D_H
#define Swing_Texture2D_H

#include "SERenderingEngineLIB.h"
#include "SETexture.h"
#include "SEPixelBuffer.h"
#include "SETextureBuffer.h"
#include "SEGPUDevice.h"

#include <string>

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/07/2013
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SETexture2D : public SETexture
{
public:
	SETexture2D();
	~SETexture2D();

    // Load a ".bmp", ".png", ".tga" or ".pfm" file and create its resource.
    bool LoadImageFromFile(SEGPUDevice* device, const std::string& fileName,
        bool generateMipMap = true, SESamplerDesc* sampler = 0);

	// Load a ".bmp" file and create its resource.
    bool LoadBMPFromFile(SEGPUDevice* device, const std::string& fileName, 
        bool generateMipMap = true, SESamplerDesc* sampler = 0);

    // Load a ".png" file and create its resource.
    bool LoadPNGFromFile(SEGPUDevice* device, const std::string& fileName,
        bool generateMipMap = true, SESamplerDesc* sampler = 0);

    // Load a ".tga" file and create its resource.
    bool LoadTGAFromFile(SEGPUDevice* device, const std::string& fileName,
        bool generateMipMap = true, SESamplerDesc* sampler = 0);

	// Load a ".pfm" file and create its resource.
    bool LoadPFMFromFile(SEGPUDevice* device, const std::string& fileName,
        bool generateMipMap = true, SESamplerDesc* sampler = 0);

	// Load texture data from system memory. User is responsible for deleting
	// the system memory data.
    bool LoadFromSystemMemory(SEGPUDevice* device, 
        SEBufferInternalFormat internalFormat, int width, int height, 
        SEBufferFormat format, SEBufferComponentType type, void* pixels,
        bool generateMipMap = true, SESamplerDesc* sampler = 0);

#ifndef __APPLE__
	// Load and create from texture buffer object.
    bool LoadFromTextureBuffer(SEGPUDevice* device, SETextureBuffer* textureBuffer,
        SEBufferInternalFormat internalFormat);
#endif

    void CreateRenderTarget(SEGPUDevice* device, int width, int height, 
        SEBufferFormat format, bool generateMipMap = false, 
        SESamplerDesc* sampler = 0, void* initData = 0);

	// Update a render target from pixel buffer.
	void UpdateFromPixelBuffer(SEPixelBuffer* pixelBuffer);

	// Generate random positions inside a unit hemisphere based on halton 
	// numbers.
    void CreateLDRandomTextureRGBF(SEGPUDevice* device, int maxSampleCount, 
        int patternSize);

    void GetImageData(void* dstPixels);

	int Width, Height;
	float LMax;
	float RevGamma;
};

typedef SESmartPointer<SETexture2D> SETexture2DPtr;

}

#endif