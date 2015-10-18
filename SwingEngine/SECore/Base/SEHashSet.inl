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

//----------------------------------------------------------------------------
template <class TKey>
SEHashSet<TKey>::SEHashSet(int iTableSize)
{
    SE_ASSERT( iTableSize > 0 );

    m_iTableSize = iTableSize;
    m_iCount = 0;
    m_iIndex = 0;
    m_pHashItem = 0;
    m_ppTable = SE_NEW SEHSItem*[m_iTableSize];
    memset( m_ppTable, 0, m_iTableSize*sizeof(SEHSItem*));
    UserHashFunction = 0;
}
//----------------------------------------------------------------------------
template <class TKey>
SEHashSet<TKey>::~SEHashSet()
{
    RemoveAll();
    SE_DELETE[] m_ppTable;
}
//----------------------------------------------------------------------------
template <class TKey>
int SEHashSet<TKey>::GetCount() const
{
    return m_iCount;
}
//----------------------------------------------------------------------------
template <class TKey>
TKey* SEHashSet<TKey>::Insert(const TKey& rKey)
{
    // 用哈希函数映射到表项索引
    int iIndex = HashFunction(rKey);
    SEHSItem* pHashItem = m_ppTable[iIndex];

    //  在当前表项列表中查找是否已经存在该关键字
    while( pHashItem )
    {
        if( rKey == pHashItem->m_Key )
        {
            // 关键字已存在
            return &pHashItem->m_Key;
        }

        pHashItem = pHashItem->m_pNextHashItem;
    }

    // 插入新值
    pHashItem = SE_NEW SEHSItem;
    pHashItem->m_Key = rKey;
    pHashItem->m_pNextHashItem = m_ppTable[iIndex];
    m_ppTable[iIndex] = pHashItem;
    m_iCount++;

    return &pHashItem->m_Key;
}
//----------------------------------------------------------------------------
template <class TKey>
TKey* SEHashSet<TKey>::Get(const TKey& rKey) const
{
    // 用哈希函数映射到表项索引
    int iIndex = HashFunction(rKey);
    SEHSItem* pHashItem = m_ppTable[iIndex];

    // 查找符合指定关键字的项
    while( pHashItem )
    {
        if( rKey == pHashItem->m_Key )
        {
            return &pHashItem->m_Key;
        }

        pHashItem = pHashItem->m_pNextHashItem;
    }

    return 0;
}
//----------------------------------------------------------------------------
template <class TKey>
bool SEHashSet<TKey>::Remove(const TKey& rKey)
{
    // 用哈希函数映射到表项索引
    int iIndex = HashFunction(rKey);
    SEHSItem* pHashItem = m_ppTable[iIndex];

    if( !pHashItem )
    {
        return false;
    }

    if( rKey == pHashItem->m_Key )
    {
        // 表项在列表头部
        SEHSItem* pTemp = pHashItem;
        m_ppTable[iIndex] = pHashItem->m_pNextHashItem;
        SE_DELETE pTemp;
        m_iCount--;
        return true;
    }

    // 表项可能在列表中,查找并删除
    SEHSItem* pPrev = pHashItem;
    SEHSItem* pCur = pHashItem->m_pNextHashItem;
    while( pCur && rKey != pCur->m_Key )
    {
        pPrev = pCur;
        pCur = pCur->m_pNextHashItem;
    }

    if( pCur )
    {
        // 找到该项
        pPrev->m_pNextHashItem = pCur->m_pNextHashItem;
        SE_DELETE pCur;
        m_iCount--;
        return true;
    }

    return false;
}
//----------------------------------------------------------------------------
template <class TKey>
void SEHashSet<TKey>::RemoveAll()
{
    if( m_iCount > 0 )
    {
        for( int iIndex = 0; iIndex < m_iTableSize; iIndex++ )
        {
            while( m_ppTable[iIndex] )
            {
                SEHSItem* pTemp = m_ppTable[iIndex];
                m_ppTable[iIndex] = m_ppTable[iIndex]->m_pNextHashItem;
                SE_DELETE pTemp;

                if( --m_iCount == 0 )
                {
                    return;
                }
            }
        }
    }
}
//----------------------------------------------------------------------------
template <class TKey>
TKey* SEHashSet<TKey>::GetFirst() const
{
    if( m_iCount > 0 )
    {
        for( m_iIndex = 0; m_iIndex < m_iTableSize; m_iIndex++ )
        {
            if( m_ppTable[m_iIndex] )
            {
                m_pHashItem = m_ppTable[m_iIndex];
                return &m_pHashItem->m_Key;
            }
        }
    }

    return 0;
}
//----------------------------------------------------------------------------
template <class TKey>
TKey* SEHashSet<TKey>::GetNext() const
{
    if( m_iCount > 0 )
    {
        m_pHashItem = m_pHashItem->m_pNextHashItem;
        if( m_pHashItem )
        {
            return &m_pHashItem->m_Key;
        }
        
        for( m_iIndex++; m_iIndex < m_iTableSize; m_iIndex++ )
        {
            if( m_ppTable[m_iIndex] )
            {
                m_pHashItem = m_ppTable[m_iIndex];
                return &m_pHashItem->m_Key;
            }
        }
    }

    return 0;
}
//----------------------------------------------------------------------------
template <class TKey>
int SEHashSet<TKey>::HashFunction(const TKey& rKey) const
{
    if( UserHashFunction )
    {
        return (*UserHashFunction)(rKey);
    }

    static double s_dHashMultiplier = 0.5 * (sqrt(5.0) - 1.0);
    unsigned int uiKey;
    SESystem::Memcpy(&uiKey, sizeof(unsigned int), &rKey, 
        sizeof(unsigned int));
    uiKey %= m_iTableSize;
    double dFraction = fmod(s_dHashMultiplier*uiKey, 1.0);

    return (int)floor(m_iTableSize * dFraction);
}
//----------------------------------------------------------------------------
