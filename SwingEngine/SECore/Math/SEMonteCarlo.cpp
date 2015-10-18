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

#include "SECorePCH.h"
#include "SEMonteCarlo.h"

using namespace Swing;

//----------------------------------------------------------------------------
void SEMonteCarlo::UniformSampleTriangle(float u1, float u2, float* u, 
    float* v)
{
    SE_ASSERT( u && v );

    float su1 = SEMathf::Sqrt(u1);
    *u = 1.0f - su1;
    *v = u2 * su1;
}
//----------------------------------------------------------------------------
void SEMonteCarlo::ConcentricSampleDisk(float u1, float u2, float* dx, 
    float* dy)
{
    float r, theta;

    // Map uniform random numbers to [-1,1]^2.
    float sx = 2 * u1 - 1;
    float sy = 2 * u2 - 1;

    // Map square to (r,\theta).

    // Handle degeneracy at the origin.
    if( sx == 0.0f && sy == 0.0f )
    {
        *dx = 0.0f;
        *dy = 0.0f;
        return;
    }

    if( sx >= -sy )
    {
        if( sx > sy )
        {
            // Handle first region of disk.
            r = sx;
            if( sy > 0.0f )
            {
                theta = sy/r;
            }
            else
            {
                theta = 8.0f + sy/r;
            }
        }
        else
        {
            // Handle second region of disk.
            r = sy;
            theta = 2.0f - sx/r;
        }
    }
    else
    {
        if( sx <= sy )
        {
            // Handle third region of disk.
            r = -sx;
            theta = 4.0f - sy/r;
        }
        else
        {
            // Handle fourth region of disk.
            r = -sy;
            theta = 6.0f + sx/r;
        }
    }

    theta *= SEMathf::PI / 4.0f;
    *dx = r * SEMathf::Cos(theta);
    *dy = r * SEMathf::Sin(theta);
}
//----------------------------------------------------------------------------
void SEMonteCarlo::StratifiedSample1D(float* samp, int nSamples, 
    SERandomNumberGenerator& rng, bool jitter)
{
    float invTot = 1.0f / nSamples;
    for( int i = 0;  i < nSamples; ++i )
    {
        float delta = jitter ? rng.RandomFloat() : 0.5f;
        float tempValue = (i + delta) * invTot; 
        *samp++ = SE_MIN(tempValue, OneMinusEpsilon);
    }
}
//----------------------------------------------------------------------------
void SEMonteCarlo::StratifiedSample2D(float* samp, int nx, int ny, 
    SERandomNumberGenerator& rng, bool jitter)
{
    float dx = 1.0f / nx, dy = 1.0f / ny;
    for( int y = 0; y < ny; ++y )
    {
        for( int x = 0; x < nx; ++x )
        {
            float jx = jitter ? rng.RandomFloat() : 0.5f;
            float jy = jitter ? rng.RandomFloat() : 0.5f;
            float tempX = (x + jx) * dx;
            float tempY = (y + jy) * dy;
            *samp++ = SE_MIN(tempX, OneMinusEpsilon);
            *samp++ = SE_MIN(tempY, OneMinusEpsilon);
        }
    }
}
//----------------------------------------------------------------------------
void SEMonteCarlo::LatinHypercube(float* samples, SE_UInt32 nSamples, 
    SE_UInt32 nDim, SERandomNumberGenerator& rng)
{
    // Generate LHS samples along diagonal.
    float delta = 1.0f / nSamples;
    for( SE_UInt32 i = 0; i < nSamples; ++i )
    {
        for( SE_UInt32 j = 0; j < nDim; ++j )
        {
            float tempValue = (i + (rng.RandomFloat())) * delta;
            samples[nDim * i + j] = SE_MIN(tempValue, OneMinusEpsilon);
        }
    }

    // Permute LHS samples in each dimension.
    for( SE_UInt32 i = 0; i < nDim; ++i )
    {
        for( SE_UInt32 j = 0; j < nSamples; ++j )
        {
            SE_UInt32 other = j + (rng.RandomUInt() % (nSamples - j));

            // Swap data.
            SE_UInt32 index1 = nDim * j + i;
            SE_UInt32 index2 = nDim * other + i;
            float tempValue = samples[index1];
            samples[index1] = samples[index2];
            samples[index2] = tempValue;
        }
    }
}
//----------------------------------------------------------------------------
void SEMonteCarlo::UniformSampleHemisphere(float u1, float u2, 
    SEVector3f& dstV)
{
    dstV.Z = u1;
    float tempValue = 1.0f - dstV.Z*dstV.Z;
    float r2 = SE_MAX(0.0f, tempValue);
    float r = sqrtf(r2);
    float phi = SEMathf::TWO_PI * u2;
    dstV.X = r * cosf(phi);
    dstV.Y = r * sinf(phi);
}
//----------------------------------------------------------------------------
float SEMonteCarlo::UniformHemispherePdf()
{
    return SEMathf::INV_TWO_PI;
}
//----------------------------------------------------------------------------
void SEMonteCarlo::UniformSampleSphere(float u1, float u2, SEVector3f& dstV)
{
    dstV.Z = 1.0f - 2.0f * u1;
    float tempValue = 1.0f - dstV.Z*dstV.Z;
    float r2 = SE_MAX(0.0f, tempValue);
    float r = sqrtf(r2);
    float phi = SEMathf::TWO_PI * u2;
    dstV.X = r * cosf(phi);
    dstV.Y = r * sinf(phi);
}
//----------------------------------------------------------------------------
float SEMonteCarlo::UniformSpherePdf()
{
    return 1.0f / (4.0f * SEMathf::PI);
}
//----------------------------------------------------------------------------
void SEMonteCarlo::UniformSampleCone(float u1, float u2, float costhetamax, 
    SEVector3f& dstV)
{
    float costheta = (1.0f - u1) + u1 * costhetamax;
    float sintheta = sqrtf(1.0f - costheta*costheta);
    float phi = u2 * SEMathf::TWO_PI;
    dstV.X = cosf(phi) * sintheta;
    dstV.Y = sinf(phi) * sintheta;
    dstV.Z = costheta;
}
//----------------------------------------------------------------------------
void SEMonteCarlo::UniformSampleCone(float u1, float u2, float costhetamax, 
    const SEVector3f& x, const SEVector3f& y, const SEVector3f& z, 
    SEVector3f& dstV)
{
    float costheta = SE_Lerp(u1, costhetamax, 1.0f);
    float sintheta = sqrtf(1.0f - costheta*costheta);
    float phi = u2 * SEMathf::TWO_PI;
    dstV = cosf(phi)*sintheta*x + sinf(phi)*sintheta*y + costheta*z;
}
//----------------------------------------------------------------------------
float SEMonteCarlo::UniformConePdf(float cosThetaMax)
{
    return 1.0f / (SEMathf::TWO_PI * (1.0f - cosThetaMax));
}
//----------------------------------------------------------------------------