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

#ifndef Swing_MonteCarlo_H
#define Swing_MonteCarlo_H

#include "SECoreLIB.h"
#include "SEPlatforms.h"
#include "SEMath.h"
#include "SEVector3.h"
#include "SERandomNumberGenerator.h"

namespace Swing
{

// Smallest floating point value less than one; all canonical random 
// samples should be <= this.
#ifdef SE_IS_WINDOWS
// Sadly, MSVC2008 (at least) doesn't support hexidecimal fp constants...
static const float OneMinusEpsilon = 0.9999999403953552f;
#else
static const float OneMinusEpsilon = 0x1.fffffep - 1;
#endif

//----------------------------------------------------------------------------
// Description:
// Date:20130515
//----------------------------------------------------------------------------
class SE_CORE_API SEMonteCarlo
{
public:
    static void UniformSampleTriangle(float u1, float u2, float* u, float* v);
    static void ConcentricSampleDisk(float u1, float u2, float* dx, float* dy);

    static void StratifiedSample1D(float* samples, int nsamples, 
        SERandomNumberGenerator& rng, bool jitter = true);
    static void StratifiedSample2D(float* samples, int nx, int ny, 
        SERandomNumberGenerator& rng, bool jitter = true);

    template<typename T> static void Shuffle(T* samp, SE_UInt32 count, 
        SE_UInt32 dims, SERandomNumberGenerator& rng);

    static void LatinHypercube(float* samples, SE_UInt32 nSamples, 
        SE_UInt32 nDim, SERandomNumberGenerator& rng);

    static void UniformSampleHemisphere(float u1, float u2, SEVector3f& dstV);
    static float UniformHemispherePdf();

    static void UniformSampleSphere(float u1, float u2, SEVector3f& dstV);
    static float UniformSpherePdf();

    static void UniformSampleCone(float u1, float u2, float costhetamax, 
        SEVector3f& dstV);
    static void UniformSampleCone(float u1, float u2, float costhetamax, 
        const SEVector3f& x, const SEVector3f& y, const SEVector3f& z, 
        SEVector3f& dstV);
    static float UniformConePdf(float cosThetaMax);

    static inline void CosineSampleHemisphere(float u1, float u2, 
        SEVector3f& dstV);
    static inline float CosineHemispherePdf(float costheta, float phi);

    static inline float PowerHeuristic(int nf, float fPdf, int ng, float gPdf);

    static inline float RadicalInverse(const int base, int index);

    static inline double PermutedRadicalInverse(SE_UInt32 n, SE_UInt32 base,
        const SE_UInt32* p);

    static inline void GeneratePermutation(SE_UInt32* buf, SE_UInt32 b, 
        SERandomNumberGenerator& rng);
};

#include "SEMonteCarlo.inl"

}

#endif
