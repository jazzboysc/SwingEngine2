// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015
//
// This part of Swing Engine is based on PBRT.
/*
    pbrt source code Copyright(c) 1998-2012 Matt Pharr and Greg Humphreys.

    This file is part of pbrt.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */

#include "SEBossaNovaPCH.h"
#include "SEImageFilm.h"
#include "SEMemory.h"
#include "SEAtomicOperation.h"
#include "SESpectrumUtility.h"
#include "SEImageIO.h"

using namespace Swing;

#define FILTER_TABLE_SIZE 16
#define FILTER_TABLE_SIZE_MINUS_ONE 15

//----------------------------------------------------------------------------
SEImageFilm::Pixel::Pixel()
{
    for( int i = 0; i < 3; ++i )
    {
        Lxyz[i] = SplatXYZ[i] = 0.0f;
    }
    WeightSum = 0.0f;
}
//----------------------------------------------------------------------------
SEImageFilm::SEImageFilm(int xres, int yres, SEFilter* filter, 
    const float crop[4], const std::string& filename, bool openWindow)
    :
    SEFilm(xres, yres)
{
    SE_ASSERT( filter );
    Filter = filter;
    memcpy(CropWindow, crop, 4*sizeof(float));
    Filename = filename;

    // Compute film image extent.
    XPixelStart = SE_CeilToInt(XResolution * CropWindow[0]);
    int tempValue = SE_CeilToInt(XResolution * CropWindow[1]) - XPixelStart;
    XPixelCount = SE_MAX(1, tempValue);
    YPixelStart = SE_CeilToInt(YResolution * CropWindow[2]);
    tempValue = SE_CeilToInt(YResolution * CropWindow[3]) - YPixelStart;
    YPixelCount = SE_MAX(1, tempValue);

    // Allocate film image storage.
    Pixels = SE_NEW SEBlockedArray<Pixel, 2>(XPixelCount, YPixelCount);

    // Precompute filter weight table.
    FilterTable = SE_NEW float[FILTER_TABLE_SIZE * FILTER_TABLE_SIZE];
    float* ftp = FilterTable;
    for( int y = 0; y < FILTER_TABLE_SIZE; ++y )
    {
        float fy = ((float)y + 0.5f) * Filter->YWidth / FILTER_TABLE_SIZE;
        for( int x = 0; x < FILTER_TABLE_SIZE; ++x )
        {
            float fx = ((float)x + 0.5f) * Filter->XWidth / FILTER_TABLE_SIZE;
            *ftp++ = Filter->Evaluate(fx, fy);
        }
    }

    // Possibly open window for image display.
    if( openWindow )
    {
        // TODO:
        SE_ASSERT( false );
    }
}
//----------------------------------------------------------------------------
SEImageFilm::~SEImageFilm()
{
    SE_DELETE Pixels;
    SE_DELETE Filter;
    SE_DELETE[] FilterTable;
}
//----------------------------------------------------------------------------
void SEImageFilm::AddSample(const SEBNCameraSample& sample, 
    const SESpectrum& L)
{
    // Compute sample's raster extent.
    float dimageX = sample.ImageX - 0.5f;
    float dimageY = sample.ImageY - 0.5f;
    int x0 = SE_CeilToInt (dimageX - Filter->XWidth);
    int x1 = SE_FloorToInt(dimageX + Filter->XWidth);
    int y0 = SE_CeilToInt (dimageY - Filter->YWidth);
    int y1 = SE_FloorToInt(dimageY + Filter->YWidth);
    x0 = SE_MAX(x0, XPixelStart);
    int tempValue = XPixelStart + XPixelCount - 1;
    x1 = SE_MIN(x1, tempValue);
    y0 = SE_MAX(y0, YPixelStart);
    tempValue = YPixelStart + YPixelCount - 1;
    y1 = SE_MIN(y1, tempValue);
    if( (x1 - x0) < 0 || (y1 - y0) < 0 )
    {
        return;
    }

    // Loop over filter support and add sample to pixel arrays.

    float xyz[3];
    L.ToXYZ(xyz);

    // Precompute x and y filter table offsets.
    int* ifx = SE_ALLOCA(int, x1 - x0 + 1);
    for( int x = x0; x <= x1; ++x )
    {
        float fx = fabsf((x - dimageX)*Filter->InvXWidth*FILTER_TABLE_SIZE);
        int tempValue = SE_FloorToInt(fx);
        ifx[x - x0] = SE_MIN(tempValue, FILTER_TABLE_SIZE_MINUS_ONE);
    }
    int* ify = SE_ALLOCA(int, y1 - y0 + 1);
    for( int y = y0; y <= y1; ++y )
    {
        float fy = fabsf((y - dimageY)*Filter->InvYWidth*FILTER_TABLE_SIZE);
        int tempValue = SE_FloorToInt(fy);
        ify[y - y0] = SE_MIN(tempValue, FILTER_TABLE_SIZE_MINUS_ONE);
    }

    // Multiple threads may operate on part of pixels simultaneously.
    bool syncNeeded = (Filter->XWidth > 0.5f || Filter->YWidth > 0.5f);

    // Update pixels influenced by the sample.
    for( int y = y0; y <= y1; ++y )
    {
        for( int x = x0; x <= x1; ++x )
        {
            // Evaluate filter value at (x,y) pixel.
            int offset = ify[y - y0]*FILTER_TABLE_SIZE + ifx[x - x0];
            float filterWt = FilterTable[offset];

            // Update pixel values with filtered sample contribution.
            Pixel& pixel = (*Pixels)(x - XPixelStart, y - YPixelStart);
            if( !syncNeeded )
            {
                pixel.Lxyz[0]   += filterWt * xyz[0];
                pixel.Lxyz[1]   += filterWt * xyz[1];
                pixel.Lxyz[2]   += filterWt * xyz[2];
                pixel.WeightSum += filterWt;
            }
            else
            {
                // Safely update Lxyz and WeightSum even with concurrency.
                SE_AtomicAdd(&pixel.Lxyz[0], filterWt * xyz[0]);
                SE_AtomicAdd(&pixel.Lxyz[1], filterWt * xyz[1]);
                SE_AtomicAdd(&pixel.Lxyz[2], filterWt * xyz[2]);
                SE_AtomicAdd(&pixel.WeightSum, filterWt);
            }
        }
    }
}
//----------------------------------------------------------------------------
void SEImageFilm::Splat(const SEBNCameraSample& sample, const SESpectrum& L)
{
    if( L.HasNaNs() )
    {
        // TODO:
        // SEImageFilm ignoring splatted spectrum with NaN values.
        return;
    }

    float xyz[3];
    L.ToXYZ(xyz);

    int x = SE_FloorToInt(sample.ImageX);
    int y = SE_FloorToInt(sample.ImageY);
    if( x < XPixelStart || x - XPixelStart >= XPixelCount ||
        y < YPixelStart || y - YPixelStart >= YPixelCount )
    {
        return;
    }

    Pixel& pixel = (*Pixels)(x - XPixelStart, y - YPixelStart);
    SE_AtomicAdd(&pixel.SplatXYZ[0], xyz[0]);
    SE_AtomicAdd(&pixel.SplatXYZ[1], xyz[1]);
    SE_AtomicAdd(&pixel.SplatXYZ[2], xyz[2]);
}
//----------------------------------------------------------------------------
void SEImageFilm::GetSampleExtent(int* xstart, int* xend, int* ystart, 
    int* yend) const
{
    *xstart = SE_FloorToInt(XPixelStart + 0.5f - Filter->XWidth);
    *xend   = SE_CeilToInt(XPixelStart + 0.5f + XPixelCount + Filter->XWidth);

    *ystart = SE_FloorToInt(YPixelStart + 0.5f - Filter->YWidth);
    *yend   = SE_CeilToInt(YPixelStart + 0.5f + YPixelCount + Filter->YWidth);
}
//----------------------------------------------------------------------------
void SEImageFilm::GetPixelExtent(int* xstart, int* xend, int* ystart, 
    int* yend) const
{
    *xstart = XPixelStart;
    *xend   = XPixelStart + XPixelCount;
    *ystart = YPixelStart;
    *yend   = YPixelStart + YPixelCount;
}
//----------------------------------------------------------------------------
void SEImageFilm::UpdateDisplay(int, int, int, int, float)
{
    // TODO:
}
//----------------------------------------------------------------------------
void SEImageFilm::WriteImage(float splatScale)
{
    // Convert image to RGB and compute final pixel values.
    int nPix = XPixelCount * YPixelCount;
    float* rgb = SE_NEW float[3*nPix];
    int offset = 0;
    for( int y = 0; y < YPixelCount; ++y )
    {
        for( int x = 0; x < XPixelCount; ++x )
        {
            // Convert pixel XYZ color to RGB.
            SESpectrumUtility::XYZToRGB((*Pixels)(x, y).Lxyz, &rgb[3*offset]);

            // Normalize pixel with weight sum.
            float weightSum = (*Pixels)(x, y).WeightSum;
            if( weightSum != 0.0f )
            {
                float invWt = 1.0f / weightSum;

                float tempValue = rgb[3*offset] * invWt;
                rgb[3*offset] = SE_MAX(0.0f, tempValue);

                tempValue = rgb[3*offset + 1] * invWt;
                rgb[3*offset + 1] = SE_MAX(0.0f, tempValue);

                tempValue = rgb[3*offset + 2] * invWt;
                rgb[3*offset + 2] = SE_MAX(0.0f, tempValue);
            }

            // Add splat value at pixel.
            float splatRGB[3];
            SESpectrumUtility::XYZToRGB((*Pixels)(x, y).SplatXYZ, splatRGB);
            rgb[3*offset  ] += splatScale * splatRGB[0];
            rgb[3*offset+1] += splatScale * splatRGB[1];
            rgb[3*offset+2] += splatScale * splatRGB[2];
            ++offset;
        }
    }

    // Write RGB image to file.
    SEImageIO::WriteImage(Filename, rgb, 0, XPixelCount, YPixelCount,
        XResolution, YResolution, XPixelStart, YPixelStart);

    // Release temporary image memory
    SE_DELETE[] rgb;
}
//----------------------------------------------------------------------------