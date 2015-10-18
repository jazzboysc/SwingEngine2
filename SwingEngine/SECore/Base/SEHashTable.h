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

#ifndef Swing_HashTable_H
#define Swing_HashTable_H

// TKey����ʵ�ֵĺ���:
//   TKey::TKey()
//   TKey& TKey::operator=(const TKey&)
//   bool TKey::operator==(const TKey&) const
//   bool TKey::operator!=(const TKey&) const
//   TKey::operator unsigned int() const
// ��ʽ����ת����unsigned int�������ɹ�ϣ������,
// ����ֵ��һ��Ҫ�ڹ�ϣ��������Χ��,��ϣ��ͨ��ȡģ������ӳ�䵽�Ϸ���Χ
// TValue����ʵ�ֵĺ���:
//   TValue::TValue()
//   TValue& TValue::operator=(const TValue&)

#include "SESystem.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20080308
//----------------------------------------------------------------------------
template <class TKey, class TValue>
class SEHashTable
{
public:
    SEHashTable(int iTableSize);
    ~SEHashTable(void);

    int GetCount(void) const;

    bool Insert(const TKey& rKey, const TValue& rValue);
    TValue* Find(const TKey& rKey) const;
    bool Remove(const TKey& rKey);
    void RemoveAll(void);

    TValue* GetFirst(TKey* pKey) const;
    TValue* GetNext(TKey* pKey) const;

    // �û��Զ���Ĺ�ϣ����
    int (*UserHashFunction)(const TKey&);

private:
    class SEHTItem
    {
    public:
        TKey m_Key;
        TValue m_Value;
        SEHTItem* m_pNextHashItem;
    };

    // Ĭ�Ϲ�ϣ����,�ɱ��û���������
    int HashFunction(const TKey& rKey) const;

    // ��ϣ��
    int m_iTableSize;
    int m_iCount;
    SEHTItem** m_ppTable;

    // �������б���ʱ�ĸ�������
    mutable int m_iIndex;
    mutable SEHTItem* m_pHashItem;
};

#include "SEHashTable.inl"

}

#endif
