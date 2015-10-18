// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015
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
#include "SESamplerRenderer.h"
#include "SESystem.h"
#include "SEMath.h"
#include "SEBitHacks.h"
#include "SEBNLight.h"

using namespace Swing;

//----------------------------------------------------------------------------
// SESamplerRenderer functions.
//----------------------------------------------------------------------------
SESamplerRenderer::SESamplerRenderer(SEBNSampler* sampler, SEBNCamera* camera, 
    SEBNSurfaceShader* surfaceShader, SEBNVolumeShader* volumeShader, 
    bool visualizeObjectIDs)
{
    Sampler = sampler;
    Camera = camera;
    SurfaceShader = surfaceShader;
    VolumeShader = volumeShader;
    VisualizeObjectIDs = visualizeObjectIDs;
    // BNDebug.
    TaskManager = SE_NEW SETaskManager();
    //TaskManager = SE_NEW SETaskManager(false);
}
//----------------------------------------------------------------------------
SESamplerRenderer::~SESamplerRenderer()
{
    SE_DELETE Sampler;
    SE_DELETE Camera;
    SE_DELETE SurfaceShader;
    SE_DELETE VolumeShader;
    SE_DELETE TaskManager;
}
//----------------------------------------------------------------------------
void SESamplerRenderer::Render(const SEBNScene* scene)
{
    // Allow shaders to do preprocessing for the scene.
    SurfaceShader->OnPreprocess(scene, Camera, this);
    VolumeShader->OnPreprocess(scene, Camera, this);

    // Allocate and initialize sample.
    SEBNSample* sample = SE_NEW SEBNSample(Sampler, SurfaceShader,
        VolumeShader, scene);

    // Create and launch sampler renderer tasks for rendering image.

    // Compute number of sampler renderer tasks to create for rendering.
    int nPixels = Camera->Film->XResolution * Camera->Film->YResolution;
    int nCPUCores = SESystem::GetCPUCoresCount();
    int nTasks = SE_MAX(32 * nCPUCores, nPixels / (16*16));
    // BNDebug.
    nTasks = RoundUpPowOfTwo(nTasks);
    //nTasks = 1;
    SEProgressReporter reporter(nTasks, "Rendering");

    // Create and launch tasks.
    std::vector<SETask*> renderTasks;
    for( int i = 0; i < nTasks; ++i )
    {
        SESamplerRendererTask* task = SE_NEW SESamplerRendererTask(scene, this, 
            Camera, &reporter, Sampler, sample, VisualizeObjectIDs, 
            nTasks - 1 - i, nTasks);

        renderTasks.push_back(task);
    }
    TaskManager->EnqueueTasks(renderTasks);
    TaskManager->WaitForAllTasks();

    for( SE_UInt32 i = 0; i < renderTasks.size(); ++i )
    {
        SE_DELETE renderTasks[i];
    }
    reporter.Done();

    // Clean up after rendering and store final image.
    SE_DELETE sample;
    Camera->Film->WriteImage();
}
//----------------------------------------------------------------------------
SESpectrum SESamplerRenderer::Li(const SEBNScene* scene, 
    const SERayDifferential& ray, const SEBNSample* sample, 
    SERandomNumberGenerator& rng, SEMemoryArena& arena, 
    SEBNIntersection* isect, SESpectrum* T) const
{
    SE_ASSERT( ray.Time == sample->Time );
    //SE_ASSERT( !ray.HasNaNs() );

    // Allocate local variables for isect and T if needed.
    SESpectrum localT;
    if( !T )
    {
        T = &localT;
    }
    SEBNIntersection localIsect;
    if( !isect )
    {
        isect = &localIsect;
    }

    SESpectrum Li = 0.0f;
    if( scene->Intersect(ray, isect) )
    {
        // Surface shading of current intersection point.
        Li = SurfaceShader->Li(scene, this, ray, *isect, sample, rng, arena);
    }
    else
    {
        // Handle ray that doesn't intersect any geometry.
        for( SE_UInt32 i = 0; i < scene->Lights.size(); ++i )
        {
           Li += scene->Lights[i]->Le(ray);
        }
    }
    SESpectrum Lvi = VolumeShader->Li(scene, this, ray, sample, rng, T, 
        arena);

    return *T * Li + Lvi;
}
//----------------------------------------------------------------------------
SESpectrum SESamplerRenderer::Transmittance(const SEBNScene* scene, 
    const SERayDifferential& ray, const SEBNSample* sample, 
    SERandomNumberGenerator& rng, SEMemoryArena& arena) const
{
    return VolumeShader->Transmittance(scene, this, ray, sample, rng, arena);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// SESamplerRendererTask functions.
//----------------------------------------------------------------------------
SESamplerRendererTask::SESamplerRendererTask(const SEBNScene* scene, 
    SEBNRenderer* renderer, SEBNCamera* camera, SEProgressReporter* reporter, 
    SEBNSampler* mainSampler, SEBNSample* sample, bool visualizeObjectIDs, 
    int taskNum, int taskCount)
{
    Scene = scene; 
    Renderer = renderer; 
    Camera = camera; 
    MainSampler = mainSampler;
    Reporter = reporter;
    OrigSample = sample; 
    VisualizeObjectIDs = visualizeObjectIDs; 
    TaskNum = taskNum; 
    TaskCount = taskCount;
}
//----------------------------------------------------------------------------
static SE_UInt32 Hash(char* key, SE_UInt32 len)
{
    SE_UInt32 hash = 0, i;
    for( hash = 0, i = 0; i < len; ++i )
    {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
} 
//----------------------------------------------------------------------------
void SESamplerRendererTask::DoWork()
{
    // Get sub sampler for this task.
    SEBNSampler* sampler = MainSampler->GetSubSampler(TaskNum, TaskCount);
    if( !sampler )
    {
        Reporter->Update();
        return;
    }

    // Declare local variables used for rendering loop.
    SEMemoryArena arena;
    SERandomNumberGenerator rng(TaskNum);

    // Allocate space for samples and intersections.
    int maxSamples = sampler->MaximumSampleCount();
    SEBNSample* samples = OrigSample->Duplicate(maxSamples);
    SERayDifferential* rays = SE_NEW SERayDifferential[maxSamples];
    SESpectrum* Ls = SE_NEW SESpectrum[maxSamples];
    SESpectrum* Ts = SE_NEW SESpectrum[maxSamples];
    SEBNIntersection* isects = SE_NEW SEBNIntersection[maxSamples];

    // Get samples from sampler and update image.
    int sampleCount = 0;
    while( (sampleCount = sampler->GetMoreSamples(samples, rng)) > 0 )
    {
        // Generate camera rays and compute radiance along rays.
        for( int i = 0; i < sampleCount; ++i )
        {
            // Find camera ray for samples[i].
            float rayWeight = Camera->GenerateRayDifferential(samples[i], 
                &rays[i]);

            // Adjust ray differentials based on distance between samples of 
            // the pixel.
            rays[i].ScaleDifferentials(sampler->InvSqrtSamplesPerPixel);

            // Evaluate radiance along camera ray.
            if( VisualizeObjectIDs )
            {
                if( rayWeight > 0.0f && 
                    Scene->Intersect(rays[i], &isects[i]) )
                {
                    // Random shading based on shape id.

                    SE_UInt32 ids[2] = 
                        { isects[i].ShapeID, isects[i].PrimitiveID };

                    SE_UInt32 h = Hash((char *)ids, sizeof(ids));
                    float rgb[3] = { float(h & 0xff), float((h >> 8) & 0xff),
                        float((h >> 16) & 0xff) };

                    Ls[i] = SESpectrum::FromRGB(rgb);
                    Ls[i] /= 255.0f;
                }
                else
                {
                    Ls[i] = 0.0f;
                }
            }
            else
            {
                if( rayWeight > 0.0f )
                {
                    Ls[i] = rayWeight * Renderer->Li(Scene, rays[i], &samples[i], 
                        rng, arena, &isects[i], &Ts[i]);
                }
                else
                {
                    Ls[i] = 0.0f;
                    Ts[i] = 1.0f;
                }

                // Issue warning if unexpected radiance value returned.
                if( Ls[i].HasNaNs() )
                {
                    //Error("Not-a-number radiance value returned "
                    //    "for image sample.  Setting to black.");
                    Ls[i] = SESpectrum(0.0f);
                }
                else if( Ls[i].y() < -1e-5 )
                {
                    //Error("Negative luminance value, %f, returned"
                    //    "for image sample.  Setting to black.", Ls[i].y());
                    Ls[i] = SESpectrum(0.0f);
                }
                else if( isinf(Ls[i].y()) )
                {
                    //Error("Infinite luminance value returned"
                    //    "for image sample.  Setting to black.");
                    Ls[i] = SESpectrum(0.0f);
                }
            }
        }

        // Report sample results to sampler, add contributions to image.
        if( sampler->ReportResults(samples, rays, Ls, isects, sampleCount) )
        {
            for( int i = 0; i < sampleCount; ++i )
            {
                Camera->Film->AddSample(samples[i], Ls[i]);
            }
        }

        // Free MemoryArena memory from computing image sample values.
        arena.FreeAll();
    }

    Camera->Film->UpdateDisplay(sampler->XPixelStart, sampler->YPixelStart, 
        sampler->XPixelEnd + 1, sampler->YPixelEnd + 1);

    // Clean up after the task is done with its image region.
    SE_DELETE sampler;
    SE_DELETE[] samples;
    SE_DELETE[] rays;
    SE_DELETE[] Ls;
    SE_DELETE[] Ts;
    SE_DELETE[] isects;

    Reporter->Update();
}
//----------------------------------------------------------------------------