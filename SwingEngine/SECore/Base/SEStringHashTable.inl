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
template <class TValue>
SEStringHashTable<TValue>::SEStringHashTable(int iTableSize)
{
    SE_ASSERT( iTableSize > 0 );

    m_iTableSize = iTableSize;
    m_iCount = 0;
    m_iIndex = 0;
    m_pHashItem = 0;
    m_ppTable = SE_NEW SESHTItem*[m_iTableSize];
    memset(m_ppTable, 0, m_iTableSize*sizeof(SESHTItem*));
}
//----------------------------------------------------------------------------
template <class TValue>
SEStringHashTable<TValue>::~SEStringHashTable()
{
    RemoveAll();
    SE_DELETE[] m_ppTable;
}
//----------------------------------------------------------------------------
template <class TValue>
int SEStringHashTable<TValue>::GetCount() const
{
    return m_iCount;
}
//----------------------------------------------------------------------------
template <class TValue>
bool SEStringHashTable<TValue>::Insert(const std::string& rKey, const TValue& 
    rValue)
{
    int iIndex = HashFunction(rKey);
    SESHTItem* pItem = m_ppTable[iIndex];

    while( pItem )
    {
        if( rKey == pItem->m_Key )
        {
            return false;
        }
        pItem = pItem->m_pNextHashItem;
    }

    pItem = SE_NEW SESHTItem;
    pItem->m_Key = rKey;
    pItem->m_Value = rValue;
    pItem->m_pNextHashItem = m_ppTable[iIndex];
    m_ppTable[iIndex] = pItem;
    m_iCount++;

    return true;
}
//----------------------------------------------------------------------------
template <class TValue>
TValue* SEStringHashTable<TValue>::Find(const std::string& rKey) const
{
    int iIndex = HashFunction(rKey);
    SESHTItem* pItem = m_ppTable[iIndex];

    while( pItem )
    {
        if( rKey == pItem->m_Key )
        {
            return &pItem->m_Value;
        }
        pItem = pItem->m_pNextHashItem;
    }

    return 0;
}
//----------------------------------------------------------------------------
template <class TValue>
bool SEStringHashTable<TValue>::Remove(const std::string& rKey)
{
    int iIndex = HashFunction(rKey);
    SESHTItem* pItem = m_ppTable[iIndex];

    if( !pItem )
    {
        return false;
    }

    if( rKey == pItem->m_Key )
    {
        SESHTItem* pSave = pItem;
        m_ppTable[iIndex] = pItem->m_pNextHashItem;
        SE_DELETE pSave;
        m_iCount--;
        return true;
    }

    SESHTItem* pPrev = pItem;
    SESHTItem* pCurr = pItem->m_pNextHashItem;
    while( pCurr && rKey != pCurr->m_Key )
    {
        pPrev = pCurr;
        pCurr = pCurr->m_pNextHashItem;
    }

    if( pCurr )
    {
        pPrev->m_pNextHashItem = pCurr->m_pNextHashItem;
        SE_DELETE pCurr;
        m_iCount--;
        return true;
    }

    return false;
}
//----------------------------------------------------------------------------
template <class TValue>
void SEStringHashTable<TValue>::RemoveAll()
{
    if( m_iCount > 0 )
    {
        for( int iIndex = 0; iIndex < m_iTableSize; iIndex++ )
        {
            while( m_ppTable[iIndex] )
            {
                SESHTItem* pSave = m_ppTable[iIndex];
                m_ppTable[iIndex] = m_ppTable[iIndex]->m_pNextHashItem;
                SE_DELETE pSave;
                if( --m_iCount == 0 )
                {
                    return;
                }
            }
        }
    }
}
//----------------------------------------------------------------------------
template <class TValue>
TValue* SEStringHashTable<TValue>::GetFirst(std::string* pKey) const
{
    if( m_iCount > 0 )
    {
        for( m_iIndex = 0; m_iIndex < m_iTableSize; m_iIndex++ )
        {
            if( m_ppTable[m_iIndex] )
            {
                m_pHashItem = m_ppTable[m_iIndex];
                *pKey = m_pHashItem->m_Key;
                return &m_pHashItem->m_Value;
            }
        }
    }

    return 0;
}
//----------------------------------------------------------------------------
template <class TValue>
TValue* SEStringHashTable<TValue>::GetNext(std::string* pKey) const
{
    if( m_iCount > 0 )
    {
        m_pHashItem = m_pHashItem->m_pNextHashItem;
        if( m_pHashItem )
        {
            *pKey = m_pHashItem->m_Key;
            return &m_pHashItem->m_Value;
        }
        
        for( m_iIndex++; m_iIndex < m_iTableSize; m_iIndex++ )
        {
            if( m_ppTable[m_iIndex] )
            {
                m_pHashItem = m_ppTable[m_iIndex];
                *pKey = m_pHashItem->m_Key;
                return &m_pHashItem->m_Value;
            }
        }
    }

    return 0;
}
//----------------------------------------------------------------------------
template <class TValue>
int SEStringHashTable<TValue>::HashFunction(const std::string& rKey) const
{
    int iHiKey = 0, iLoKey = 0;
    for( int i = 0; i < (int)rKey.length(); i++ )
    {
        if( i & 1 )
        {
            iHiKey += (int)rKey[i];
        }
        else
        {
            iLoKey += (int)rKey[i];
        }
    }
    return abs(iLoKey * iHiKey) % m_iTableSize;
}
//----------------------------------------------------------------------------
