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

#ifndef Swing_BNPhotonMapShader_H
#define Swing_BNPhotonMapShader_H

#include "SEBossaNovaLIB.h"
#include "SEBNSurfaceShader.h"
#include "SEKdTree.h"
#include "SETaskManager.h"
#include "SEProgressReporter.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20130615
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SEBNPhoton
{
public:
    SEBNPhoton(const SEVector3f& position, const SESpectrum& alpha, 
        const SEVector3f& wi);
    SEBNPhoton();

    SEVector3f Position;
    SESpectrum Alpha;
    SEVector3f Wi;
};

//----------------------------------------------------------------------------
// Description:
// Date:20130615
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SEBNRadiancePhoton
{
public:
    SEBNRadiancePhoton(const SEVector3f& position, const SEVector3f& normal);
    SEBNRadiancePhoton();

    SEVector3f Position;
    SEVector3f Normal;
    SESpectrum Lo;
};

//----------------------------------------------------------------------------
// Description:
// Date:20130615
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SEBNClosePhoton
{
public:
    SEBNClosePhoton(const SEBNPhoton* photon = 0, 
        float maxDistanceSquared = SEMathf::MAX_REAL);

    bool operator<(const SEBNClosePhoton& rhsPhoton) const;

    const SEBNPhoton* Photon;
    float DistanceSquared;
};

//----------------------------------------------------------------------------
// Description:
// Date:20130615
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SEBNPhotonProcess
{
public:
    SEBNPhotonProcess(SE_UInt32 lookupCount, SEBNClosePhoton* photonBuffer);

    void operator()(const SEVector3f& p, const SEBNPhoton& photon, 
        float distSquared, float& maxDistSquared);

    SEBNClosePhoton* Photons;
    SE_UInt32 LookupCount;
    SE_UInt32 FoundCount;
};

//----------------------------------------------------------------------------
// Description:
// Date:20130615
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SEBNRadiancePhotonProcess
{
public:
    SEBNRadiancePhotonProcess(const SEVector3f& normal);

    void operator()(const SEVector3f& p, 
        const SEBNRadiancePhoton& radiancePhoton, float distSquared, 
        float& maxDistSquared);

    const SEVector3f& Normal;
    const SEBNRadiancePhoton* Photon;
};

class SEBNPhotonMapShader;
//----------------------------------------------------------------------------
// Description:
// Date:20130615
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SEPhotonShootingTask : public SETask
{
public:
    SEPhotonShootingTask(int taskNum, float time, SEMutex& mutex, 
        SEBNPhotonMapShader* photonMapShader,
        SEProgressReporter& progressReporter, bool& abortTasks, 
        int& directPathsCount, std::vector<SEBNPhoton>& directPhotons, 
        std::vector<SEBNPhoton>& indirectPhotons, 
        std::vector<SEBNPhoton>& causticPhotons,
        std::vector<SEBNRadiancePhoton>& radiancePhotons, 
        std::vector<SESpectrum>& radiancePhotonReflectances, 
        std::vector<SESpectrum>& radiancePhotonTransmittances,
        SE_UInt32& shotCount, SEDistribution1D* lightDistribution, 
        const SEBNScene* scene, const SEBNRenderer* renderer);

    void DoWork();

    int TaskNum;
    float Time;
    SEMutex& Mutex;
    SEBNPhotonMapShader* PhotonMapShader;
    SEProgressReporter& ProgressReporter;
    bool& AbortTasks;
    int& DirectPathsCount;

    std::vector<SEBNPhoton>& DirectPhotons;
    std::vector<SEBNPhoton>& IndirectPhotons;
    std::vector<SEBNPhoton>& CausticPhotons;
    std::vector<SEBNRadiancePhoton>& RadiancePhotons;

    std::vector<SESpectrum>& RadiancePhotonReflectances;
    std::vector<SESpectrum>& RadiancePhotonTransmittances;

    SE_UInt32& ShotCount;
    const SEDistribution1D* LightDistribution;
    const SEBNScene* Scene;
    const SEBNRenderer* Renderer;
};

//----------------------------------------------------------------------------
// Description:
// Date:20130615
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SEComputeRadianceTask : public SETask
{
public:
    SEComputeRadianceTask(SEProgressReporter& progress, SE_UInt32 taskNum, 
        SE_UInt32 taskCount, std::vector<SEBNRadiancePhoton>& radiancePhotons,
        const std::vector<SESpectrum>& radiancePhotonReflectances,
        const std::vector<SESpectrum>& radiancePhotonTransmittances,
        SE_UInt32 lookupCount, float maxDistSquared,
        int directPathsCount, SEKdTree<SEBNPhoton>* directMap,
        int indirectPathsCount, SEKdTree<SEBNPhoton>* indirectMap,
        int causticPathsCount, SEKdTree<SEBNPhoton>* causticMap);

    void DoWork();

private:
    SEProgressReporter& Progress;

    SE_UInt32 TaskNum;
    SE_UInt32 TaskCount;

    std::vector<SEBNRadiancePhoton>& RadiancePhotons;
    const std::vector<SESpectrum>& RadiancePhotonReflectances;
    const std::vector<SESpectrum>& RadiancePhotonTransmittances;

    SE_UInt32 LookupCount;
    float MaxDistSquared;
    int DirectPathsCount, IndirectPathsCount, CausticPathsCount;

    SEKdTree<SEBNPhoton>* DirectMap;
    SEKdTree<SEBNPhoton>* IndirectMap;
    SEKdTree<SEBNPhoton>* CausticMap;
};

//----------------------------------------------------------------------------
// Description:
// Date:20130615
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SEBNPhotonMapShader : public SEBNSurfaceShader
{
public:
    SEBNPhotonMapShader(int causticPhotonsWantedCount, 
        int indirectPhotonsWantedCount, int lookupCount, int maxSpecularDepth, 
        int maxPhotonDepth, float maxDist, bool finalGather, int gatherSamples, 
        float gatherAngle);
    ~SEBNPhotonMapShader();

    void OnRequestSamples(SEBNSampler* sampler, SEBNSample* sample, 
        const SEBNScene* scene);

    void OnPreprocess(const SEBNScene* scene, const SEBNCamera* camera, 
        const SEBNRenderer* renderer);

    // Surface shading interface implementation.
    SESpectrum Li(const SEBNScene* scene, const SEBNRenderer* renderer,
        const SERayDifferential& ray, const SEBNIntersection& isect, 
        const SEBNSample* sample, SERandomNumberGenerator& rng, 
        SEMemoryArena& arena) const;

private:
    friend class SEPhotonShootingTask;

    enum { MAX_PHOTON_PATH_SHOT_COUNT = 500000 };

    SE_UInt32 CausticPhotonsWantedCount;
    SE_UInt32 IndirectPhotonsWantedCount;
    SE_UInt32 LookupCount;

    float MaxDistSquared;
    int MaxSpecularDepth, MaxPhotonDepth;

    bool FinalGather;
    int GatherSamples;
    float InvGatherSamples;
    float CosGatherAngle;
    float ConstantConePdf;

    // Declare sample parameters for light source sampling.
    SEBNLightSampleOffsets* LightSampleOffsets;
    SEBSDFSampleOffsets* BsdfSampleOffsets;
    SEBSDFSampleOffsets BsdfGatherSampleOffsets, IndirGatherSampleOffsets;

    int CausticPathsCount, IndirectPathsCount;
    SEKdTree<SEBNPhoton>* CausticMap;
    SEKdTree<SEBNPhoton>* IndirectMap;
    SEKdTree<SEBNRadiancePhoton>* RadianceMap;

    SETaskManager* TaskManager;
};

}

#endif