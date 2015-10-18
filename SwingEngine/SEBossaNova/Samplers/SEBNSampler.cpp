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
#include "SEBNSampler.h"
#include "SEBNSurfaceShader.h"
#include "SEBNVolumeShader.h"
#include "SEBNScene.h"
#include "SEMath.h"

using namespace Swing;

//----------------------------------------------------------------------------
// SEBNCameraSample functions.
//----------------------------------------------------------------------------
SEBNCameraSample::SEBNCameraSample()
{
}
//----------------------------------------------------------------------------
SEBNCameraSample::~SEBNCameraSample()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// SEBNSample functions.
//----------------------------------------------------------------------------
SEBNSample::SEBNSample(SEBNSampler* sampler, SEBNSurfaceShader* surfaceShader, 
    SEBNVolumeShader* volumeShader, const SEBNScene* scene)
{
    if( surfaceShader )
    {
        surfaceShader->OnRequestSamples(sampler, this, scene);
    }

    if( volumeShader )
    {
        volumeShader->OnRequestSamples(sampler, this, scene);
    }

    AllocateSampleMemory();
}
//----------------------------------------------------------------------------
SEBNSample::SEBNSample()
{
    OneD = TwoD = 0;
}
//----------------------------------------------------------------------------
SEBNSample::~SEBNSample()
{
    if( OneD != 0 )
    {
        SE_FreeAligned(OneD[0]);
        SE_FreeAligned(OneD);
    }
}
//----------------------------------------------------------------------------
SE_UInt32 SEBNSample::Add1DSamples(SE_UInt32 oneDSampleCount)
{
    SE_ASSERT( oneDSampleCount > 0 );
    N1D.push_back(oneDSampleCount);

    return N1D.size() - 1;
}
//----------------------------------------------------------------------------
SE_UInt32 SEBNSample::Add2DSamples(SE_UInt32 twoDSampleCount)
{
    SE_ASSERT( twoDSampleCount > 0 );
    N2D.push_back(twoDSampleCount);

    return N2D.size() - 1;
}
//----------------------------------------------------------------------------
SEBNSample* SEBNSample::Duplicate(int count) const
{
    SE_ASSERT( count > 0 );
    SEBNSample* ret = SE_NEW SEBNSample[count];

    for( int i = 0; i < count; ++i )
    {
        ret[i].N1D = N1D;
        ret[i].N2D = N2D;
        ret[i].AllocateSampleMemory();
    }

    return ret;
}
//----------------------------------------------------------------------------
void SEBNSample::AllocateSampleMemory()
{
    // Allocate storage for sample pointers.
    int nPtrs = N1D.size() + N2D.size();
    if( !nPtrs )
    {
        OneD = TwoD = 0;

        return;
    }
    OneD = SE_AllocateAligned<float*>(nPtrs);
    TwoD = OneD + N1D.size();

    // Compute total number of sample values needed.
    int totSamples = 0;
    for( SE_UInt32 i = 0; i < N1D.size(); ++i )
    {
        totSamples += N1D[i];
    }
    for( SE_UInt32 i = 0; i < N2D.size(); ++i )
    {
        totSamples += 2 * N2D[i];
    }

    // Allocate storage for sample values.
    float* mem = SE_AllocateAligned<float>(totSamples);
    for( SE_UInt32 i = 0; i < N1D.size(); ++i )
    {
        OneD[i] = mem;
        mem += N1D[i];
    }
    for( SE_UInt32 i = 0; i < N2D.size(); ++i )
    {
        TwoD[i] = mem;
        mem += 2 * N2D[i];
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// SEBNSampler functions.
//----------------------------------------------------------------------------
SEBNSampler::SEBNSampler(int xstart, int xend, int ystart, int yend, int spp,
    float sopen, float sclose)
    :
    XPixelStart(xstart), 
    XPixelEnd(xend), 
    YPixelStart(ystart),
    YPixelEnd(yend), 
    SamplesPerPixel(spp), 
    InvSqrtSamplesPerPixel(1.0f / SEMathf::Sqrt((float)spp)),
    ShutterOpen(sopen),
    ShutterClose(sclose)
{
}
//----------------------------------------------------------------------------
SEBNSampler::~SEBNSampler()
{
}
//----------------------------------------------------------------------------
bool SEBNSampler::ReportResults(SEBNSample*, const SERayDifferential*,
    const SESpectrum*, const SEBNIntersection*, int)
{
    return true;
}
//----------------------------------------------------------------------------
void SEBNSampler::ComputeSubWindow(int num, int count, int* newXStart,
    int* newXEnd, int* newYStart, int* newYEnd) const
{
    // Determine how many tiles to use in each dimension, nx and ny.
    int dx = XPixelEnd - XPixelStart, dy = YPixelEnd - YPixelStart;
    int nx = count, ny = 1;

    while( (nx & 0x1) == 0 && 2 * dx * ny < dy * nx )
    {
        nx >>= 1;
        ny <<= 1;
    }
    SE_ASSERT( nx * ny == count );

    // Compute x and y pixel sample range for sub-window.
    int xo = num % nx, yo = num / nx;
    float tx0 = float(xo) / float(nx), tx1 = float(xo+1) / float(nx);
    float ty0 = float(yo) / float(ny), ty1 = float(yo+1) / float(ny);
    *newXStart = SE_FloorToInt(SE_Lerp(tx0, (float)XPixelStart, 
        (float)XPixelEnd));
    *newXEnd   = SE_FloorToInt(SE_Lerp(tx1, (float)XPixelStart, 
        (float)XPixelEnd));
    *newYStart = SE_FloorToInt(SE_Lerp(ty0, (float)YPixelStart, 
        (float)YPixelEnd));
    *newYEnd   = SE_FloorToInt(SE_Lerp(ty1, (float)YPixelStart, 
        (float)YPixelEnd));
}
//----------------------------------------------------------------------------