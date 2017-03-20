// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_TextureSampler_H
#define Swing_TextureSampler_H

#include "SERenderingEngineLIB.h"
#include "SETexture.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 01/15/2015
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SETextureSampler
{
public:
    SETextureSampler();
    ~SETextureSampler();

    void SetTexture(SETexture* texture);
    SETexture* GetTexture() const;

    void Enable();

    SESamplerDesc Sampler;

private:
    SETexturePtr mTexture;
};

}

#endif