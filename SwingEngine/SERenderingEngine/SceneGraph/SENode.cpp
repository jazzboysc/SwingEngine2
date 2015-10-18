// Swing Engine Version 2 Source Code 
// Most of techniques in the engine are mainly based on David Eberly's
// Wild Magic 4 and 5 open-source game engine. The author of Swing Engine 
// learned a lot from Eberly's experience of architecture and algorithm.
// Several subsystems are totally new, and others are reorganized or
// reimplimented based on Wild Magic's subsystems.
// Copyright (c) 2007-2011
//
// David Eberly's permission:
// Geometric Tools, LLC
// Copyright (c) 1998-2010
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt

#include "SERenderingEnginePCH.h"
#include "SENode.h"

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, SENode, SESpatial);
SE_IMPLEMENT_STREAM(SENode);

//----------------------------------------------------------------------------
SENode::SENode()
{
}
//----------------------------------------------------------------------------
SENode::~SENode()
{
    for( int i = 0; i < (int)m_Child.size(); ++i )
    {
        if( m_Child[i] )
        {
            m_Child[i]->SetParent(0);
            m_Child[i] = 0;
        }
    }
}
//----------------------------------------------------------------------------
int SENode::AttachChild(SESpatial* pChild)
{
    // 一些开发者希望node节点对象可以有多个父节点,从而使场景视图成为DAG.
    // 实际上不是这样,场景视图结构是一个树型结构.
    // 这个断言的使用,使开发者意识到当前子节点的原有亲子关系正要被破坏,
    // 为了安全考虑,你应该首先调用DetachChild函数使子节点脱离原有亲子关系,
    // 之后再调用AttachChild或SetChild函数.
    // 但是一定要注意到,调用DetachChild时有可能使你的子节点引用计数为0从而自我
    // 析构,安全的做法是,用一个智能指针引用该子节点,然后调用DetachChild,如下
    // 所示:
    //
    //     SENode* pNode0 = SE_NEW SENode;
    //     SESpatial* pChild0 = <...>;
    //     pNode0->AttachChild(pChild0);  // child at index 0
    //     SENode* pNode1 = <...>;
    //
    //     // 这将会产生断言,因为pChild0已经有父节点(pNode0).
    //     pNode1->AttachChild(pChild0);
    //
    //     // 取而代之我们应这样做,同时防止子节点自我析构.
    //     SESpatialPtr spSaveChild = pNode0->GetChild(0);
    //     pNode0->DetachChild(spSaveChild);
    //     pNode1->AttachChild(spSaveChild);

    SE_ASSERT( pChild && !pChild->GetParent() );

    pChild->SetParent(this);

    // 是否有空slot
    int iCount = (int)m_Child.size();
    for( int i = 0; i < iCount; ++i )
    {
        if( m_Child[i] == 0 )
        {
            // 有则插入
            m_Child[i] = pChild;

            return i;
        }
    }

    // 插入在尾部
    m_Child.push_back(pChild);

    return iCount;
}
//----------------------------------------------------------------------------
int SENode::DetachChild(SESpatial* pChild)
{
    if( pChild )
    {
        // 是否存在
        for( int i = 0; i < (int)m_Child.size(); ++i )
        {
            if( m_Child[i] == pChild )
            {
                // 存在则移除
                pChild->SetParent(0);
                m_Child[i] = 0;

                return i;
            }
        }
    }

    return -1;
}
//----------------------------------------------------------------------------
SESpatialPtr SENode::DetachChildAt(int i)
{
    if( 0 <= i && i < (int)m_Child.size() )
    {
        SESpatialPtr spChild = m_Child[i];
        if( spChild )
        {
            // 存在则移除
            spChild->SetParent(0);
            m_Child[i] = 0;
        }

        return spChild;
    }

    return 0;
}
//----------------------------------------------------------------------------
SESpatialPtr SENode::SetChild(int i, SESpatial* pChild)
{
    // 一些开发者希望node节点对象可以有多个父节点,从而使场景视图成为DAG.
    // 实际上不是这样,场景视图结构是一个树型结构.
    // 这个断言的使用,使开发者意识到当前子节点的原有亲子关系正要被破坏,
    // 为了安全考虑,你应该首先调用DetachChild函数使子节点脱离原有亲子关系,
    // 之后再调用AttachChild或SetChild函数.
    // 但是一定要注意到,调用DetachChild时有可能使你的子节点引用计数为0从而自我
    // 析构,安全的做法是,用一个智能指针引用该子节点,然后调用DetachChild,如下
    // 所示:
    //
    //     SENode* pNode0 = SE_NEW SENode;
    //     SESpatial* pChild0 = <...>;
    //     pNode0->AttachChild(pChild0);  // child at index 0
    //     SENode* pNode1 = <...>;
    //
    //     // 这将会产生断言,因为pChild0已经有父节点(pNode0).
    //     pNode1->AttachChild(pChild0);
    //
    //     // 取而代之我们应这样做,同时防止子节点自我析构.
    //     SESpatialPtr spSaveChild = pNode0->GetChild(0);
    //     pNode0->DetachChild(spSaveChild);
    //     pNode1->AttachChild(spSaveChild);

    if( pChild )
    {
        SE_ASSERT( !pChild->GetParent() );
    }

    if( 0 <= i && i < (int)m_Child.size() )
    {
        // 移除slot中旧有子节点
        SESpatialPtr spPreviousChild = m_Child[i];
        if( spPreviousChild )
        {
            spPreviousChild->SetParent(0);
        }

        // 加入新的子节点到slot
        if( pChild )
        {
            pChild->SetParent(this);
        }

        m_Child[i] = pChild;

        return spPreviousChild;
    }

    // 索引超过范围,则加入尾部
    pChild->SetParent(this);
    m_Child.push_back(pChild);

    return 0;
}
//----------------------------------------------------------------------------
SESpatialPtr SENode::GetChild(int i)
{
    if( 0 <= i && i < (int)m_Child.size() )
    {
        return m_Child[i];
    }

    return 0;
}
//----------------------------------------------------------------------------
void SENode::UpdateWorldData(double dAppTime)
{
    SESpatial::UpdateWorldData(dAppTime);

    // SENode节点有责任进行AB递归,从而完成树的遍历.
    for( int i = 0; i < (int)m_Child.size(); i++ )
    {
        SESpatial* pChild = m_Child[i];
        if( pChild )
        {
            pChild->UpdateGS(dAppTime, false);
        }
    }
}
//----------------------------------------------------------------------------
void SENode::UpdateWorldBound()
{
    if( !WorldBoundIsCurrent )
    {
        // 这是一个贪心算法的BV合并,
        // 最终产生的BV包含所有子节点的BV,但并不能保证是最小BV,
        // 能够产生最小BV的算法时间复杂度较高且极其复杂,因此不予采用.
        bool bFoundFirstBound = false;
        for( int i = 0; i < (int)m_Child.size(); i++ )
        {
            SESpatial* pChild = m_Child[i];
            if( pChild )
            {
                if( bFoundFirstBound )
                {
                    // 用当前world bound与子节点world bound合并
                    WorldBound->GrowToContain(pChild->WorldBound);
                }
                else
                {
                    // 首先把world bound设置为第一个非空子节点的world bound
                    bFoundFirstBound = true;
                    WorldBound->CopyFrom(pChild->WorldBound);
                }
            }
        }
    }
}
//----------------------------------------------------------------------------
void SENode::GetUnculledSet(SECuller& rCuller, bool bNoCull)
{
    // SENode节点有责任进行AB递归,从而完成场景视图树的遍历.
    for( int i = 0; i < (int)m_Child.size(); ++i )
    {
        SESpatial* pChild = m_Child[i];
        if( pChild )
        {
            pChild->OnGetUnculledSet(rCuller, bNoCull);
        }
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// name and unique id
//----------------------------------------------------------------------------
SEObject* SENode::GetObjectByName(const std::string& rName)
{
    SEObject* pFound = SESpatial::GetObjectByName(rName);
    if( pFound )
    {
        return pFound;
    }

    for( int i = 0; i < (int)m_Child.size(); i++ )
    {
        SESpatial* pChild = m_Child[i];
        if( pChild )
        {
            pFound = pChild->GetObjectByName(rName);
            if( pFound )
            {
                return pFound;
            }
        }
    }

    return 0;
}
//----------------------------------------------------------------------------
void SENode::GetAllObjectsByName(const std::string& rName,
    std::vector<SEObject*>& rObjects)
{
    SESpatial::GetAllObjectsByName(rName, rObjects);

    for( int i = 0; i < (int)m_Child.size(); i++ )
    {
        SESpatial* pChild = m_Child[i];
        if( pChild )
        {
            pChild->GetAllObjectsByName(rName, rObjects);
        }
    }
}
//----------------------------------------------------------------------------
SEObject* SENode::GetObjectByID(unsigned int uiID)
{
    SEObject* pFound = SESpatial::GetObjectByID(uiID);
    if( pFound )
    {
        return pFound;
    }

    for( int i = 0; i < (int)m_Child.size(); i++ )
    {
        SESpatial* pChild = m_Child[i];
        if( pChild )
        {
            pFound = pChild->GetObjectByID(uiID);
            if( pFound )
            {
                return pFound;
            }
        }
    }

    return 0;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// streaming
//----------------------------------------------------------------------------
void SENode::Load(SEStream& rStream, SEStream::SELink* pLink)
{
    SE_BEGIN_DEBUG_STREAM_LOAD;

    SESpatial::Load(rStream, pLink);

    // link data
    int iCount;
    rStream.Read(iCount);
    m_Child.resize(iCount);

    for( int i = 0; i < iCount; ++i )
    {
        SEObject* pObject;
        rStream.Read(pObject);
        pLink->Add(pObject);
    }

    SE_END_DEBUG_STREAM_LOAD(SENode);
}
//----------------------------------------------------------------------------
void SENode::Link(SEStream& rStream, SEStream::SELink* pLink)
{
    SESpatial::Link(rStream, pLink);

    for( int i = 0; i < (int)m_Child.size(); i++ )
    {
        SEObject* pLinkID = pLink->GetLinkID();
        SESpatial* pChild = (SESpatial*)rStream.GetFromMap(pLinkID);
        if( pChild )
        {
            SetChild(i, pChild);
        }
    }
}
//----------------------------------------------------------------------------
bool SENode::Register(SEStream& rStream) const
{
    if( !SESpatial::Register(rStream) )
    {
        return false;
    }

    for( int i = 0; i < (int)m_Child.size(); ++i )
    {
        if( m_Child[i] )
        {
            m_Child[i]->Register(rStream);
        }
    }

    return true;
}
//----------------------------------------------------------------------------
void SENode::Save(SEStream& rStream) const
{
    SE_BEGIN_DEBUG_STREAM_SAVE;

    SESpatial::Save(rStream);

    // link data
    rStream.Write((int)m_Child.size());
    for( int i = 0; i < (int)m_Child.size(); ++i )
    {
        rStream.Write(m_Child[i]);
    }

    SE_END_DEBUG_STREAM_SAVE(SENode);
}
//----------------------------------------------------------------------------
int SENode::GetDiskUsed(const SEStreamVersion& rVersion) const
{
    return SESpatial::GetDiskUsed(rVersion) +
        sizeof(int) +  // m_Child count
        ((int)m_Child.size())*SE_STREAM_PTRSIZE(m_Child[0]);
}
//----------------------------------------------------------------------------