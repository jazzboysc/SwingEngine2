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

#ifndef Swing_BNKdTree_H
#define Swing_BNKdTree_H

#include "SEBossaNovaLIB.h"
#include "SEBNAggregate.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20130517
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SEBNKdTreeNode
{
public:
    SEBNKdTreeNode();
    ~SEBNKdTreeNode();

    void InitLeaf(SE_UInt32* primIDs, int primCount, SEMemoryArena& arena);
    void InitInterior(SE_UInt32 axis, SE_UInt32 aboveChild, 
        float splitPosition);

    bool IsLeaf() const;

    // Data member accessors.
    float GetSplitPosition() const;
    SE_UInt32 GetPrimitiveCount() const;
    SE_UInt32 GetSplitAxis() const;
    SE_UInt32 GetAboveChild() const;

public:
    union
    {
        float      SplitPosition; // Interior
        SE_UInt32  OnePrimitive;  // Leaf
        SE_UInt32* Primitives;    // Leaf
    };

private:
    union
    {
        SE_UInt32 Flags;          // Both
        SE_UInt32 PrimitiveCount; // Leaf
        SE_UInt32 AboveChild;     // Interior
    };
};

//----------------------------------------------------------------------------
// Description:
// Date:20130517
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SEBNBoundEdge
{
public:
    SEBNBoundEdge();
    SEBNBoundEdge(float edgePosition, int primitiveNumber, bool starting);

    bool operator < (const SEBNBoundEdge& edge) const;

public:
    float EdgePosition;
    int PrimitiveNumber;
    enum { START = 1, END = 0 } Type;
};

//----------------------------------------------------------------------------
// Description:
// Date:20130517
//----------------------------------------------------------------------------
class SE_BOSSA_NOVA_API SEBNKdTree : public SEBNAggregate
{
    SE_DECLARE_RTTI;

public:
    SEBNKdTree(const std::vector<SEBNPrimitivePtr>& primitives,
        int isectCost = 80, int traversalCost = 1, float emptyBonus = 0.5f, 
        int maxPrims = 1, int maxDepth = -1);
    ~SEBNKdTree();

    // Bounding box accessor.
    SEAxisAlignedBox3f GetWorldBound() const;

    // Ray intersection stuff.
    bool CanIntersect() const;
    bool Intersect(const SERay3f& ray, SEBNIntersection* isect) const;
    bool IntersectP(const SERay3f& ray) const;

private:
    // Helper function used for building Kd-tree.
    void BuildTree(int nodeNum, const SEAxisAlignedBox3f& nodeBounds,
        const std::vector<SEAxisAlignedBox3f>& primBounds, SE_UInt32* primNums, 
        int nPrimitives, int depth, SEBNBoundEdge* edges[3], SE_UInt32* prims0, 
        SE_UInt32* prims1, int badRefines = 0);

private:
    // Hint parameters used for building Kd-tree.
    int IsectCost, TraversalCost, MaxPrims, MaxDepth;
    float EmptyBonus;

    // Kd-tree data structure.
    SEBNKdTreeNode* Nodes;
    int AllocedNodesCount, NextFreeNode;

    // Root node bounding box that encompasses all the primitives managed by 
    // the tree.
    SEAxisAlignedBox3f Bounds;

    // Internal memory allocator used when building Kd-tree.
    SEMemoryArena Arena;

    // All the primitives managed by the tree.
    std::vector<SEBNPrimitivePtr> Primitives;
};

typedef SESmartPointer<SEBNKdTree> SEBNKdTreePtr;

}

#endif