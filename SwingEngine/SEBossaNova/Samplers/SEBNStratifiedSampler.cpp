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

#include "SEBossaNovaPCH.h"
#include "SEBNStratifiedSampler.h"
#include "SEMonteCarlo.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEBNStratifiedSampler::SEBNStratifiedSampler(int xstart, int xend, int ystart,
    int yend, int xs, int ys, bool jitter, float sopen, float sclose)
    :
    SEBNSampler(xstart, xend, ystart, yend, xs * ys, sopen, sclose)
{
    JitterSamples = jitter;
    CurXPos = XPixelStart;
    CurYPos = YPixelStart;
    XPixelSamples = xs;
    YPixelSamples = ys;
    SampleBuffer = SE_NEW float[5 * XPixelSamples * YPixelSamples];
}
//----------------------------------------------------------------------------
SEBNStratifiedSampler::~SEBNStratifiedSampler()
{
    SE_DELETE[] SampleBuffer;
}
//----------------------------------------------------------------------------
int SEBNStratifiedSampler::RoundSize(int size) const
{
    return size;
}
//----------------------------------------------------------------------------
SEBNSampler* SEBNStratifiedSampler::GetSubSampler(int num, int count)
{
    int x0, x1, y0, y1;
    ComputeSubWindow(num, count, &x0, &x1, &y0, &y1);

    if( x0 == x1 || y0 == y1 )
    {
        return 0;
    }

    return SE_NEW SEBNStratifiedSampler(x0, x1, y0, y1, XPixelSamples,
        YPixelSamples, JitterSamples, ShutterOpen, ShutterClose);
}
//----------------------------------------------------------------------------
int SEBNStratifiedSampler::GetMoreSamples(SEBNSample* samples, 
    SERandomNumberGenerator& rng)
{
    if( CurYPos == YPixelEnd )
    {
        return 0;
    }

    int nSamples = XPixelSamples * YPixelSamples;

    // Generate stratified camera samples for (CurXPos, CurYPos).

    // Generate initial stratified samples into SampleBuffer memory.
    float* bufp = SampleBuffer;
    float* imageSamples = bufp; bufp += 2 * nSamples;
    float* lensSamples = bufp;  bufp += 2 * nSamples;
    float* timeSamples = bufp;

    SEMonteCarlo::StratifiedSample2D(imageSamples, XPixelSamples, YPixelSamples,
        rng, JitterSamples);
    SEMonteCarlo::StratifiedSample2D(lensSamples, XPixelSamples, YPixelSamples,
        rng, JitterSamples);
    SEMonteCarlo::StratifiedSample1D(timeSamples, XPixelSamples * YPixelSamples,
        rng, JitterSamples);

    // Shift stratified image samples to pixel coordinates.
    for( int o = 0; o < 2 * XPixelSamples * YPixelSamples; o += 2 )
    {
        imageSamples[o]   += CurXPos;
        imageSamples[o+1] += CurYPos;
    }

    // Decorrelate sample dimensions.
    SEMonteCarlo::Shuffle(lensSamples, XPixelSamples*YPixelSamples, 2, rng);
    SEMonteCarlo::Shuffle(timeSamples, XPixelSamples*YPixelSamples, 1, rng);

    // Initialize stratified samples with camera sample values.
    for( int i = 0; i < nSamples; ++i )
    {
        samples[i].ImageX = imageSamples[2*i];
        samples[i].ImageY = imageSamples[2*i+1];
        samples[i].LensU = lensSamples[2*i];
        samples[i].LensV = lensSamples[2*i+1];
        samples[i].Time = SE_Lerp(timeSamples[i], ShutterOpen, ShutterClose);

        // Generate stratified samples for shaders.
        for( SE_UInt32 j = 0; j < samples[i].N1D.size(); ++j )
        {
            SEMonteCarlo::LatinHypercube(samples[i].OneD[j], 
                samples[i].N1D[j], 1, rng);

            for( SE_UInt32 k = 0; k < samples[i].N1D[j]; ++k )
            {
                SE_ASSERT( samples[i].OneD[j][k] < 1.0f );
            }
        }
        for( SE_UInt32 j = 0; j < samples[i].N2D.size(); ++j )
        {
            SEMonteCarlo::LatinHypercube(samples[i].TwoD[j], 
                samples[i].N2D[j], 2, rng);
        }
    }

    // Advance to next pixel for stratified sampling.
    if( ++CurXPos == XPixelEnd )
    {
        CurXPos = XPixelStart;
        ++CurYPos;
    }

    return nSamples;
}
//----------------------------------------------------------------------------
int SEBNStratifiedSampler::MaximumSampleCount()
{
    return XPixelSamples * YPixelSamples;
}
//----------------------------------------------------------------------------