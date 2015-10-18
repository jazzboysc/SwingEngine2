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

#include "SECorePCH.h"
#include "SEObject.h"
#include "SEController.h"

#include <atomic>

using namespace Swing;

const SERTTI SEObject::TYPE("Swing.SEObject", 0);
SE_AtomicInt32 SEObject::msNextID = SEObject::FirstObjectID - 1;
SEHashTable<unsigned int, SEObject*>* SEObject::InUse = 0;
char SEObject::NameAppend = '#';
SEStringHashTable<FactoryFunction>* SEObject::msFactoryFunctionTable = 0;
bool SEObject::msIsStreamRegistered = false;

struct SEObject::Internal
{
    Internal()
        :
        msInitialized(false),
        msMutex(0)
    {
    }

    std::atomic<bool> msInitialized;
    SEMutex* msMutex;
};

SEObject::Internal SEObject::msInternal;

//----------------------------------------------------------------------------
SEObject::SEObject()
{
    mID = SE_AtomicAdd(&msNextID, 1);

    if( mID == 1 )
    {
        InUse = SE_NEW SEHashTable<unsigned int, SEObject*>(FACTORY_MAP_SIZE);
        msInternal.msMutex = SEMutex::Create();
        SE_ASSERT( msInternal.msMutex );

        std::atomic_thread_fence(std::memory_order_release);
        msInternal.msInitialized.store(true, std::memory_order_relaxed);
    }

    while( !msInternal.msInitialized.load(std::memory_order_relaxed) )
    {
        // Wait until object 1 creates the global data.
    }

    std::atomic_thread_fence(std::memory_order_acquire);
    SEMutexLock lock(*msInternal.msMutex);
    InUse->Insert(mID, this);
}
//----------------------------------------------------------------------------
SEObject::~SEObject()
{
    SEMutexLock lock(*msInternal.msMutex);
    SE_ASSERT( InUse );
    bool bFound = InUse->Remove(mID);
    SE_ASSERT( bFound );
    (void)bFound;
}
//----------------------------------------------------------------------------
void SEObject::PrintInUse(const char* pFileName, const char* pMessage)
{
    SEMutexLock lock(*msInternal.msMutex);
    FILE* pFile = SESystem::Fopen(pFileName, "at");
    SE_ASSERT( pFile );

    SESystem::Fprintf(pFile, "%s\n", pMessage);

    SE_ASSERT( InUse );
    unsigned int uiID = 0;

    SEObject** ppObject = InUse->GetFirst(&uiID);
    while( ppObject )
    {
        SESystem::Fprintf(pFile, "id = %6d , type = %s\n", uiID,
            (const char*)(*ppObject)->GetType().GetName());
        ppObject = InUse->GetNext(&uiID);
    }

    SESystem::Fprintf(pFile, "\n\n");
    SESystem::Fclose(pFile);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// copying
//----------------------------------------------------------------------------
SEObjectPtr SEObject::Copy(bool bUniqueNames) const
{
    // Save the object to a memory buffer.
    SEStream tempSaveStream;
    tempSaveStream.Insert((SEObject*)this);
    char* pBuffer = 0;
    int iBufferSize = 0;
    bool bSuccessful = tempSaveStream.Save(pBuffer, iBufferSize);
    SE_ASSERT( bSuccessful );
    if( !bSuccessful )
    {
        return 0;
    }

    // Load the object from the memory buffer.
    SEStream tempLoadStream;
    bSuccessful = tempLoadStream.Load(pBuffer, iBufferSize);
    SE_ASSERT( bSuccessful );

    if( !bSuccessful )
    {
        return 0;
    }
    SE_DELETE[] pBuffer;

    if( bUniqueNames )
    {
        // Generate unique names.
        for( int i = 0; i < tempLoadStream.GetOrderedCount(); i++ )
        {
            SEObject* pObject = tempLoadStream.GetOrderedObject(i);
            SE_ASSERT( pObject );

            const std::string& rName = pObject->GetName();
            int iLength = (int)rName.length();
            if( iLength > 0 )
            {
                // Object has a name, append a character to make it unique.
                const char* pName = rName.c_str();
                char* acNewName = SE_NEW char[iLength + 2];
                const size_t uiSize = (size_t)(iLength + 2);
                SESystem::Strcpy(acNewName, uiSize, pName);
                acNewName[iLength] = NameAppend;
                acNewName[iLength+1] = 0;
                pObject->SetName(std::string(acNewName));
                SE_DELETE[] acNewName;
            }
        }
    }

    return tempLoadStream.GetObjectAt(0);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// name and unique id
//----------------------------------------------------------------------------
SEObject* SEObject::GetObjectByName(const std::string& rName)
{
    return (rName == mName ? this : 0);
}
//----------------------------------------------------------------------------
void SEObject::GetAllObjectsByName(const std::string& rName, 
    std::vector<SEObject*>& rObjects)
{
    if( rName == mName )
    {
        rObjects.push_back(this);
    }
}
//----------------------------------------------------------------------------
SEObject* SEObject::GetObjectByID(unsigned int uiID)
{
    return (uiID == mID ? this : 0);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// streaming
//----------------------------------------------------------------------------
bool SEObject::RegisterFactory()
{
    if( !msIsStreamRegistered )
    {
        SEMain::AddInitializer(SEObject::InitializeFactory);
        SEMain::AddTerminator(SEObject::TerminateFactory);
        msIsStreamRegistered = true;
    }
    return msIsStreamRegistered;
}
//----------------------------------------------------------------------------
void SEObject::InitializeFactory()
{
    if( !msFactoryFunctionTable )
    {
        msFactoryFunctionTable = SE_NEW SEStringHashTable<FactoryFunction>(
            FACTORY_MAP_SIZE);
    }
    msFactoryFunctionTable->Insert(TYPE.GetName(), (FactoryFunction)Factory);
}
//----------------------------------------------------------------------------
void SEObject::TerminateFactory()
{
    SE_DELETE msFactoryFunctionTable;
    msFactoryFunctionTable = 0;
}
//----------------------------------------------------------------------------
SEObject* SEObject::Factory(SEStream&)
{
    SE_ASSERT( false );

    return 0;
}
//----------------------------------------------------------------------------
void SEObject::Load(SEStream& rStream, SEStream::SELink* pLink)
{
    SE_BEGIN_DEBUG_STREAM_LOAD;

    // 获取save时存入的object内存指针,稍后用于link阶段
    SEObject* pLinkID;
    rStream.Read(pLinkID);
    rStream.InsertInMap(pLinkID, pLink);

    // 获取name
    rStream.Read(mName);

    SE_END_DEBUG_STREAM_LOAD(SEObject);
}
//----------------------------------------------------------------------------
void SEObject::Link(SEStream&, SEStream::SELink*)
{
    // Stub for derived classes.
}
//----------------------------------------------------------------------------
bool SEObject::Register(SEStream& rStream) const
{
    SEObject* pThis = (SEObject*)this;
    // 该object是否已插入到hash表,确保唯一性
    if( rStream.InsertInMap(pThis, 0) )
    {
        // 所有object按照深度优先的顺序,在遍历场景树时被收集,
        // 该object不能重复被收集
        rStream.InsertInOrdered(pThis);

        return true;
    }

    return false;
}
//----------------------------------------------------------------------------
void SEObject::Save(SEStream& rStream) const
{
    SE_BEGIN_DEBUG_STREAM_SAVE;

    // RTTI name用于load时的工厂函数表查找
    rStream.Write(std::string(GetType().GetName()));

    // 该内存地址用于load/link时的object unique ID
    rStream.Write((SEObject*)this);

    // 写入name
    rStream.Write(mName);

    SE_END_DEBUG_STREAM_SAVE(SEObject);
}
//----------------------------------------------------------------------------
int SEObject::GetDiskUsed(const SEStreamVersion&) const
{
    int iUsed = GetType().GetDiskUsed();
    iUsed += SE_STREAM_PTRSIZE(this);
    iUsed += sizeof(int) + (int)mName.length();

    return iUsed;
}
//----------------------------------------------------------------------------