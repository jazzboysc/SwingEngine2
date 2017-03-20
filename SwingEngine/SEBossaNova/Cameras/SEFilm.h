// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017
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

#ifndef Swing_Film_H
#define Swing_Film_H

#include "SEBossaNovaLIB.h"
#include "SEBNSampler.h"
#include "SESpectrum.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20130520
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SEFilm
{
public:
    SEFilm(int xres, int yres);
    virtual ~SEFilm();

    virtual void AddSample(const SEBNCameraSample& sample,
        const SESpectrum& L) = 0;
    virtual void Splat(const SEBNCameraSample& sample, 
        const SESpectrum& L) = 0;

    virtual void GetSampleExtent(int* xstart, int* xend, int* ystart, 
        int* yend) const = 0;
    virtual void GetPixelExtent(int* xstart, int* xend, int* ystart, 
        int* yend) const = 0;

    virtual void UpdateDisplay(int x0, int y0, int x1, int y1, 
        float splatScale = 1.0f);
    virtual void WriteImage(float splatScale = 1.0f) = 0;

public:
    const int XResolution, YResolution;
};

}

#endif