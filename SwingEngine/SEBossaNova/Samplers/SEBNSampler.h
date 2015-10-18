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

#ifndef Swing_BNSampler_H
#define Swing_BNSampler_H

#include "SEBossaNovaLIB.h"
#include "SEPlatforms.h"
#include "SEMemory.h"
#include "SERandomNumberGenerator.h"
#include "SERayDifferential.h"
#include "SESpectrum.h"
#include "SEBNIntersection.h"
#include <vector>

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20130520
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SEBNCameraSample
{
public:
    SEBNCameraSample();
    ~SEBNCameraSample();

    float ImageX, ImageY;
    float LensU, LensV;
    float Time;
};

class SEBNSampler;
class SEBNSurfaceShader;
class SEBNVolumeShader;
class SEBNScene;
//----------------------------------------------------------------------------
// Description:
// Date:20130522
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SEBNSample : public SEBNCameraSample
{
public:
    SEBNSample(SEBNSampler* sampler, SEBNSurfaceShader* surfaceShader, 
        SEBNVolumeShader* volumeShader, const SEBNScene* scene);
    ~SEBNSample();

    SE_UInt32 Add1DSamples(SE_UInt32 oneDSampleCount);
    SE_UInt32 Add2DSamples(SE_UInt32 twoDSampleCount);

    SEBNSample* Duplicate(int count) const;

public:
    std::vector<SE_UInt32> N1D, N2D;
    float** OneD;
    float** TwoD;

private:
    SEBNSample();

    void AllocateSampleMemory();
};

//----------------------------------------------------------------------------
// Description:
// Date:20130522
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SEBNSampler
{
public:
    SEBNSampler(int xstart, int xend, int ystart, int yend, int spp, 
        float sopen, float sclose);
    virtual ~SEBNSampler();

    virtual int GetMoreSamples(SEBNSample* samples, 
        SERandomNumberGenerator& rng) = 0;
    virtual int MaximumSampleCount() = 0;

    virtual bool ReportResults(SEBNSample* samples, 
        const SERayDifferential* rays, const SESpectrum* Ls, 
        const SEBNIntersection* isects, int count);

    virtual SEBNSampler* GetSubSampler(int num, int count) = 0;
    virtual int RoundSize(int size) const = 0;

public:
    const int XPixelStart, XPixelEnd, YPixelStart, YPixelEnd;
    const int SamplesPerPixel;
    const float InvSqrtSamplesPerPixel;
    const float ShutterOpen, ShutterClose;

protected:
    void ComputeSubWindow(int num, int count, int* xstart, int* xend, 
        int* ystart, int* yend) const;
};

}

#endif