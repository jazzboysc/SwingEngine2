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

#include "SECorePCH.h"
#include "SEColor.h"
#include "SEHalfFloat.h"

using namespace Swing;

//----------------------------------------------------------------------------
// Convert from the specified format to 32-bit RGBA color.
//----------------------------------------------------------------------------
void SEColor::ConvertFromR5G6B5(int iTexelCount, const char* pSrcTexels,
    SEFloat4* pDstTexels)
{
    const unsigned short* pSrc = (const unsigned short*)pSrcTexels;
    SEFloat4* pDst = pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pSrc, ++pDst )
    {
        (*pDst)[2] = (float)((*pSrc & 0x001Fu));
        (*pDst)[1] = (float)((*pSrc & 0x07E0u) >>  5);
        (*pDst)[0] = (float)((*pSrc & 0xF800u) >> 11);
        (*pDst)[3] = 0.0f;
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertFromA1R5G5B5(int iTexelCount, const char* pSrcTexels,
    SEFloat4* pDstTexels)
{
    const unsigned short* pSrc = (const unsigned short*)pSrcTexels;
    SEFloat4* pDst = pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pSrc, ++pDst )
    {
        (*pDst)[2] = (float)((*pSrc & 0x001Fu));
        (*pDst)[1] = (float)((*pSrc & 0x03E0u) >>  5);
        (*pDst)[0] = (float)((*pSrc & 0x7C00u) >> 10);
        (*pDst)[3] = (float)((*pSrc & 0x8000u) >> 15);
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertFromA4R4G4B4(int iTexelCount, const char* pSrcTexels,
    SEFloat4* pDstTexels)
{
    const unsigned short* pSrc = (const unsigned short*)pSrcTexels;
    SEFloat4* pDst = pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pSrc, ++pDst )
    {
        (*pDst)[2] = (float)((*pSrc & 0x000Fu));
        (*pDst)[1] = (float)((*pSrc & 0x00F0u) >>  4);
        (*pDst)[0] = (float)((*pSrc & 0x0F00u) >>  8);
        (*pDst)[3] = (float)((*pSrc & 0xF000u) >> 12);
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertFromA8(int iTexelCount, const char* pSrcTexels,
    SEFloat4* pDstTexels)
{
    const unsigned char* pSrc = (const unsigned char*)pSrcTexels;
    SEFloat4* pDst = pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pDst )
    {
        (*pDst)[0] = 0.0f;
        (*pDst)[1] = 0.0f;
        (*pDst)[2] = 0.0f;
        (*pDst)[3] = (float)(*pSrc++);
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertFromL8(int iTexelCount, const char* pSrcTexels,
    SEFloat4* pDstTexels)
{
    const unsigned char* pSrc = (const unsigned char*)pSrcTexels;
    SEFloat4* pDst = pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pDst )
    {
        float fLuminance = (float)(*pSrc++);
        (*pDst)[0] = fLuminance;
        (*pDst)[1] = fLuminance;
        (*pDst)[2] = fLuminance;
        (*pDst)[3] = 255.0f;
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertFromA8L8(int iTexelCount, const char* pSrcTexels,
    SEFloat4* pDstTexels)
{
    const unsigned char* pSrc = (const unsigned char*)pSrcTexels;
    SEFloat4* pDst = pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pDst )
    {
        float fLuminance = (float)(*pSrc++);
        float fAlpha = (float)(*pSrc++);
        (*pDst)[0] = fLuminance;
        (*pDst)[1] = fLuminance;
        (*pDst)[2] = fLuminance;
        (*pDst)[3] = fAlpha;
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertFromR8G8B8(int iTexelCount, const char* pSrcTexels,
    SEFloat4* pDstTexels)
{
    const unsigned char* pSrc = (const unsigned char*)pSrcTexels;
    SEFloat4* pDst = pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pDst )
    {
        (*pDst)[2] = (float)(*pSrc++);
        (*pDst)[1] = (float)(*pSrc++);
        (*pDst)[0] = (float)(*pSrc++);
        (*pDst)[3] = 0.0f;
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertFromA8R8G8B8(int iTexelCount, const char* pSrcTexels,
    SEFloat4* pDstTexels)
{
    const unsigned char* pSrc = (const unsigned char*)pSrcTexels;
    SEFloat4* pDst = pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pDst )
    {
        (*pDst)[2] = (float)(*pSrc++);
        (*pDst)[1] = (float)(*pSrc++);
        (*pDst)[0] = (float)(*pSrc++);
        (*pDst)[3] = (float)(*pSrc++);
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertFromA8B8G8R8(int iTexelCount, const char* pSrcTexels,
    SEFloat4* pDstTexels)
{
    const unsigned char* pSrc = (const unsigned char*)pSrcTexels;
    SEFloat4* pDst = pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pDst )
    {
        (*pDst)[0] = (float)(*pSrc++);
        (*pDst)[1] = (float)(*pSrc++);
        (*pDst)[2] = (float)(*pSrc++);
        (*pDst)[3] = (float)(*pSrc++);
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertFromL16(int iTexelCount, const char* pSrcTexels,
    SEFloat4* pDstTexels)
{
    const unsigned short* pSrc = (const unsigned short*)pSrcTexels;
    SEFloat4* pDst = pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pDst )
    {
        float fLuminance = (float)(*pSrc++);
        (*pDst)[0] = fLuminance;
        (*pDst)[1] = fLuminance;
        (*pDst)[2] = fLuminance;
        (*pDst)[3] = 65535.0f;
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertFromG16R16(int iTexelCount, const char* pSrcTexels,
    SEFloat4* pDstTexels)
{
    const unsigned short* pSrc = (const unsigned short*)pSrcTexels;
    SEFloat4* pDst = pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pDst )
    {
        (*pDst)[0] = (float)(*pSrc++);
        (*pDst)[1] = (float)(*pSrc++);
        (*pDst)[2] = 0.0f;
        (*pDst)[3] = 0.0f;
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertFromA16B16G16R16(int iTexelCount, const char* pSrcTexels,
    SEFloat4* pDstTexels)
{
    const unsigned short* pSrc = (const unsigned short*)pSrcTexels;
    SEFloat4* pDst = pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pDst )
    {
        (*pDst)[0] = (float)(*pSrc++);
        (*pDst)[1] = (float)(*pSrc++);
        (*pDst)[2] = (float)(*pSrc++);
        (*pDst)[3] = (float)(*pSrc++);
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertFromR16F(int iTexelCount, const char* pSrcTexels,
    SEFloat4* pDstTexels)
{
    const SEHalfFloat* pSrc = (const SEHalfFloat*)pSrcTexels;
    SEFloat4* pDst = pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pDst )
    {
        (*pDst)[0] = ToFloat(*pSrc++);
        (*pDst)[1] = 0.0f;
        (*pDst)[2] = 0.0f;
        (*pDst)[3] = 0.0f;
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertFromG16R16F(int iTexelCount, const char* pSrcTexels,
    SEFloat4* pDstTexels)
{
    const SEHalfFloat* pSrc = (const SEHalfFloat*)pSrcTexels;
    SEFloat4* pDst = pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pDst )
    {
        (*pDst)[0] = ToFloat(*pSrc++);
        (*pDst)[1] = ToFloat(*pSrc++);
        (*pDst)[2] = 0.0f;
        (*pDst)[3] = 0.0f;
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertFromA16B16G16R16F(int iTexelCount, const char* pSrcTexels,
    SEFloat4* pDstTexels)
{
    const SEHalfFloat* pSrc = (const SEHalfFloat*)pSrcTexels;
    SEFloat4* pDst = pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pDst )
    {
        (*pDst)[0] = ToFloat(*pSrc++);
        (*pDst)[1] = ToFloat(*pSrc++);
        (*pDst)[2] = ToFloat(*pSrc++);
        (*pDst)[3] = ToFloat(*pSrc++);
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertFromR32F(int iTexelCount, const char* pSrcTexels,
    SEFloat4* pDstTexels)
{
    const float* pSrc = (const float*)pSrcTexels;
    SEFloat4* pDst = pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pDst )
    {
        (*pDst)[0] = *pSrc++;
        (*pDst)[1] = 0.0f;
        (*pDst)[2] = 0.0f;
        (*pDst)[3] = 0.0f;
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertFromG32R32F(int iTexelCount, const char* pSrcTexels,
    SEFloat4* pDstTexels)
{
    const float* pSrc = (const float*)pSrcTexels;
    SEFloat4* pDst = pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pDst )
    {
        (*pDst)[0] = *pSrc++;
        (*pDst)[1] = *pSrc++;
        (*pDst)[2] = 0.0f;
        (*pDst)[3] = 0.0f;
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertFromA32B32G32R32F(int iTexelCount, const char* pSrcTexels,
    SEFloat4* pDstTexels)
{
    const float* pSrc = (const float*)pSrcTexels;
    SEFloat4* pDst = pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pDst )
    {
        (*pDst)[0] = *pSrc++;
        (*pDst)[1] = *pSrc++;
        (*pDst)[2] = *pSrc++;
        (*pDst)[3] = *pSrc++;
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Convert to the specified format from 32-bit RGBA color.
//----------------------------------------------------------------------------
void SEColor::ConvertToR5G6B5(int iTexelCount, const SEFloat4* pSrcTexels,
    char* pDstTexels)
{
    const SEFloat4* pSrc = pSrcTexels;
    unsigned short* pDst = (unsigned short*)pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pSrc )
    {
        unsigned short r = (unsigned short)(*pSrc)[0];
        unsigned short g = (unsigned short)(*pSrc)[1];
        unsigned short b = (unsigned short)(*pSrc)[2];
        *pDst++ = b | (g << 5) | (r << 11);
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertToA1R5G5B5(int iTexelCount, const SEFloat4* pSrcTexels,
    char* pDstTexels)
{
    const SEFloat4* pSrc = pSrcTexels;
    unsigned short* pDst = (unsigned short*)pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pSrc )
    {
        unsigned short r = (unsigned short)(*pSrc)[0];
        unsigned short g = (unsigned short)(*pSrc)[1];
        unsigned short b = (unsigned short)(*pSrc)[2];
        unsigned short a = (unsigned short)(*pSrc)[3];
        *pDst++ = b | (g << 5) | (r << 10) | (a << 15);
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertToA4R4G4B4(int iTexelCount, const SEFloat4* pSrcTexels,
    char* pDstTexels)
{
    const SEFloat4* pSrc = pSrcTexels;
    unsigned short* pDst = (unsigned short*)pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pSrc )
    {
        unsigned short r = (unsigned short)(*pSrc)[0];
        unsigned short g = (unsigned short)(*pSrc)[1];
        unsigned short b = (unsigned short)(*pSrc)[2];
        unsigned short a = (unsigned short)(*pSrc)[3];
        *pDst++ = b | (g << 4) | (r << 8) | (a << 12);
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertToA8(int iTexelCount, const SEFloat4* pSrcTexels,
    char* pDstTexels)
{
    const SEFloat4* pSrc = pSrcTexels;
    unsigned char* pDst = (unsigned char*)pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pSrc )
    {
        *pDst++ = (unsigned char)(*pSrc)[3];
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertToL8(int iTexelCount, const SEFloat4* pSrcTexels,
    char* pDstTexels)
{
    const SEFloat4* pSrc = pSrcTexels;
    unsigned char* pDst = (unsigned char*)pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pSrc )
    {
        *pDst++ = (unsigned char)(*pSrc)[0];
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertToA8L8(int iTexelCount, const SEFloat4* pSrcTexels,
    char* pDstTexels)
{
    const SEFloat4* pSrc = pSrcTexels;
    unsigned char* pDst = (unsigned char*)pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pSrc )
    {
        *pDst++ = (unsigned char)(*pSrc)[0];
        *pDst++ = (unsigned char)(*pSrc)[3];
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertToR8G8B8(int iTexelCount, const SEFloat4* pSrcTexels,
    char* pDstTexels)
{
    const SEFloat4* pSrc = pSrcTexels;
    unsigned char* pDst = (unsigned char*)pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pSrc )
    {
        *pDst++ = (unsigned char)(*pSrc)[2];
        *pDst++ = (unsigned char)(*pSrc)[1];
        *pDst++ = (unsigned char)(*pSrc)[0];
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertToA8R8G8B8(int iTexelCount, const SEFloat4* pSrcTexels,
    char* pDstTexels)
{
    const SEFloat4* pSrc = pSrcTexels;
    unsigned char* pDst = (unsigned char*)pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pSrc )
    {
        *pDst++ = (unsigned char)(*pSrc)[2];
        *pDst++ = (unsigned char)(*pSrc)[1];
        *pDst++ = (unsigned char)(*pSrc)[0];
        *pDst++ = (unsigned char)(*pSrc)[3];
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertToA8B8G8R8(int iTexelCount, const SEFloat4* pSrcTexels,
    char* pDstTexels)
{
    const SEFloat4* pSrc = pSrcTexels;
    unsigned char* pDst = (unsigned char*)pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pSrc )
    {
        *pDst++ = (unsigned char)(*pSrc)[0];
        *pDst++ = (unsigned char)(*pSrc)[1];
        *pDst++ = (unsigned char)(*pSrc)[2];
        *pDst++ = (unsigned char)(*pSrc)[3];
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertToL16(int iTexelCount, const SEFloat4* pSrcTexels,
    char* pDstTexels)
{
    const SEFloat4* pSrc = pSrcTexels;
    unsigned short* pDst = (unsigned short*)pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pSrc )
    {
        *pDst++ = (unsigned short)(*pSrc)[0];
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertToG16R16(int iTexelCount, const SEFloat4* pSrcTexels,
    char* pDstTexels)
{
    const SEFloat4* pSrc = pSrcTexels;
    unsigned short* pDst = (unsigned short*)pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pSrc )
    {
        *pDst++ = (unsigned short)(*pSrc)[0];
        *pDst++ = (unsigned short)(*pSrc)[1];
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertToA16B16G16R16(int iTexelCount, 
    const SEFloat4* pSrcTexels, char* pDstTexels)
{
    const SEFloat4* pSrc = pSrcTexels;
    unsigned short* pDst = (unsigned short*)pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pSrc )
    {
        *pDst++ = (unsigned short)(*pSrc)[0];
        *pDst++ = (unsigned short)(*pSrc)[1];
        *pDst++ = (unsigned short)(*pSrc)[2];
        *pDst++ = (unsigned short)(*pSrc)[3];
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertToR16F(int iTexelCount, const SEFloat4* pSrcTexels,
    char* pDstTexels)
{
    const SEFloat4* pSrc = pSrcTexels;
    SEHalfFloat* pDst = (SEHalfFloat*)pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pSrc )
    {
        *pDst++ = ToHalf((*pSrc)[0]);
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertToG16R16F(int iTexelCount, const SEFloat4* pSrcTexels,
    char* pDstTexels)
{
    const SEFloat4* pSrc = pSrcTexels;
    SEHalfFloat* pDst = (SEHalfFloat*)pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pSrc )
    {
        *pDst++ = ToHalf((*pSrc)[0]);
        *pDst++ = ToHalf((*pSrc)[1]);
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertToA16B16G16R16F(int iTexelCount, 
    const SEFloat4* pSrcTexels, char* pDstTexels)
{
    const SEFloat4* pSrc = pSrcTexels;
    SEHalfFloat* pDst = (SEHalfFloat*)pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pSrc )
    {
        *pDst++ = ToHalf((*pSrc)[0]);
        *pDst++ = ToHalf((*pSrc)[1]);
        *pDst++ = ToHalf((*pSrc)[2]);
        *pDst++ = ToHalf((*pSrc)[3]);
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertToR32F(int iTexelCount, const SEFloat4* pSrcTexels,
    char* pDstTexels)
{
    const SEFloat4* pSrc = pSrcTexels;
    float* pDst = (float*)pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pSrc )
    {
        *pDst++ = (*pSrc)[0];
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertToG32R32F(int iTexelCount, const SEFloat4* pSrcTexels,
    char* pDstTexels)
{
    const SEFloat4* pSrc = pSrcTexels;
    float* pDst = (float*)pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pSrc )
    {
        *pDst++ = (*pSrc)[0];
        *pDst++ = (*pSrc)[1];
    }
}
//----------------------------------------------------------------------------
void SEColor::ConvertToA32B32G32R32F(int iTexelCount, 
    const SEFloat4* pSrcTexels, char* pDstTexels)
{
    const SEFloat4* pSrc = pSrcTexels;
    float* pDst = (float*)pDstTexels;
    for( int i = 0; i < iTexelCount; ++i, ++pSrc )
    {
        *pDst++ = (*pSrc)[0];
        *pDst++ = (*pSrc)[1];
        *pDst++ = (*pSrc)[2];
        *pDst++ = (*pSrc)[3];
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
SEColor::ConvertFromFunction SEColor::FromFunction[CF_COUNT] =
{
    0,
    &ConvertFromR5G6B5,
    &ConvertFromA1R5G5B5,
    &ConvertFromA4R4G4B4,
    &ConvertFromA8,
    &ConvertFromL8,
    &ConvertFromA8L8,
    &ConvertFromR8G8B8,
    &ConvertFromA8R8G8B8,
    &ConvertFromA8B8G8R8,
    &ConvertFromL16,
    &ConvertFromG16R16,
    &ConvertFromA16B16G16R16,
    &ConvertFromR16F,
    &ConvertFromG16R16F,
    &ConvertFromA16B16G16R16F,
    &ConvertFromR32F,
    &ConvertFromG32R32F,
    &ConvertFromA32B32G32R32F,
    0,
    0,
    0,
    0
};
//----------------------------------------------------------------------------
SEColor::ConvertToFunction SEColor::ToFunction[CF_COUNT] =
{
    0,
    &ConvertToR5G6B5,
    &ConvertToA1R5G5B5,
    &ConvertToA4R4G4B4,
    &ConvertToA8,
    &ConvertToL8,
    &ConvertToA8L8,
    &ConvertToR8G8B8,
    &ConvertToA8R8G8B8,
    &ConvertToA8B8G8R8,
    &ConvertToL16,
    &ConvertToG16R16,
    &ConvertToA16B16G16R16,
    &ConvertToR16F,
    &ConvertToG16R16F,
    &ConvertToA16B16G16R16F,
    &ConvertToR32F,
    &ConvertToG32R32F,
    &ConvertToA32B32G32R32F,
    0,
    0,
    0,
    0
};
//----------------------------------------------------------------------------
