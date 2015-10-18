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
template <class TKey, class TValue>
SEHashTable<TKey, TValue>::SEHashTable(int iTableSize)
{
    SE_ASSERT( iTableSize > 0 );

    m_iTableSize = iTableSize;
    m_iCount = 0;
    m_iIndex = 0;
    m_pHashItem = 0;
    m_ppTable = SE_NEW SEHTItem*[m_iTableSize];
    memset(m_ppTable, 0, m_iTableSize*sizeof(SEHTItem*));
    UserHashFunction = 0;
}
//----------------------------------------------------------------------------
template <class TKey, class TValue>
SEHashTable<TKey, TValue>::~SEHashTable()
{
    RemoveAll();
    SE_DELETE[] m_ppTable;
}
//----------------------------------------------------------------------------
template <class TKey, class TValue>
int SEHashTable<TKey, TValue>::GetCount() const
{
    return m_iCount;
}
//----------------------------------------------------------------------------
template <class TKey, class TValue>
bool SEHashTable<TKey, TValue>::Insert(const TKey& rKey, const TValue& rValue)
{
    int iIndex = HashFunction(rKey);
    SEHTItem* pHashItem = m_ppTable[iIndex];

    while( pHashItem )
    {
        if(rKey == pHashItem->m_Key)
        {
            return false;
        }

        pHashItem = pHashItem->m_pNextHashItem;
    }

    pHashItem = SE_NEW SEHTItem;
    pHashItem->m_Key = rKey;
    pHashItem->m_Value = rValue;
    pHashItem->m_pNextHashItem = m_ppTable[iIndex];
    m_ppTable[iIndex] = pHashItem;
    m_iCount++;

    return true;
}
//----------------------------------------------------------------------------
template <class TKey, class TValue>
TValue* SEHashTable<TKey, TValue>::Find(const TKey& rKey) const
{
    // �ù�ϣ����ӳ�䵽��������
    int iIndex = HashFunction(rKey);
    SEHTItem* pHashItem = m_ppTable[iIndex];

    // ���ҷ���ָ���ؼ��ֵ���
    while( pHashItem )
    {
        if( rKey == pHashItem->m_Key )
        {
            return &pHashItem->m_Value;
        }

        pHashItem = pHashItem->m_pNextHashItem;
    }

    return 0;
}
//----------------------------------------------------------------------------
template <class TKey, class TValue>
bool SEHashTable<TKey, TValue>::Remove(const TKey& rKey)
{
    // �ù�ϣ����ӳ�䵽��������
    int iIndex = HashFunction(rKey);
    SEHTItem* pHashItem = m_ppTable[iIndex];

    if( !pHashItem )
    {
        return false;
    }

    if( rKey == pHashItem->m_Key )
    {
        // �������б�ͷ��
        SEHTItem* pTemp = pHashItem;
        m_ppTable[iIndex] = pHashItem->m_pNextHashItem;
        SE_DELETE pTemp;
        m_iCount--;
        return true;
    }

    // ����������б���,���Ҳ�ɾ��
    SEHTItem* pPrev = pHashItem;
    SEHTItem* pCur = pHashItem->m_pNextHashItem;
    while( pCur && rKey != pCur->m_Key )
    {
        pPrev = pCur;
        pCur = pCur->m_pNextHashItem;
    }

    if( pCur )
    {
        // �ҵ�����
        pPrev->m_pNextHashItem = pCur->m_pNextHashItem;
        SE_DELETE pCur;
        m_iCount--;
        return true;
    }

    return false;
}
//----------------------------------------------------------------------------
template <class TKey, class TValue>
void SEHashTable<TKey, TValue>::RemoveAll()
{
    if( m_iCount > 0 )
    {
        for( int iIndex = 0; iIndex < m_iTableSize; iIndex++ )
        {
            while( m_ppTable[iIndex] )
            {
                SEHTItem* pTemp = m_ppTable[iIndex];
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
template <class TKey, class TValue>
TValue* SEHashTable<TKey, TValue>::GetFirst(TKey* pKey) const
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
template <class TKey, class TValue>
TValue* SEHashTable<TKey, TValue>::GetNext(TKey* pKey) const
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
template <class TKey, class TValue>
int SEHashTable<TKey, TValue>::HashFunction(const TKey& rKey) const
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
