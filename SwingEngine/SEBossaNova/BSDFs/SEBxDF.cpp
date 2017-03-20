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

#include "SEBossaNovaPCH.h"
#include "SEBxDF.h"
#include "SEMonteCarlo.h"
#include "SEBSDFUtility.h"
#include "SEMath.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEBxDF::SEBxDF(BxDFType type)
    :
    Type(type)
{
}
//----------------------------------------------------------------------------
SEBxDF::~SEBxDF()
{
}
//----------------------------------------------------------------------------
bool SEBxDF::MatchesFlags(SEBxDF::BxDFType flags) const
{
    return (Type & flags) == Type;
}
//----------------------------------------------------------------------------
SESpectrum SEBxDF::Sample_f(const SEVector3f& wo, SEVector3f* wi,
    float u1, float u2, float* pdf) const
{
    // Cosine-sample the hemisphere, flipping the direction if necessary.
    SEMonteCarlo::CosineSampleHemisphere(u1, u2, *wi);
    if( wo.Z < 0.0f )
    {
        wi->Z *= -1.0f;
    }
    *pdf = Pdf(wo, *wi);

    return f(wo, *wi);
}
//----------------------------------------------------------------------------
SESpectrum SEBxDF::rho(const SEVector3f& wo, int nSamples,
    const float* samples) const
{
    SESpectrum r = 0.0f;

    for( int i = 0; i < nSamples; ++i )
    {
        // Estimate one term of the estimator.
        SEVector3f wi;
        float pdf = 0.0f;
        SESpectrum f = Sample_f(wo, &wi, samples[2*i], samples[2*i+1], &pdf);

        if( pdf > 0.0f )
        {
            r += f * SE_AbsCosTheta(wi) / pdf;
        }
    }

    return r / float(nSamples);
}
//----------------------------------------------------------------------------
SESpectrum SEBxDF::rho(int nSamples, const float* samples1,
    const float* samples2) const
{
    SESpectrum r = 0.0f;

    for( int i = 0; i < nSamples; ++i )
    {
        // Estimate one term of the estimator.
        SEVector3f wo, wi;
        SEMonteCarlo::UniformSampleHemisphere(samples1[2*i], samples1[2*i+1], 
            wo);
        float pdf_o = SEMathf::INV_TWO_PI, pdf_i = 0.0f;
        SESpectrum f = Sample_f(wo, &wi, samples2[2*i], samples2[2*i+1], 
            &pdf_i);

        if( pdf_i > 0.0f )
        {
            r += f*SE_AbsCosTheta(wi)*SE_AbsCosTheta(wo) / (pdf_o*pdf_i);
        }
    }

    return r / (SEMathf::PI * nSamples);
}
//----------------------------------------------------------------------------
float SEBxDF::Pdf(const SEVector3f& wi, const SEVector3f& wo) const
{
    bool sameHemisphere = SE_SameHemisphere(wo, wi);
    return sameHemisphere ? SE_AbsCosTheta(wi) * SEMathf::INV_PI : 0.0f;
}
//----------------------------------------------------------------------------