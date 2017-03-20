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

#ifndef Swing_BNDirectLightingShader_H
#define Swing_BNDirectLightingShader_H

#include "SEBossaNovaLIB.h"
#include "SEBNSurfaceShader.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20130604
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SEBNDirectLightingShader : public SEBNSurfaceShader
{
public:
    enum LightStrategy
    {
        SAMPLE_ALL_UNIFORM, 
        SAMPLE_ONE_UNIFORM
    };

    SEBNDirectLightingShader(LightStrategy strategy = SAMPLE_ALL_UNIFORM, 
        int maxDepth = 5);
    ~SEBNDirectLightingShader();

    void OnRequestSamples(SEBNSampler* sampler, SEBNSample* sample, 
        const SEBNScene* scene);

    // Surface shading interface implementation.
    SESpectrum Li(const SEBNScene* scene, const SEBNRenderer* renderer,
        const SERayDifferential& ray, const SEBNIntersection& isect,
        const SEBNSample* sample, SERandomNumberGenerator& rng,
        SEMemoryArena& arena) const;

private:
    LightStrategy Strategy;
    int MaxDepth;

    // Sample parameters for light source sampling.
    SEBNLightSampleOffsets* LightSampleOffsets;
    SEBSDFSampleOffsets* BsdfSampleOffsets;
    int LightNumOffset;
};

}

#endif