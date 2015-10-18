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
#include "SEBNInstancedPrimitive.h"

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, SEBNInstancedPrimitive, SEBNPrimitive);

//----------------------------------------------------------------------------
SEBNInstancedPrimitive::SEBNInstancedPrimitive(SEBNPrimitive* archetype, 
    const SETransformation& archetypeToWorld)
    : 
    Archetype(archetype), 
    ArchetypeToWorld(archetypeToWorld)
{
}
//----------------------------------------------------------------------------
SEAxisAlignedBox3f SEBNInstancedPrimitive::GetWorldBound() const
{
    SEAxisAlignedBox3f resBox, archetypeBox;

    archetypeBox = Archetype->GetWorldBound();
    ArchetypeToWorld.ApplyForwardToAABB(archetypeBox, resBox);

    return resBox;
}
//----------------------------------------------------------------------------
bool SEBNInstancedPrimitive::Intersect(const SERay3f& r,
    SEBNIntersection* isect) const
{
    // Transform world space ray to archetype space.
    SERay3f archetypeRay;
    ArchetypeToWorld.ApplyInverseToRay(r, archetypeRay);

    // Test intersection in archetype space.
    if( !Archetype->Intersect(archetypeRay, isect) )
    {
        return false;
    }

    // Update intersection information for world space ray.

    r.MaxT = archetypeRay.MaxT;
    isect->PrimitiveID = PrimitiveID;

    if( !ArchetypeToWorld.IsIdentity() )
    {
        // Compute object-to-world transformation for instance.
        isect->ObjectToWorld.Product(isect->ObjectToWorld, ArchetypeToWorld);


        // Transform instance's differential geometry to world space.

        SEVector3f tempRes;
        ArchetypeToWorld.ApplyForwardToPoint(isect->DG.p, tempRes);
        isect->DG.p = tempRes;

        ArchetypeToWorld.ApplyForwardToNormal(isect->DG.nn, tempRes);
        isect->DG.nn = tempRes;
        isect->DG.nn.Normalize();

        ArchetypeToWorld.ApplyForwardToVector(isect->DG.dpdu, tempRes);
        isect->DG.dpdu = tempRes;

        ArchetypeToWorld.ApplyForwardToVector(isect->DG.dpdv, tempRes);
        isect->DG.dpdv = tempRes;

        ArchetypeToWorld.ApplyForwardToNormal(isect->DG.dndu, tempRes);
        isect->DG.dndu = tempRes;

        ArchetypeToWorld.ApplyForwardToNormal(isect->DG.dndv, tempRes);
        isect->DG.dndv = tempRes;
    }

    return true;
}
//----------------------------------------------------------------------------
bool SEBNInstancedPrimitive::IntersectP(const SERay3f& r) const
{
    // Transform world space ray to archetype space.
    SERay3f archetypeRay;
    ArchetypeToWorld.ApplyInverseToRay(r, archetypeRay);

    // Test intersection in archetype space.
    return Archetype->IntersectP(archetypeRay);
}
//----------------------------------------------------------------------------
const SEBNAreaLight* SEBNInstancedPrimitive::GetAreaLight() const
{
    return 0;
}
//----------------------------------------------------------------------------
SEBSDF* SEBNInstancedPrimitive::GetBSDF(const SEDifferentialGeometry&,
    const SETransformation&, SEMemoryArena&) const
{
    return 0;
}
//----------------------------------------------------------------------------
SEBSSRDF* SEBNInstancedPrimitive::GetBSSRDF(const SEDifferentialGeometry&,
    const SETransformation&, SEMemoryArena&) const
{
    return 0;
}
//----------------------------------------------------------------------------