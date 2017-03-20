// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERenderingEnginePCH.h"
#include "SETextureSampler.h"

using namespace Swing;

//----------------------------------------------------------------------------
SETextureSampler::SETextureSampler()
{
}
//----------------------------------------------------------------------------
SETextureSampler::~SETextureSampler()
{
    mTexture = 0;
}
//----------------------------------------------------------------------------
void SETextureSampler::SetTexture(SETexture* texture)
{
    mTexture = texture;
}
//----------------------------------------------------------------------------
SETexture* SETextureSampler::GetTexture() const
{
    return mTexture;
}
//----------------------------------------------------------------------------
void SETextureSampler::Enable()
{
}
//----------------------------------------------------------------------------