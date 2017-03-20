// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERenderingEnginePCH.h"
#include "SETexture2DArray.h"

using namespace Swing;

//----------------------------------------------------------------------------
SETexture2DArray::SETexture2DArray()
{
    mType = TT_Texture2DArray;
	Width = 0;
	Height = 0;
    Depth = 0;
	IsRenderTarget = false;
}
//----------------------------------------------------------------------------
SETexture2DArray::~SETexture2DArray()
{
}
//----------------------------------------------------------------------------
void SETexture2DArray::CreateRenderTarget(SEGPUDevice* device, int width, 
	int height, int depth, SEBufferFormat format,bool generateMipMap)
{
	Width = width;
	Height = height;
    Depth = depth;
	IsRenderTarget = true;
	mFormat = format;

	switch (mFormat)
	{
	case BF_RGB:
		// TODO:
		SE_ASSERT( false );
		break;

	case BF_RGBF:
		mInternalFormat = BIF_RGB32F;
		mComponentType = BCT_Float;
		break;

    case BF_RGBAF:
        mInternalFormat = BIF_RGBA32F;
        mComponentType = BCT_Float;
        break;

	case BF_RGBA16F:
		mInternalFormat = BIF_RGBA16F;
		mComponentType = BCT_Half_Float;
		break;

	case BF_R32UI:
#ifndef __APPLE__
		mInternalFormat = BIF_R32UI;
        mComponentType = BCT_Unsigned_Int;
#else
        SE_ASSERT( false );
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

    mTextureHandle = device->Tex2DArrayLoadFromSystemMemory(
		this, mInternalFormat, Width, 
		Height, Depth, mFormat, mComponentType, generateMipMap, 0);
	this->HasMipMap = generateMipMap;
}
//--------------------------------------------------------------------------