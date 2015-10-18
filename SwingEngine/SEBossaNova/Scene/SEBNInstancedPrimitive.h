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

#ifndef Swing_BNInstancedPrimitive_H
#define Swing_BNInstancedPrimitive_H

#include "SEBossaNovaLIB.h"
#include "SEBNPrimitive.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20130516
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SEBNInstancedPrimitive : public SEBNPrimitive
{
    SE_DECLARE_RTTI;

public:
    SEBNInstancedPrimitive(SEBNPrimitive* archetype, 
        const SETransformation& archetypeToWorld);

    // Bounding box accessor.
    SEAxisAlignedBox3f GetWorldBound() const;

    // Ray intersection stuff.
    bool Intersect(const SERay3f& r, SEBNIntersection* in) const;
    bool IntersectP(const SERay3f& r) const;

    // Area light accessor.
    const SEBNAreaLight* GetAreaLight() const;

    // BSDF stuff.
    SEBSDF* GetBSDF(const SEDifferentialGeometry& dg,
        const SETransformation& objectToWorld, SEMemoryArena& arena) const;
    SEBSSRDF* GetBSSRDF(const SEDifferentialGeometry& dg,
        const SETransformation& objectToWorld, SEMemoryArena& arena) const;

private:
    SETransformation ArchetypeToWorld;
    SEBNPrimitivePtr Archetype;
};

typedef SESmartPointer<SEBNInstancedPrimitive> SEBNInstancedPrimitivePtr;

}

#endif