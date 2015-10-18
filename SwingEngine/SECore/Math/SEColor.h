// Swing Engine Version 2 Source Code 
// Most of techniques in the engine are mainly based on David Eberly's
// Wild Magic 4 and 5 open-source game engine. The author of Swing Engine 
// learned a lot from Eberly's experience of architecture and algorithm.
// Several subsystems are totally new, and others are reorganized or
// reimplimented based on Wild Magic's subsystems.
// Copyright (c) 2007-2011
//
// David Eberly's permission:
// Geometric Tools, LLC
// Copyright (c) 1998-2010
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt

#ifndef Swing_Color_H
#define Swing_Color_H

#include "SECoreLIB.h"
#include "SEFloat4.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description: The SEColor class contains only static functions.
// Date:20100624
//----------------------------------------------------------------------------
class SE_CORE_API SEColor
{
public:
    // Make a 32-bit RGB color from 8-bit channels. The alpha channel is set 
    // to 255.
    inline static unsigned int MakeR8G8B8(unsigned char ucRed,
        unsigned char ucGreen, unsigned char ucBlue);

    // Make a 32-bit RGB color from 8-bit channels.
    inline static unsigned int MakeR8G8B8A8(unsigned char ucRed,
        unsigned char ucGreen, unsigned char ucBlue, unsigned char ucAlpha);

    // Extract 8-bit channels from a 32-bit RGBA color. The alpha channel is 
    // ignored.
    inline static void ExtractR8G8B8(unsigned int uiColor,
        unsigned char& rucRed, unsigned char& rucGreen, unsigned char& 
        rucBlue);

    // Extract 8-bit channels from a 32-bit RGBA color.
    inline static void ExtractR8G8B8A8(unsigned int uiColor,
        unsigned char& rucRed, unsigned char& rucGreen, unsigned char& 
        rucBlue, unsigned char& rucAlpha);

    enum
    {
        CF_COUNT = 23
    };

    // Convert from the specified format to 32-bit RGBA.
    static void ConvertFromR5G6B5(int iTexelCount, const char* pSrcTexels,
        SEFloat4* pDstTexels);

    static void ConvertFromA1R5G5B5(int iTexelCount, const char* pSrcTexels,
        SEFloat4* pDstTexels);

    static void ConvertFromA4R4G4B4(int iTexelCount, const char* pSrcTexels,
        SEFloat4* pDstTexels);

    static void ConvertFromA8(int iTexelCount, const char* pSrcTexels,
        SEFloat4* pDstTexels);

    static void ConvertFromL8(int iTexelCount, const char* pSrcTexels,
        SEFloat4* pDstTexels);

    static void ConvertFromA8L8(int iTexelCount, const char* pSrcTexels,
        SEFloat4* pDstTexels);

    static void ConvertFromR8G8B8(int iTexelCount, const char* pSrcTexels,
        SEFloat4* pDstTexels);

    static void ConvertFromA8R8G8B8(int iTexelCount, const char* pSrcTexels,
        SEFloat4* pDstTexels);

    static void ConvertFromA8B8G8R8(int iTexelCount, const char* pSrcTexels,
        SEFloat4* pDstTexels);

    static void ConvertFromL16(int iTexelCount, const char* pSrcTexels,
        SEFloat4* pDstTexels);

    static void ConvertFromG16R16(int iTexelCount, const char* pSrcTexels,
        SEFloat4* pDstTexels);

    static void ConvertFromA16B16G16R16(int iTexelCount, 
        const char* pSrcTexels, SEFloat4* pDstTexels);

    static void ConvertFromR16F(int iTexelCount, const char* pSrcTexels,
        SEFloat4* pDstTexels);

    static void ConvertFromG16R16F(int iTexelCount, const char* pSrcTexels,
        SEFloat4* pDstTexels);

    static void ConvertFromA16B16G16R16F(int iTexelCount, 
        const char* pSrcTexels, SEFloat4* pDstTexels);

    static void ConvertFromR32F(int iTexelCount, const char* pSrcTexels,
        SEFloat4* pDstTexels);

    static void ConvertFromG32R32F(int iTexelCount, const char* pSrcTexels,
        SEFloat4* pDstTexels);

    static void ConvertFromA32B32G32R32F(int iTexelCount, 
        const char* pSrcTexels, SEFloat4* pDstTexels);

    // Convert to the specified format from 32-bit RGBA.
    static void ConvertToR5G6B5(int iTexelCount, const SEFloat4* pSrcTexels,
        char* pDstTexels);

    static void ConvertToA1R5G5B5(int iTexelCount, const SEFloat4* pSrcTexels,
        char* pDstTexels);

    static void ConvertToA4R4G4B4(int iTexelCount, const SEFloat4* pSrcTexels,
        char* pDstTexels);

    static void ConvertToA8(int iTexelCount, const SEFloat4* pSrcTexels,
        char* pDstTexels);

    static void ConvertToL8(int iTexelCount, const SEFloat4* pSrcTexels,
        char* pDstTexels);

    static void ConvertToA8L8(int iTexelCount, const SEFloat4* pSrcTexels,
        char* pDstTexels);

    static void ConvertToR8G8B8(int iTexelCount, const SEFloat4* pSrcTexels,
        char* pDstTexels);

    static void ConvertToA8R8G8B8(int iTexelCount, const SEFloat4* pSrcTexels,
        char* pDstTexels);

    static void ConvertToA8B8G8R8(int iTexelCount, const SEFloat4* pSrcTexels,
        char* pDstTexels);

    static void ConvertToL16(int iTexelCount, const SEFloat4* pSrcTexels,
        char* pDstTexels);

    static void ConvertToG16R16(int iTexelCount, const SEFloat4* pSrcTexels,
        char* pDstTexels);

    static void ConvertToA16B16G16R16(int iTexelCount, 
        const SEFloat4* pSrcTexels, char* pDstTexels);

    static void ConvertToR16F(int iTexelCount, const SEFloat4* pSrcTexels,
        char* pDstTexels);

    static void ConvertToG16R16F(int iTexelCount, const SEFloat4* pSrcTexels,
        char* pDstTexels);

    static void ConvertToA16B16G16R16F(int iTexelCount, 
        const SEFloat4* pSrcTexels, char* pDstTexels);

    static void ConvertToR32F(int iTexelCount, const SEFloat4* pSrcTexels,
        char* pDstTexels);

    static void ConvertToG32R32F(int iTexelCount, const SEFloat4* pSrcTexels,
        char* pDstTexels);

    static void ConvertToA32B32G32R32F(int iTexelCount, 
        const SEFloat4* pSrcTexels, char* pDstTexels);

    // For the convenience of applications when looking up functions.
    typedef void (*ConvertFromFunction)(int, const char*, SEFloat4*);
    static ConvertFromFunction FromFunction[CF_COUNT];

    typedef void (*ConvertToFunction)(int, const SEFloat4*, char*);
    static ConvertToFunction ToFunction[CF_COUNT];
};

#include "SEColor.inl"
}

#endif
