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
#include "SEBNKdTree.h"
#include "SEMemory.h"
#include "SEMath.h"
#include <algorithm>

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, SEBNKdTree, SEBNAggregate);

//----------------------------------------------------------------------------
// Kd-tree traversal internal use.
//----------------------------------------------------------------------------
struct KdToDo
{
    const SEBNKdTreeNode* Node;
    float TMin, TMax;
};

//----------------------------------------------------------------------------
// SEBNKdTreeNode functions.
//----------------------------------------------------------------------------
SEBNKdTreeNode::SEBNKdTreeNode()
{
}
//----------------------------------------------------------------------------
SEBNKdTreeNode::~SEBNKdTreeNode()
{
}
//----------------------------------------------------------------------------
void SEBNKdTreeNode::InitLeaf(SE_UInt32* primIDs, int primCount, 
    SEMemoryArena& arena)
{
    Flags = 3;
    PrimitiveCount |= (primCount << 2);

    // Store primitive ids for leaf node.
    if( primCount == 0 )
    {
        OnePrimitive = 0;
    }
    else if( primCount == 1 )
    {
        OnePrimitive = primIDs[0];
    }
    else
    {
        Primitives = arena.Alloc<SE_UInt32>(primCount);
        for( int i = 0; i < primCount; ++i )
        {
            Primitives[i] = primIDs[i];
        }
    }
}
//----------------------------------------------------------------------------
void SEBNKdTreeNode::InitInterior(SE_UInt32 axis, SE_UInt32 aboveChild, 
    float splitPosition)
{
    SplitPosition = splitPosition;
    Flags = axis;
    AboveChild |= (aboveChild << 2);
}
//----------------------------------------------------------------------------
bool SEBNKdTreeNode::IsLeaf() const
{
    return (Flags & 3) == 3;
}
//----------------------------------------------------------------------------
float SEBNKdTreeNode::GetSplitPosition() const
{
    return SplitPosition;
}
//----------------------------------------------------------------------------
SE_UInt32 SEBNKdTreeNode::GetPrimitiveCount() const
{
    return PrimitiveCount >> 2;
}
//----------------------------------------------------------------------------
SE_UInt32 SEBNKdTreeNode::GetSplitAxis() const
{
    return Flags & 3;
}
//----------------------------------------------------------------------------
SE_UInt32 SEBNKdTreeNode::GetAboveChild() const
{
    return AboveChild >> 2;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// SEBNKdTree functions.
//----------------------------------------------------------------------------
SEBNBoundEdge::SEBNBoundEdge()
{
}
//----------------------------------------------------------------------------
SEBNBoundEdge::SEBNBoundEdge(float edgePosition, int primitiveNumber, 
    bool starting)
{
    EdgePosition = edgePosition;
    PrimitiveNumber = primitiveNumber;
    Type = starting ? START : END;
}
//----------------------------------------------------------------------------
bool SEBNBoundEdge::operator<(const SEBNBoundEdge& edge) const
{
    SE_ASSERT( (int)Type == (int)edge.Type );

    if( EdgePosition == edge.EdgePosition )
    {
        return PrimitiveNumber < edge.PrimitiveNumber;
    }
    else
    {
        return EdgePosition < edge.EdgePosition;
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// SEBNKdTree functions.
//----------------------------------------------------------------------------
SEBNKdTree::SEBNKdTree(const std::vector<SEBNPrimitivePtr>& primitives,
    int isectCost, int traversalCost, float emptyBonus, int maxPrims, 
    int maxDepth)
    :
    IsectCost(isectCost), 
    TraversalCost(traversalCost), 
    MaxPrims(maxPrims), 
    MaxDepth(maxDepth), 
    EmptyBonus(emptyBonus)
{
    // Refine all primitives so that the new primitives are all intersectable.
    for( SE_UInt32 i = 0; i < primitives.size(); ++i )
    {
        primitives[i]->FullyRefine(Primitives);
    }

    // Build kd-tree for accelerator.

    NextFreeNode = AllocedNodesCount = 0;
    if( MaxDepth <= 0 )
    {
        int tempLog2FloorToInt = SE_Log2FloorToInt(float(Primitives.size()));
        MaxDepth = SE_RoundToInt(8 + 1.3f * tempLog2FloorToInt);
    }

    // Compute bounds for kd-tree construction.
    std::vector<SEAxisAlignedBox3f> primBounds;
    primBounds.reserve(Primitives.size());
    for( SE_UInt32 i = 0; i < Primitives.size(); ++i )
    {
        SEAxisAlignedBox3f box = Primitives[i]->GetWorldBound();
        Bounds = Union(Bounds, box);
        primBounds.push_back(box);
    }

    // Allocate working memory for kd-tree construction.
    SEBNBoundEdge* edges[3];
    for( int i = 0; i < 3; ++i )
    {
        edges[i] = SE_NEW SEBNBoundEdge[2 * Primitives.size()];
    }
    SE_UInt32* prims0 = SE_NEW SE_UInt32[Primitives.size()];
    SE_UInt32* prims1 = SE_NEW SE_UInt32[(MaxDepth + 1) * Primitives.size()];

    // Initialize primNums for kd-tree construction.
    SE_UInt32* primNums = SE_NEW SE_UInt32[Primitives.size()];
    for( SE_UInt32 i = 0; i < Primitives.size(); ++i )
    {
        primNums[i] = i;
    }

    // Start recursive construction of kd-tree.
    BuildTree(0, Bounds, primBounds, primNums, Primitives.size(), MaxDepth, 
        edges, prims0, prims1);

    // Free working memory for kd-tree construction.
    SE_DELETE[] primNums;
    for( int i = 0; i < 3; ++i )
    {
        SE_DELETE[] edges[i];
    }
    SE_DELETE[] prims0;
    SE_DELETE[] prims1;
}
//----------------------------------------------------------------------------
SEBNKdTree::~SEBNKdTree()
{
    SE_FreeAligned(Nodes);
    Nodes = 0;
}
//----------------------------------------------------------------------------
SEAxisAlignedBox3f SEBNKdTree::GetWorldBound() const
{
    return Bounds;
}
//----------------------------------------------------------------------------
bool SEBNKdTree::CanIntersect() const
{
    return true;
}
//----------------------------------------------------------------------------
bool SEBNKdTree::Intersect(const SERay3f& ray, SEBNIntersection* isect) const
{
    // Compute initial parametric range of ray inside kd-tree extent.
    float tmin, tmax;
    if( !Bounds.FindIntersection(ray, &tmin, &tmax) )
    {
        return false;
    }

    // Prepare to traverse kd-tree for ray.
    SEVector3f invDir(1.0f/ray.Direction.X, 1.0f/ray.Direction.Y, 
        1.0f/ray.Direction.Z);
#define MAX_TODO 64
    KdToDo todo[MAX_TODO];
    int todoPos = 0;

    // Traverse kd-tree nodes in order for ray.
    bool hit = false;
    const SEBNKdTreeNode* node = &Nodes[0];
    while( node != 0 )
    {
        // Bail out if we found a hit closer than the current node.
        if( ray.MaxT < tmin )
        {
            break;
        }

        if( !node->IsLeaf() )
        {
            // Process kd-tree interior node.

            // Compute parametric distance along ray to split plane.
            int axis = node->GetSplitAxis();
            float splitPosition = node->GetSplitPosition();
            float tplane = (splitPosition - ray.Origin[axis]) * invDir[axis];

            // Get node children pointers for ray.

            const SEBNKdTreeNode* firstChild;
            const SEBNKdTreeNode* secondChild;

            int belowFirst = (ray.Origin[axis] <  splitPosition) ||
                (ray.Origin[axis] == splitPosition && ray.Direction[axis] <= 0);

            if( belowFirst )
            {
                firstChild = node + 1;
                secondChild = &Nodes[node->GetAboveChild()];
            }
            else
            {
                firstChild = &Nodes[node->GetAboveChild()];
                secondChild = node + 1;
            }

            // Advance to next child node, possibly enqueue other child.
            if( tplane > tmax || tplane <= 0 )
            {
                node = firstChild;
            }
            else if( tplane < tmin )
            {
                node = secondChild;
            }
            else
            {
                // Enqueue secondChild in todo list.
                todo[todoPos].Node = secondChild;
                todo[todoPos].TMin = tplane;
                todo[todoPos].TMax = tmax;
                ++todoPos;

                node = firstChild;
                tmax = tplane;
            }
        }
        else
        {
            // Check for intersections inside leaf node.

            SE_UInt32 nPrimitives = node->GetPrimitiveCount();
            if( nPrimitives == 1 )
            {
                const SEBNPrimitivePtr& prim = Primitives[node->OnePrimitive];

                // Check one primitive inside leaf node.
                if( prim->Intersect(ray, isect) )
                {
                    hit = true;
                }
            }
            else
            {
                SE_UInt32* prims = node->Primitives;
                for( SE_UInt32 i = 0; i < nPrimitives; ++i )
                {
                    const SEBNPrimitivePtr& prim = Primitives[prims[i]];

                    // Check one primitive inside leaf node.
                    if( prim->Intersect(ray, isect) )
                    {
                        hit = true;
                    }
                }
            }

            // Grab next node to process from todo list.
            if( todoPos > 0 )
            {
                --todoPos;
                node = todo[todoPos].Node;
                tmin = todo[todoPos].TMin;
                tmax = todo[todoPos].TMax;
            }
            else
            {
                break;
            }
        }
    }

    return hit;
}
//----------------------------------------------------------------------------
bool SEBNKdTree::IntersectP(const SERay3f& ray) const
{
    // Compute initial parametric range of ray inside kd-tree extent.
    float tmin, tmax;
    if( !Bounds.FindIntersection(ray, &tmin, &tmax) )
    {
        return false;
    }

    // Prepare to traverse kd-tree for ray.
    SEVector3f invDir(1.0f/ray.Direction.X, 1.0f/ray.Direction.Y, 
        1.0f/ray.Direction.Z);
#define MAX_TODO 64
    KdToDo todo[MAX_TODO];
    int todoPos = 0;

    const SEBNKdTreeNode* node = &Nodes[0];
    while( node != 0 )
    {
        if( node->IsLeaf() )
        {
            // Check for shadow ray intersections inside leaf node.

            SE_UInt32 nPrimitives = node->GetPrimitiveCount();
            if( nPrimitives == 1 )
            {
                const SEBNPrimitivePtr& prim = Primitives[node->OnePrimitive];

                if( prim->IntersectP(ray) )
                {
                    return true;
                }
            }
            else
            {
                SE_UInt32* prims = node->Primitives;
                for( SE_UInt32 i = 0; i < nPrimitives; ++i )
                {
                    const SEBNPrimitivePtr& prim = Primitives[prims[i]];

                    if( prim->IntersectP(ray) )
                    {
                        return true;
                    }
                }
            }

            // Grab next node to process from todo list.
            if( todoPos > 0 )
            {
                --todoPos;
                node = todo[todoPos].Node;
                tmin = todo[todoPos].TMin;
                tmax = todo[todoPos].TMax;
            }
            else
            {
                break;
            }
        }
        else
        {
            // Process kd-tree interior node.

            // Compute parametric distance along ray to split plane.
            int axis = node->GetSplitAxis();
            float splitPosition = node->GetSplitPosition();
            float tplane = (splitPosition - ray.Origin[axis]) * invDir[axis];

            // Get node children pointers for ray.

            const SEBNKdTreeNode* firstChild;
            const SEBNKdTreeNode* secondChild;

            int belowFirst = (ray.Origin[axis] <  splitPosition) ||
                (ray.Origin[axis] == splitPosition && ray.Direction[axis] <= 0);

            if( belowFirst )
            {
                firstChild = node + 1;
                secondChild = &Nodes[node->GetAboveChild()];
            }
            else
            {
                firstChild = &Nodes[node->GetAboveChild()];
                secondChild = node + 1;
            }

            // Advance to next child node, possibly enqueue other child.
            if( tplane > tmax || tplane <= 0 )
            {
                node = firstChild;
            }
            else if( tplane < tmin )
            {
                node = secondChild;
            }
            else
            {
                // Enqueue secondChild in todo list.
                todo[todoPos].Node = secondChild;
                todo[todoPos].TMin = tplane;
                todo[todoPos].TMax = tmax;
                ++todoPos;

                node = firstChild;
                tmax = tplane;
            }
        }
    }

    return false;
}
//----------------------------------------------------------------------------
void SEBNKdTree::BuildTree(int nodeNum, const SEAxisAlignedBox3f& nodeBounds,
    const std::vector<SEAxisAlignedBox3f>& primBounds, SE_UInt32* primNums, 
    int nPrimitives, int depth, SEBNBoundEdge* edges[3], SE_UInt32* prims0, 
    SE_UInt32* prims1, int badRefines)
{
    SE_ASSERT( nodeNum == NextFreeNode );

    // Get next free node from Nodes array.
    if( NextFreeNode == AllocedNodesCount )
    {
        int tempValue = 2 * AllocedNodesCount;
        int newCount = SE_MAX(tempValue, 512);
        SEBNKdTreeNode* newNodes = SE_AllocateAligned<SEBNKdTreeNode>(newCount);

        if( AllocedNodesCount > 0 )
        {
            memcpy(newNodes, Nodes, AllocedNodesCount * sizeof(SEBNKdTreeNode));
            SE_FreeAligned(Nodes);
        }

        Nodes = newNodes;
        AllocedNodesCount = newCount;
    }
    ++NextFreeNode;

    // Initialize leaf node if termination criteria met.
    if( nPrimitives <= MaxPrims || depth == 0 )
    {
        Nodes[nodeNum].InitLeaf(primNums, nPrimitives, Arena);

        return;
    }

    // Initialize interior node and continue recursion.

    // Choose split axis position for interior node.
    int bestAxis = -1, bestOffset = -1;
    float bestCost = SEMathf::MAX_REAL;
    float oldCost = IsectCost * float(nPrimitives);
    float totalSA = nodeBounds.GetSurfaceArea();
    float invTotalSA = 1.0f / totalSA;
    SEVector3f delta = nodeBounds.GetDelta();

    // Choose which axis to split along.
    int axis = nodeBounds.GetMaximumExtent();
    int retries = 0;
    int edgeCount = 2 * nPrimitives;

    // Get max primitive number.
    SE_UInt32 maxPrimNum = 0;
    for( int i = 0; i < nPrimitives; ++i )
    {
        if( primNums[i] > maxPrimNum )
        {
            maxPrimNum = primNums[i];
        }
    }

retrySplit:

    // Initialize edges for axis.
    std::vector<SEBNBoundEdge> startEdges, endEdges;
    startEdges.reserve(nPrimitives);
    endEdges.reserve(nPrimitives);
    for( int i = 0; i < nPrimitives; ++i )
    {
        int pn = primNums[i];
        const SEAxisAlignedBox3f& box = primBounds[pn];
        startEdges.push_back(SEBNBoundEdge(box.Min[axis], pn, true));
        endEdges.push_back(SEBNBoundEdge(box.Max[axis], pn, false));
    }
    std::sort(startEdges.begin(), startEdges.end());
    std::sort(endEdges.begin(), endEdges.end());

    int curSPos = 0;
    int curEPos = 0;
    int curPos = 0;
    std::vector<bool> startEdgesAdded(maxPrimNum + 1, false);
    startEdgesAdded[startEdges[curSPos].PrimitiveNumber] = true;
    edges[axis][curPos++] = startEdges[curSPos++];
    while( curSPos < nPrimitives )
    {
        bool found = startEdgesAdded[endEdges[curEPos].PrimitiveNumber];
        if( found )
        {
            if( endEdges[curEPos].EdgePosition <= 
                startEdges[curSPos].EdgePosition )
            {
                edges[axis][curPos++] = endEdges[curEPos++];
            }
            else
            {
                startEdgesAdded[startEdges[curSPos].PrimitiveNumber] = true;
                edges[axis][curPos++] = startEdges[curSPos++];
            }
        }
        else
        {
            startEdgesAdded[startEdges[curSPos].PrimitiveNumber] = true;
            edges[axis][curPos++] = startEdges[curSPos++];
        }
    }
    while( curEPos < nPrimitives )
    {
        edges[axis][curPos++] = endEdges[curEPos++];
    }

    // Compute cost of all splits for axis to find best split position.
    int nBelow = 0, nAbove = nPrimitives;
    for( int i = 0; i < edgeCount; ++i )
    {
        if( edges[axis][i].Type == SEBNBoundEdge::END )
        {
            --nAbove;
        }

        float edgePosition = edges[axis][i].EdgePosition;
        if( edgePosition > nodeBounds.Min[axis] && 
            edgePosition < nodeBounds.Max[axis] )
        {
            // Compute cost for split at ith edge.

            int otherAxis0 = (axis + 1) % 3;
            int otherAxis1 = (axis + 2) % 3;

            float belowSA = 2 * (delta[otherAxis0] * delta[otherAxis1] +
                (edgePosition - nodeBounds.Min[axis]) * 
                (delta[otherAxis0] + delta[otherAxis1]));

            float aboveSA = 2 * (delta[otherAxis0] * delta[otherAxis1] +
                (nodeBounds.Max[axis] - edgePosition) *
                (delta[otherAxis0] + delta[otherAxis1]));

            float pBelow = belowSA * invTotalSA;
            float pAbove = aboveSA * invTotalSA;
            float eb = (nAbove == 0 || nBelow == 0) ? EmptyBonus : 0.0f;
            float cost = TraversalCost +
                IsectCost * (1.0f - eb) * (pBelow*nBelow + pAbove*nAbove);

            // Update best split if this is lowest cost so far.
            if( cost < bestCost )
            {
                bestCost = cost;
                bestAxis = axis;
                bestOffset = i;
            }
        }

        if( edges[axis][i].Type == SEBNBoundEdge::START )
        {
            ++nBelow;
        }
    }
    SE_ASSERT( nBelow == nPrimitives && nAbove == 0 );

    // Try another axis to split.
    if( bestAxis == -1 && retries < 2 )
    {
        ++retries;
        axis = (axis + 1) % 3;
        goto retrySplit;
    }

    if( bestCost > oldCost )
    {
        ++badRefines;
    }

    // Create leaf if no good splits were found.
    if( (bestCost > 4.0f * oldCost && nPrimitives < 16 ) ||
        bestAxis == -1 || badRefines == 3 )
    {
        Nodes[nodeNum].InitLeaf(primNums, nPrimitives, Arena);

        return;
    }

    // Classify primitives with respect to best split position.
    int n0 = 0, n1 = 0;
    for( int i = 0; i < bestOffset; ++i )
    {
        if( edges[bestAxis][i].Type == SEBNBoundEdge::START )
        {
            prims0[n0++] = edges[bestAxis][i].PrimitiveNumber;
        }
    }
    for( int i = bestOffset + 1; i < edgeCount; ++i )
    {
        if( edges[bestAxis][i].Type == SEBNBoundEdge::END )
        {
            prims1[n1++] = edges[bestAxis][i].PrimitiveNumber;
        }
    }

    // Recursively initialize children nodes.

    // Get bounding boxes for two children nodes.
    SEAxisAlignedBox3f bounds0 = nodeBounds;
    SEAxisAlignedBox3f bounds1 = nodeBounds;
    float splitPosition = edges[bestAxis][bestOffset].EdgePosition;
    bounds0.Max[bestAxis] = bounds1.Min[bestAxis] = splitPosition;

    // Build left child tree.
    BuildTree(nodeNum + 1, bounds0, primBounds, prims0, n0, depth - 1, edges,
        prims0, prims1 + nPrimitives, badRefines);

    // Initialize current interior node.
    SE_UInt32 aboveChild = NextFreeNode;
    Nodes[nodeNum].InitInterior(bestAxis, aboveChild, splitPosition);

    // Build right child tree.
    BuildTree(aboveChild, bounds1, primBounds, prims1, n1, depth - 1, edges, 
        prims0, prims1 + nPrimitives, badRefines);
}
//----------------------------------------------------------------------------