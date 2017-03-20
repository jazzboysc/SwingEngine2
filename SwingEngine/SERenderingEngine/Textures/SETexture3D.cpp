// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERenderingEnginePCH.h"
#include "SETexture3D.h"
#include "SEGPUDevice.h"

using namespace Swing;

//----------------------------------------------------------------------------
SETexture3D::SETexture3D()
{
    mType = TT_Texture3D;
	Width = 0;
	Height = 0;
	Depth = 0;
}
//----------------------------------------------------------------------------
SETexture3D::~SETexture3D()
{
}
//----------------------------------------------------------------------------
bool SETexture3D::LoadFromSystemMemory(SEGPUDevice* device,
    SEBufferInternalFormat internalFormat, int width, int height, int depth,
    SEBufferFormat format, SEBufferComponentType type, void* pixels)
{
    if( mTextureHandle )
    {
        SE_ASSERT(false);
        return false;
    }

    Width = width;
    Height = height;
    Depth = depth;
    mInternalFormat = internalFormat;
    mFormat = format;
    mComponentType = type;

    mTextureHandle = device->Texture3DLoadFromSystemMemory(
        this, mInternalFormat, Width, Height, Depth, mFormat, mComponentType,
        pixels);

	return true;
}
//----------------------------------------------------------------------------
void SETexture3D::CreateRenderTarget(SEGPUDevice* device, int width, int height, 
    int depth, SEBufferFormat format)
{
    if( mTextureHandle )
    {
        SE_ASSERT(false);
        return;
    }

    Width = width;
    Height = height;
    Depth = depth;
    IsRenderTarget = true;
    mFormat = format;

    switch( mFormat )
    {
    case BF_RGB:
        mInternalFormat = BIF_RGB8;
        mComponentType = BCT_Unsigned_Byte;
        break;

    case BF_RGBA:
        mInternalFormat = BIF_RGBA8;
        mComponentType = BCT_Unsigned_Byte;
        break;

    case BF_RGBF:
        mInternalFormat = BIF_RGB32F;
        mComponentType = BCT_Float;
        break;

    case BF_RGBAF:
        mInternalFormat = BIF_RGBA32F;
        mComponentType = BCT_Float;
        break;

    case BF_R32UI:
#ifndef __APPLE__
        mInternalFormat = BIF_R32UI;
        mComponentType = BCT_Unsigned_Int;
#else
        SE_ASSERT(false);
#endif
        break;

    case BF_R32F:
        mInternalFormat = BIF_R32F;
        mComponentType = BCT_Float;
        break;

    case BF_Depth:
        mInternalFormat = BIF_Depth24;
        mComponentType = BCT_Unsigned_Byte;
        break;

    default:
        break;
    }

    mTextureHandle = device->Texture3DLoadFromSystemMemory(
        this, mInternalFormat, Width, Height, Depth, mFormat, mComponentType,
        0);
}
//----------------------------------------------------------------------------
void SETexture3D::UpdateFromPixelBuffer(SEPixelBuffer* pixelBuffer)
{
    SE_ASSERT(mTextureHandle);
    mTextureHandle->Device->Texture3DUpdateFromPixelBuffer(
        this, pixelBuffer);
}
//--------------------------------------------------------------------------