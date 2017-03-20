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
#include "SEBNTriangleMesh.h"
#include "SEBNTriangle.h"

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, SEBNTriangleMesh, SEBNShape);

//----------------------------------------------------------------------------
SEBNTriangleMesh::SEBNTriangleMesh(const SETransformation* objectToWorld,
    bool reverseOrientation, int triangleCount, int vertexCount, 
    const int* indexBuffer, const SEVector3f* p, const SEVector3f* n, 
    const SEVector3f* s, const float* uv,
    SEBNTexture<float>* alphaTexture)
    :
    SEBNShape(objectToWorld, reverseOrientation),
    AlphaTexture(alphaTexture)
{
    SE_ASSERT( indexBuffer && p );

    TriangleCount = triangleCount;
    SEIndexBuffer = SE_NEW int[3 * TriangleCount];
    memcpy(SEIndexBuffer, indexBuffer, 3 * TriangleCount * sizeof(int));

    VertexCount = vertexCount;
    P = SE_NEW SEVector3f[VertexCount];
    for( int i = 0; i < VertexCount; ++i )
    {
        // Transform mesh vertices to world space.
        ObjectToWorld.ApplyForwardToPoint(p[i], P[i]);
    }

    // Copy other vertex data, if present.

    if( n )
    {
        N = SE_NEW SEVector3f[VertexCount];
        memcpy(N, n, VertexCount * sizeof(SEVector3f));
    }
    else
    {
        N = 0;
    }

    if( s )
    {
        S = SE_NEW SEVector3f[VertexCount];
        memcpy(S, s, VertexCount * sizeof(SEVector3f));
    }
    else
    {
        S = 0;
    }

    if( uv )
    {
        UV = SE_NEW float[2 * VertexCount];
        memcpy(UV, uv, 2 * VertexCount * sizeof(float));
    }
    else
    {
        UV = 0;
    }
}
//----------------------------------------------------------------------------
SEBNTriangleMesh::~SEBNTriangleMesh()
{
    SE_DELETE[] SEIndexBuffer;
    SE_DELETE[] P;
    SE_DELETE[] S;
    SE_DELETE[] N;
    SE_DELETE[] UV;
}
//----------------------------------------------------------------------------
SEAxisAlignedBox3f SEBNTriangleMesh::GetObjectBound() const
{
    SEVector3f tempP;
    ObjectToWorld.ApplyInverseToPoint(P[0], tempP);
    SEAxisAlignedBox3f objectBounds(tempP);

    for( int i = 1; i < VertexCount; ++i )
    {
        ObjectToWorld.ApplyInverseToPoint(P[i], tempP);
        objectBounds = Union(objectBounds, tempP);
    }

    return objectBounds;
}
//----------------------------------------------------------------------------
SEAxisAlignedBox3f SEBNTriangleMesh::GetWorldBound() const
{
    SEAxisAlignedBox3f worldBounds(P[0]);

    for( int i = 1; i < VertexCount; ++i )
    {
        worldBounds = Union(worldBounds, P[i]);
    }

    return worldBounds;
}
//----------------------------------------------------------------------------
bool SEBNTriangleMesh::CanIntersect() const
{
    return false;
}
//----------------------------------------------------------------------------
void SEBNTriangleMesh::Refine(std::vector<SEBNShapePtr>& refined) const
{
    for( int i = 0; i < TriangleCount; ++i )
    {
        SEBNTriangle* triangle = SE_NEW SEBNTriangle(&ObjectToWorld, 
            ReverseOrientation, (SEBNTriangleMesh*)this, i);
        refined.push_back(triangle);
    }
}
//----------------------------------------------------------------------------