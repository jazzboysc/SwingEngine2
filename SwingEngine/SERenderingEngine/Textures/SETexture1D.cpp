// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SETexture1D.h"
#include "SEMonteCarlo.h"

using namespace Swing;

//----------------------------------------------------------------------------
SETexture1D::SETexture1D()
{
    mType = TT_Texture1D;
	Width = 0;
}
//----------------------------------------------------------------------------
SETexture1D::~SETexture1D()
{
}
//----------------------------------------------------------------------------
bool SETexture1D::LoadFromSystemMemory(SEGPUDevice* device, 
    SEBufferInternalFormat internalFormat, int width, SEBufferFormat format, 
    SEBufferComponentType type, void* pixels)
{
    if( mTextureHandle )
    {
        return false;
    }

    mTextureHandle = device->Texture1DLoadFromSystemMemory(
        this, internalFormat, width, format, type, pixels);

    Width = width;
    mInternalFormat = internalFormat;
    mFormat = format;
    mComponentType = type;

    return true;
}
//----------------------------------------------------------------------------
void SETexture1D::UpdateFromPixelBuffer(SEPixelBuffer* pixelBuffer)
{
    mTextureHandle->Device->Texture1DUpdateFromPixelBuffer(
        this, pixelBuffer);
}
//--------------------------------------------------------------------------
void SETexture1D::CreateUniformRandomTexture(SEGPUDevice* device, 
    int sampleCount, int channelCount)
{
    SetupRandomTextureFormat(sampleCount, channelCount);

    float* pixels = SE_NEW float[sampleCount * channelCount];
    for( int i = 0; i < sampleCount; ++i )
    {
        for( int j = 0; j < channelCount; ++j )
        {
            pixels[channelCount*i + j] = SEMathf::UnitRandom();
        }
    }

    mTextureHandle = device->Texture1DLoadFromSystemMemory(
        this, mInternalFormat, Width, mFormat, mComponentType, pixels);

    SE_DELETE[] pixels;
}
//--------------------------------------------------------------------------
static int PrimeNumbers[4] = { 2, 3, 5, 7 };
//--------------------------------------------------------------------------
void SETexture1D::CreateHaltonRandomTexture(SEGPUDevice* device, 
    int sampleCount, int channelCount)
{
    SetupRandomTextureFormat(sampleCount, channelCount);

    int haltonIndex = 1;
    float* pixels = SE_NEW float[sampleCount * channelCount];
    for( int i = 0; i < sampleCount; ++i )
    {
        for( int j = 0; j < channelCount; ++j )
        {
            pixels[channelCount*i + j] = SEMonteCarlo::RadicalInverse(
                PrimeNumbers[j], haltonIndex);
        }
        haltonIndex++;
    }

    mTextureHandle = device->Texture1DLoadFromSystemMemory(
        this, mInternalFormat, Width, mFormat, mComponentType, pixels);

    SE_DELETE[] pixels;
}
//--------------------------------------------------------------------------
void SETexture1D::GetDataFromGPUMemory(void* dstData)
{
    mTextureHandle->Device->Texture1DGetDataFromGPUMemory(
        this, dstData);
}
//--------------------------------------------------------------------------
void SETexture1D::SetupRandomTextureFormat(int sampleCount, int channelCount)
{
    Width = sampleCount;
    mComponentType = BCT_Float;

    switch( channelCount )
    {
    case 1:
        mInternalFormat = BIF_R32F;
        mFormat = BF_R;
        break;

    case 2:
        mInternalFormat = BIF_RG32F;
        mFormat = BF_RG;
        break;

    case 3:
        mInternalFormat = BIF_RGB32F;
        mFormat = BF_RGB;
        break;

    case 4:
        mInternalFormat = BIF_RGBA32F;
        mFormat = BF_RGBA;
        break;

    default:
        SE_ASSERT(false);
        break;
    }
}
//--------------------------------------------------------------------------