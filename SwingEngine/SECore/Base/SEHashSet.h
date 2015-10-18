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

#ifndef Swing_HashSet_H
#define Swing_HashSet_H

// TKey����ʵ�ֵĺ���:
//   TKey::TKey()
//   TKey& TKey::operator=(const TKey&)
//   bool TKey::operator==(const TKey&) const
//   bool TKey::operator!=(const TKey&) const
//   TKey::operator unsigned int() const
// ��ʽ����ת����unsigned int�������ɹ�ϣ������,
// ����ֵ��һ��Ҫ�ڹ�ϣ��������Χ��,��ϣ��ͨ��ȡģ������ӳ�䵽�Ϸ���Χ

#include "SESystem.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20080310
//----------------------------------------------------------------------------
template <class TKey>
class SEHashSet
{
public:
    SEHashSet(int iTableSize);
    ~SEHashSet(void);

    int GetCount(void) const;

    // ����ָ��ʵ�ʴ洢λ�õ�ָ��,�Ӷ������߿���ֱ�ӷ��ʸñ���,
    // �⽫��������TKey���ӳ�Ա��������ؼ��ֱȽ�
    TKey* Insert(const TKey& rKey);

    TKey* Get(const TKey& rKey) const;

    bool Remove(const TKey& rKey);
    void RemoveAll(void);

    TKey* GetFirst(void) const;
    TKey* GetNext(void) const;

    // �û��Զ���Ĺ�ϣ����
    int (*UserHashFunction)(const TKey&);

private:
    class SEHSItem
    {
    public:
        TKey m_Key;
        SEHSItem* m_pNextHashItem;
    };

    // Ĭ�Ϲ�ϣ����,�ɱ��û���������
    int HashFunction(const TKey& rKey) const;

    // ��ϣ��
    int m_iTableSize;
    int m_iCount;
    SEHSItem** m_ppTable;

    // �������б���ʱ�ĸ�������
    mutable int m_iIndex;
    mutable SEHSItem* m_pHashItem;
};

#include "SEHashSet.inl"

}

#endif
