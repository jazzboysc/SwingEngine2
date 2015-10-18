// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SETexture.h"
#include "SEGPUDevice.h"

using namespace Swing;

//----------------------------------------------------------------------------
SETexture::SETexture()
    :
    IsRenderTarget(false),
    IsHDRTexture(false),
    IsTextureBuffer(false),
    HasMipMap(false),
    mTextureHandle(0),
    mFormat(BF_Unknown),
    mInternalFormat(BIF_Unknown),
    mComponentType(BCT_Unknown)
{
}
//----------------------------------------------------------------------------
SETexture::~SETexture()
{
    if( mTextureHandle )
    {
        mTextureHandle->Device->DeleteTexture(this);
        SE_DELETE mTextureHandle;
    }
}
//----------------------------------------------------------------------------
void SETexture::BindToImageUnit(unsigned int unit, SEBufferAccess access, bool layered)
{
	if( mTextureHandle )
	{
		mTextureHandle->Device->TextureBindToImageUnit(this, unit, layered, access);
	}
}
//--------------------------------------------------------------------------
void SETexture::BindToSampler(unsigned int index, SESamplerDesc* sampler)
{
    if( !HasMipMap && sampler->MinFilter == FT_Linear_Linear )
    {
        sampler->MinFilter = FT_Linear;
    }

	if( mTextureHandle )
	{
		mTextureHandle->Device->TextureBindToSampler(this, index, sampler);
	}
}
//--------------------------------------------------------------------------
void SETexture::GenerateMipmap()
{
    SE_ASSERT(HasMipMap);

	if( mTextureHandle )
	{
		mTextureHandle->Device->TextureGenerateMipmap(this);
	}
}
//--------------------------------------------------------------------------