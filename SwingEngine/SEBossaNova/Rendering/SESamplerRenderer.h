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

#ifndef Swing_SamplerRenderer_H
#define Swing_SamplerRenderer_H

#include "SEBossaNovaLIB.h"
#include "SEBNRenderer.h"
#include "SEBNCamera.h"
#include "SEBNSurfaceShader.h"
#include "SEBNVolumeShader.h"
#include "SETaskManager.h"
#include "SEProgressReporter.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20130602
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SESamplerRenderer : public SEBNRenderer
{
public:
    SESamplerRenderer(SEBNSampler* sampler, SEBNCamera* camera, 
        SEBNSurfaceShader* surfaceShader, SEBNVolumeShader* volumeShader, 
        bool visualizeObjectIDs);
    ~SESamplerRenderer();

    void Render(const SEBNScene* scene);

    SESpectrum Li(const SEBNScene* scene, const SERayDifferential& ray,
        const SEBNSample* sample, SERandomNumberGenerator& rng, 
        SEMemoryArena& arena, SEBNIntersection* isect = 0, 
        SESpectrum* T = 0) const;

    SESpectrum Transmittance(const SEBNScene* scene, 
        const SERayDifferential& ray, const SEBNSample* sample, 
        SERandomNumberGenerator& rng, SEMemoryArena& arena) const;

private:
    bool VisualizeObjectIDs;
    SEBNSampler* Sampler;
    SEBNCamera* Camera;
    SEBNSurfaceShader* SurfaceShader;
    SEBNVolumeShader* VolumeShader;
    SETaskManager* TaskManager;
};

//----------------------------------------------------------------------------
// Description:
// Date:20130602
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SESamplerRendererTask : public SETask
{
public:
    SESamplerRendererTask(const SEBNScene* scene, SEBNRenderer* renderer, 
        SEBNCamera* camera, SEProgressReporter* reporter, 
        SEBNSampler* mainSampler, SEBNSample* sample, bool visualizeObjectIDs, 
        int taskNum, int taskCount);

    // Task interface.
    void DoWork();

private:
    const SEBNScene* Scene;
    const SEBNRenderer* Renderer;
    SEBNCamera* Camera;
    SEBNSampler* MainSampler;
    SEProgressReporter* Reporter;
    SEBNSample* OrigSample;
    bool VisualizeObjectIDs;
    int TaskNum, TaskCount;
};

}

#endif