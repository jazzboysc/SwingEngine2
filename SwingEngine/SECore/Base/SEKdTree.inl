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

//----------------------------------------------------------------------------
// SEKdTree functions.
//----------------------------------------------------------------------------
template <typename NodeData>
SEKdTree<NodeData>::SEKdTree(const std::vector<NodeData>& rData)
{
    m_uiNodeCount = rData.size();
    m_uiNextFreeNode = 1;
    m_Nodes = SE_AllocateAligned<SEKdNode>(m_uiNodeCount);
    m_NodeData = SE_AllocateAligned<NodeData>(m_uiNodeCount);

    std::vector<const NodeData*> tempBuildNodes(m_uiNodeCount, 0);
    for( SE_UInt32 i = 0; i < m_uiNodeCount; ++i )
    {
        tempBuildNodes[i] = &rData[i];
    }

    // Begin the kd-tree building process.
    RecursiveBuild(0, 0, m_uiNodeCount, &tempBuildNodes[0]);
}
//----------------------------------------------------------------------------
template <typename NodeData>
SEKdTree<NodeData>::~SEKdTree()
{
    SE_FreeAligned(m_Nodes);
    SE_FreeAligned(m_NodeData);
}
//----------------------------------------------------------------------------
template <typename NodeData>
void SEKdTree<NodeData>::RecursiveBuild(SE_UInt32 uiNodeNum, int iStart, 
    int iEnd, const NodeData** apBuildNodes)
{
    // Create leaf node of kd-tree if we've reached the bottom.
    if( iStart + 1 == iEnd )
    {
        m_Nodes[uiNodeNum].InitLeaf();
        m_NodeData[uiNodeNum] = *apBuildNodes[iStart];
        return;
    }

    // Choose split direction and partition data.

    // Compute bounds of data from iStart to iEnd.
    SEAxisAlignedBox3f bound;
    for( int i = iStart; i < iEnd; ++i )
    {
        bound = Union(bound, apBuildNodes[i]->Position);
    }
    int splitAxis = bound.GetMaximumExtent();
    int splitPos = (iStart + iEnd)/2;
    std::nth_element(&apBuildNodes[iStart], &apBuildNodes[splitPos],
        &apBuildNodes[iEnd], SECompareKdNode<NodeData>(splitAxis));

    // Allocate kd-tree node and continue recursively.
    m_Nodes[uiNodeNum].Init(apBuildNodes[splitPos]->Position[splitAxis], 
        splitAxis);
    m_NodeData[uiNodeNum] = *apBuildNodes[splitPos];
    if( iStart < splitPos )
    {
        m_Nodes[uiNodeNum].HasLeftChild = 1;
        SE_UInt32 childNum = m_uiNextFreeNode++;
        RecursiveBuild(childNum, iStart, splitPos, apBuildNodes);
    }
    if( splitPos + 1 < iEnd )
    {
        m_Nodes[uiNodeNum].RightChild = m_uiNextFreeNode++;
        RecursiveBuild(m_Nodes[uiNodeNum].RightChild, splitPos + 1, iEnd, 
            apBuildNodes);
    }
}
//----------------------------------------------------------------------------
template <typename NodeData> template <typename LookupProc>
void SEKdTree<NodeData>::Lookup(const SEVector3f& rP, LookupProc& rProc,
    float& rMaxDistSquared) const
{
    PrivateLookup(0, rP, rProc, rMaxDistSquared);
}
//----------------------------------------------------------------------------
template <typename NodeData> template <typename LookupProc>
void SEKdTree<NodeData>::PrivateLookup(SE_UInt32 uiNodeNum, 
    const SEVector3f& rP, LookupProc& rProcess, float& rMaxDistSquared) const
{
    SEKdNode* node = &m_Nodes[uiNodeNum];

    // Process kd-tree node's children.
    int axis = node->SplitAxis;
    if( axis != 3 )
    {
        float dist2 = 
            (rP[axis] - node->SplitPos) * (rP[axis] - node->SplitPos);

        if( rP[axis] <= node->SplitPos )
        {
            if( node->HasLeftChild )
            {
                PrivateLookup(uiNodeNum + 1, rP, rProcess, rMaxDistSquared);
            }

            if( dist2 < rMaxDistSquared && node->RightChild < m_uiNodeCount )
            {
                PrivateLookup(node->RightChild, rP, rProcess, 
                    rMaxDistSquared);
            }
        }
        else
        {
            if( node->RightChild < m_uiNodeCount )
            {
                PrivateLookup(node->RightChild, rP, rProcess, 
                    rMaxDistSquared);
            }

            if( dist2 < rMaxDistSquared && node->HasLeftChild )
            {
                PrivateLookup(uiNodeNum + 1, rP, rProcess, rMaxDistSquared);
            }
        }
    }

    // Hand kd-tree node to processing function.
    SEVector3f dV = m_NodeData[uiNodeNum].Position - rP;
    float dist2 = dV.GetSquaredLength();
    if( dist2 < rMaxDistSquared )
    {
        rProcess(rP, m_NodeData[uiNodeNum], dist2, rMaxDistSquared);
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// SECompareKdNode functions.
//----------------------------------------------------------------------------
template <typename NodeData>
SECompareKdNode<NodeData>::SECompareKdNode(int iAxis)
{
    Axis = iAxis;
}
//----------------------------------------------------------------------------
template <typename NodeData>
bool SECompareKdNode<NodeData>::operator()(const NodeData* pD1, 
    const NodeData* pD2) const
{
    return pD1->Position[Axis] == pD2->Position[Axis] ? (pD1 < pD2) :
        pD1->Position[Axis] < pD2->Position[Axis];
}
//----------------------------------------------------------------------------