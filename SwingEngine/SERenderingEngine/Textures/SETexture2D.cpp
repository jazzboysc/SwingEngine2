// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERenderingEnginePCH.h"
#include "SETexture2D.h"
#include "bmpread.h"
#include "png.h"
#include "SETarga.h"
#include "SETerminal.h"
#include "SEMonteCarlo.h"

#include <algorithm>

using namespace Swing;

//----------------------------------------------------------------------------
SETexture2D::SETexture2D()
{
    mType = TT_Texture2D;
	Width = 0;
	Height = 0;
	LMax = 0.0f;
	RevGamma = 0.0f;
}
//----------------------------------------------------------------------------
SETexture2D::~SETexture2D()
{
}
//----------------------------------------------------------------------------
bool SETexture2D::LoadImageFromFile(SEGPUDevice* device, 
    const std::string& fileName, bool generateMipMap, SESamplerDesc* sampler)
{
    size_t pos = fileName.find_last_of('.');
    if( pos == std::string::npos )
    {
        return false;
    }

    std::string ext = fileName.substr(pos);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    bool res = false;
    if( ext == ".bmp" )
    {
        res = LoadBMPFromFile(device, fileName, generateMipMap, sampler);
    }
    else if( ext == ".png" )
    {
        res = LoadPNGFromFile(device, fileName, generateMipMap, sampler);
    }
    else if( ext == ".tga" )
    {
        res = LoadTGAFromFile(device, fileName, generateMipMap, sampler);
    }
    else if( ext == ".pfm" )
    {
        res = LoadPFMFromFile(device, fileName, generateMipMap, sampler);
    }
    else
    {
        SE_ASSERT(false);
    }

    return res;
}
//----------------------------------------------------------------------------
bool SETexture2D::LoadBMPFromFile(SEGPUDevice* device, 
    const std::string& fileName, bool generateMipMap, 
    SESamplerDesc* sampler)
{
    if( mTextureHandle )
    {
        SE_ASSERT(false);
        return false;
    }

	IsRenderTarget = false;

	bmpread_t bitmap;
	if( !bmpread(fileName.c_str(), 0, &bitmap) )
	{
		SETerminal::Output(SETerminal::OC_Error, "Failed loading texture %s\n", 
            fileName.c_str());
        SE_ASSERT( false );
        
		return false;
	}
	Width = bitmap.width;
	Height = bitmap.height;

    // TODO:
    // Check this later.
    mInternalFormat = BIF_RGB8;
    mFormat = BF_RGB;
    mComponentType = BCT_Unsigned_Byte;

    mTextureHandle = device->Texture2DLoadFromSystemMemory(
        this, mInternalFormat, bitmap.width, bitmap.height, mFormat, 
        mComponentType, generateMipMap, sampler, bitmap.rgb_data);
    HasMipMap = generateMipMap;

    bmpread_free(&bitmap);
    
#ifdef RTGI_OUTPUT_TEXTURE_RESOURCE_LOADING
    SETerminal::Output(SETerminal::OC_Success, "Loading texture %s finished\n", 
        fileName.c_str());
#endif

	return true;
}
//----------------------------------------------------------------------------
bool SETexture2D::LoadPNGFromFile(SEGPUDevice* device, 
    const std::string& fileName, bool generateMipMap, 
    SESamplerDesc* sampler)
{
    if( mTextureHandle )
    {
        SE_ASSERT(false);
        return false;
    }

    IsRenderTarget = false;

    png_image png;
    memset(&png, 0, sizeof(png_image));
    png.version = PNG_IMAGE_VERSION;

    if( png_image_begin_read_from_file(&png, fileName.c_str()) )
    {
        png_bytep buffer;

        png.format = PNG_FORMAT_RGBA;
        buffer = (png_bytep)malloc(PNG_IMAGE_SIZE(png));

        if( png_image_finish_read(&png, 0, buffer, 0, 0) )
        {
            Width = png.width;
            Height = png.height;

            mInternalFormat = BIF_RGBA8;
            mFormat = BF_RGBA;
            mComponentType = BCT_Unsigned_Byte;

            mTextureHandle = device->Texture2DLoadFromSystemMemory(
                this, mInternalFormat, Width, Height, mFormat,
                mComponentType, generateMipMap, sampler, buffer);
            HasMipMap = generateMipMap;

#ifdef RTGI_OUTPUT_TEXTURE_RESOURCE_LOADING
            SETerminal::Output(SETerminal::OC_Success, 
                "Loading texture %s finished\n", fileName.c_str());
#endif
            free(buffer);

            return true;
        }
        else
        {
            free(buffer);
        }
    }

    SETerminal::Output(SETerminal::OC_Error, "Failed loading texture %s\n",
        fileName.c_str());
    SE_ASSERT(false);

    return false;
}
//----------------------------------------------------------------------------
bool SETexture2D::LoadTGAFromFile(SEGPUDevice* device, 
    const std::string& fileName, bool generateMipMap, SESamplerDesc* sampler)
{
    if( mTextureHandle )
    {
        SE_ASSERT(false);
        return false;
    }

    IsRenderTarget = false;

    tga_image tga;
    memset(&tga, 0, sizeof(tga_image));
    tga_result res = tga_read(&tga, fileName.c_str());
    if( res == TGA_NOERR )
    {
        SE_ASSERT(tga.image_type == TGA_IMAGE_TYPE_BGR);
        tga_swap_red_blue(&tga);

        Width = tga.width;
        Height = tga.height;

        // TODO:
        // Check this later.
        mInternalFormat = BIF_RGB8;
        mFormat = BF_RGB;
        mComponentType = BCT_Unsigned_Byte;

        mTextureHandle = device->Texture2DLoadFromSystemMemory(
            this, mInternalFormat, tga.width, tga.height, mFormat,
            mComponentType, generateMipMap, sampler, tga.image_data);
        HasMipMap = generateMipMap;

#ifdef RTGI_OUTPUT_TEXTURE_RESOURCE_LOADING
        SETerminal::Output(SETerminal::OC_Success,
            "Loading texture %s finished\n", fileName.c_str());
#endif

        tga_free_buffers(&tga);

        return true;
    }

    SETerminal::Output(SETerminal::OC_Error, "Failed loading texture %s\n",
        fileName.c_str());
    SE_ASSERT(false);

    return false;
}
//----------------------------------------------------------------------------
bool SETexture2D::LoadPFMFromFile(SEGPUDevice* device, 
    const std::string& fileName, bool generateMipMap, 
    SESamplerDesc* sampler)
{
	// This function is based on the PFM loader of Thorsten Grosch and Tobias 
	// Ritschel's demo.

    if( mTextureHandle )
    {
        SE_ASSERT(false);
        return false;
    }

	IsRenderTarget = false;
	IsHDRTexture = true;

    // Init some variables.
    char imageformat[1024]; 
    float f[1];

    std::string extendedFileName = "Textures/" + fileName;
	 
    // Open the file handle  .
    FILE* infile = fopen(extendedFileName.c_str(), "rb");
 
    if( infile == 0 )
	{ 
        SETerminal::Output(SETerminal::OC_Error, "Error loading %s !\n", 
            fileName.c_str());
		return false; 
    } 
 
    // Read the header.
    fscanf(infile, " %s %d %d ", (char*)&imageformat, &Width, &Height);
#ifdef RTGI_OUTPUT_TEXTURE_RESOURCE_LOADING
    SETerminal::Output(SETerminal::OC_Success, "Image format %s Width %d Height %d\n", 
        imageformat, Width, Height);
#endif
   
    float* pixels = (float*)(malloc(Width * Height * 3 * sizeof(float)));

    // Go ahead with the data.
    fscanf(infile, "%f", &f[0]); 
    fgetc(infile); 
 
    float red, green, blue;
    float *p = pixels; 
 
    // Read the values and store them.
    for( int j = 0; j < Height ; j++ )
	{ 
		for( int i = 0; i < Width ; i++ )
		{ 
			fread(f, 4, 1, infile); 
			red = f[0];
		     
			fread(f, 4, 1, infile); 
			green = f[0]; 
		     
			fread(f, 4, 1, infile); 
			blue = f[0]; 
		     
			*p++ = red;
			*p++ = green;
			*p++ = blue;
	 
			float L = (red + green + blue) / 3.0f; 
			if( L > LMax )
			{
			    LMax = L;
			}
		} 
    }

    mInternalFormat = BIF_RGB32F;
    mFormat = BF_RGB;
    mComponentType = BCT_Float;

    mTextureHandle = device->Texture2DLoadFromSystemMemory(
        this, mInternalFormat, Width, Height, mFormat, mComponentType, 
        generateMipMap, sampler, pixels);
    HasMipMap = generateMipMap;

	free(pixels);

#ifdef RTGI_OUTPUT_TEXTURE_RESOURCE_LOADING
    SETerminal::Output(SETerminal::OC_Success, "Loading texture %s finished\n", 
        fileName.c_str());
#endif
    
    RevGamma = 1.0f / 2.2f; 
	fclose(infile);

	return true;
}
//----------------------------------------------------------------------------
bool SETexture2D::LoadFromSystemMemory(SEGPUDevice* device,
    SEBufferInternalFormat internalFormat, int width, int height,
    SEBufferFormat format, SEBufferComponentType type, void* pixels, 
    bool generateMipMap, SESamplerDesc* sampler)
{
    if( mTextureHandle )
    {
        SE_ASSERT(false);
        return false;
    }

    Width = width;
    Height = height;
    mInternalFormat = internalFormat;
    mFormat = format;
    mComponentType = type;

    mTextureHandle = device->Texture2DLoadFromSystemMemory(
        this, mInternalFormat, Width, Height, mFormat, mComponentType, 
        generateMipMap, sampler, pixels);
    HasMipMap = generateMipMap;

	return true;
}
//----------------------------------------------------------------------------
#ifndef __APPLE__
bool SETexture2D::LoadFromTextureBuffer(SEGPUDevice* device, 
    SETextureBuffer* textureBuffer, SEBufferInternalFormat internalFormat)
{
    if( mTextureHandle )
    {
        return false;
    }

	IsTextureBuffer = true;
	mInternalFormat = internalFormat;

    mTextureHandle = device->Texture2DLoadFromTextureBuffer(
        this, textureBuffer, internalFormat);

	return true;
}
#endif
//----------------------------------------------------------------------------
void SETexture2D::CreateRenderTarget(SEGPUDevice* device, int width, int height,
    SEBufferFormat format, bool generateMipMap, SESamplerDesc* sampler, 
    void* initData)
{
    if( mTextureHandle )
    {
        SE_ASSERT(false);
        return;
    }

	Width = width;
	Height = height;
	IsRenderTarget = true;
	mFormat = format;

	switch (mFormat)
	{
	case BF_RGB:
        mInternalFormat = BIF_RGB8;
        mComponentType = BCT_Unsigned_Byte;
		break;

    case BF_RGBA:
        mInternalFormat = BIF_RGBA8;
        mComponentType = BCT_Unsigned_Byte;
        break;

	case BF_RG:
		mInternalFormat = BIF_RG32F;
		mComponentType = BCT_Float;
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

    mTextureHandle = device->Texture2DLoadFromSystemMemory(
        this, mInternalFormat, Width, Height, mFormat, mComponentType, 
        generateMipMap, sampler, initData);
    HasMipMap = generateMipMap;
}
//--------------------------------------------------------------------------
void SETexture2D::UpdateFromPixelBuffer(SEPixelBuffer* pixelBuffer)
{
    SE_ASSERT(mTextureHandle);
    mTextureHandle->Device->Texture2DUpdateFromPixelBuffer(this, pixelBuffer);
}
//--------------------------------------------------------------------------
void SETexture2D::CreateLDRandomTextureRGBF(SEGPUDevice* device, 
    int maxSampleCount, int patternSize)
{
	// This function is based on the PFM loader of Thorsten Grosch and 
	// Tobias Ritschel's demo.

    if( mTextureHandle )
    {
        SE_ASSERT(false);
        return;
    }

	int patternSizeSquared = patternSize * patternSize;

	int haltonIndex = 1;
	float* const pixels = 
		SE_NEW float[3 * maxSampleCount * patternSizeSquared];
	
	for( int i = 0; i < patternSizeSquared; i++ )
	{
		for( int j = 0; j < maxSampleCount; j++ )
		{
			SEVector3f sample;
			do
			{
                sample = SEVector3f(2.0f * SEMonteCarlo::RadicalInverse(2, haltonIndex) - 1.0f,
                    2.0f * SEMonteCarlo::RadicalInverse(3, haltonIndex) - 1.0f,
                    SEMonteCarlo::RadicalInverse(5, haltonIndex));
				haltonIndex++;
			} while( sample.GetLength() > 1.0f );

			pixels[(i * maxSampleCount + j) * 3 + 0] = sample[0];
			pixels[(i * maxSampleCount + j) * 3 + 1] = sample[1];
			pixels[(i * maxSampleCount + j) * 3 + 2] = sample[2];
		}
	}

    Width = maxSampleCount;
    Height = patternSizeSquared;
    mInternalFormat = BIF_RGB32F;
    mFormat = BF_RGB;
    mComponentType = BCT_Float;
	
    mTextureHandle = device->Texture2DLoadFromSystemMemory(
        this, mInternalFormat, Width, Height, mFormat, mComponentType,
        false, 0, pixels);

	SE_DELETE[] pixels;
}
//--------------------------------------------------------------------------
void SETexture2D::GetImageData(void* dstPixels)
{
    SE_ASSERT(mTextureHandle);
    mTextureHandle->Device->Texture2DGetImageData(this, 
        dstPixels);
}
//--------------------------------------------------------------------------