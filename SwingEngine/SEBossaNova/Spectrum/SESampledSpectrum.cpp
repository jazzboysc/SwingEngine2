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
#include "SESampledSpectrum.h"
#include "SERGBSpectrum.h"
#include "SEAssert.h"
#include "SEMath.h"
#include <vector>

//----------------------------------------------------------------------------
#if defined(SE_IS_WINDOWS)
#pragma warning (disable : 4244) // int -> float conversion
#endif
//----------------------------------------------------------------------------

using namespace Swing;

SESampledSpectrum SESampledSpectrum::X;
SESampledSpectrum SESampledSpectrum::Y;
SESampledSpectrum SESampledSpectrum::Z;
SESampledSpectrum SESampledSpectrum::rgbRefl2SpectWhite;
SESampledSpectrum SESampledSpectrum::rgbRefl2SpectCyan;
SESampledSpectrum SESampledSpectrum::rgbRefl2SpectMagenta;
SESampledSpectrum SESampledSpectrum::rgbRefl2SpectYellow;
SESampledSpectrum SESampledSpectrum::rgbRefl2SpectRed;
SESampledSpectrum SESampledSpectrum::rgbRefl2SpectGreen;
SESampledSpectrum SESampledSpectrum::rgbRefl2SpectBlue;
SESampledSpectrum SESampledSpectrum::rgbIllum2SpectWhite;
SESampledSpectrum SESampledSpectrum::rgbIllum2SpectCyan;
SESampledSpectrum SESampledSpectrum::rgbIllum2SpectMagenta;
SESampledSpectrum SESampledSpectrum::rgbIllum2SpectYellow;
SESampledSpectrum SESampledSpectrum::rgbIllum2SpectRed;
SESampledSpectrum SESampledSpectrum::rgbIllum2SpectGreen;
SESampledSpectrum SESampledSpectrum::rgbIllum2SpectBlue;

