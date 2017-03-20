// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERenderingEnginePCH.h"
#include "SETextureCube.h"
#include "bmpread.h"

using namespace Swing;

//----------------------------------------------------------------------------
SETextureCube::SETextureCube()
{
    mType = TT_TextureCube;
    Width = 0;
    Height = 0;
}
//----------------------------------------------------------------------------
SETextureCube::~SETextureCube()
{
}
//----------------------------------------------------------------------------
bool SETextureCube::LoadFromFile(SEGPUDevice* device, const std::string& pX, 
    const std::string& nX, const std::string& pY, const std::string& nY, 
    const std::string& pZ, const std::string& nZ, bool generateMipMap)
{
    if( mTextureHandle )
    {
        SE_ASSERT(false);
        return false;
    }

	bmpread_t pXBitmap, nXBitmap, pYBitmap, nYBitmap, pZBitmap, nZBitmap;
	int resPX = bmpread(pX.c_str(), 0, &pXBitmap);
	int resNX = bmpread(nX.c_str(), 0, &nXBitmap);
	int resPY = bmpread(pY.c_str(), 0, &pYBitmap);
	int resNY = bmpread(nY.c_str(), 0, &nYBitmap);
	int resPZ = bmpread(pZ.c_str(), 0, &pZBitmap);
	int resNZ = bmpread(nZ.c_str(), 0, &nZBitmap);
    if( !resPX || !resNX || !resPY || !resNY || !resPZ || !resNZ || 
        (pXBitmap.width != pXBitmap.height) )
	{
        bmpread_free(&pXBitmap);
        bmpread_free(&nXBitmap);
        bmpread_free(&pYBitmap);
        bmpread_free(&nYBitmap);
        bmpread_free(&pZBitmap);
        bmpread_free(&nZBitmap);

        SE_ASSERT(false);
		return false;
	}

    Width = pXBitmap.width;
    Height = pXBitmap.height;
    mFormat = BF_RGB;
    mInternalFormat = BIF_RGB8;
    mComponentType = BCT_Unsigned_Byte;

    mTextureHandle = device->TextureCubeLoadFromSystemMemory(
        this, mInternalFormat, Width, Height, mFormat, mComponentType, 
        generateMipMap, pXBitmap.rgb_data, nXBitmap.rgb_data, 
        pYBitmap.rgb_data, nYBitmap.rgb_data, pZBitmap.rgb_data, 
        nZBitmap.rgb_data);
    HasMipMap = generateMipMap;

	bmpread_free(&pXBitmap);
	bmpread_free(&nXBitmap);
	bmpread_free(&pYBitmap);
	bmpread_free(&nYBitmap);
	bmpread_free(&pZBitmap);
	bmpread_free(&nZBitmap);

	return true;
}
//----------------------------------------------------------------------------