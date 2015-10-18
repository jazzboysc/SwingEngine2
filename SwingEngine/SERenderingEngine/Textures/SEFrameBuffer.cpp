// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SEFrameBuffer.h"

using namespace Swing;

//--------------------------------------------------------------------------
SEFrameBuffer::SEFrameBuffer(SEGPUDevice* device)
{
    mFBO = device->CreateFrameBuffer(this);
    mWidth = 0;
    mHeight = 0;
    mDepth = 0;
	mColorTextureCount = 0;
	mColorBuffers = 0;
	mDepthTexture = 0;
	mStencilTexture = 0;
}
//--------------------------------------------------------------------------
SEFrameBuffer::~SEFrameBuffer()
{
    mFBO->Device->DeleteFrameBuffer(this);
	SE_DELETE[] mColorBuffers;
}
//--------------------------------------------------------------------------
SEFBOHandle* SEFrameBuffer::GetFBOHandle()
{
	return mFBO;
}
//--------------------------------------------------------------------------
void SEFrameBuffer::SetRenderTargets(unsigned int colorTextureCount, 
    SETexture** colorTextures, SETexture* depthTexture, SETexture* stencilTexture)
{
    if( colorTextureCount > 0 )
    {
        SE_ASSERT(colorTextureCount <= FBO_MAX_COLOR_TARGETS && 
            colorTextures);

        mColorTextures.clear();
        if( mColorBuffers )
        {
            SE_DELETE[] mColorBuffers;
        }

        mColorTextureCount = colorTextureCount;
        mColorBuffers = SE_NEW unsigned int[colorTextureCount];

        // Only support uniform size targets.
        SETextureType textureType = colorTextures[0]->GetType();
        switch( textureType )
        {
        case TT_Texture1D:
            mWidth = ((SETexture1D*)colorTextures[0])->Width;
            break;

        case TT_Texture2D:
            mWidth = ((SETexture2D*)colorTextures[0])->Width;
            mHeight = ((SETexture2D*)colorTextures[0])->Height;
            break;

        case TT_Texture2DArray:
            mWidth = ((SETexture2DArray*)colorTextures[0])->Width;
            mHeight = ((SETexture2DArray*)colorTextures[0])->Height;
            mDepth = ((SETexture2DArray*)colorTextures[0])->Depth;
            break;

        default:
            SE_ASSERT(false);
            break;
        }
    }
    mDepthTexture = depthTexture;
    mStencilTexture = stencilTexture;

    mFBO->Device->FrameBufferSetRenderTargets(this, 
        colorTextureCount, colorTextures, depthTexture, stencilTexture);
}
//--------------------------------------------------------------------------
void SEFrameBuffer::ReserveColorTargets(unsigned int colorTextureCount, 
    int width, int height, int depth)
{
    if( colorTextureCount > 0 )
    {
        SE_ASSERT(colorTextureCount <= FBO_MAX_COLOR_TARGETS);

        mWidth = width;
        mHeight = height;
        mDepth = depth;

        mColorTextures.clear();
        mColorTextures.reserve(colorTextureCount);
        if( mColorBuffers )
        {
            SE_DELETE[] mColorBuffers;
        }
        mColorBuffers = SE_NEW unsigned int[colorTextureCount];
    }
}
//--------------------------------------------------------------------------
void SEFrameBuffer::SetColorTarget(unsigned int attachPoint, 
    SETexture* colorTexture)
{
    SE_ASSERT(attachPoint >= 0 && attachPoint <
        (unsigned int)mColorTextures.size());
    
    mColorTextures[attachPoint] = colorTexture;
    mFBO->Device->FrameBufferSetColorTarget(this, attachPoint, colorTexture);
}
//--------------------------------------------------------------------------
void SEFrameBuffer::SetDepthTarget(SETexture* depthTexture)
{
    mDepthTexture = depthTexture;
    mFBO->Device->FrameBufferSetDepthTarget(this, mDepthTexture);
}
//--------------------------------------------------------------------------
void SEFrameBuffer::SetStencilTarget(SETexture* stencilTexture)
{
    mStencilTexture = stencilTexture;
    mFBO->Device->FrameBufferSetStencilTarget(this, mStencilTexture);
}
//--------------------------------------------------------------------------
void SEFrameBuffer::Enable()
{
    mFBO->Device->FrameBufferEnable(this);
}
//--------------------------------------------------------------------------
void SEFrameBuffer::Disable()
{
    mFBO->Device->FrameBufferDisable(this);
}
//--------------------------------------------------------------------------