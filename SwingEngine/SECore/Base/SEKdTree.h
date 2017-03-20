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

#ifndef Swing_KdTree_H
#define Swing_KdTree_H

#include "SEPlatforms.h"
#include "SEMemory.h"
#include "SEAxisAlignedBox3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20130614
//----------------------------------------------------------------------------
class SEKdNode
{
public:
    void Init(float fSplitPos, SE_UInt32 uiSplitAxis)
    {
        SplitPos = fSplitPos;
        SplitAxis = uiSplitAxis;
        RightChild = (1<<29) - 1;
        HasLeftChild = 0;
    }
    void InitLeaf()
    {
        SplitAxis = 3;
        RightChild = (1<<29) - 1;
        HasLeftChild = 0;
    }

public:
    float SplitPos;
    SE_UInt32 SplitAxis   : 2;
    SE_UInt32 HasLeftChild: 1;
    SE_UInt32 RightChild  :29;
};

//----------------------------------------------------------------------------
// Description:
// Date:20130614
//----------------------------------------------------------------------------
template <typename NodeData>
class SEKdTree
{
public:
    SEKdTree(const std::vector<NodeData>& rData);
    ~SEKdTree();

    template <typename LookupProc> void Lookup(const SEVector3f& rP,
            LookupProc& rProcess, float& rMaxDistSquared) const;

private:
    void RecursiveBuild(SE_UInt32 uiNodeNum, int iStart, int iEnd,
        const NodeData** apBuildNodes);

    template <typename LookupProc> void PrivateLookup(SE_UInt32 uiNodeNum,
        const SEVector3f& rP, LookupProc& rProcess, 
        float& rMaxDistSquared) const;

private:
    SEKdNode* m_Nodes;
    NodeData* m_NodeData;
    SE_UInt32 m_uiNodeCount, m_uiNextFreeNode;
};

//----------------------------------------------------------------------------
// Description:
// Date:20130614
//----------------------------------------------------------------------------
template <typename NodeData>
class SECompareKdNode
{
public:
    SECompareKdNode(int iAxis);

    bool operator()(const NodeData* pD1, const NodeData* pD2) const;

public:
    int Axis;
};

#include "SEKdTree.inl"

}

#endif