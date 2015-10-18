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
#include "SESpecularTransmission.h"
#include "SEBSDFUtility.h"

using namespace Swing;

//----------------------------------------------------------------------------
SESpecularTransmission::SESpecularTransmission(const SESpectrum& t, float ei,
    float et)
    :
    SEBxDF(
        SEBxDF::BxDFType(SEBxDF::BSDF_TRANSMISSION | SEBxDF::BSDF_SPECULAR)),
    Fresnel(ei, et)
{
    T = t;
    Etai = ei;
    Etat = et;
}
//----------------------------------------------------------------------------
SESpectrum SESpecularTransmission::f(const SEVector3f&, 
    const SEVector3f&) const
{
    return SESpectrum(0.0f);
}
//----------------------------------------------------------------------------
SESpectrum SESpecularTransmission::Sample_f(const SEVector3f& wo, 
    SEVector3f* wi, float, float, float* pdf) const
{
    // Figure out which eta is incident and which is transmitted.
    bool entering = SE_CosTheta(wo) > 0.0f;
    float ei = Etai, et = Etat;
    if( !entering )
    {
        // Swap data.
        float tmp = ei;
        ei = et;
        et = tmp;
    }

    // Compute transmitted ray direction.
    float sini2 = SE_SinTheta2(wo);
    float eta = ei / et;
    float sint2 = eta * eta * sini2;

    // Handle total internal reflection for transmission.
    if( sint2 >= 1.0f )
    {
        return SESpectrum(0.0f);
    }

    float tempValue = 1.0f - sint2;
    float cost = sqrtf(SE_MAX(0.0f, tempValue));
    if( entering )
    {
        cost = -cost;
    }
    float sintOverSini = eta;
    *wi = SEVector3f(sintOverSini * -wo.X, sintOverSini * -wo.Y, cost);
    *pdf = 1.0f;
    SESpectrum F = Fresnel.Evaluate(SE_CosTheta(wo));

    return /*(ei*ei)/(et*et) * */ (SESpectrum(1.0f) - F) * T /
        SE_AbsCosTheta(*wi);
}
//----------------------------------------------------------------------------
float SESpecularTransmission::Pdf(const SEVector3f&, const SEVector3f&) const
{
    return 0.0f;
}
//----------------------------------------------------------------------------