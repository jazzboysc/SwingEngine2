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
#include "SEBNPhotonMapShader.h"
#include "SEPermutedHalton.h"

using namespace Swing;

//----------------------------------------------------------------------------
// Local utility functions.
//----------------------------------------------------------------------------
inline bool Unsuccessful(SE_UInt32 needed, SE_UInt32 found, SE_UInt32 shot)
{
    return (found < needed && (found == 0 || found < shot / 1024));
}
//----------------------------------------------------------------------------
inline float Kernel(const SEBNPhoton* photon, const SEVector3f& p,
    float maxDistSquared)
{
    float distSquared = (photon->Position - p).GetSquaredLength();
    float s = (1.0f - distSquared / maxDistSquared);

    return 3.0f * SEMathf::INV_PI * s * s;
}
//----------------------------------------------------------------------------
static SESpectrum EstimateRadianceWithPhotonMap(SEKdTree<SEBNPhoton>* map, 
    int pathCount, int lookupCount, SEBNClosePhoton* lookupBuf, SEBSDF* bsdf, 
    SERandomNumberGenerator& rng, const SEBNIntersection& isect, 
    const SEVector3f& wo, float maxDistSquared)
{
    SESpectrum L(0.0f);

    SEBxDF::BxDFType nonSpecular = SEBxDF::BxDFType(SEBxDF::BSDF_REFLECTION |
        SEBxDF::BSDF_TRANSMISSION | SEBxDF::BSDF_DIFFUSE | 
        SEBxDF::BSDF_GLOSSY);

    if( map && bsdf->GetComponentCount(nonSpecular) > 0 )
    {
        // Do photon map lookup at intersection point.
        SEBNPhotonProcess proc(lookupCount, lookupBuf);
        map->Lookup(isect.DG.p, proc, maxDistSquared);

        // Estimate reflected radiance due to incident photons.
        SEBNClosePhoton* photons = proc.Photons;
        int nFound = proc.FoundCount;
        SEVector3f Nf; 
        SEVector3f::Faceforward(bsdf->DGShading.nn, wo, Nf);
        if( bsdf->GetComponentCount(SEBxDF::BxDFType(SEBxDF::BSDF_REFLECTION |
            SEBxDF::BSDF_TRANSMISSION | SEBxDF::BSDF_GLOSSY)) > 0 )
        {
            // Compute exitant radiance from photons for glossy surface.

            for( int i = 0; i < nFound; ++i )
            {
                const SEBNPhoton* p = photons[i].Photon;
                float k = Kernel(p, isect.DG.p, maxDistSquared);
                L += (k / (pathCount * maxDistSquared)) * 
                    bsdf->f(wo, p->Wi) * p->Alpha;
            }
        }
        else
        {
            // Compute exitant radiance from photons for diffuse surface.

            SESpectrum Lr(0.0f), Lt(0.0f);
            for( int i = 0; i < nFound; ++i )
            {
                float d = Nf.Dot(photons[i].Photon->Wi);
                if( d > 0.0f )
                {
                    float k = Kernel(photons[i].Photon, isect.DG.p, 
                        maxDistSquared);
                    Lr += (k / (pathCount * maxDistSquared)) * 
                        photons[i].Photon->Alpha;
                }
                else
                {
                    float k = Kernel(photons[i].Photon, isect.DG.p, 
                        maxDistSquared);
                    Lt += (k / (pathCount * maxDistSquared)) * 
                        photons[i].Photon->Alpha;
                }
            }
            L += Lr * bsdf->rho(wo, rng, SEBxDF::BSDF_ALL_REFLECTION) * 
                SEMathf::INV_PI + 
                Lt * bsdf->rho(wo, rng, SEBxDF::BSDF_ALL_TRANSMISSION) * 
                SEMathf::INV_PI;
        }
    }

    return L;
}
//----------------------------------------------------------------------------
static SESpectrum EstimateIrradianceWithPhotonMap(SEKdTree<SEBNPhoton>* map, 
    int count, int lookupCount, SEBNClosePhoton* lookupBuf, 
    float maxDistSquared, const SEVector3f& p, const SEVector3f& n)
{
    if( !map )
    {
        return 0.0f;
    }

    // Lookup nearby photons at irradiance computation point.
    SEBNPhotonProcess proc(lookupCount, lookupBuf);
    float md2 = maxDistSquared;
    map->Lookup(p, proc, md2);
    SE_ASSERT( md2 > 0.0f );

    if( proc.FoundCount == 0 )
    {
        return SESpectrum(0.0f);
    }

    // Accumulate irradiance value from nearby photons.
    SEBNClosePhoton* photons = proc.Photons;
    SESpectrum E(0.0f);
    for( SE_UInt32 i = 0; i < proc.FoundCount; ++i )
    {
        float d = n.Dot(photons[i].Photon->Wi);
        if( d > 0.0f )
        {
            E += photons[i].Photon->Alpha;
        }
    }

    return E / (count * md2 * SEMathf::PI);
}

