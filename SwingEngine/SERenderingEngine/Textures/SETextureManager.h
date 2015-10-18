// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_TextureManager_H
#define Swing_TextureManager_H

#include "SERenderingEngineLIB.h"
#include "SEGPUDevice.h"
#include "SETexture1D.h"
#include "SETexture2D.h"
#include "SETexture2DArray.h"
#include "SETexture3D.h"
#include "SETextureCube.h"

#include <string>
#include <unordered_map>

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 01/15/2015
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SETextureManager
{
public:
    ~SETextureManager();

    // Singleton class method called by framework.
    static void Initialize();
    static void Terminate();

    // User access.
    static SETextureManager* GetInstance();
    SETexture2D* FindOrCreateTexture2D(SEGPUDevice* device, 
        const std::string& name);

private:
    std::unordered_map<std::string, SETexturePtr> mRepository;

    SETextureManager();
    static SETextureManager* mInstance;
};

}

#endif