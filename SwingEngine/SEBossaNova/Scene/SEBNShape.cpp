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
#include "SEBNShape.h"

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, SEBNShape, SEObject);

SE_UInt32 SEBNShape::NextShapeID = 1;

//----------------------------------------------------------------------------
SEBNShape::SEBNShape(const SETransformation* objectToWorld, 
    bool reverseOrientation)
    :
    ObjectToWorld(*objectToWorld), 
    ReverseOrientation(reverseOrientation),
    ShapeID(NextShapeID++)
{
}
//----------------------------------------------------------------------------
SEBNShape::SEBNShape()
    :
    ReverseOrientation(0),
    ShapeID(0)
{
    SE_ASSERT( false );
}
//----------------------------------------------------------------------------
SEBNShape::~SEBNShape()
{
}
//----------------------------------------------------------------------------
SEAxisAlignedBox3f SEBNShape::GetWorldBound() const
{
    SEAxisAlignedBox3f objBox, worldBox;
    objBox = GetObjectBound();
    ObjectToWorld.ApplyForwardToAABB(objBox, worldBox);

    return worldBox;
}
//----------------------------------------------------------------------------
bool SEBNShape::CanIntersect() const
{
    return true;
}
//----------------------------------------------------------------------------
void SEBNShape::Refine(std::vector<SEBNShapePtr>&) const
{
    // Unimplemented SEBNShape::Refine() method called.
    SE_ASSERT( false );
}
//----------------------------------------------------------------------------
bool SEBNShape::Intersect(const SERay3f&, float*, float*, 
    SEDifferentialGeometry*) const
{
    // Unimplemented SEBNShape::Intersect() method called.
    SE_ASSERT( false );
    return false;
}
//----------------------------------------------------------------------------
bool SEBNShape::IntersectP(const SERay3f&) const
{
    // Unimplemented SEBNShape::IntersectP() method called.
    SE_ASSERT( false );
    return false;
}
//----------------------------------------------------------------------------
void SEBNShape::GetShadingGeometry(const SETransformation&,
    const SEDifferentialGeometry& dg, SEDifferentialGeometry* dgShading) const
{
    *dgShading = dg;
}
//----------------------------------------------------------------------------
float SEBNShape::GetArea() const
{
    // Unimplemented SEBNShape::GetArea() method called.
    SE_ASSERT( false );
    return 0.0f;
}
//----------------------------------------------------------------------------
SEVector3f SEBNShape::Sample(float, float, SEVector3f*) const
{
    // Unimplemented SEBNShape::Sample() method called.
    SE_ASSERT( false );
    return SEVector3f::ZERO;
}
//----------------------------------------------------------------------------
float SEBNShape::GetPDF(const SEVector3f&) const
{
    return 1.0f / GetArea();
}
//----------------------------------------------------------------------------
SEVector3f SEBNShape::Sample(const SEVector3f&, float u1, float u2,
    SEVector3f* Ns) const
{
    return Sample(u1, u2, Ns);
}
//----------------------------------------------------------------------------
float SEBNShape::GetPDF(const SEVector3f& p, const SEVector3f& wi) const
{
    // Intersect sample ray with area light geometry.
    SEDifferentialGeometry dgLight;
    SERay3f ray(p, wi, 1e-3f);

    // temporary hack to ignore alpha mask.
    ray.Depth = -1;

    float tHit, rayEpsilon;
    if( !Intersect(ray, &tHit, &rayEpsilon, &dgLight) )
    {
        return 0.0f;
    }

    // Convert light sample weight to solid angle measure.
    SEVector3f pHit = ray(tHit);
    float distanceSquared = (pHit - p).GetSquaredLength();
    float absDot = SEMathf::FAbs(dgLight.nn.Dot(-wi));
    float pdf = distanceSquared / (absDot * GetArea());

    if( isinf(pdf) )
    {
        pdf = 0.0f;
    }
    return pdf;
}
//----------------------------------------------------------------------------