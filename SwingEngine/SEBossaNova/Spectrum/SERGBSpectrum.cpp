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
#include "SERGBSpectrum.h"
#include "SEAssert.h"
#include <vector>

using namespace Swing;

//----------------------------------------------------------------------------
SERGBSpectrum::SERGBSpectrum(float v) 
    : SECoefficientSpectrum<3>(v)
{
}
//----------------------------------------------------------------------------
SERGBSpectrum::SERGBSpectrum(const SECoefficientSpectrum<3>& v)
    : SECoefficientSpectrum<3>(v)
{
}
//----------------------------------------------------------------------------
SERGBSpectrum::SERGBSpectrum(const SERGBSpectrum& s, 
    SESpectrumUtility::SpectrumType)
{
    *this = s;
}
//----------------------------------------------------------------------------
SERGBSpectrum SERGBSpectrum::FromRGB(const float rgb[3], 
    SESpectrumUtility::SpectrumType)
{
    SERGBSpectrum s;

    s.c[0] = rgb[0];
    s.c[1] = rgb[1];
    s.c[2] = rgb[2];
    SE_ASSERT( !s.HasNaNs() );

    return s;
}
//----------------------------------------------------------------------------
void SERGBSpectrum::ToRGB(float* rgb) const
{
    SE_ASSERT( rgb );

    rgb[0] = c[0];
    rgb[1] = c[1];
    rgb[2] = c[2];
}
//----------------------------------------------------------------------------
const SERGBSpectrum& SERGBSpectrum::ToRGBSpectrum() const
{
    return *this;
}
//----------------------------------------------------------------------------
void SERGBSpectrum::ToXYZ(float xyz[3]) const
{
    SESpectrumUtility::RGBToXYZ(c, xyz);
}
//----------------------------------------------------------------------------
SERGBSpectrum SERGBSpectrum::FromXYZ(const float xyz[3],
    SESpectrumUtility::SpectrumType)
{
    SERGBSpectrum r;
    SESpectrumUtility::XYZToRGB(xyz, r.c);

    return r;
}
//----------------------------------------------------------------------------
float SERGBSpectrum::y() const
{
    const float YWeight[3] = { 0.212671f, 0.715160f, 0.072169f };
    return YWeight[0] * c[0] + YWeight[1] * c[1] + YWeight[2] * c[2];
}
//----------------------------------------------------------------------------
SERGBSpectrum SERGBSpectrum::FromSampled(const float* lambda, const float* v,
    int n)
{
    // Sort samples if unordered, use sorted for returned spectrum.
    if( !SESpectrumUtility::SpectrumSamplesSorted(lambda, v, n) )
    {
        std::vector<float> slambda(&lambda[0], &lambda[n]);
        std::vector<float> sv(&v[0], &v[n]);
        SESpectrumUtility::SortSpectrumSamples(&slambda[0], &sv[0], n);

        return FromSampled(&slambda[0], &sv[0], n);
    }

    float xyz[3] = { 0.0f, 0.0f, 0.0f };
    float yint = 0.0f;

    for( int i = 0; i < SESpectrumUtility::nCIESamples; ++i )
    {
        yint += SESpectrumUtility::CIE_Y[i];
        float val = SESpectrumUtility::InterpolateSpectrumSamples(lambda, v, n,
            SESpectrumUtility::CIE_Lambda[i]);

        xyz[0] += val * SESpectrumUtility::CIE_X[i];
        xyz[1] += val * SESpectrumUtility::CIE_Y[i];
        xyz[2] += val * SESpectrumUtility::CIE_Z[i];
    }

    xyz[0] /= yint;
    xyz[1] /= yint;
    xyz[2] /= yint;

    return FromXYZ(xyz);
}
//----------------------------------------------------------------------------