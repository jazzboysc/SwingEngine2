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
#include "SEBSDF.h"
#include "SEBSDFSample.h"
#include "SEMath.h"
#include "SEMonteCarlo.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEBSDF::SEBSDF(const SEDifferentialGeometry& dgs, 
    const SEVector3f& normalGeometry, float eta)
    :
    DGShading(dgs), 
    Eta(eta)
{
    NormalGeometry = normalGeometry;

    N = DGShading.nn;
    S = DGShading.dpdu;
    S.Normalize();
    T = N.Cross(S);
    
    BxDFCount = 0;
}

//----------------------------------------------------------------------------
SEBSDF::~SEBSDF()
{
}
//----------------------------------------------------------------------------
SESpectrum SEBSDF::f(const SEVector3f& woWorld, const SEVector3f& wiWorld, 
    SEBxDF::BxDFType flags) const
{
    SEVector3f wi = WorldToLocal(wiWorld);
    SEVector3f wo = WorldToLocal(woWorld);

    float d1 = wiWorld.Dot(NormalGeometry);
    float d2 = woWorld.Dot(NormalGeometry);
    if( d1 * d2 > 0 )
    {
        // Ignore BTDFs.
        flags = SEBxDF::BxDFType(flags & ~SEBxDF::BSDF_TRANSMISSION);
    }
    else 
    {
        // Ignore BRDFs.
        flags = SEBxDF::BxDFType(flags & ~SEBxDF::BSDF_REFLECTION);
    }

    SESpectrum f = 0.0f;
    for( int i = 0; i < BxDFCount; ++i )
    {
        if( BxDFs[i]->MatchesFlags(flags) )
        {
            f += BxDFs[i]->f(wo, wi);
        }
    }

    return f;
}
//----------------------------------------------------------------------------
SESpectrum SEBSDF::Sample_f(const SEVector3f& woWorld, SEVector3f* wiWorld, 
    const SEBSDFSample& bsdfSample, float* pdf, SEBxDF::BxDFType flags, 
    SEBxDF::BxDFType* sampledType) const
{
    int matchingComps = GetComponentCount(flags);
    if( matchingComps == 0 )
    {
        *pdf = 0.0f;
        if( sampledType )
        {
            *sampledType = SEBxDF::BxDFType(0);
        }

        return SESpectrum(0.0f);
    }

    // Choose which BxDF to sample.
    int tempValue1 = SE_FloorToInt(bsdfSample.UComponent * matchingComps);
    int tempValue2 = matchingComps - 1;
    int which = SE_MIN(tempValue1, tempValue2);
    SEBxDF* bxdf = 0;
    int count = which;
    for( int i = 0; i < BxDFCount; ++i )
    {
        if( BxDFs[i]->MatchesFlags(flags) && count-- == 0 )
        {
            bxdf = BxDFs[i];
            break;
        }
    }
    SE_ASSERT( bxdf );

    // Sample chosen BxDF.
    SEVector3f wo = WorldToLocal(woWorld);
    SEVector3f wi;
    *pdf = 0.0f;
    SESpectrum f = bxdf->Sample_f(wo, &wi, bsdfSample.UDir[0], 
        bsdfSample.UDir[1], pdf);
    if( *pdf == 0.0f )
    {
        if( sampledType )
        {
            *sampledType = SEBxDF::BxDFType(0);
        }

        return 0.0f;
    }
    if( sampledType )
    {
        *sampledType = bxdf->Type;
    }
    *wiWorld = LocalToWorld(wi);

    // Compute overall PDF with all matching BxDFs.
    if( !(bxdf->Type & SEBxDF::BSDF_SPECULAR) && matchingComps > 1 )
    {
        for( int i = 0; i < BxDFCount; ++i )
        {
            if( BxDFs[i] != bxdf && BxDFs[i]->MatchesFlags(flags) )
            {
                *pdf += BxDFs[i]->Pdf(wo, wi);
            }
        }
    }
    if( matchingComps > 1 )
    {
        *pdf /= matchingComps;
    }

    // Compute value of BSDF for sampled direction.
    if( !(bxdf->Type & SEBxDF::BSDF_SPECULAR) )
    {
        f = 0.0f;

        float d1 = wiWorld->Dot(NormalGeometry);
        float d2 = woWorld.Dot(NormalGeometry);
        if( d1 * d2 > 0 )
        {
            // Ignore BTDFs.
            flags = SEBxDF::BxDFType(flags & ~SEBxDF::BSDF_TRANSMISSION);
        }
        else
        {
            // Ignore BRDFs.
            flags = SEBxDF::BxDFType(flags & ~SEBxDF::BSDF_REFLECTION);
        }

        for( int i = 0; i < BxDFCount; ++i )
        {
            if( BxDFs[i]->MatchesFlags(flags) )
            {
                f += BxDFs[i]->f(wo, wi);
            }
        }
    }

    return f;
}
//----------------------------------------------------------------------------
SESpectrum SEBSDF::rho(SERandomNumberGenerator& rng, 
    SEBxDF::BxDFType flags, int sqrtSamples) const
{
    int nSamples = sqrtSamples * sqrtSamples;

    float* s1 = SE_ALLOCA(float, 2 * nSamples);
    SEMonteCarlo::StratifiedSample2D(s1, sqrtSamples, sqrtSamples, rng);

    float* s2 = SE_ALLOCA(float, 2 * nSamples);
    SEMonteCarlo::StratifiedSample2D(s2, sqrtSamples, sqrtSamples, rng);

    SESpectrum ret(0.0f);
    for( int i = 0; i < BxDFCount; ++i )
    {
        if( BxDFs[i]->MatchesFlags(flags) )
        {
            ret += BxDFs[i]->rho(nSamples, s1, s2);
        }
    }

    return ret;
}
//----------------------------------------------------------------------------
SESpectrum SEBSDF::rho(const SEVector3f& wo, SERandomNumberGenerator& rng, 
    SEBxDF::BxDFType flags, int sqrtSamples) const
{
    int nSamples = sqrtSamples * sqrtSamples;

    float* s1 = SE_ALLOCA(float, 2 * nSamples);
    SEMonteCarlo::StratifiedSample2D(s1, sqrtSamples, sqrtSamples, rng);

    SESpectrum ret(0.0f);
    for( int i = 0; i < BxDFCount; ++i )
    {
        if( BxDFs[i]->MatchesFlags(flags) )
        {
            ret += BxDFs[i]->rho(wo, nSamples, s1);
        }
    }

    return ret;
}
//----------------------------------------------------------------------------
float SEBSDF::Pdf(const SEVector3f& woWorld, const SEVector3f& wiWorld,
    SEBxDF::BxDFType flags) const
{
    if( BxDFCount == 0 )
    {
        return 0.0f;
    }

    SEVector3f wo = WorldToLocal(woWorld);
    SEVector3f wi = WorldToLocal(wiWorld);

    float pdf = 0.0f;
    int matchingComps = 0;
    for( int i = 0; i < BxDFCount; ++i )
    {
        if( BxDFs[i]->MatchesFlags(flags) )
        {
            ++matchingComps;
            pdf += BxDFs[i]->Pdf(wo, wi);
        }
    }
    float v = matchingComps > 0 ? pdf / matchingComps : 0.0f;

    return v;
}
//----------------------------------------------------------------------------
SEVector3f SEBSDF::WorldToLocal(const SEVector3f& v) const
{
    SEVector3f res;

    res.X = v.Dot(S);
    res.Y = v.Dot(T);
    res.Z = v.Dot(N);

    return res;
}
//----------------------------------------------------------------------------
SEVector3f SEBSDF::LocalToWorld(const SEVector3f& v) const
{
    SEVector3f res;

    res.X = S.X * v.X + T.X * v.Y + N.X * v.Z;
    res.Y = S.Y * v.X + T.Y * v.Y + N.Y * v.Z;
    res.Z = S.Z * v.X + T.Z * v.Y + N.Z * v.Z;

    return res;
}
//----------------------------------------------------------------------------