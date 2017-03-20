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

#ifndef Swing_BxDF_H
#define Swing_BxDF_H

#include "SEBossaNovaLIB.h"
#include "SEVector3.h"
#include "SESpectrum.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20130525
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SEBxDF
{
public:
    enum BxDFType
    {
        BSDF_REFLECTION   = 1<<0,
        BSDF_TRANSMISSION = 1<<1,
        BSDF_DIFFUSE      = 1<<2,
        BSDF_GLOSSY       = 1<<3,
        BSDF_SPECULAR     = 1<<4,
        BSDF_ALL_TYPES        = BSDF_DIFFUSE |
                                BSDF_GLOSSY |
                                BSDF_SPECULAR,
        BSDF_ALL_REFLECTION   = BSDF_REFLECTION |
                                BSDF_ALL_TYPES,
        BSDF_ALL_TRANSMISSION = BSDF_TRANSMISSION |
                                BSDF_ALL_TYPES,
        BSDF_ALL              = BSDF_ALL_REFLECTION |
                                BSDF_ALL_TRANSMISSION
    };

    SEBxDF(BxDFType type);
    virtual ~SEBxDF();

    bool MatchesFlags(BxDFType flags) const;

    // BSDF evaluation.
    virtual SESpectrum f(const SEVector3f& wo, 
        const SEVector3f& wi) const = 0;
    virtual SESpectrum Sample_f(const SEVector3f& wo, SEVector3f* wi,
        float u1, float u2, float* pdf) const;

    // Reflectance evaluation.
    virtual SESpectrum rho(const SEVector3f& wo, int nSamples,
        const float* samples) const;
    virtual SESpectrum rho(int nSamples, const float* samples1,
        const float* samples2) const;

    // Pdf evaluation of a given pair of wi and wo.
    virtual float Pdf(const SEVector3f& wi, const SEVector3f& wo) const;

    // BxDF Public Data.
    const BxDFType Type;
};

}

#endif