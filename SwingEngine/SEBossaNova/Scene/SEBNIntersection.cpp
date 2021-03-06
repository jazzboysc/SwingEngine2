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
#include "SEBNIntersection.h"
#include "SEBNPrimitive.h"
#include "SEBNAreaLight.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEBNIntersection::SEBNIntersection()
{
    Primitive = 0;
    ShapeID = PrimitiveID = 0;
    RayEpsilon = 0.0f;
}
//----------------------------------------------------------------------------
SEBSDF* SEBNIntersection::GetBSDF(const SERayDifferential& ray,
    SEMemoryArena& arena) const
{
    DG.ComputeDifferentials(ray);
    SEBSDF* bsdf = Primitive->GetBSDF(DG, ObjectToWorld, arena);

    return bsdf;
}
//----------------------------------------------------------------------------
SEBSSRDF* SEBNIntersection::GetBSSRDF(const SERayDifferential& ray,
    SEMemoryArena& arena) const
{
    DG.ComputeDifferentials(ray);
    SEBSSRDF* bssrdf = Primitive->GetBSSRDF(DG, ObjectToWorld, arena);

    return bssrdf;
}
//----------------------------------------------------------------------------
SESpectrum SEBNIntersection::Le(const SEVector3f& wo) const
{
    const SEBNAreaLight* areaLight = Primitive->GetAreaLight();
    if( areaLight )
    {
        return areaLight->L(DG.p, DG.nn, wo);
    }
    
    return SESpectrum(0.0f);
}
//----------------------------------------------------------------------------