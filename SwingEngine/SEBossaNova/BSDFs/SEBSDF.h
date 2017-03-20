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

#ifndef Swing_BSDF_H
#define Swing_BSDF_H

#include "SEBossaNovaLIB.h"
#include "SEBxDF.h"
#include "SEVector3.h"
#include "SEDifferentialGeometry.h"
#include "SERandomNumberGenerator.h"
#include "SEAssert.h"

namespace Swing
{

class SEBSDFSample;
//----------------------------------------------------------------------------
// Description:
// Date:20130515
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SEBSDF
{
public:
    SEBSDF(const SEDifferentialGeometry& dgs, const SEVector3f& normalGeometry,
         float eta = 1.0f);

    // BSDF evaluation.
    SESpectrum f(const SEVector3f& woWorld, const SEVector3f& wiWorld, 
        SEBxDF::BxDFType flags = SEBxDF::BSDF_ALL) const;
    SESpectrum Sample_f(const SEVector3f& woWorld, SEVector3f* wiWorld, 
        const SEBSDFSample& bsdfSample, float* pdf, 
        SEBxDF::BxDFType flags = SEBxDF::BSDF_ALL,
        SEBxDF::BxDFType* sampledType = 0) const;

    // Reflectance evaluation.
    SESpectrum rho(SERandomNumberGenerator& rng, 
        SEBxDF::BxDFType flags = SEBxDF::BSDF_ALL, int sqrtSamples = 6) const;
    SESpectrum rho(const SEVector3f& wo, SERandomNumberGenerator& rng, 
        SEBxDF::BxDFType flags = SEBxDF::BSDF_ALL, int sqrtSamples = 6) const;

    // Pdf evaluation of a given pair of wi and wo in world space.
    float Pdf(const SEVector3f& woWorld, const SEVector3f& wiWorld,
        SEBxDF::BxDFType flags = SEBxDF::BSDF_ALL) const;

    // BxDF stuff.
    inline void AddComponent(SEBxDF* bxdf);
    inline int GetComponentCount() const;
    inline int GetComponentCount(SEBxDF::BxDFType flags) const;

    // Shading space transformation stuff.
    SEVector3f WorldToLocal(const SEVector3f& v) const;
    SEVector3f LocalToWorld(const SEVector3f& v) const;

public:
    const SEDifferentialGeometry DGShading;
    const float Eta;

private:
    ~SEBSDF();

    SEVector3f NormalGeometry;

    // Local shading coordinate frame.
    SEVector3f S, T, N;

    // BxDF array.
    enum { MAX_BxDFS = 8 };
    int BxDFCount;
    SEBxDF* BxDFs[MAX_BxDFS];
};

#include "SEBSDF.inl"

}

#endif
