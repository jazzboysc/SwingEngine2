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
#include "SEBNPathTracingShader.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEBNPathTracingShader::SEBNPathTracingShader(int maxDepth)
{
    MaxDepth = maxDepth;
}
//----------------------------------------------------------------------------
SEBNPathTracingShader::~SEBNPathTracingShader()
{
}
//----------------------------------------------------------------------------
void SEBNPathTracingShader::OnRequestSamples(SEBNSampler*, SEBNSample* sample,
    const SEBNScene*)
{
    for( int i = 0; i < SAMPLE_DEPTH; ++i )
    {
        LightSampleOffsets[i] = SEBNLightSampleOffsets(1, sample);
        LightNumOffset[i] = sample->Add1DSamples(1);
        BsdfSampleOffsets[i] = SEBSDFSampleOffsets(1, sample);
        PathSampleOffsets[i] = SEBSDFSampleOffsets(1, sample);
    }
}
//----------------------------------------------------------------------------
SESpectrum SEBNPathTracingShader::Li(const SEBNScene* scene, 
    const SEBNRenderer* renderer, const SERayDifferential& r, 
    const SEBNIntersection& isect, const SEBNSample* sample, 
    SERandomNumberGenerator& rng, SEMemoryArena& arena) const
{
    // Declare common path integration variables.
    SESpectrum L = 0.0f;
    SESpectrum pathThroughput = 1.0f;
    SERayDifferential ray(r);
    bool specularBounce = false;
    SEBNIntersection localIsect;
    const SEBNIntersection* isectp = &isect;

    for( int bounces = 0; ; ++bounces ) 
    {
        // Possibly add emitted light at path vertex.
        if( bounces == 0 || specularBounce )
        {
            L += pathThroughput * isectp->Le(-ray.Direction);
        }

        // Sample illumination from lights to find path contribution.
        SEBSDF* bsdf = isectp->GetBSDF(ray, arena);
        const SEVector3f& p = bsdf->DGShading.p;
        const SEVector3f& n = bsdf->DGShading.nn;
        SEVector3f wo = -ray.Direction;
        if( bounces < SAMPLE_DEPTH )
        {
            L += pathThroughput * SE_UniformSampleOneLight(scene, renderer, 
                arena, p, n, wo, isectp->RayEpsilon, ray.Time, bsdf, sample, 
                rng, LightNumOffset[bounces], &LightSampleOffsets[bounces],
                &BsdfSampleOffsets[bounces]);
        }
        else
        {
            L += pathThroughput * SE_UniformSampleOneLight(scene, renderer, 
                arena, p, n, wo, isectp->RayEpsilon, ray.Time, bsdf, sample, 
                rng);
        }

        // Sample BSDF to get new path direction.

        // Get outgoingBSDFSample for sampling new path direction.
        SEBSDFSample outgoingBSDFSample;
        if( bounces < SAMPLE_DEPTH )
        {
            outgoingBSDFSample = SEBSDFSample(sample, 
                PathSampleOffsets[bounces], 0);
        }
        else
        {
            outgoingBSDFSample = SEBSDFSample(rng);
        }

        SEVector3f wi;
        float pdf;
        SEBxDF::BxDFType flags;
        SESpectrum f = bsdf->Sample_f(wo, &wi, outgoingBSDFSample, &pdf,
            SEBxDF::BSDF_ALL, &flags);
        if( f.IsBlack() || pdf == 0.0f )
        {
            break;
        }

        specularBounce = (flags & SEBxDF::BSDF_SPECULAR) != 0;
        float absCostheta = fabsf(wi.Dot(n));
        pathThroughput *= f * (absCostheta / pdf);
        ray = SERayDifferential(p, wi, ray, isectp->RayEpsilon);

        // Possibly terminate the path.
        if( bounces > 3 ) 
        {
            float pathThroughputLumi = pathThroughput.y();
            float continueProbability = SE_MIN(0.5f, pathThroughputLumi);
            if( rng.RandomFloat() > continueProbability )
            {
                break;
            }

            pathThroughput /= continueProbability;
        }
        if( bounces == MaxDepth )
        {
            break;
        }

        // Find next vertex of path.
        if( !scene->Intersect(ray, &localIsect) ) 
        {
            if( specularBounce )
            {
                for( SE_UInt32 i = 0; i < scene->Lights.size(); ++i )
                {
                   L += pathThroughput * scene->Lights[i]->Le(ray);
                }
            }

            break;
        }
        pathThroughput *= renderer->Transmittance(scene, ray, 0, rng, arena);
        isectp = &localIsect;
    }

    return L;
}
//----------------------------------------------------------------------------