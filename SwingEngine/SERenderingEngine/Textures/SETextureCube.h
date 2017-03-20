// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_TextureCube_H
#define Swing_TextureCube_H

#include "SERenderingEngineLIB.h"
#include "SETexture.h"
#include "SEGPUDevice.h"

#include <string>

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/07/2013
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SETextureCube : public SETexture
{
public:
	SETextureCube();
	~SETextureCube();

	// Load six ".bmp" files and create a cube-map resource.
	bool LoadFromFile(SEGPUDevice* device, const std::string& pX, 
        const std::string& nX, const std::string& pY, const std::string& nY, 
        const std::string& pZ, const std::string& nZ, 
        bool generateMipMap = true);

    int Width, Height;
};

typedef SESmartPointer<SETextureCube> SETextureCubePtr;

}

#endif