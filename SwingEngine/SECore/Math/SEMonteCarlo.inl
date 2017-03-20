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

//----------------------------------------------------------------------------
template<typename T>
void SEMonteCarlo::Shuffle(T* samp, SE_UInt32 count, SE_UInt32 dims, 
    SERandomNumberGenerator& rng)
{
    for( SE_UInt32 i = 0; i < count; ++i )
    {
        SE_UInt32 other = i + (rng.RandomUInt() % (count - i));
        for( SE_UInt32 j = 0; j < dims; ++j )
        {
            // Swap data.
            SE_UInt32 index1 = dims*i + j;
            SE_UInt32 index2 = dims*other + j;
            T tempValue = samp[index1];
            samp[index1] = samp[index2];
            samp[index2] = tempValue;
        }
    }
}
//----------------------------------------------------------------------------
inline void SEMonteCarlo::CosineSampleHemisphere(float u1, float u2, 
    SEVector3f& dstV)
{
    ConcentricSampleDisk(u1, u2, &dstV.X, &dstV.Y);
    float tempValue = 1.0f - dstV.X*dstV.X - dstV.Y*dstV.Y;
    float z2 = SE_MAX(0.0f, tempValue);
    dstV.Z = sqrtf(z2);
}
//----------------------------------------------------------------------------
inline float SEMonteCarlo::CosineHemispherePdf(float costheta, float)
{
    return costheta * SEMathf::INV_PI;
}
//----------------------------------------------------------------------------
inline float SEMonteCarlo::PowerHeuristic(int nf, float fPdf, int ng, 
    float gPdf)
{
    float f = nf * fPdf, g = ng * gPdf;
    return (f*f) / (f*f + g*g);
}
//----------------------------------------------------------------------------
inline float SEMonteCarlo::RadicalInverse(const int base, int index)
{
    float x = 0.0f;
    float f = 1.0f / base;

    while( index )
    {
        x += f * (float)(index % base);
        index /= base;
        f *= 1.0f / base;
    }
    return x;
}
//----------------------------------------------------------------------------
inline double SEMonteCarlo::PermutedRadicalInverse(SE_UInt32 n, 
    SE_UInt32 base, const SE_UInt32* p)
{
    double val = 0.0;
    double invBase = 1.0 / base, invBi = invBase;

    while( n > 0 )
    {
        SE_UInt32 d_i = p[n % base];
        val += d_i * invBi;
        n = (SE_UInt32)(n * invBase);
        invBi *= invBase;
    }

    return val;
}
//----------------------------------------------------------------------------
inline void SEMonteCarlo::GeneratePermutation(SE_UInt32* buf, SE_UInt32 b, 
    SERandomNumberGenerator& rng)
{
    for( SE_UInt32 i = 0; i < b; ++i )
    {
        buf[i] = i;
    }

    Shuffle(buf, b, 1, rng);
}
//----------------------------------------------------------------------------