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

#ifndef Swing_StreamMCR_H
#define Swing_StreamMCR_H

//----------------------------------------------------------------------------
#define SE_DECLARE_STREAM \
public: \
    static bool RegisterFactory(void); \
    static void InitializeFactory(void); \
    static SEObject* Factory(SEStream& rStream); \
    virtual void Load(SEStream& rStream, SEStream::SELink* pLink); \
    virtual void Link(SEStream& rStream, SEStream::SELink* pLink); \
    virtual bool Register(SEStream& rStream) const; \
    virtual void Save(SEStream& rStream) const; \
    virtual int GetDiskUsed(const SEStreamVersion& rVersion) const; \
private: \
    static bool msIsStreamRegistered
//----------------------------------------------------------------------------
#define SE_DECLARE_TEMPLATE_STREAM \
public: \
    static bool RegisterFactory(void); \
    static void InitializeFactory(void); \
    static SEObject* Factory(SEStream& rStream); \
    virtual void Load(SEStream& rStream, SEStream::SELink* pLink); \
    virtual void Link(SEStream& rStream, SEStream::SELink* pLink); \
    virtual bool Register(SEStream& rStream) const; \
    virtual void Save(SEStream& rStream) const; \
    virtual int GetDiskUsed(const SEStreamVersion& rVersion) const; \
private: \
    SE_CORE_API static bool msIsStreamRegistered
//----------------------------------------------------------------------------
#define SE_IMPLEMENT_STREAM(classname) \
bool classname::msIsStreamRegistered = false; \
\
bool classname::RegisterFactory() \
{ \
    if( !msIsStreamRegistered ) \
    { \
        SEMain::AddInitializer(classname::InitializeFactory); \
        msIsStreamRegistered = true; \
    } \
    return msIsStreamRegistered; \
} \
\
void classname::InitializeFactory() \
{ \
    if( !msFactoryFunctionTable ) \
    { \
        msFactoryFunctionTable = SE_NEW SEStringHashTable<FactoryFunction>( \
            FACTORY_MAP_SIZE); \
    } \
    msFactoryFunctionTable->Insert(TYPE.GetName(), (FactoryFunction)Factory); \
} \
\
SEObject* classname::Factory(SEStream& rStream) \
{ \
    classname* pObject = SE_NEW classname; \
    SEStream::SELink* pLink = SE_NEW SEStream::SELink(pObject); \
    pObject->Load(rStream, pLink); \
    return pObject; \
}
//----------------------------------------------------------------------------
#define SE_IMPLEMENT_ABSTRACT_STREAM(classname) \
bool classname::msIsStreamRegistered = false; \
\
bool classname::RegisterFactory() \
{ \
    if( !msIsStreamRegistered ) \
    { \
        SEMain::AddInitializer(classname::InitializeFactory); \
        msIsStreamRegistered = true; \
    } \
    return msIsStreamRegistered; \
} \
\
void classname::InitializeFactory() \
{ \
    if( !msFactoryFunctionTable ) \
    { \
        msFactoryFunctionTable = SE_NEW SEStringHashTable<FactoryFunction>( \
            FACTORY_MAP_SIZE); \
    } \
    msFactoryFunctionTable->Insert(TYPE.GetName(), (FactoryFunction)Factory); \
} \
\
SEObject* classname::Factory(SEStream&) \
{ \
    SE_ASSERT(false); \
    return 0; \
}
//----------------------------------------------------------------------------
#define SE_IMPLEMENT_TEMPLATE_STREAM(classname) \
template <> \
bool classname::msIsStreamRegistered = false;
//----------------------------------------------------------------------------
#define SE_IMPLEMENT_DEFAULT_STREAM(classname, baseclassname) \
void classname::Load(SEStream& rStream, SEStream::SELink* pLink) \
{ \
    SE_BEGIN_DEBUG_STREAM_LOAD; \
    baseclassname::Load(rStream, pLink); \
    SE_END_DEBUG_STREAM_LOAD(classname); \
} \
\
void classname::Link(SEStream& rStream, SEStream::SELink* pLink) \
{ \
    baseclassname::Link(rStream, pLink); \
} \
\
bool classname::Register(SEStream& rStream) const \
{ \
    return baseclassname::Register(rStream); \
} \
\
void classname::Save(SEStream& rStream) const \
{ \
    SE_BEGIN_DEBUG_STREAM_SAVE; \
    baseclassname::Save(rStream); \
    SE_END_DEBUG_STREAM_SAVE(classname); \
} \
\
int classname::GetDiskUsed(const SEStreamVersion& rVersion) const \
{ \
    return baseclassname::GetDiskUsed(rVersion); \
} \
//----------------------------------------------------------------------------
#define SE_REGISTER_STREAM(classname) \
static bool gs_bStreamRegistered_##classname = \
    classname::RegisterFactory()
//----------------------------------------------------------------------------
#define SE_INVOKE_STREAM(classname) \
(void)gs_bStreamRegistered_##classname
//----------------------------------------------------------------------------
#ifdef _DEBUG
#define SE_BEGIN_DEBUG_STREAM_LOAD \
    int iBeginLoad_ = rStream.GetBufferNext()
#else
#define SE_BEGIN_DEBUG_STREAM_LOAD
#endif
//----------------------------------------------------------------------------
#ifdef _DEBUG
#define SE_END_DEBUG_STREAM_LOAD(classname) \
    int iEndLoad_ = rStream.GetBufferNext(); \
    int iUsedLoad_ = iEndLoad_ - iBeginLoad_ + GetType().GetDiskUsed(); \
    int iUsedReported_ = classname::GetDiskUsed(rStream.GetVersion()); \
    SE_ASSERT( iUsedLoad_ == iUsedReported_ )
#else
#define SE_END_DEBUG_STREAM_LOAD(classname)
#endif
//----------------------------------------------------------------------------
#ifdef _DEBUG
#define SE_BEGIN_DEBUG_STREAM_SAVE \
    int iBeginSave_ = rStream.GetBufferNext()
#else
#define SE_BEGIN_DEBUG_STREAM_SAVE
#endif
//----------------------------------------------------------------------------
#ifdef _DEBUG
#define SE_END_DEBUG_STREAM_SAVE(classname) \
    int iEndSave_ = rStream.GetBufferNext(); \
    int iUsedSave_ = iEndSave_ - iBeginSave_; \
    int iUsedReported_ = classname::GetDiskUsed( \
        SEStreamVersion(SEStreamVersion::MAJOR, SEStreamVersion::MINOR)); \
    SE_ASSERT( iUsedSave_ == iUsedReported_ )
#else
#define SE_END_DEBUG_STREAM_SAVE(classname)
#endif
//----------------------------------------------------------------------------
// The SE_STREAM_PTRSIZE macro is used only by GetDiskUsed.  A pointer is written
// to 8 bytes on disk, whether running on a 32-bit or 64-bit system.  This
// change was required to support 64-bit platforms, where pointers are
// potentially 8 bytes in size.
#define SE_STREAM_PTRSIZE(value) 8
//----------------------------------------------------------------------------

#endif
