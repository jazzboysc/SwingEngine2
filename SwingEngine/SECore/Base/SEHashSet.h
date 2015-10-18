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

// TKey必须实现的函数:
//   TKey::TKey()
//   TKey& TKey::operator=(const TKey&)
//   bool TKey::operator==(const TKey&) const
//   bool TKey::operator!=(const TKey&) const
//   TKey::operator unsigned int() const
// 隐式类型转换到unsigned int用来生成哈希表索引,
// 返回值不一定要在哈希表索引范围内,哈希表通过取模运算来映射到合法范围

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

    // 返回指向实际存储位置的指针,从而调用者可以直接访问该表项,
    // 这将允许若干TKey的子成员变量参与关键字比较
    TKey* Insert(const TKey& rKey);

    TKey* Get(const TKey& rKey) const;

    bool Remove(const TKey& rKey);
    void RemoveAll(void);

    TKey* GetFirst(void) const;
    TKey* GetNext(void) const;

    // 用户自定义的哈希函数
    int (*UserHashFunction)(const TKey&);

private:
    class SEHSItem
    {
    public:
        TKey m_Key;
        SEHSItem* m_pNextHashItem;
    };

    // 默认哈希函数,可被用户函数覆盖
    int HashFunction(const TKey& rKey) const;

    // 哈希表
    int m_iTableSize;
    int m_iCount;
    SEHSItem** m_ppTable;

    // 迭代所有表项时的辅助变量
    mutable int m_iIndex;
    mutable SEHSItem* m_pHashItem;
};

#include "SEHashSet.inl"

}

#endif
