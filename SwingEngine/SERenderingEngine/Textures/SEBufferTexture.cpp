// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SEBufferTexture.h"
#include "SEGPUDevice.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEBufferTexture::SEBufferTexture()
{
    mType = TT_Texture1D;
}
//----------------------------------------------------------------------------
SEBufferTexture::~SEBufferTexture()
{
}
//----------------------------------------------------------------------------
bool SEBufferTexture::LoadFromTextureBuffer(SEGPUDevice* device, 
    SETextureBuffer* textureBuffer, SEBufferInternalFormat internalFormat)
{
    if( mTextureHandle )
    {
        return false;
    }

    IsTextureBuffer = true;
    mInternalFormat = internalFormat;

    mTextureHandle = device->BufferTextureLoadFromTextureBuffer(this, 
        textureBuffer, internalFormat);

    return true;
}
//----------------------------------------------------------------------------