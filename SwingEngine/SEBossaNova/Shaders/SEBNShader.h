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

#ifndef Swing_BNShader_H
#define Swing_BNShader_H

#include "SEBossaNovaLIB.h"
#include "SEBNScene.h"
#include "SEBNCamera.h"
#include "SEBNRenderer.h"
#include "SEBNSampler.h"
#include "SESpectrum.h"
#include "SEMemoryArena.h"
#include "SEBNLight.h"
#include "SEBSDFSample.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20130522
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SEBNShader : public SEObject
{
    SE_DECLARE_RTTI;

public:
    SEBNShader();
    virtual ~SEBNShader();

    virtual void OnPreprocess(const SEBNScene* scene, 
        const SEBNCamera* camera, const SEBNRenderer* renderer);

    // Called by sample object to give shader object an opportunity to request 
    // additional samples.
    virtual void OnRequestSamples(SEBNSampler* sampler, SEBNSample* sample,
        const SEBNScene* scene);
};

typedef SESmartPointer<SEBNShader> SEBNShaderPtr;

//----------------------------------------------------------------------------
// Common functions used by shaders.
// Position p, normal n, outgoing direction wo are in world space. 
//----------------------------------------------------------------------------
SE_BOSSA_NOVA_API SESpectrum SE_EstimateDirectLighting(const SEBNScene* scene, 
    const SEBNRenderer* renderer, SEMemoryArena& arena, 
    const SEBNLight* light, const SEVector3f& p, const SEVector3f& n, 
    const SEVector3f& wo, float rayEpsilon, float time, const SEBSDF* bsdf,
    SERandomNumberGenerator& rng, const SEBNLightSample& lightSample, 
    const SEBSDFSample& bsdfSample, SEBxDF::BxDFType flags);
//----------------------------------------------------------------------------
SE_BOSSA_NOVA_API SESpectrum SE_UniformSampleAllLights(const SEBNScene* scene, 
    const SEBNRenderer* renderer, SEMemoryArena& arena, const SEVector3f& p, 
    const SEVector3f &n, const SEVector3f &wo, float rayEpsilon, float time, 
    SEBSDF* bsdf, const SEBNSample* sample, SERandomNumberGenerator& rng,
    const SEBNLightSampleOffsets* lightSampleOffsets,
    const SEBSDFSampleOffsets* bsdfSampleOffsets);
//----------------------------------------------------------------------------
SE_BOSSA_NOVA_API SESpectrum SE_UniformSampleOneLight(const SEBNScene* scene, 
    const SEBNRenderer* renderer, SEMemoryArena& arena, const SEVector3f& p, 
    const SEVector3f& n, const SEVector3f& wo, float rayEpsilon, float time, 
    SEBSDF* bsdf, const SEBNSample* sample, SERandomNumberGenerator& rng, 
    int lightNumOffset = -1, 
    const SEBNLightSampleOffsets* lightSampleOffset = 0,
    const SEBSDFSampleOffsets* bsdfSampleOffset = 0);
//----------------------------------------------------------------------------
SE_BOSSA_NOVA_API SESpectrum SE_SpecularReflect(const SERayDifferential& ray,
    SEBSDF* bsdf, SERandomNumberGenerator& rng, const SEBNIntersection& isect,
    const SEBNRenderer* renderer, const SEBNScene* scene,
    const SEBNSample* sample, SEMemoryArena& arena);
//----------------------------------------------------------------------------
SE_BOSSA_NOVA_API SESpectrum SE_SpecularTransmit(const SERayDifferential& ray,
    SEBSDF* bsdf, SERandomNumberGenerator& rng, const SEBNIntersection& isect,
    const SEBNRenderer* renderer, const SEBNScene* scene,
    const SEBNSample* sample, SEMemoryArena& arena);
//----------------------------------------------------------------------------
SE_BOSSA_NOVA_API SEDistribution1D* SE_ComputeLightSamplingCDF(
    const SEBNScene* scene);
//----------------------------------------------------------------------------

}

#endif