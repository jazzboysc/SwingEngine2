// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SEAssetUtilityPCH.h"
#include "SEImageIO.h"
#include "SETarga.h"
#include "SEMath.h"

using namespace Swing;

//----------------------------------------------------------------------------
void SEImageIO::WriteImage(const std::string& name, float* pixels, 
    float* alpha, int xRes, int yRes, int totalXRes, int totalYRes, 
    int xOffset, int yOffset)
{
    SE_ASSERT( name.size() >= 5 );

    uint32_t suffixOffset = name.size() - 4;

    if( !strcmp(name.c_str() + suffixOffset, ".tga") ||
        !strcmp(name.c_str() + suffixOffset, ".TGA") )
    {
        WriteImageTGA(name, pixels, alpha, xRes, yRes, totalXRes, totalYRes, 
            xOffset, yOffset);

        return;
    }
}
//----------------------------------------------------------------------------
void SEImageIO::WriteImageTGA(const std::string& name, float* pixels,
    float*, int xRes, int yRes, int, int,
    int, int)
{
    // Reformat to BGR layout.
    uint8_t* outBuf = SE_NEW uint8_t[3 * xRes * yRes];
    uint8_t* dst = outBuf;
    for( int y = 0; y < yRes; ++y )
    {
        for( int x = 0; x < xRes; ++x )
        {
#define TO_BYTE(v) (uint8_t(SE_Clamp(255.0f * powf((v), 1.0f/2.2f), 0.0f, 255.0f)))
            dst[0] = TO_BYTE(pixels[3*(y*xRes + x) + 2]);
            dst[1] = TO_BYTE(pixels[3*(y*xRes + x) + 1]);
            dst[2] = TO_BYTE(pixels[3*(y*xRes + x) + 0]);
            dst += 3;
        }
    }

    tga_result result;
    result = tga_write_bgr(name.c_str(), outBuf, (uint16_t)xRes, 
        (uint16_t)yRes, 24);
    if( result != TGA_NOERR)
    {
        // TODO:
        //Error("Unable to write output file \"%s\" (%s)", name.c_str(),
        //      tga_error(result));
    }

    SE_DELETE[] outBuf;
}
//----------------------------------------------------------------------------