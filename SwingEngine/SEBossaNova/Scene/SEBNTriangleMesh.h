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

#ifndef Swing_BNTriangleMesh_H
#define Swing_BNTriangleMesh_H

#include "SEBossaNovaLIB.h"
#include "SEBNShape.h"
#include "SEBNTexture.h"

namespace Swing
{

class SEBNTriangle;
//----------------------------------------------------------------------------
// Description:
// Date:20130513
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SEBNTriangleMesh : public SEBNShape
{
    SE_DECLARE_RTTI;

public:
    SEBNTriangleMesh(const SETransformation* objectToWorld, 
        bool reverseOrientation, int triangleCount, int vertexCount, 
        const int* indexBuffer, const SEVector3f* p, const SEVector3f* n, 
        const SEVector3f* s, const float* uv, 
        SEBNTexture<float>* alphaTexture);
    ~SEBNTriangleMesh();

    // Bounding box accessor.
    SEAxisAlignedBox3f GetObjectBound() const;
    SEAxisAlignedBox3f GetWorldBound() const;

    // Ray intersection stuff.
    bool CanIntersect() const;
    void Refine(std::vector<SEBNShapePtr>& refined) const;

    friend class SEBNTriangle;
    // TODO:
    //template <typename T> friend class VertexTexture;

protected:
    // Index buffer data.
    int TriangleCount;
    int* SEIndexBuffer;

    // Vertex buffer data.
    int VertexCount;
    SEVector3f* P; // World space vertex data.
    SEVector3f* N; // Object space normal data.
    SEVector3f* S; // Object space tangent data.
    float* UV;

    SEBNFloatTexturePtr AlphaTexture;
};

typedef SESmartPointer<SEBNTriangleMesh> SEBNTriangleMeshPtr;

}

#endif