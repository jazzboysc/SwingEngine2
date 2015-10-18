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

#ifndef Swing_StringHashTable_H
#define Swing_StringHashTable_H

#include "SECoreLIB.h"
#include "SESystem.h"

// TValue必须实现的函数:
//   TValue::TValue()
//   TValue& TValue::operator=(const TValue&)
namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20080308
//----------------------------------------------------------------------------
template <class TValue>
class SEStringHashTable
{
public:
    SEStringHashTable(int iTableSize);
    ~SEStringHashTable(void);

    int GetCount(void) const;

    bool Insert(const std::string& rKey, const TValue& rValue);
    TValue* Find(const std::string& rKey) const;
    bool Remove(const std::string& rKey);
    void RemoveAll(void);

    TValue* GetFirst(std::string* pKey) const;
    TValue* GetNext(std::string* pKey) const;

private:
    class SESHTItem
    {
    public:
        SESHTItem(void) : m_Key(""){}
        ~SESHTItem(void){}

    public:
        std::string m_Key;
        TValue m_Value;
        SESHTItem* m_pNextHashItem;
    };

    // 哈希函数
    int HashFunction(const std::string& rKey) const;

    // 哈希表
    int m_iTableSize;
    int m_iCount;
    SESHTItem** m_ppTable;

    // 迭代所有表项时的辅助变量
    mutable int m_iIndex;
    mutable SESHTItem* m_pHashItem;
};

#include "SEStringHashTable.inl"

}

#endif
