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
    // һЩ������ϣ��node�ڵ��������ж�����ڵ�,�Ӷ�ʹ������ͼ��ΪDAG.
    // ʵ���ϲ�������,������ͼ�ṹ��һ�����ͽṹ.
    // ������Ե�ʹ��,ʹ��������ʶ����ǰ�ӽڵ��ԭ�����ӹ�ϵ��Ҫ���ƻ�,
    // Ϊ�˰�ȫ����,��Ӧ�����ȵ���DetachChild����ʹ�ӽڵ�����ԭ�����ӹ�ϵ,
    // ֮���ٵ���AttachChild��SetChild����.
    // ����һ��Ҫע�⵽,����DetachChildʱ�п���ʹ����ӽڵ����ü���Ϊ0�Ӷ�����
    // ����,��ȫ��������,��һ������ָ�����ø��ӽڵ�,Ȼ�����DetachChild,����
    // ��ʾ:
    //
    //     SENode* pNode0 = SE_NEW SENode;
    //     SESpatial* pChild0 = <...>;
    //     pNode0->AttachChild(pChild0);  // child at index 0
    //     SENode* pNode1 = <...>;
    //
    //     // �⽫���������,��ΪpChild0�Ѿ��и��ڵ�(pNode0).
    //     pNode1->AttachChild(pChild0);
    //
    //     // ȡ����֮����Ӧ������,ͬʱ��ֹ�ӽڵ���������.
    //     SESpatialPtr spSaveChild = pNode0->GetChild(0);
    //     pNode0->DetachChild(spSaveChild);
    //     pNode1->AttachChild(spSaveChild);

    SE_ASSERT( pChild && !pChild->GetParent() );

    pChild->SetParent(this);

    // �Ƿ��п�slot
    int iCount = (int)m_Child.size();
    for( int i = 0; i < iCount; ++i )
    {
        if( m_Child[i] == 0 )
        {
            // �������
            m_Child[i] = pChild;

            return i;
        }
    }

    // ������β��
    m_Child.push_back(pChild);

    return iCount;
}
//----------------------------------------------------------------------------
int SENode::DetachChild(SESpatial* pChild)
{
    if( pChild )
    {
        // �Ƿ����
        for( int i = 0; i < (int)m_Child.size(); ++i )
        {
            if( m_Child[i] == pChild )
            {
                // �������Ƴ�
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
            // �������Ƴ�
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
    // һЩ������ϣ��node�ڵ��������ж�����ڵ�,�Ӷ�ʹ������ͼ��ΪDAG.
    // ʵ���ϲ�������,������ͼ�ṹ��һ�����ͽṹ.
    // ������Ե�ʹ��,ʹ��������ʶ����ǰ�ӽڵ��ԭ�����ӹ�ϵ��Ҫ���ƻ�,
    // Ϊ�˰�ȫ����,��Ӧ�����ȵ���DetachChild����ʹ�ӽڵ�����ԭ�����ӹ�ϵ,
    // ֮���ٵ���AttachChild��SetChild����.
    // ����һ��Ҫע�⵽,����DetachChildʱ�п���ʹ����ӽڵ����ü���Ϊ0�Ӷ�����
    // ����,��ȫ��������,��һ������ָ�����ø��ӽڵ�,Ȼ�����DetachChild,����
    // ��ʾ:
    //
    //     SENode* pNode0 = SE_NEW SENode;
    //     SESpatial* pChild0 = <...>;
    //     pNode0->AttachChild(pChild0);  // child at index 0
    //     SENode* pNode1 = <...>;
    //
    //     // �⽫���������,��ΪpChild0�Ѿ��и��ڵ�(pNode0).
    //     pNode1->AttachChild(pChild0);
    //
    //     // ȡ����֮����Ӧ������,ͬʱ��ֹ�ӽڵ���������.
    //     SESpatialPtr spSaveChild = pNode0->GetChild(0);
    //     pNode0->DetachChild(spSaveChild);
    //     pNode1->AttachChild(spSaveChild);

    if( pChild )
    {
        SE_ASSERT( !pChild->GetParent() );
    }

    if( 0 <= i && i < (int)m_Child.size() )
    {
        // �Ƴ�slot�о����ӽڵ�
        SESpatialPtr spPreviousChild = m_Child[i];
        if( spPreviousChild )
        {
            spPreviousChild->SetParent(0);
        }

        // �����µ��ӽڵ㵽slot
        if( pChild )
        {
            pChild->SetParent(this);
        }

        m_Child[i] = pChild;

        return spPreviousChild;
    }

    // ����������Χ,�����β��
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

    // SENode�ڵ������ν���AB�ݹ�,�Ӷ�������ı���.
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
        // ����һ��̰���㷨��BV�ϲ�,
        // ���ղ�����BV���������ӽڵ��BV,�������ܱ�֤����СBV,
        // �ܹ�������СBV���㷨ʱ�临�ӶȽϸ��Ҽ��临��,��˲������.
        bool bFoundFirstBound = false;
        for( int i = 0; i < (int)m_Child.size(); i++ )
        {
            SESpatial* pChild = m_Child[i];
            if( pChild )
            {
                if( bFoundFirstBound )
                {
                    // �õ�ǰworld bound���ӽڵ�world bound�ϲ�
                    WorldBound->GrowToContain(pChild->WorldBound);
                }
                else
                {
                    // ���Ȱ�world bound����Ϊ��һ���ǿ��ӽڵ��world bound
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
    // SENode�ڵ������ν���AB�ݹ�,�Ӷ���ɳ�����ͼ���ı���.
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