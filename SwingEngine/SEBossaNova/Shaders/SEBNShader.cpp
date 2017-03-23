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
#include "SEBNShader.h"
#include "SEMonteCarlo.h"

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, SEBNShader, SEObject);

//----------------------------------------------------------------------------
SEBNShader::SEBNShader()
{
}
//----------------------------------------------------------------------------
SEBNShader::~SEBNShader()
{
}
//----------------------------------------------------------------------------
void SEBNShader::OnPreprocess(const SEBNScene*, const SEBNCamera*,
    const SEBNRenderer*)
{
}
//----------------------------------------------------------------------------
void SEBNShader::OnRequestSamples(SEBNSampler*, SEBNSample*, const SEBNScene*)
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Common functions used by shaders.
//----------------------------------------------------------------------------
SESpectrum Swing::SE_EstimateDirectLighting(const SEBNScene* scene, 
    const SEBNRenderer* renderer, SEMemoryArena& arena, 
    const SEBNLight* light, const SEVector3f& p, const SEVector3f& n, 
    const SEVector3f& wo, float rayEpsilon, float time, const SEBSDF* bsdf,
    SERandomNumberGenerator& rng, const SEBNLightSample& lightSample, 
    const SEBSDFSample& bsdfSample, SEBxDF::BxDFType flags)
{
    SESpectrum Ld(0.0f);

    // Sample light source with multiple importance sampling.
    SEVector3f wi;
    float lightPdf, bsdfPdf;
    SEVisibilityTester visibility;
    SESpectrum Li = light->Sample_L(p, rayEpsilon, lightSample, time, &wi, 
        &lightPdf, &visibility);

    if( lightPdf > 0.0f && !Li.IsBlack() )
    {
        SESpectrum f = bsdf->f(wo, wi, flags);
        if( !f.IsBlack() && visibility.IsUnoccluded(scene) )
        {
            // Add light's contribution to reflected radiance.
            Li *= visibility.Transmittance(scene, renderer, 0, rng, arena);
            if( light->IsDeltaLight() )
            {
                float absCostheta = fabsf(wi.Dot(n));
                Ld += f * Li * (absCostheta / lightPdf);
            }
            else
            {
                bsdfPdf = bsdf->Pdf(wo, wi, flags);
                float weight = SEMonteCarlo::PowerHeuristic(1, lightPdf, 1, 
                    bsdfPdf);
                float absCostheta = fabsf(wi.Dot(n));
                Ld += f * Li * (absCostheta * weight / lightPdf);
            }
        }
    }

    // Sample BSDF with multiple importance sampling.
    if( !light->IsDeltaLight() )
    {
        SEBxDF::BxDFType sampledType;
        SESpectrum f = bsdf->Sample_f(wo, &wi, bsdfSample, &bsdfPdf, flags,
            &sampledType);

        if( !f.IsBlack() && bsdfPdf > 0.0f )
        {
            float weight = 1.0f;
            if( !(sampledType & SEBxDF::BSDF_SPECULAR) )
            {
                lightPdf = light->GetPDF(p, wi);
                if( lightPdf == 0.0f )
                {
                    return Ld;
                }

                weight = SEMonteCarlo::PowerHeuristic(1, bsdfPdf, 1, 
                    lightPdf);
            }

            // Add light contribution from BSDF sampling.
            SEBNIntersection lightIsect;
            SESpectrum _Li(0.0f);
            SERayDifferential ray(p, wi, rayEpsilon, SEMathf::MAX_REAL, 
                time);
            if( scene->Intersect(ray, &lightIsect) )
            {
                if( (const SEBNLight*)lightIsect.Primitive->GetAreaLight() 
                    == light )
                {
                    _Li = lightIsect.Le(-wi);
                }
            }
            else
            {
                _Li = light->Le(ray);
            }
            if( !_Li.IsBlack() )
            {
                _Li *= renderer->Transmittance(scene, ray, 0, rng, arena);
                float absCostheta = fabsf(wi.Dot(n));
                Ld += f * _Li * absCostheta * weight / bsdfPdf;
            }
        }
    }

    return Ld;
}
//----------------------------------------------------------------------------
SESpectrum Swing::SE_UniformSampleAllLights(const SEBNScene *scene, 
    const SEBNRenderer* renderer, SEMemoryArena& arena, const SEVector3f& p, 
    const SEVector3f &n, const SEVector3f &wo, float rayEpsilon, float time, 
    SEBSDF* bsdf, const SEBNSample* sample, SERandomNumberGenerator& rng,
    const SEBNLightSampleOffsets* lightSampleOffsets,
    const SEBSDFSampleOffsets* bsdfSampleOffsets)
{
    SESpectrum L(0.0f);

    for( SE_UInt32 i = 0; i < scene->Lights.size(); ++i )
    {
        SEBNLight* light = scene->Lights[i];
        int nSamples = lightSampleOffsets ? 
            lightSampleOffsets[i].SampleCount : 1;

        // Estimate direct lighting from light samples.
        SESpectrum Ld(0.0f);
        for( int j = 0; j < nSamples; ++j )
        {
            // Find light and BSDF sample values for direct lighting estimate.
            SEBNLightSample lightSample;
            SEBSDFSample bsdfSample;
            if( lightSampleOffsets != 0 && bsdfSampleOffsets != 0 )
            {
                lightSample = 
                    SEBNLightSample(sample, lightSampleOffsets[i], j);
                bsdfSample = SEBSDFSample(sample, bsdfSampleOffsets[i], j);
            }
            else
            {
                lightSample = SEBNLightSample(rng);
                bsdfSample = SEBSDFSample(rng);
            }

            SEBxDF::BxDFType typeSpecified = 
                SEBxDF::BxDFType(SEBxDF::BSDF_ALL & ~SEBxDF::BSDF_SPECULAR);

            Ld += SE_EstimateDirectLighting(scene, renderer, arena, light, 
                p, n, wo, rayEpsilon, time, bsdf, rng, lightSample, 
                bsdfSample, typeSpecified);
        }

        L += Ld / (float)nSamples;
    }

    return L;
}
//----------------------------------------------------------------------------
SESpectrum Swing::SE_UniformSampleOneLight(const SEBNScene* scene, 
    const SEBNRenderer* renderer, SEMemoryArena& arena, const SEVector3f& p, 
    const SEVector3f& n, const SEVector3f& wo, float rayEpsilon, float time, 
    SEBSDF* bsdf, const SEBNSample* sample, SERandomNumberGenerator& rng, 
    int lightNumOffset, const SEBNLightSampleOffsets* lightSampleOffset,
    const SEBSDFSampleOffsets* bsdfSampleOffset)
{
    int nLights = (int)scene->Lights.size();
    if( nLights == 0 )
    {
        return SESpectrum(0.0f);
    }

    // Randomly choose a single light to sample.
    int lightNum = 0;
    if( lightNumOffset != -1 )
    {
        lightNum = SE_FloorToInt(sample->OneD[lightNumOffset][0] * nLights);
    }
    else
    {
        lightNum = SE_FloorToInt(rng.RandomFloat() * nLights);
    }
    lightNum = SE_MIN(lightNum, nLights - 1);
    SEBNLight* light = scene->Lights[lightNum];

    // Initialize light and bsdf samples for single light sample.
    SEBNLightSample lightSample;
    SEBSDFSample bsdfSample;
    if( lightSampleOffset != 0 && bsdfSampleOffset != 0 )
    {
        lightSample = SEBNLightSample(sample, *lightSampleOffset, 0);
        bsdfSample = SEBSDFSample(sample, *bsdfSampleOffset, 0);
    }
    else
    {
        lightSample = SEBNLightSample(rng);
        bsdfSample = SEBSDFSample(rng);
    }

    SEBxDF::BxDFType typeSpecified = 
        SEBxDF::BxDFType(SEBxDF::BSDF_ALL & ~SEBxDF::BSDF_SPECULAR);

    SESpectrum Ld = SE_EstimateDirectLighting(scene, renderer, arena, light, 
        p, n, wo, rayEpsilon, time, bsdf, rng, lightSample, bsdfSample, 
        typeSpecified);

    return (float)nLights * Ld;
        ;
}
//----------------------------------------------------------------------------
SESpectrum Swing::SE_SpecularReflect(const SERayDifferential& ray,
    SEBSDF* bsdf, SERandomNumberGenerator& rng, const SEBNIntersection& isect,
    const SEBNRenderer* renderer, const SEBNScene* scene,
    const SEBNSample* sample, SEMemoryArena& arena)
{
    SEVector3f wo = -ray.Direction;
    SEVector3f wi;
    float pdf;
    const SEVector3f& p = bsdf->DGShading.p;
    const SEVector3f& n = bsdf->DGShading.nn;
    SESpectrum f = bsdf->Sample_f(wo, &wi, SEBSDFSample(rng), &pdf,
        SEBxDF::BxDFType(SEBxDF::BSDF_REFLECTION | SEBxDF::BSDF_SPECULAR));

    SESpectrum L = 0.0f;
    if( pdf > 0.0f && !f.IsBlack() && fabsf(wi.Dot(n)) != 0.0f )
    {
        // Compute ray differential rd for specular reflection.
        SERayDifferential rd(p, wi, ray, isect.RayEpsilon);
        if( ray.HasDifferentials )
        {
            rd.HasDifferentials = true;
            rd.RxOrigin = p + isect.DG.dpdx;
            rd.RyOrigin = p + isect.DG.dpdy;

            // Compute differential reflected directions.
            SEVector3f dndx = bsdf->DGShading.dndu * bsdf->DGShading.dudx +
                bsdf->DGShading.dndv * bsdf->DGShading.dvdx;
            SEVector3f dndy = bsdf->DGShading.dndu * bsdf->DGShading.dudy +
                bsdf->DGShading.dndv * bsdf->DGShading.dvdy;
            SEVector3f dwodx = -ray.RxDirection - wo;
            SEVector3f dwody = -ray.RyDirection - wo;
            float dDNdx = dwodx.Dot(n) + wo.Dot(dndx);
            float dDNdy = dwody.Dot(n) + wo.Dot(dndy);
            rd.RxDirection = 
                wi - dwodx + 2 * SEVector3f(wo.Dot(n) * dndx + dDNdx * n);
            rd.RyDirection = 
                wi - dwody + 2 * SEVector3f(wo.Dot(n) * dndy + dDNdy * n);
        }

        SESpectrum Li = renderer->Li(scene, rd, sample, rng, arena);
        float absCosi = fabsf(wi.Dot(n));
        L = f * Li * absCosi / pdf;
    }

    return L;
}
//----------------------------------------------------------------------------
SESpectrum Swing::SE_SpecularTransmit(const SERayDifferential& ray,
    SEBSDF* bsdf, SERandomNumberGenerator& rng, const SEBNIntersection& isect,
    const SEBNRenderer* renderer, const SEBNScene* scene,
    const SEBNSample* sample, SEMemoryArena& arena)
{
    SEVector3f wo = -ray.Direction;
    SEVector3f wi;
    float pdf;
    const SEVector3f& p = bsdf->DGShading.p;
    const SEVector3f& n = bsdf->DGShading.nn;
    SESpectrum f = bsdf->Sample_f(wo, &wi, SEBSDFSample(rng), &pdf,
        SEBxDF::BxDFType(SEBxDF::BSDF_TRANSMISSION | SEBxDF::BSDF_SPECULAR));

    SESpectrum L = 0.0f;
    if( pdf > 0.0f && !f.IsBlack() && fabsf(wi.Dot(n)) != 0.0f )
    {
        // Compute ray differential rd for specular transmission.
        SERayDifferential rd(p, wi, ray, isect.RayEpsilon);
        if( ray.HasDifferentials )
        {
            rd.HasDifferentials = true;
            rd.RxOrigin = p + isect.DG.dpdx;
            rd.RyOrigin = p + isect.DG.dpdy;
        
            float eta = bsdf->Eta;
            SEVector3f w = -wo;
            if( wo.Dot(n) < 0.0f )
            {
                eta = 1.0f / eta;
            }
        
            SEVector3f dndx = bsdf->DGShading.dndu * bsdf->DGShading.dudx + 
                bsdf->DGShading.dndv * bsdf->DGShading.dvdx;
            SEVector3f dndy = bsdf->DGShading.dndu * bsdf->DGShading.dudy + 
                bsdf->DGShading.dndv * bsdf->DGShading.dvdy;
        
            SEVector3f dwodx = -ray.RxDirection - wo;
            SEVector3f dwody = -ray.RyDirection - wo;
            float dDNdx = dwodx.Dot(n) + wo.Dot(dndx);
            float dDNdy = dwody.Dot(n) + wo.Dot(dndy);
        
            float mu = eta * w.Dot(n) - wi.Dot(n);
            float dmudx = (eta - (eta*eta*w.Dot(n))/wi.Dot(n)) * dDNdx;
            float dmudy = (eta - (eta*eta*w.Dot(n))/wi.Dot(n)) * dDNdy;
        
            rd.RxDirection = wi + eta * dwodx - 
                SEVector3f(mu * dndx + dmudx * n);
            rd.RyDirection = wi + eta * dwody - 
                SEVector3f(mu * dndy + dmudy * n);
        }

        SESpectrum Li = renderer->Li(scene, rd, sample, rng, arena);
        float absCosi = fabsf(wi.Dot(n));
        L = f * Li * absCosi / pdf;
    }

    return L;
}
//----------------------------------------------------------------------------
SEDistribution1D* Swing::SE_ComputeLightSamplingCDF(const SEBNScene* scene)
{
    SE_UInt32 nLights = (SE_UInt32)scene->Lights.size();
    SE_ASSERT( nLights > 0 );

    std::vector<float> lightPower(nLights, 0.0f);
    for( SE_UInt32 i = 0; i < nLights; ++i )
    {
        lightPower[i] = scene->Lights[i]->GetPower(scene).y();
    }

    return SE_NEW SEDistribution1D(&lightPower[0], nLights);
}
//----------------------------------------------------------------------------