//----------------------------------------------------------------------------
SESampledSpectrum::SESampledSpectrum(float v)
{
    for( int i = 0; i < SESpectrumUtility::nSpectralSamples; ++i )
    {
        c[i] = v;
    }
}
//----------------------------------------------------------------------------
SESampledSpectrum::SESampledSpectrum(
    const SECoefficientSpectrum<SESpectrumUtility::nSpectralSamples>& v)
    : SECoefficientSpectrum<SESpectrumUtility::nSpectralSamples>(v)
{
}
//----------------------------------------------------------------------------
SESampledSpectrum::SESampledSpectrum(const SERGBSpectrum& r, 
    SESpectrumUtility::SpectrumType t)
{
    float rgb[3];
    r.ToRGB(rgb);
    *this = SESampledSpectrum::FromRGB(rgb, t);
}
//----------------------------------------------------------------------------
SESampledSpectrum SESampledSpectrum::FromSampled(const float* lambda, 
    const float* v, int n)
{
    SE_ASSERT( lambda && v );

    // Sort samples if unordered, use sorted for returned spectrum.
    if( !SESpectrumUtility::SpectrumSamplesSorted(lambda, v, n) )
    {
        std::vector<float> slambda(&lambda[0], &lambda[n]);
        std::vector<float> sv(&v[0], &v[n]);
        SESpectrumUtility::SortSpectrumSamples(&slambda[0], &sv[0], n);

        return FromSampled(&slambda[0], &sv[0], n);
    }

    SESampledSpectrum r;
    for( int i = 0; i < SESpectrumUtility::nSpectralSamples; ++i )
    {
        // Compute average value of given SPD over $i$th sample's range.

        float t0 = float(i) / float(SESpectrumUtility::nSpectralSamples);
        float lambda0 = SE_Lerp(t0, SESpectrumUtility::SampledLambdaStart, 
            SESpectrumUtility::SampledLambdaEnd);

        float t1 = float(i + 1) / float(SESpectrumUtility::nSpectralSamples);
        float lambda1 = SE_Lerp(t1, SESpectrumUtility::SampledLambdaStart, 
            SESpectrumUtility::SampledLambdaEnd);

        r.c[i] = SESpectrumUtility::AverageSpectrumSamples(lambda, v, n, 
            lambda0, lambda1);
    }
    return r;
}
//----------------------------------------------------------------------------
void SESampledSpectrum::Init()
{
    // Compute XYZ matching functions for SampledSpectrum.
    for( int i = 0; i < SESpectrumUtility::nSpectralSamples; ++i )
    {
        float t0 = float(i) / float(SESpectrumUtility::nSpectralSamples);
        float wl0 = SE_Lerp(t0, SESpectrumUtility::SampledLambdaStart, 
            SESpectrumUtility::SampledLambdaEnd);

        float t1 = float(i + 1) / float(SESpectrumUtility::nSpectralSamples);
        float wl1 = SE_Lerp(t1, SESpectrumUtility::SampledLambdaStart, 
            SESpectrumUtility::SampledLambdaEnd);

        X.c[i] = SESpectrumUtility::AverageSpectrumSamples(
            SESpectrumUtility::CIE_Lambda, SESpectrumUtility::CIE_X, 
            SESpectrumUtility::nCIESamples, wl0, wl1);

        Y.c[i] = SESpectrumUtility::AverageSpectrumSamples(
            SESpectrumUtility::CIE_Lambda, SESpectrumUtility::CIE_Y, 
            SESpectrumUtility::nCIESamples, wl0, wl1);

        Z.c[i] = SESpectrumUtility::AverageSpectrumSamples(
            SESpectrumUtility::CIE_Lambda, SESpectrumUtility::CIE_Z, 
            SESpectrumUtility::nCIESamples, wl0, wl1);
	}

    // Compute RGB to spectrum functions for SampledSpectrum.
    for( int i = 0; i < SESpectrumUtility::nSpectralSamples; ++i )
    {
        float t0 = float(i) / float(SESpectrumUtility::nSpectralSamples);
        float wl0 = SE_Lerp(t0, SESpectrumUtility::SampledLambdaStart, 
            SESpectrumUtility::SampledLambdaEnd);

        float t1 = float(i + 1) / float(SESpectrumUtility::nSpectralSamples);
        float wl1 = SE_Lerp(t1, SESpectrumUtility::SampledLambdaStart, 
            SESpectrumUtility::SampledLambdaEnd);

        rgbRefl2SpectWhite.c[i] = SESpectrumUtility::AverageSpectrumSamples(
            SESpectrumUtility::RGB2SpectLambda, 
            SESpectrumUtility::RGBRefl2SpectWhite, 
            SESpectrumUtility::nRGB2SpectSamples, wl0, wl1);

        rgbRefl2SpectCyan.c[i] = SESpectrumUtility::AverageSpectrumSamples(
            SESpectrumUtility::RGB2SpectLambda, 
            SESpectrumUtility::RGBRefl2SpectCyan,
            SESpectrumUtility::nRGB2SpectSamples, wl0, wl1);

        rgbRefl2SpectMagenta.c[i] = SESpectrumUtility::AverageSpectrumSamples(
            SESpectrumUtility::RGB2SpectLambda, 
            SESpectrumUtility::RGBRefl2SpectMagenta,
            SESpectrumUtility::nRGB2SpectSamples, wl0, wl1);

        rgbRefl2SpectYellow.c[i] = SESpectrumUtility::AverageSpectrumSamples(
            SESpectrumUtility::RGB2SpectLambda, 
            SESpectrumUtility::RGBRefl2SpectYellow,
            SESpectrumUtility::nRGB2SpectSamples, wl0, wl1);

        rgbRefl2SpectRed.c[i] = SESpectrumUtility::AverageSpectrumSamples(
            SESpectrumUtility::RGB2SpectLambda, 
            SESpectrumUtility::RGBRefl2SpectRed,
            SESpectrumUtility::nRGB2SpectSamples, wl0, wl1);

        rgbRefl2SpectGreen.c[i] = SESpectrumUtility::AverageSpectrumSamples(
            SESpectrumUtility::RGB2SpectLambda, 
            SESpectrumUtility::RGBRefl2SpectGreen,
            SESpectrumUtility::nRGB2SpectSamples, wl0, wl1);

        rgbRefl2SpectBlue.c[i] = SESpectrumUtility::AverageSpectrumSamples(
            SESpectrumUtility::RGB2SpectLambda, 
            SESpectrumUtility::RGBRefl2SpectBlue,
            SESpectrumUtility::nRGB2SpectSamples, wl0, wl1);
    

        rgbIllum2SpectWhite.c[i] = SESpectrumUtility::AverageSpectrumSamples(
            SESpectrumUtility::RGB2SpectLambda, 
            SESpectrumUtility::RGBIllum2SpectWhite,
            SESpectrumUtility::nRGB2SpectSamples, wl0, wl1);

        rgbIllum2SpectCyan.c[i] = SESpectrumUtility::AverageSpectrumSamples(
            SESpectrumUtility::RGB2SpectLambda, 
            SESpectrumUtility::RGBIllum2SpectCyan,
            SESpectrumUtility::nRGB2SpectSamples, wl0, wl1);

        rgbIllum2SpectMagenta.c[i] = SESpectrumUtility::AverageSpectrumSamples(
            SESpectrumUtility::RGB2SpectLambda, 
            SESpectrumUtility::RGBIllum2SpectMagenta,
            SESpectrumUtility::nRGB2SpectSamples, wl0, wl1);

        rgbIllum2SpectYellow.c[i] = SESpectrumUtility::AverageSpectrumSamples(
            SESpectrumUtility::RGB2SpectLambda, 
            SESpectrumUtility::RGBIllum2SpectYellow,
            SESpectrumUtility::nRGB2SpectSamples, wl0, wl1);

        rgbIllum2SpectRed.c[i] = SESpectrumUtility::AverageSpectrumSamples(
            SESpectrumUtility::RGB2SpectLambda, 
            SESpectrumUtility::RGBIllum2SpectRed,
            SESpectrumUtility::nRGB2SpectSamples, wl0, wl1);

        rgbIllum2SpectGreen.c[i] = SESpectrumUtility::AverageSpectrumSamples(
            SESpectrumUtility::RGB2SpectLambda, 
            SESpectrumUtility::RGBIllum2SpectGreen,
            SESpectrumUtility::nRGB2SpectSamples, wl0, wl1);

        rgbIllum2SpectBlue.c[i] = SESpectrumUtility::AverageSpectrumSamples(
            SESpectrumUtility::RGB2SpectLambda, 
            SESpectrumUtility::RGBIllum2SpectBlue,
            SESpectrumUtility::nRGB2SpectSamples, wl0, wl1);
    }
}
//----------------------------------------------------------------------------
void SESampledSpectrum::ToXYZ(float xyz[3]) const
{
    xyz[0] = xyz[1] = xyz[2] = 0.0f;

    for( int i = 0; i < SESpectrumUtility::nSpectralSamples; ++i )
    {
        xyz[0] += X.c[i] * c[i];
        xyz[1] += Y.c[i] * c[i];
        xyz[2] += Z.c[i] * c[i];
    }

    float scale = float(SESpectrumUtility::SampledLambdaRange) /
        float(SESpectrumUtility::CIE_Y_Integral * 
        SESpectrumUtility::nSpectralSamples);

    xyz[0] *= scale;
    xyz[1] *= scale;
    xyz[2] *= scale;
}
//----------------------------------------------------------------------------
float SESampledSpectrum::y() const
{
    float yy = 0.f;

    for( int i = 0; i < SESpectrumUtility::nSpectralSamples; ++i )
    {
        yy += Y.c[i] * c[i];
    }

    return yy * float(SESpectrumUtility::SampledLambdaRange) /
        float(SESpectrumUtility::CIE_Y_Integral * 
        SESpectrumUtility::nSpectralSamples);
}
//----------------------------------------------------------------------------
void SESampledSpectrum::ToRGB(float rgb[3]) const
{
    float xyz[3];
    ToXYZ(xyz);
    SESpectrumUtility::XYZToRGB(xyz, rgb);
}
//----------------------------------------------------------------------------
SERGBSpectrum SESampledSpectrum::ToRGBSpectrum() const
{
    float rgb[3];
    ToRGB(rgb);

    return SERGBSpectrum::FromRGB(rgb);
}
//----------------------------------------------------------------------------
SESampledSpectrum SESampledSpectrum::FromRGB(const float rgb[3],
    SESpectrumUtility::SpectrumType type)
{
    SESampledSpectrum r;

    if( type == SESpectrumUtility::SPECTRUM_REFLECTANCE )
    {
        // Convert reflectance spectrum to RGB.

        if( rgb[0] <= rgb[1] && rgb[0] <= rgb[2] )
        {
            // Compute reflectance SampledSpectrum with rgb[0] as minimum
            r += rgb[0] * rgbRefl2SpectWhite;
            if( rgb[1] <= rgb[2] )
            {
                r += (rgb[1] - rgb[0]) * rgbRefl2SpectCyan;
                r += (rgb[2] - rgb[1]) * rgbRefl2SpectBlue;
            }
            else
            {
                r += (rgb[2] - rgb[0]) * rgbRefl2SpectCyan;
                r += (rgb[1] - rgb[2]) * rgbRefl2SpectGreen;
            }
        }
        else if( rgb[1] <= rgb[0] && rgb[1] <= rgb[2] )
        {
            // Compute reflectance SampledSpectrum with rgb[1] as minimum
            r += rgb[1] * rgbRefl2SpectWhite;
            if( rgb[0] <= rgb[2] )
            {
                r += (rgb[0] - rgb[1]) * rgbRefl2SpectMagenta;
                r += (rgb[2] - rgb[0]) * rgbRefl2SpectBlue;
            }
            else
            {
                r += (rgb[2] - rgb[1]) * rgbRefl2SpectMagenta;
                r += (rgb[0] - rgb[2]) * rgbRefl2SpectRed;
            }
        }
        else
        {
            // Compute reflectance SampledSpectrum with rgb[2] as minimum
            r += rgb[2] * rgbRefl2SpectWhite;
            if( rgb[0] <= rgb[1] )
            {
                r += (rgb[0] - rgb[2]) * rgbRefl2SpectYellow;
                r += (rgb[1] - rgb[0]) * rgbRefl2SpectGreen;
            }
            else
            {
                r += (rgb[1] - rgb[2]) * rgbRefl2SpectYellow;
                r += (rgb[0] - rgb[1]) * rgbRefl2SpectRed;
            }
        }
        r *= 0.94f;
    }
    else
    {
        // Convert illuminant spectrum to RGB.

        if( rgb[0] <= rgb[1] && rgb[0] <= rgb[2] )
        {
            // Compute illuminant SampledSpectrum with rgb[0] as minimum
            r += rgb[0] * rgbIllum2SpectWhite;
            if( rgb[1] <= rgb[2] )
            {
                r += (rgb[1] - rgb[0]) * rgbIllum2SpectCyan;
                r += (rgb[2] - rgb[1]) * rgbIllum2SpectBlue;
            }
            else
            {
                r += (rgb[2] - rgb[0]) * rgbIllum2SpectCyan;
                r += (rgb[1] - rgb[2]) * rgbIllum2SpectGreen;
            }
        }
        else if( rgb[1] <= rgb[0] && rgb[1] <= rgb[2] )
        {
            // Compute illuminant SampledSpectrum with rgb[1] as minimum
            r += rgb[1] * rgbIllum2SpectWhite;
            if( rgb[0] <= rgb[2] )
            {
                r += (rgb[0] - rgb[1]) * rgbIllum2SpectMagenta;
                r += (rgb[2] - rgb[0]) * rgbIllum2SpectBlue;
            }
            else
            {
                r += (rgb[2] - rgb[1]) * rgbIllum2SpectMagenta;
                r += (rgb[0] - rgb[2]) * rgbIllum2SpectRed;
            }
        }
        else
        {
            // Compute illuminant SampledSpectrum with rgb[2] as minimum
            r += rgb[2] * rgbIllum2SpectWhite;
            if( rgb[0] <= rgb[1] )
            {
                r += (rgb[0] - rgb[2]) * rgbIllum2SpectYellow;
                r += (rgb[1] - rgb[0]) * rgbIllum2SpectGreen;
            }
            else
            {
                r += (rgb[1] - rgb[2]) * rgbIllum2SpectYellow;
                r += (rgb[0] - rgb[1]) * rgbIllum2SpectRed;
            }
        }
        r *= 0.86445f;
    }

    return r.Clamp();
}
//----------------------------------------------------------------------------
SESampledSpectrum SESampledSpectrum::FromXYZ(const float xyz[3],
    SESpectrumUtility::SpectrumType type)
{
    float rgb[3];
    SESpectrumUtility::XYZToRGB(xyz, rgb);

    return FromRGB(rgb, type);
}
//----------------------------------------------------------------------------