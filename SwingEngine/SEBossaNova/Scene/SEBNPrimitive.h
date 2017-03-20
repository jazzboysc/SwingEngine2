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

#ifndef Swing_BNPrimitive_H
#define Swing_BNPrimitive_H

#include "SEBossaNovaLIB.h"
#include "SEObject.h"
#include "SEAxisAlignedBox3.h"
#include "SERay3.h"
#include "SEDifferentialGeometry.h"
#include "SETransformation.h"
#include "SEMemoryArena.h"
#include "SEBSDF.h"
#include "SEBSSRDF.h"
#include "SEBNIntersection.h"
#include <vector>

namespace Swing
{

class SEBNAreaLight;
class SEBNPrimitive;
typedef SESmartPointer<SEBNPrimitive> SEBNPrimitivePtr;

//----------------------------------------------------------------------------
// Description:
// Date:20130515
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SEBNPrimitive : public SEObject
{
    SE_DECLARE_RTTI;

public:
    SEBNPrimitive();
    virtual ~SEBNPrimitive();

    // Bounding box accessor.
    virtual SEAxisAlignedBox3f GetWorldBound() const = 0;

    // Ray intersection stuff.
    virtual bool CanIntersect() const;
    virtual bool Intersect(const SERay3f& r, SEBNIntersection* in) const = 0;
    virtual bool IntersectP(const SERay3f& r) const = 0;
    virtual void Refine(std::vector<SEBNPrimitivePtr>& refined) const;
    void FullyRefine(std::vector<SEBNPrimitivePtr>& refined) const;

    // Area light accessor.
    virtual const SEBNAreaLight* GetAreaLight() const = 0;

    // BSDF stuff.
    virtual SEBSDF* GetBSDF(const SEDifferentialGeometry& dg,
        const SETransformation& objectToWorld, SEMemoryArena& arena) const = 0;
    virtual SEBSSRDF* GetBSSRDF(const SEDifferentialGeometry& dg,
        const SETransformation& objectToWorld, SEMemoryArena& arena) const = 0;

public:
    const SE_UInt32 PrimitiveID;
    static SE_UInt32 NextPrimitiveID;
};

}

#endif