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

#ifndef Swing_CoefficientSpectrum_H
#define Swing_CoefficientSpectrum_H

#include "SEBossaNovaLIB.h"
#include "SEMath.h"
#include "SEAssert.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20130416
//----------------------------------------------------------------------------
template <int nSamples>
class SECoefficientSpectrum
{
public:
    SECoefficientSpectrum(float v = 0.f);

#ifdef _DEBUG

    SECoefficientSpectrum(const SECoefficientSpectrum& s);
    SECoefficientSpectrum& operator=(const SECoefficientSpectrum& s);

#endif

    SECoefficientSpectrum  operator + (const SECoefficientSpectrum& s2) const;
    SECoefficientSpectrum  operator - (const SECoefficientSpectrum& s2) const;
    SECoefficientSpectrum  operator / (const SECoefficientSpectrum& s2) const;
    SECoefficientSpectrum  operator * (const SECoefficientSpectrum& sp) const;
    SECoefficientSpectrum  operator - () const;
    SECoefficientSpectrum  operator / (float a) const;
    SECoefficientSpectrum  operator * (float a) const;

    SECoefficientSpectrum& operator += (const SECoefficientSpectrum& s2);
    SECoefficientSpectrum& operator *= (const SECoefficientSpectrum& sp);
    SECoefficientSpectrum& operator *= (float a);
    SECoefficientSpectrum& operator /= (float a);

    inline float  operator[] (int i) const;
    inline float& operator[] (int i);

    bool operator == (const SECoefficientSpectrum& sp) const;
    bool operator != (const SECoefficientSpectrum& sp) const;

    bool IsBlack() const;

    SECoefficientSpectrum Clamp(float low = 0, 
        float high = SEMathf::MAX_REAL) const;
    bool HasNaNs() const;

    //bool Write(FILE* f) const;
    //bool Read(FILE* f);
    //void Print(FILE* f) const;

protected:
    float c[nSamples];
};

//----------------------------------------------------------------------------
template <int nSamples>
inline SECoefficientSpectrum<nSamples> operator*(float a, 
    const SECoefficientSpectrum<nSamples>& s);
//----------------------------------------------------------------------------
template <int nSamples>
SECoefficientSpectrum<nSamples> Sqrt(const SECoefficientSpectrum<nSamples>& s);
//----------------------------------------------------------------------------
template <int nSamples>
SECoefficientSpectrum<nSamples> Exp(const SECoefficientSpectrum<nSamples>& s);
//----------------------------------------------------------------------------
template <int nSamples>
inline SECoefficientSpectrum<nSamples> Pow(
    const SECoefficientSpectrum<nSamples>& s, float e);
//----------------------------------------------------------------------------

#include "SECoefficientSpectrum.inl"

}

#endif