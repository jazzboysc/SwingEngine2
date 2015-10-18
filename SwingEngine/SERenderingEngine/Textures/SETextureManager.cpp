// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SETextureManager.h"

using namespace Swing;

SETextureManager* SETextureManager::mInstance = 0;

//----------------------------------------------------------------------------
SETextureManager::SETextureManager()
{
}
//----------------------------------------------------------------------------
SETextureManager::~SETextureManager()
{
    std::unordered_map<std::string, SETexturePtr>::iterator it = 
        mRepository.begin();
    for( ; it != mRepository.end(); ++it )
    {
        it->second = 0;
    }
    mRepository.clear();
}
//----------------------------------------------------------------------------
void SETextureManager::Initialize()
{
    // TODO:
    // Unsafe in multi-threading context.
    if( !mInstance )
    {
        mInstance = SE_NEW SETextureManager();
    }
}
//----------------------------------------------------------------------------
void SETextureManager::Terminate()
{
    SE_DELETE mInstance;
    mInstance = 0;
}
//----------------------------------------------------------------------------
SETextureManager* SETextureManager::GetInstance()
{
    return mInstance;
}
//----------------------------------------------------------------------------
SETexture2D* SETextureManager::FindOrCreateTexture2D(SEGPUDevice* device, 
    const std::string& name)
{
    std::unordered_map<std::string, SETexturePtr>::iterator it =
        mRepository.find(name);
    if( it != mRepository.end() )
    {
        SETextureType tt = it->second->GetType();
        if( tt == TT_Texture2D )
        {
            return (SETexture2D*)(SETexture*)it->second;
        }
        else
        {
            SE_ASSERT(false);
            return 0;
        }
    }

    SETexture2D* texture = SE_NEW SETexture2D();
    texture->LoadImageFromFile(device, name);
    mRepository[name] = texture;

    return texture;
}
//----------------------------------------------------------------------------