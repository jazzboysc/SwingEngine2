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
template <class T>
SESharedArray<T>::SESharedArray(int iCount, T* pArray)
{
    m_iCount = iCount;
    m_pArray = pArray;
}
//----------------------------------------------------------------------------
template <class T>
SESharedArray<T>::SESharedArray(const SESharedArray& rShared)
{
    m_pArray = 0;
    *this = rShared;
}
//----------------------------------------------------------------------------
template <class T>
SESharedArray<T>::~SESharedArray()
{
    SE_DELETE[] m_pArray;
}
//----------------------------------------------------------------------------
template <class T>
SESharedArray<T>& SESharedArray<T>::operator=(const SESharedArray& rShared)
{
    SE_DELETE[] m_pArray;
    m_iCount = rShared.m_iCount;
    if( rShared.m_pArray )
    {
        m_pArray = SE_NEW T[m_iCount];
        for( int i = 0; i < m_iCount; i++ )
        {
            m_pArray[i] = rShared.m_pArray[i];
        }
    }
    else
    {
        m_pArray = 0;
    }

    return *this;
}
//----------------------------------------------------------------------------
template <class T>
int SESharedArray<T>::GetCount() const
{
    return m_iCount;
}
//----------------------------------------------------------------------------
template <class T>
T* SESharedArray<T>::GetData() const
{
    return m_pArray;
}
//----------------------------------------------------------------------------
template <class T>
SESharedArray<T>::operator const T*() const
{
    return m_pArray;
}
//----------------------------------------------------------------------------
template <class T>
SESharedArray<T>::operator T*()
{
    return m_pArray;
}
//----------------------------------------------------------------------------
template <class T>
const T& SESharedArray<T>::operator[](int i) const
{
    SE_ASSERT( 0 <= i && i < m_iCount );

    return m_pArray[i];
}
//----------------------------------------------------------------------------
template <class T>
T& SESharedArray<T>::operator[](int i)
{
    SE_ASSERT( 0 <= i && i < m_iCount );

    return m_pArray[i];
}
//----------------------------------------------------------------------------
template <class T>
void SESharedArray<T>::SetActiveCount(int iActiveCount)
{
    SE_ASSERT( iActiveCount >= 0 );

    m_iCount = iActiveCount;
}
//----------------------------------------------------------------------------
template <class T> 
SEObject* SESharedArray<T>::Factory(SEStream& rStream) 
{ 
    SESharedArray<T>* pObject = SE_NEW SESharedArray<T>; 
    SEStream::SELink* pLink = SE_NEW SEStream::SELink(pObject);
    pObject->Load(rStream, pLink);

    return pObject; 
} 
//----------------------------------------------------------------------------
template <class T> 
void SESharedArray<T>::InitializeFactory() 
{ 
    if( !msFactoryFunctionTable )
    { 
        msFactoryFunctionTable = SE_NEW SEStringHashTable<FactoryFunction>(FACTORY_MAP_SIZE);
    } 
    msFactoryFunctionTable->Insert(TYPE.GetName(), (FactoryFunction)SESharedArray<T>::Factory);
} 
//----------------------------------------------------------------------------
template <class T> 
bool SESharedArray<T>::RegisterFactory() 
{ 
    if( !msIsStreamRegistered ) 
    { 
        SEMain::AddInitializer(SESharedArray<T>::InitializeFactory); 
        msIsStreamRegistered = true; 
    }

    return msIsStreamRegistered;
}
//----------------------------------------------------------------------------
template <class T>
SEObject* SESharedArray<T>::GetObjectByName(const std::string& rName)
{
    return SEObject::GetObjectByName(rName);
}
//----------------------------------------------------------------------------
template <class T>
void SESharedArray<T>::GetAllObjectsByName(const std::string& rName,
    std::vector<SEObject*>& rObjects)
{
    SEObject::GetAllObjectsByName(rName,rObjects);
}
//----------------------------------------------------------------------------
template <class T>
SEObject* SESharedArray<T>::GetObjectByID(unsigned int uiID)
{
    return SEObject::GetObjectByID(uiID);
}
//----------------------------------------------------------------------------
template <class T>
void SESharedArray<T>::Load(SEStream& rStream, SEStream::SELink* pLink)
{
    SE_BEGIN_DEBUG_STREAM_LOAD;

    SEObject::Load(rStream, pLink);
    rStream.Read(m_iCount);
    m_pArray = SE_NEW T[m_iCount];
    rStream.Read(m_iCount, m_pArray);

    SE_END_DEBUG_STREAM_LOAD(SESharedArray<T>);
}
//----------------------------------------------------------------------------
template <class T>
void SESharedArray<T>::Link(SEStream& rStream, SEStream::SELink* pLink)
{
    SEObject::Link(rStream,pLink);
}
//----------------------------------------------------------------------------
template <class T>
bool SESharedArray<T>::Register(SEStream& rStream) const
{
    return SEObject::Register(rStream);
}
//----------------------------------------------------------------------------
template <class T>
void SESharedArray<T>::Save(SEStream& rStream) const
{
    SE_BEGIN_DEBUG_STREAM_SAVE;

    SEObject::Save(rStream);
    rStream.Write(m_iCount);
    rStream.Write(m_iCount, m_pArray);

    SE_END_DEBUG_STREAM_SAVE(SESharedArray<T>);
}
//----------------------------------------------------------------------------
template <class T>
int SESharedArray<T>::GetDiskUsed(const SEStreamVersion& rVersion) const
{
    return SEObject::GetDiskUsed(rVersion) + sizeof(m_iCount) + 
        m_iCount*sizeof(T);
}
//----------------------------------------------------------------------------