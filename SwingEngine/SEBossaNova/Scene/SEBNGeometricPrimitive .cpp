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
#include "SEBNGeometricPrimitive.h"

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, SEBNGeometricPrimitive, SEBNPrimitive);

//----------------------------------------------------------------------------
SEBNGeometricPrimitive::SEBNGeometricPrimitive(SEBNShape* shape, 
    SEBNMaterial* material, SEBNAreaLight* areaLight)
    :
    Shape(shape), 
    SEMaterial(material), 
    AreaLight(areaLight)
{
}
//----------------------------------------------------------------------------
SEBNGeometricPrimitive::~SEBNGeometricPrimitive()
{
}
//----------------------------------------------------------------------------
SEAxisAlignedBox3f SEBNGeometricPrimitive::GetWorldBound() const
{
    return Shape->GetWorldBound();
}
//----------------------------------------------------------------------------
bool SEBNGeometricPrimitive::CanIntersect() const
{
    return Shape->CanIntersect();
}
//----------------------------------------------------------------------------
bool SEBNGeometricPrimitive::Intersect(const SERay3f& r, 
    SEBNIntersection* isect) const
{
    SE_ASSERT( isect );

    float tHit, rayEpsilon;
    if( !Shape->Intersect(r, &tHit, &rayEpsilon, &isect->DG) )
    {
        return false;
    }

    // Fill the intersection object.
    isect->Primitive = this;
    isect->ObjectToWorld = Shape->ObjectToWorld;
    isect->ShapeID = Shape->ShapeID;
    isect->PrimitiveID = PrimitiveID;
    isect->RayEpsilon = rayEpsilon;
    r.MaxT = tHit;

    return true;
}
//----------------------------------------------------------------------------
bool SEBNGeometricPrimitive::IntersectP(const SERay3f& r) const
{
    return Shape->IntersectP(r);
}
//----------------------------------------------------------------------------
void SEBNGeometricPrimitive::Refine(std::vector<SEBNPrimitivePtr>& refined)
    const
{
    std::vector<SEBNShapePtr> r;
    Shape->Refine(r);

    for( int i = 0; i < (int)r.size(); ++i )
    {
        SEBNGeometricPrimitive* gp = SE_NEW SEBNGeometricPrimitive(r[i],
            SEMaterial, AreaLight);
        refined.push_back(gp);
    }
}
//----------------------------------------------------------------------------
const SEBNAreaLight* SEBNGeometricPrimitive::GetAreaLight() const
{
    return AreaLight;
}
//----------------------------------------------------------------------------
SEBSDF* SEBNGeometricPrimitive::GetBSDF(const SEDifferentialGeometry& dg,
    const SETransformation& objectToWorld, SEMemoryArena& arena) const
{
    SEDifferentialGeometry dgs;
    Shape->GetShadingGeometry(objectToWorld, dg, &dgs);

    return SEMaterial->GetBSDF(dg, dgs, arena);
}
//----------------------------------------------------------------------------
SEBSSRDF* SEBNGeometricPrimitive::GetBSSRDF(const SEDifferentialGeometry& dg,
    const SETransformation& objectToWorld, SEMemoryArena& arena) const
{
    SEDifferentialGeometry dgs;
    Shape->GetShadingGeometry(objectToWorld, dg, &dgs);

    return SEMaterial->GetBSSRDF(dg, dgs, arena);
}
//----------------------------------------------------------------------------