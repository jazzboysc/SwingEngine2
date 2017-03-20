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
#include "SESpecularReflection.h"
#include "SEBSDFUtility.h"

using namespace Swing;

//----------------------------------------------------------------------------
SESpecularReflection::SESpecularReflection(const SESpectrum& r, SEFresnel* f)
    :
    SEBxDF(
        SEBxDF::BxDFType(SEBxDF::BSDF_REFLECTION | SEBxDF::BSDF_SPECULAR)),
    R(r),
    Fresnel(f)
{
}
//----------------------------------------------------------------------------
SESpectrum SESpecularReflection::f(const SEVector3f&, const SEVector3f&) const
{
    return SESpectrum(0.0f);
}
//----------------------------------------------------------------------------
SESpectrum SESpecularReflection::Sample_f(const SEVector3f& wo, 
    SEVector3f* wi, float, float, float* pdf) const
{
    // Compute perfect specular reflection direction.
    *wi = SEVector3f(-wo.X, -wo.Y, wo.Z);
    *pdf = 1.0f;

    return Fresnel->Evaluate(SE_CosTheta(wo)) * R / SE_AbsCosTheta(*wi);
}
//----------------------------------------------------------------------------
float SESpecularReflection::Pdf(const SEVector3f&, const SEVector3f&) const
{
    return 0.0f;
}
//----------------------------------------------------------------------------