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

#ifndef Swing_SampledSpectrum_H
#define Swing_SampledSpectrum_H

#include "SEBossaNovaLIB.h"
#include "SECoefficientSpectrum.h"
#include "SESpectrumUtility.h"
#include "SERGBSpectrum.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20130416
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SESampledSpectrum : 
    public SECoefficientSpectrum<SESpectrumUtility::nSpectralSamples>
{
public:
    SESampledSpectrum(float v = 0.0f);
    SESampledSpectrum(
        const SECoefficientSpectrum<SESpectrumUtility::nSpectralSamples>& v);
    SESampledSpectrum(const SERGBSpectrum& r, 
        SESpectrumUtility::SpectrumType type = 
        SESpectrumUtility::SPECTRUM_REFLECTANCE);

    float y() const;
    void ToXYZ(float xyz[3]) const;
    void ToRGB(float rgb[3]) const;
    SERGBSpectrum ToRGBSpectrum() const;

    static void Init();
    static SESampledSpectrum FromSampled(const float* lambda, const float* v, 
        int n);
    static SESampledSpectrum FromRGB(const float rgb[3], 
        SESpectrumUtility::SpectrumType type = 
        SESpectrumUtility::SPECTRUM_REFLECTANCE);
    static SESampledSpectrum FromXYZ(const float xyz[3],
        SESpectrumUtility::SpectrumType type = 
        SESpectrumUtility::SPECTRUM_REFLECTANCE);

protected:
    using SECoefficientSpectrum<SESpectrumUtility::nSpectralSamples>::c;

private:
    static SESampledSpectrum X;
    static SESampledSpectrum Y;
    static SESampledSpectrum Z;
    static SESampledSpectrum rgbRefl2SpectWhite;
    static SESampledSpectrum rgbRefl2SpectCyan;
    static SESampledSpectrum rgbRefl2SpectMagenta;
    static SESampledSpectrum rgbRefl2SpectYellow;
    static SESampledSpectrum rgbRefl2SpectRed;
    static SESampledSpectrum rgbRefl2SpectGreen;
    static SESampledSpectrum rgbRefl2SpectBlue;
    static SESampledSpectrum rgbIllum2SpectWhite;
    static SESampledSpectrum rgbIllum2SpectCyan;
    static SESampledSpectrum rgbIllum2SpectMagenta;
    static SESampledSpectrum rgbIllum2SpectYellow;
    static SESampledSpectrum rgbIllum2SpectRed;
    static SESampledSpectrum rgbIllum2SpectGreen;
    static SESampledSpectrum rgbIllum2SpectBlue;
};

}

#endif