//----------------------------------------------------------------------------
// SEBNPhoton functions.
//----------------------------------------------------------------------------
SEBNPhoton::SEBNPhoton(const SEVector3f& position, const SESpectrum& alpha, 
    const SEVector3f& wi)
    :
    Position(position), 
    Alpha(alpha),
    Wi(wi)
{
}
//----------------------------------------------------------------------------
SEBNPhoton::SEBNPhoton()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// SEBNRadiancePhoton functions.
//----------------------------------------------------------------------------
SEBNRadiancePhoton::SEBNRadiancePhoton(const SEVector3f& position, 
    const SEVector3f& normal)
    :
    Position(position),
    Normal(normal),
    Lo(0.0f)
{
}
//----------------------------------------------------------------------------
SEBNRadiancePhoton::SEBNRadiancePhoton()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// SEBNClosePhoton functions.
//----------------------------------------------------------------------------
SEBNClosePhoton::SEBNClosePhoton(const SEBNPhoton* photon, 
    float maxDistanceSquared)
    :
    Photon(photon),
    DistanceSquared(maxDistanceSquared)
{
}
//----------------------------------------------------------------------------
bool SEBNClosePhoton::operator<(const SEBNClosePhoton& rhsPhoton) const
{
    return DistanceSquared == rhsPhoton.DistanceSquared ?
        (Photon < rhsPhoton.Photon) : 
        (DistanceSquared < rhsPhoton.DistanceSquared);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// SEBNPhotonProcess functions.
//----------------------------------------------------------------------------
SEBNPhotonProcess::SEBNPhotonProcess(SE_UInt32 lookupCount, 
    SEBNClosePhoton* photonBuffer)
{
    Photons = photonBuffer;
    LookupCount = lookupCount;
    FoundCount = 0;
}
//----------------------------------------------------------------------------
inline void SEBNPhotonProcess::operator()(const SEVector3f&,
    const SEBNPhoton& photon, float distSquared, float& maxDistSquared)
{
    if( FoundCount < LookupCount )
    {
        // Add photon to unordered array of photons.
        Photons[FoundCount++] = SEBNClosePhoton(&photon, distSquared);

        if( FoundCount == LookupCount )
        {
            std::make_heap(&Photons[0], &Photons[LookupCount]);

            // Update lookup radius.
            maxDistSquared = Photons[0].DistanceSquared;
        }
    }
    else
    {
        // Remove most distant photon from heap and add new photon.
        std::pop_heap(&Photons[0], &Photons[LookupCount]);
        Photons[LookupCount - 1] = SEBNClosePhoton(&photon, distSquared);
        std::push_heap(&Photons[0], &Photons[LookupCount]);

        // Update lookup radius.
        maxDistSquared = Photons[0].DistanceSquared;
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// SEBNRadiancePhotonProcess functions.
//----------------------------------------------------------------------------
SEBNRadiancePhotonProcess::SEBNRadiancePhotonProcess(const SEVector3f& normal)
    : 
    Normal(normal)
{
    Photon = 0;
}
//----------------------------------------------------------------------------
void SEBNRadiancePhotonProcess::operator()(const SEVector3f&, 
    const SEBNRadiancePhoton& radiancePhoton, float distSquared, 
    float& maxDistSquared)
{
    float d = radiancePhoton.Normal.Dot(Normal);
    if( d > 0.0f )
    {
        Photon = &radiancePhoton;

        // Update lookup radius.
        maxDistSquared = distSquared;
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// SEPhotonShootingTask functions.
//----------------------------------------------------------------------------
SEPhotonShootingTask::SEPhotonShootingTask(int taskNum, float time, 
    SEMutex& mutex, SEBNPhotonMapShader* photonMapShader,
    SEProgressReporter& progressReporter, bool& abortTasks, 
    int& directPathsCount, std::vector<SEBNPhoton>& directPhotons, 
    std::vector<SEBNPhoton>& indirectPhotons, 
    std::vector<SEBNPhoton>& causticPhotons,
    std::vector<SEBNRadiancePhoton>& radiancePhotons, 
    std::vector<SESpectrum>& radiancePhotonReflectances, 
    std::vector<SESpectrum>& radiancePhotonTransmittances,
    SE_UInt32& shotCount, SEDistribution1D* lightDistribution, 
    const SEBNScene* scene, const SEBNRenderer* renderer)
    :
    TaskNum(taskNum),
    Time(time),
    Mutex(mutex),
    PhotonMapShader(photonMapShader),
    ProgressReporter(progressReporter),
    AbortTasks(abortTasks), 
    DirectPathsCount(directPathsCount),
    DirectPhotons(directPhotons), 
    IndirectPhotons(indirectPhotons), 
    CausticPhotons(causticPhotons),
    RadiancePhotons(radiancePhotons), 
    RadiancePhotonReflectances(radiancePhotonReflectances), 
    RadiancePhotonTransmittances(radiancePhotonTransmittances),
    ShotCount(shotCount), 
    LightDistribution(lightDistribution), 
    Scene(scene), 
    Renderer(renderer)
{
}
//----------------------------------------------------------------------------
void SEPhotonShootingTask::DoWork()
{
    // Declare local variables for the task.
    SEMemoryArena arena;
    SERandomNumberGenerator rng(31 * TaskNum);
    std::vector<SEBNPhoton> localDirectPhotons;
    std::vector<SEBNPhoton> localIndirectPhotons;
    std::vector<SEBNPhoton> localCausticPhotons;
    std::vector<SEBNRadiancePhoton> localRadiancePhotons;
    SE_UInt32 totalPaths = 0;
    bool causticDone = (PhotonMapShader->CausticPhotonsWantedCount == 0);
    bool indirectDone = (PhotonMapShader->IndirectPhotonsWantedCount == 0);
    SEPermutedHalton halton(6, rng);
    std::vector<SESpectrum> localRpReflectances;
    std::vector<SESpectrum> localRpTransmittances;

    while( true )
    {
        // Follow photon paths for a block of samples.
        const SE_UInt32 blockSize = 4096;
        for( SE_UInt32 i = 0; i < blockSize; ++i )
        {
            float u[6];
            halton.Sample(++totalPaths, u);

            // Choose light to shoot photon from.
            float lightPdf;
            int lightNum = LightDistribution->SampleDiscrete(u[0], &lightPdf);
            const SEBNLight* light = Scene->Lights[lightNum];

            // Generate photonRay from light source.
            SERayDifferential photonRay;
            float pdf;
            SEBNLightSample lightSample(u[1], u[2], u[3]);
            SEVector3f lightSurfaceNormal;
            SESpectrum Le = light->Sample_L(Scene, lightSample, u[4], u[5],
                Time, &photonRay, &lightSurfaceNormal, &pdf);
            if( pdf == 0.0f || Le.IsBlack() )
            {
                continue;
            }

            // Initialize alpha value.
            float absCostheta = 
                fabsf(lightSurfaceNormal.Dot(photonRay.Direction));
            SESpectrum alpha = Le * (absCostheta / (pdf * lightPdf));

            if( !alpha.IsBlack() )
            {
                // Follow photon path through scene and record intersections.

                bool specularPath = true;
                SEBNIntersection photonIsect;
                int nIntersections = 0;
                while( Scene->Intersect(photonRay, &photonIsect) )
                {
                    ++nIntersections;

                    // Handle photon/surface intersection.

                    alpha *= Renderer->Transmittance(Scene, photonRay, 0, rng, 
                        arena);

                    // Determine if none specular component exists.
                    SEBSDF* photonBSDF = photonIsect.GetBSDF(photonRay, arena);
                    SEBxDF::BxDFType specularType = SEBxDF::BxDFType(
                        SEBxDF::BSDF_REFLECTION | SEBxDF::BSDF_TRANSMISSION | 
                        SEBxDF::BSDF_SPECULAR);
                    bool hasNonSpecular = (photonBSDF->GetComponentCount() >
                        photonBSDF->GetComponentCount(specularType));

                    SEVector3f wo = -photonRay.Direction;
                    if( hasNonSpecular )
                    {
                        // Deposit photon at surface that has non-specular BSDF
                        // component.

                        SEBNPhoton photon(photonIsect.DG.p, alpha, wo);
                        bool depositedPhoton = false;
                        if( specularPath && nIntersections > 1 )
                        {
                            if( !causticDone )
                            {
                                // Deposit caustic photon at surface.
                                depositedPhoton = true;
                                localCausticPhotons.push_back(photon);
                            }
                        }
                        else
                        {
                            // Deposit either direct or indirect photon.
                            // Stop depositing direct photons once indirectDone
                            // is true; don't want to waste memory storing too 
                            // many if we're going a long time trying to get 
                            // enough caustic photons desposited.
                            if( nIntersections == 1 && !indirectDone && 
                                PhotonMapShader->FinalGather )
                            {
                                // Deposit direct photon at surface.
                                depositedPhoton = true;
                                localDirectPhotons.push_back(photon);
                            }
                            else if( nIntersections > 1 && !indirectDone )
                            {
                                // Deposit indirect photon at surface.
                                depositedPhoton = true;
                                localIndirectPhotons.push_back(photon);
                            }
                        }

                        // Possibly create radiance photon at photon 
                        // intersection point.
                        if( depositedPhoton && PhotonMapShader->FinalGather &&
                            rng.RandomFloat() < 0.125f )
                        {
                            SEVector3f n = photonIsect.DG.nn;
                            SEVector3f::Faceforward(n, -photonRay.Direction, n);
                            SEBNRadiancePhoton radiancePhoton(photonIsect.DG.p, 
                                n);
                            localRadiancePhotons.push_back(radiancePhoton);

                            SESpectrum rho_r = photonBSDF->rho(rng, 
                                SEBxDF::BSDF_ALL_REFLECTION);
                            localRpReflectances.push_back(rho_r);

                            SESpectrum rho_t = photonBSDF->rho(rng, 
                                SEBxDF::BSDF_ALL_TRANSMISSION);
                            localRpTransmittances.push_back(rho_t);
                        }
                    }

                    // Stop bouncing if max photon depth has been reached.
                    if( nIntersections >= PhotonMapShader->MaxPhotonDepth )
                    {
                        break;
                    }

                    // Sample new photon ray direction.
                    SEVector3f wi;
                    float _pdf;
                    SEBxDF::BxDFType flags;
                    SESpectrum fr = photonBSDF->Sample_f(wo, &wi, 
                        SEBSDFSample(rng), &_pdf, SEBxDF::BSDF_ALL, &flags);
                    if( fr.IsBlack() || _pdf == 0.0f )
                    {
                        break;
                    }

                    float absCosi = fabsf(wi.Dot(photonBSDF->DGShading.nn));
                    SESpectrum anew = alpha * fr * (absCosi / _pdf);

                    // Possibly terminate photon path with Russian roulette.
                    float luminanceRatio = anew.y() / alpha.y();
                    float continueProb = SE_MIN(1.0f, luminanceRatio);
                    if( rng.RandomFloat() > continueProb )
                    {
                        break;
                    }

                    alpha = anew / continueProb;
                    specularPath &= ((flags & SEBxDF::BSDF_SPECULAR) != 0);
                    
                    if( indirectDone && !specularPath )
                    {
                        break;
                    }

                    photonRay = SERayDifferential(photonIsect.DG.p, wi, 
                        photonRay, photonIsect.RayEpsilon);
                }
            }

            arena.FreeAll();
        }

        // Merge local photon data with data in photon map shader.
        {
            // Begin mutex scope.

            SEMutexLock lock(Mutex);

            // Give up if we're not storing enough photons.
            if( AbortTasks )
            {
                return;
            }

            bool causticUnsuccessful = Unsuccessful(
                PhotonMapShader->CausticPhotonsWantedCount, 
                CausticPhotons.size(), blockSize);

            bool indirectUnsuccessful = Unsuccessful(
                PhotonMapShader->IndirectPhotonsWantedCount,
                IndirectPhotons.size(), blockSize);

            if( ShotCount > SEBNPhotonMapShader::MAX_PHOTON_PATH_SHOT_COUNT 
                && (causticUnsuccessful || indirectUnsuccessful) )
            {
                // Unable to store enough photons. Giving up.
                CausticPhotons.erase(CausticPhotons.begin(), 
                    CausticPhotons.end());
                IndirectPhotons.erase(IndirectPhotons.begin(), 
                    IndirectPhotons.end());
                RadiancePhotons.erase(RadiancePhotons.begin(), 
                    RadiancePhotons.end());

                // Notify all the other tasks to stop.
                AbortTasks = true;

                return;
            }

            ProgressReporter.Update(localIndirectPhotons.size() + 
                localCausticPhotons.size());
            ShotCount += blockSize;

            // Merge direct and indirect photons into shared array.
            if( !indirectDone )
            {
                PhotonMapShader->IndirectPathsCount += blockSize;
                for( SE_UInt32 i = 0; i < localIndirectPhotons.size(); ++i )
                {
                    IndirectPhotons.push_back(localIndirectPhotons[i]);
                }
                localIndirectPhotons.erase(localIndirectPhotons.begin(),
                    localIndirectPhotons.end());

                if( IndirectPhotons.size() >= 
                    PhotonMapShader->IndirectPhotonsWantedCount )
                {
                    indirectDone = true;
                }

                DirectPathsCount += blockSize;
                for( SE_UInt32 i = 0; i < localDirectPhotons.size(); ++i )
                {
                    DirectPhotons.push_back(localDirectPhotons[i]);
                }
                localDirectPhotons.erase(localDirectPhotons.begin(),
                    localDirectPhotons.end());
            }

            // Merge caustic, and radiance photons into shared array.
            if( !causticDone )
            {
                PhotonMapShader->CausticPathsCount += blockSize;
                for( SE_UInt32 i = 0; i < localCausticPhotons.size(); ++i )
                {
                    CausticPhotons.push_back(localCausticPhotons[i]);
                }
                localCausticPhotons.erase(localCausticPhotons.begin(), 
                    localCausticPhotons.end());

                if( CausticPhotons.size() >= 
                    PhotonMapShader->CausticPhotonsWantedCount )
                {
                    causticDone = true;
                }
            }
        
            for( SE_UInt32 i = 0; i < localRadiancePhotons.size(); ++i )
            {
                RadiancePhotons.push_back(localRadiancePhotons[i]);
            }
            localRadiancePhotons.erase(localRadiancePhotons.begin(), 
                localRadiancePhotons.end());

            for( SE_UInt32 i = 0; i < localRpReflectances.size(); ++i )
            {
                RadiancePhotonReflectances.push_back(localRpReflectances[i]);
            }
            localRpReflectances.erase(localRpReflectances.begin(), 
                localRpReflectances.end());

            for( SE_UInt32 i = 0; i < localRpTransmittances.size(); ++i )
            {
                RadiancePhotonTransmittances.push_back(
                    localRpTransmittances[i]);
            }
            localRpTransmittances.erase(localRpTransmittances.begin(), 
                localRpTransmittances.end());

            // End mutex scope.
        }

        // Exit task if enough photons have been found.
        if( indirectDone && causticDone )
        {
            break;
        }
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// SEComputeRadianceTask functions.
//----------------------------------------------------------------------------
SEComputeRadianceTask::SEComputeRadianceTask(SEProgressReporter& progress, 
    SE_UInt32 taskNum, SE_UInt32 taskCount, 
    std::vector<SEBNRadiancePhoton>& radiancePhotons,
    const std::vector<SESpectrum>& radiancePhotonReflectances,
    const std::vector<SESpectrum>& radiancePhotonTransmittances,
    SE_UInt32 lookupCount, float maxDistSquared,
    int directPathsCount, SEKdTree<SEBNPhoton>* directMap,
    int indirectPathsCount, SEKdTree<SEBNPhoton>* indirectMap,
    int causticPathsCount, SEKdTree<SEBNPhoton>* causticMap)
    :
    Progress(progress),
    TaskNum(taskNum),
    TaskCount(taskCount),
    RadiancePhotons(radiancePhotons),
    RadiancePhotonReflectances(radiancePhotonReflectances),
    RadiancePhotonTransmittances(radiancePhotonTransmittances),
    LookupCount(lookupCount),
    MaxDistSquared(maxDistSquared),
    DirectPathsCount(directPathsCount),
    IndirectPathsCount(indirectPathsCount),
    CausticPathsCount(causticPathsCount),
    DirectMap(directMap),
    IndirectMap(indirectMap),
    CausticMap(causticMap)
{
}
//----------------------------------------------------------------------------
void SEComputeRadianceTask::DoWork()
{
    // Compute range of radiance photons to process in task.
    SE_UInt32 taskSize = RadiancePhotons.size() / TaskCount;
    SE_UInt32 excess = RadiancePhotons.size() % TaskCount;
    SE_UInt32 rpStart = SE_MIN(TaskNum, excess) * (taskSize + 1) +
        SE_MAX(0, (int)TaskNum - (int)excess) * taskSize;
    SE_UInt32 rpEnd = rpStart + taskSize + (TaskNum < excess ? 1 : 0);
    if( TaskNum == TaskCount - 1 )
    {
        SE_ASSERT( rpEnd == RadiancePhotons.size() );
    }

    SEBNClosePhoton* lookupBuf = SE_NEW SEBNClosePhoton[LookupCount];
    for( SE_UInt32 i = rpStart; i < rpEnd; ++i )
    {
        // Compute radiance for radiance photon i.

        SEBNRadiancePhoton& rp = RadiancePhotons[i];
        const SESpectrum& rho_r = RadiancePhotonReflectances[i];
        const SESpectrum& rho_t = RadiancePhotonTransmittances[i];

        if( !rho_r.IsBlack() )
        {
            // Accumulate outgoing radiance due to reflected irradiance.
            SESpectrum E = EstimateIrradianceWithPhotonMap(DirectMap, 
                DirectPathsCount, LookupCount, lookupBuf, MaxDistSquared, 
                rp.Position, rp.Normal) +

                EstimateIrradianceWithPhotonMap(IndirectMap, 
                IndirectPathsCount, LookupCount, lookupBuf, MaxDistSquared, 
                rp.Position, rp.Normal) +

                EstimateIrradianceWithPhotonMap(CausticMap, CausticPathsCount,
                LookupCount, lookupBuf, MaxDistSquared, rp.Position, 
                rp.Normal);

            rp.Lo += SEMathf::INV_PI * rho_r * E;
        }

        if( !rho_t.IsBlack() )
        {
            // Accumulate outgoing radiance due to transmitted irradiance.
            SESpectrum E = EstimateIrradianceWithPhotonMap(DirectMap, 
                DirectPathsCount, LookupCount, lookupBuf, MaxDistSquared, 
                rp.Position, -rp.Normal) +

                EstimateIrradianceWithPhotonMap(IndirectMap, 
                IndirectPathsCount, LookupCount, lookupBuf, MaxDistSquared, 
                rp.Position, -rp.Normal) +

                EstimateIrradianceWithPhotonMap(CausticMap, CausticPathsCount, 
                LookupCount, lookupBuf, MaxDistSquared, rp.Position, 
                -rp.Normal);

            rp.Lo += SEMathf::INV_PI * rho_t * E;
        }
    }
    SE_DELETE[] lookupBuf;
    Progress.Update();
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// SEBNPhotonMapShader functions.
//----------------------------------------------------------------------------
SEBNPhotonMapShader::SEBNPhotonMapShader(int causticPhotonsWantedCount, 
    int indirectPhotonsWantedCount, int lookupCount, int maxSpecularDepth, 
    int maxPhotonDepth, float maxDist, bool finalGather, int gatherSamples, 
    float gatherAngle)
{
    CausticPhotonsWantedCount = causticPhotonsWantedCount;
    IndirectPhotonsWantedCount = indirectPhotonsWantedCount;
    LookupCount = lookupCount;
    MaxSpecularDepth = maxSpecularDepth;
    MaxPhotonDepth = maxPhotonDepth;
    MaxDistSquared = maxDist * maxDist;
    FinalGather = finalGather;
    CosGatherAngle = SEMathf::Cos(SEMathf::DEG_TO_RAD * gatherAngle);
    ConstantConePdf = SEMonteCarlo::UniformConePdf(CosGatherAngle);
    GatherSamples = gatherSamples;
    InvGatherSamples = 1.0f / (float)GatherSamples;
    CausticPathsCount = IndirectPathsCount = 0;
    CausticMap = IndirectMap = 0;
    RadianceMap = 0;
    LightSampleOffsets = 0;
    BsdfSampleOffsets = 0;
    TaskManager = SE_NEW SETaskManager();
}
//----------------------------------------------------------------------------
SEBNPhotonMapShader::~SEBNPhotonMapShader()
{
    SE_DELETE[] LightSampleOffsets;
    SE_DELETE[] BsdfSampleOffsets;
    SE_DELETE CausticMap;
    SE_DELETE IndirectMap;
    SE_DELETE RadianceMap;
    SE_DELETE TaskManager;
}
//----------------------------------------------------------------------------
void SEBNPhotonMapShader::OnRequestSamples(SEBNSampler* sampler, 
    SEBNSample* sample, const SEBNScene* scene)
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

    // Request samples for final gathering.
    if( FinalGather )
    {
        int tempValue = GatherSamples / 2;
        GatherSamples = SE_MAX(1, tempValue);
        if( sampler )
        {
            GatherSamples = sampler->RoundSize(GatherSamples);
        }
        InvGatherSamples = 1.0f / (float)GatherSamples;

        BsdfGatherSampleOffsets = SEBSDFSampleOffsets(GatherSamples, sample);
        IndirGatherSampleOffsets = SEBSDFSampleOffsets(GatherSamples, sample);
    }
}
//----------------------------------------------------------------------------
void SEBNPhotonMapShader::OnPreprocess(const SEBNScene* scene,
    const SEBNCamera* camera, const SEBNRenderer* renderer)
{
    if( scene->Lights.size() == 0 )
    {
        return;
    }

    // Declare shared variables for photon shooting.
    SEMutex* mutex = SEMutex::Create();
    int nDirectPaths = 0;
    std::vector<SEBNPhoton> causticPhotons, directPhotons, indirectPhotons;
    std::vector<SEBNRadiancePhoton> radiancePhotons;
    bool abortTasks = false;
    causticPhotons.reserve(CausticPhotonsWantedCount);
    indirectPhotons.reserve(IndirectPhotonsWantedCount);
    SE_UInt32 nshot = 0;
    std::vector<SESpectrum> rpReflectances, rpTransmittances;

    // Compute light power CDF for photon shooting.
    SEDistribution1D* lightDistribution = SE_ComputeLightSamplingCDF(scene);

    // Run parallel tasks for photon shooting.
    SEProgressReporter progress(CausticPhotonsWantedCount + 
        IndirectPhotonsWantedCount, "Shooting photons");
    std::vector<SETask*> photonShootingTasks;
    int nTasks = SESystem::GetCPUCoresCount();
    for( int i = 0; i < nTasks; ++i )
    {
        float time = camera ? camera->ShutterOpen : 0.0f;
        SEPhotonShootingTask* task = SE_NEW SEPhotonShootingTask(i, time, 
            *mutex, this, progress, abortTasks, nDirectPaths,
            directPhotons, indirectPhotons, causticPhotons, radiancePhotons,
            rpReflectances, rpTransmittances, nshot, lightDistribution, 
            scene, renderer);

        photonShootingTasks.push_back(task);
    }
    TaskManager->EnqueueTasks(photonShootingTasks);
    TaskManager->WaitForAllTasks();

    for( SE_UInt32 i = 0; i < photonShootingTasks.size(); ++i )
    {
        SE_DELETE photonShootingTasks[i];
    }
    SEMutex::Destroy(mutex);
    progress.Done();

    // Build kd-trees for indirect and caustic photons.
    SEKdTree<SEBNPhoton>* directMap = 0;
    if( directPhotons.size() > 0 )
    {
        directMap = SE_NEW SEKdTree<SEBNPhoton>(directPhotons);
    }
    if( causticPhotons.size() > 0 )
    {
        CausticMap = SE_NEW SEKdTree<SEBNPhoton>(causticPhotons);
    }
    if( indirectPhotons.size() > 0 )
    {
        IndirectMap = SE_NEW SEKdTree<SEBNPhoton>(indirectPhotons);
    }

    // Precompute radiance at a subset of the photons.
    if( FinalGather && radiancePhotons.size() )
    {
        // Launch tasks to compute photon radiances.
        std::vector<SETask*> radianceTasks;
        SE_UInt32 taskCount = 64;
        SEProgressReporter progRadiance(taskCount, 
            "Computing photon radiances");

        for( SE_UInt32 i = 0; i < taskCount; ++i )
        {
            SEComputeRadianceTask* task = SE_NEW SEComputeRadianceTask(
                progRadiance, i, taskCount, radiancePhotons, rpReflectances, 
                rpTransmittances, LookupCount, MaxDistSquared, nDirectPaths,
                directMap, IndirectPathsCount, IndirectMap, CausticPathsCount,
                CausticMap);

            radianceTasks.push_back(task);
        }
        TaskManager->EnqueueTasks(radianceTasks);
        TaskManager->WaitForAllTasks();

        for( SE_UInt32 i = 0; i < radianceTasks.size(); ++i )
        {
            SE_DELETE radianceTasks[i];
        }
        progRadiance.Done();

        RadianceMap = SE_NEW SEKdTree<SEBNRadiancePhoton>(radiancePhotons);
    }

    SE_DELETE directMap;
}
//----------------------------------------------------------------------------
SESpectrum SEBNPhotonMapShader::Li(const SEBNScene* scene, 
    const SEBNRenderer* renderer, const SERayDifferential& ray, 
    const SEBNIntersection& isect, const SEBNSample* sample, 
    SERandomNumberGenerator& rng, SEMemoryArena& arena) const
{
    SESpectrum L(0.0f);
    SEVector3f wo = -ray.Direction;

    // Compute emitted light if ray hit an area light source.
    L += isect.Le(wo);

    // Evaluate BSDF at hit point
    SEBSDF* bsdf = isect.GetBSDF(ray, arena);
    const SEVector3f& p = bsdf->DGShading.p;
    const SEVector3f& n = bsdf->DGShading.nn;

    // Compute direct lighting for photon map shader.
    L += SE_UniformSampleAllLights(scene, renderer, arena, p, n,
        wo, isect.RayEpsilon, ray.Time, bsdf, sample, rng,
        LightSampleOffsets, BsdfSampleOffsets);

    // Compute caustic lighting for photon map shader.
    SEBNClosePhoton* lookupBuf = arena.Alloc<SEBNClosePhoton>(LookupCount);
    L += EstimateRadianceWithPhotonMap(CausticMap, CausticPathsCount, 
        LookupCount, lookupBuf, bsdf, rng, isect, wo, MaxDistSquared);

    // Compute indirect lighting for photon map shader.
    if( FinalGather && IndirectMap != 0 )
    {
    #if 1
        // Do one-bounce final gather for photon map shader.

        SEBxDF::BxDFType nonSpecular = SEBxDF::BxDFType(
            SEBxDF::BSDF_REFLECTION | SEBxDF::BSDF_TRANSMISSION | 
            SEBxDF::BSDF_DIFFUSE | SEBxDF::BSDF_GLOSSY);

        if( bsdf->GetComponentCount(nonSpecular) > 0 )
        {
            // Find indirect photons around point for importance sampling.
            const SE_UInt32 nIndirSamplePhotons = 50;
            SEBNPhotonProcess proc(nIndirSamplePhotons,
                arena.Alloc<SEBNClosePhoton>(nIndirSamplePhotons));
            float searchDist2 = MaxDistSquared;
            while( proc.FoundCount < nIndirSamplePhotons )
            {
                float md2 = searchDist2;
                proc.FoundCount = 0;
                IndirectMap->Lookup(p, proc, md2);
                searchDist2 *= 2.0f;
            }

            // Copy photon directions to local array.
            SEVector3f* photonDirs = arena.Alloc<SEVector3f>(
                nIndirSamplePhotons);
            for( SE_UInt32 i = 0; i < nIndirSamplePhotons; ++i )
            {
                photonDirs[i] = proc.Photons[i].Photon->Wi;
            }

            // Use BSDF to do final gathering.
            SESpectrum Li = 0.0f;
            for( int i = 0; i < GatherSamples; ++i )
            {
                // Sample random direction from BSDF for final gather ray.
                SEVector3f wi;
                float pdf;
                SEBSDFSample bsdfSample(sample, BsdfGatherSampleOffsets, i);
                SESpectrum fr = bsdf->Sample_f(wo, &wi, bsdfSample, &pdf, 
                    SEBxDF::BxDFType(SEBxDF::BSDF_ALL & 
                    ~SEBxDF::BSDF_SPECULAR));
                if( fr.IsBlack() || pdf == 0.0f )
                {
                    continue;
                }
                SE_ASSERT( pdf >= 0.0f );

                // Trace BSDF final gather ray and accumulate radiance.
                SERayDifferential bounceRay(p, wi, ray, isect.RayEpsilon);
                SEBNIntersection gatherIsect;
                if( scene->Intersect(bounceRay, &gatherIsect) )
                {
                    // Compute exitant radiance using radiance photons.
                    SESpectrum Lindir = 0.0f;
                    SEVector3f gatherNormal = gatherIsect.DG.nn;
                    SEVector3f::Faceforward(gatherNormal, 
                        -bounceRay.Direction, gatherNormal);
                    SEBNRadiancePhotonProcess _proc(gatherNormal);
                    float md2 = SEMathf::MAX_REAL;
                    RadianceMap->Lookup(gatherIsect.DG.p, _proc, md2);
                    if( _proc.Photon != 0 )
                    {
                        Lindir = _proc.Photon->Lo;
                    }
                    Lindir *= renderer->Transmittance(scene, bounceRay, 0, 
                        rng, arena);

                    // Compute MIS weight for BSDF-sampled gather ray.

                    // Compute PDF for photon-sampling of direction wi.
                    float photonPdf = 0.0f;
                    for( SE_UInt32 j = 0; j < nIndirSamplePhotons; ++j )
                    {
                        if( photonDirs[j].Dot(wi) > 0.999f*CosGatherAngle )
                        {
                            photonPdf += ConstantConePdf;
                        }
                    }
                    photonPdf /= nIndirSamplePhotons;
                    float wt = SEMonteCarlo::PowerHeuristic(GatherSamples, 
                        pdf, GatherSamples, photonPdf);
                    float absCosi = fabsf(wi.Dot(n));
                    Li += fr * Lindir * (absCosi * wt / pdf);
                }
            }
            L += Li * InvGatherSamples;

            // Use nearby photons to do final gathering.
            Li = 0.0f;
            for( int i = 0; i < GatherSamples; ++i )
            {
                // Sample random direction using photons for final gather 
                // ray.

                SEBSDFSample gatherSample(sample, 
                    IndirGatherSampleOffsets, i);

                int photonNum = SE_MIN((int)nIndirSamplePhotons - 1,
                    SE_FloorToInt(gatherSample.UComponent * 
                    nIndirSamplePhotons));

                // Sample gather ray direction from photonNum.
                SEVector3f vx, vy;
                SEVector3f::GetOrthonormalBasis(vx, vy, 
                    photonDirs[photonNum], false);
                SEVector3f wi;
                SEMonteCarlo::UniformSampleCone(gatherSample.UDir[0], 
                    gatherSample.UDir[1], CosGatherAngle, vx, vy, 
                    photonDirs[photonNum], wi);

                SESpectrum fr = bsdf->f(wo, wi);
                if( fr.IsBlack() )
                {
                    continue;
                }

                // Trace photon-sampled final gather ray and accumulate 
                // radiance.
                SERayDifferential bounceRay(p, wi, ray, isect.RayEpsilon);
                SEBNIntersection gatherIsect;
                if( scene->Intersect(bounceRay, &gatherIsect) )
                {
                    // Compute exitant radiance using radiance photons.
                    SESpectrum Lindir = 0.0f;
                    SEVector3f gatherNormal = gatherIsect.DG.nn;
                    SEVector3f::Faceforward(gatherNormal, 
                        -bounceRay.Direction, gatherNormal);
                    SEBNRadiancePhotonProcess _proc(gatherNormal);
                    float md2 = SEMathf::MAX_REAL;
                    RadianceMap->Lookup(gatherIsect.DG.p, _proc, md2);
                    if( _proc.Photon != 0 )
                    {
                        Lindir = _proc.Photon->Lo;
                    }
                    Lindir *= renderer->Transmittance(scene, bounceRay, 0, 
                        rng, arena);

                    // Compute PDF for photon-sampling of direction wi.
                    float photonPdf = 0.0f;
                    for( SE_UInt32 j = 0; j < nIndirSamplePhotons; ++j )
                    {
                        if( photonDirs[j].Dot(wi) > 0.999f*CosGatherAngle )
                        {
                            photonPdf += ConstantConePdf;
                        }
                    }
                    photonPdf /= nIndirSamplePhotons;

                    // Compute MIS weight for photon-sampled gather ray.
                    float bsdfPdf = bsdf->Pdf(wo, wi);
                    float wt = SEMonteCarlo::PowerHeuristic(GatherSamples, 
                        photonPdf, GatherSamples, bsdfPdf);
                    float absCosi = fabsf(wi.Dot(n));
                    Li += fr * Lindir * (absCosi * wt / photonPdf);
                }
            }
            L += Li * InvGatherSamples;
        }

    #else
        // For debugging / examples: use the photon map directly.
        SEVector3f nn;
        SEVector3f::Faceforward(n, -ray.Direction, nn);
        SEBNRadiancePhotonProcess proc(nn);
        float md2 = SEMathf::MAX_REAL;
        RadianceMap->Lookup(p, proc, md2);
        if( proc.photon )
        {
            L += proc.Photon->Lo;
        }
    #endif
    }
    else if( IndirectMap != 0 )
    {
        L += EstimateRadianceWithPhotonMap(IndirectMap, IndirectPathsCount, 
            LookupCount, lookupBuf, bsdf, rng, isect, wo, MaxDistSquared);
    }

    // Trace rays for specular reflection and refraction.
    if( ray.Depth + 1 < MaxSpecularDepth)
    {
        L += SE_SpecularReflect(ray, bsdf, rng, isect, renderer, scene, 
            sample, arena);
        L += SE_SpecularTransmit(ray, bsdf, rng, isect, renderer, scene, 
            sample, arena);
    }

    return L;
}
//----------------------------------------------------------------------------