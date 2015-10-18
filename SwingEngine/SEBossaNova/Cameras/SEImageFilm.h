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

#ifndef Swing_ImageFilm_H
#define Swing_ImageFilm_H

#include "SEBossaNovaLIB.h"
#include "SEFilm.h"
#include "SEFilter.h"
#include "SESystem.h"
#include "SEBlockedArray.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20130520
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SEImageFilm : public SEFilm
{
public:
    SEImageFilm(int xres, int yres, SEFilter* filter, const float crop[4],
        const std::string& filename, bool openWindow);
    ~SEImageFilm();

    void AddSample(const SEBNCameraSample& sample, const SESpectrum& L);
    void Splat(const SEBNCameraSample& sample, const SESpectrum& L);

    void GetSampleExtent(int* xstart, int* xend, int* ystart, int* yend) const;
    void GetPixelExtent(int* xstart, int* xend, int* ystart, int* yend) const;

    void UpdateDisplay(int x0, int y0, int x1, int y1, float splatScale);
    void WriteImage(float splatScale);

private:
    SEFilter* Filter;
    float CropWindow[4];
    std::string Filename;
    int XPixelStart, YPixelStart, XPixelCount, YPixelCount;

    class Pixel
    {
    public:
        Pixel();

        float Lxyz[3];
        float WeightSum;
        float SplatXYZ[3];
        float Pad;
    };

    SEBlockedArray<Pixel, 2>* Pixels;
    float* FilterTable;
};

}

#endif