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
#include "SEBNDirectLightingShader.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEBNDirectLightingShader::SEBNDirectLightingShader(
    SEBNDirectLightingShader::LightStrategy strategy, int maxDepth)
{
    MaxDepth = maxDepth;
    Strategy = strategy;
    LightSampleOffsets = 0;
    BsdfSampleOffsets = 0;
}
//----------------------------------------------------------------------------
SEBNDirectLightingShader::~SEBNDirectLightingShader()
{
    SE_DELETE[] LightSampleOffsets;
    SE_DELETE[] BsdfSampleOffsets;
}
//----------------------------------------------------------------------------
void SEBNDirectLightingShader::OnRequestSamples(SEBNSampler* sampler, 
    SEBNSample* sample, const SEBNScene* scene)
{
    if( Strategy == SEBNDirectLightingShader::SAMPLE_ALL_UNIFORM)
    {
        // Allocate and request samples for sampling all lights.
        SE_UInt32 nLights = scene->Lights.size();
        LightSampleOffsets = SE_NEW SEBNLightSampleOffsets[nLights];
        BsdfSampleOffsets = SE_NEW SEBSDFSampleOffsets[nLights];

        for( SE_UInt32 i = 0; i < nLights; ++i )
        {
            const SEBNLight* light = scene->Lights[i];
            int nSamples = light->SampleCount;
            if( sampler )
            {
                nSamples = sampler->RoundSize(nSamples);
            }
            LightSampleOffsets[i] = SEBNLightSampleOffsets(nSamples, sample);
            BsdfSampleOffsets[i] = SEBSDFSampleOffsets(nSamples, sample);
        }

        LightNumOffset = -1;
    }
    else
    {
        // Allocate and request samples for sampling one light.
        LightSampleOffsets = SE_NEW SEBNLightSampleOffsets[1];
        LightSampleOffsets[0] = SEBNLightSampleOffsets(1, sample);
        LightNumOffset = sample->Add1DSamples(1);
        BsdfSampleOffsets = SE_NEW SEBSDFSampleOffsets[1];
        BsdfSampleOffsets[0] = SEBSDFSampleOffsets(1, sample);
    }
}
//----------------------------------------------------------------------------
SESpectrum SEBNDirectLightingShader::Li(const SEBNScene* scene, 
    const SEBNRenderer* renderer, const SERayDifferential& ray, 
    const SEBNIntersection& isect, const SEBNSample* sample, 
    SERandomNumberGenerator& rng, SEMemoryArena& arena) const
{
    SESpectrum L(0.0f);

    // Evaluate BSDF at hit point.
    SEBSDF* bsdf = isect.GetBSDF(ray, arena);
    SEVector3f wo = -ray.Direction;
    const SEVector3f& p = bsdf->DGShading.p;
    const SEVector3f& n = bsdf->DGShading.nn;

    // Compute emitted light if ray hit an area light source.
    L += isect.Le(wo);

    // Compute direct lighting.
    if( scene->Lights.size() > 0 )
    {
        // Apply direct lighting strategy.
        switch( Strategy )
        {
            case SEBNDirectLightingShader::SAMPLE_ALL_UNIFORM:

                L += SE_UniformSampleAllLights(scene, renderer, arena, p, n, 
                    wo, isect.RayEpsilon, ray.Time, bsdf, sample, rng,
                    LightSampleOffsets, BsdfSampleOffsets);

                break;

            case SEBNDirectLightingShader::SAMPLE_ONE_UNIFORM:

                L += SE_UniformSampleOneLight(scene, renderer, arena, p, n, 
                    wo, isect.RayEpsilon, ray.Time, bsdf, sample, rng,
                    LightNumOffset, LightSampleOffsets, BsdfSampleOffsets);

                break;
        }
    }

    if( ray.Depth + 1 < MaxDepth )
    {
        // Trace rays for specular reflection and refraction.
        L += SE_SpecularReflect(ray, bsdf, rng, isect, renderer, scene, 
            sample, arena);
        L += SE_SpecularTransmit(ray, bsdf, rng, isect, renderer, scene, 
            sample, arena);
    }

    return L;
}
//----------------------------------------------------------------------------