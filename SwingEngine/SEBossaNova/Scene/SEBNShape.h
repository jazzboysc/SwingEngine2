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

#ifndef Swing_BNShape_H
#define Swing_BNShape_H

#include "SEBossaNovaLIB.h"
#include "SEObject.h"
#include "SETransformation.h"
#include "SEAxisAlignedBox3.h"
#include "SERay3.h"
#include "SEVector3.h"
#include "SEDifferentialGeometry.h"
#include "SEAssert.h"
#include <vector>

namespace Swing
{

class SEBNShape;
typedef SESmartPointer<SEBNShape> SEBNShapePtr;

//----------------------------------------------------------------------------
// Description:
// Date:20130511
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SEBNShape : public SEObject
{
    SE_DECLARE_RTTI;

public:
    SEBNShape(const SETransformation* objectToWorld, bool reverseOrientation);
    virtual ~SEBNShape();

    // Bounding box accessor.
    virtual SEAxisAlignedBox3f GetObjectBound() const = 0;
    virtual SEAxisAlignedBox3f GetWorldBound() const;

    // Ray intersection stuff.
    virtual bool CanIntersect() const;
    virtual void Refine(std::vector<SEBNShapePtr>& refined) const;
    virtual bool Intersect(const SERay3f& ray, float* tHit,
        float* rayEpsilon, SEDifferentialGeometry* dg) const;
    virtual bool IntersectP(const SERay3f& ray) const;

    // Shading geometry access.
    virtual void GetShadingGeometry(const SETransformation& objectToWorld,
        const SEDifferentialGeometry& dg, 
        SEDifferentialGeometry* dgShading) const;

    // Get surface area.
    virtual float GetArea() const;

    // Sampling stuff.
    virtual SEVector3f Sample(float u1, float u2, SEVector3f* Ns) const;
    virtual float GetPDF(const SEVector3f& PShape) const;
    virtual SEVector3f Sample(const SEVector3f& P, float u1, float u2,
        SEVector3f* Ns) const;
    virtual float GetPDF(const SEVector3f& p, const SEVector3f& wi) const;

public:
    SETransformation ObjectToWorld;
    const bool ReverseOrientation;
    const SE_UInt32  ShapeID;
    static SE_UInt32 NextShapeID;

protected:
    // Abstract base class.
    SEBNShape();
};

}

#endif