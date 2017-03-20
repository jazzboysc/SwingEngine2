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

#ifndef Swing_SpectrumUtility_H
#define Swing_SpectrumUtility_H

#include "SEBossaNovaLIB.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20130417
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SESpectrumUtility
{
public:
    static const int SampledLambdaStart = 400;
    static const int SampledLambdaEnd = 700;
    static const int SampledLambdaRange;
    static const int nSpectralSamples = 30;

    // Spectrum samples helper function.
    static bool SpectrumSamplesSorted(const float* lambda, const float* vals, 
        int n);
    static void SortSpectrumSamples(float* lambda, float* vals, int n);
    static float AverageSpectrumSamples(const float* lambda, const float* vals,
        int n, float lambdaStart, float lambdaEnd);

    // Color space transforms.
    static void XYZToRGB(const float xyz[3], float rgb[3]);
    static void RGBToXYZ(const float rgb[3], float xyz[3]);

    static void Blackbody(const float* wl, int n, float temp, float* vals);
    static float InterpolateSpectrumSamples(const float* lambda, 
        const float* vals, int n, float l);

    enum SpectrumType
    {
        SPECTRUM_REFLECTANCE, 
        SPECTRUM_ILLUMINANT
    };

    // Spectral Data Declarations.
    static const int nCIESamples = 471;
    static const float CIE_X[nCIESamples];
    static const float CIE_Y[nCIESamples];
    static const float CIE_Z[nCIESamples];
    static const float CIE_Lambda[nCIESamples];
    static const float CIE_Y_Integral;

    static const int nRGB2SpectSamples = 32;
    static const float RGB2SpectLambda[nRGB2SpectSamples];
    static const float RGBRefl2SpectWhite[nRGB2SpectSamples];
    static const float RGBRefl2SpectCyan[nRGB2SpectSamples];
    static const float RGBRefl2SpectMagenta[nRGB2SpectSamples];
    static const float RGBRefl2SpectYellow[nRGB2SpectSamples];
    static const float RGBRefl2SpectRed[nRGB2SpectSamples];
    static const float RGBRefl2SpectGreen[nRGB2SpectSamples];
    static const float RGBRefl2SpectBlue[nRGB2SpectSamples];
    static const float RGBIllum2SpectWhite[nRGB2SpectSamples];
    static const float RGBIllum2SpectCyan[nRGB2SpectSamples];
    static const float RGBIllum2SpectMagenta[nRGB2SpectSamples];
    static const float RGBIllum2SpectYellow[nRGB2SpectSamples];
    static const float RGBIllum2SpectRed[nRGB2SpectSamples];
    static const float RGBIllum2SpectGreen[nRGB2SpectSamples];
    static const float RGBIllum2SpectBlue[nRGB2SpectSamples];
};

}

#endif