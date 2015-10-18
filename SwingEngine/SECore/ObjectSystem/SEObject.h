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

#ifndef Swing_Object_H
#define Swing_Object_H

#include "SECoreLIB.h"
#include "SEPlatforms.h"
#include "SEMain.h"
#include "SERTTI.h"
#include "SENameIdMCR.h"
#include "SEReferencable.h"
#include "SESmartPointer.h"
#include "SEStream.h"
#include "SEStringHashTable.h"
#include "SEAtomicOperation.h"
#include "SEMutex.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20080329
//----------------------------------------------------------------------------
class SE_CORE_API SEObject : public SEReferencable
{
public:
    virtual ~SEObject(void);

protected:
    // Abstract base class.
    SEObject(void);

// RTTI system.
public:
    static const SERTTI TYPE;
    virtual const SERTTI& GetType(void) const;
    inline bool IsExactly(const SERTTI& rType) const;
    inline bool IsDerived(const SERTTI& rType) const;
    inline bool IsExactlyTypeOf(const SEObject* pObject) const;
    inline bool IsDerivedTypeOf(const SEObject* pObject) const;

// Copying system.
public:
    SESmartPointer<SEObject> Copy(bool bUniqueNames = true) const;
    static char NameAppend;

// Name-ID system.
public:
    inline void SetName(const std::string& rName);
    inline const std::string& GetName(void) const;
    inline unsigned int GetID(void) const;
    inline static unsigned int GetNextID(void);
    virtual SEObject* GetObjectByName(const std::string& rName);
    virtual void GetAllObjectsByName(const std::string& rName, 
        std::vector<SEObject*>& rObjects);
    virtual SEObject* GetObjectByID(unsigned int uiID);

private:
    enum{ FirstObjectID = 1 };
    std::string mName;
    unsigned int mID;
    static SE_AtomicInt32 msNextID;

// Object tracking system.
public:
    static SEHashTable<unsigned int, SEObject*>* InUse;
    static void PrintInUse(const char* pFileName, const char* pMessage);

// Streaming system.
// Internal use
public:
    enum { FACTORY_MAP_SIZE = 256 };
    static SEStringHashTable<FactoryFunction>* msFactoryFunctionTable;
    static bool RegisterFactory(void);
    static void InitializeFactory(void);
    static void TerminateFactory(void);
    static SEObject* Factory(SEStream& rStream);

    virtual void Load(SEStream& rStream, SEStream::SELink* pLink);
    virtual void Link(SEStream& rStream, SEStream::SELink* pLink);
    virtual bool Register(SEStream& rStream) const;
    virtual void Save(SEStream& rStream) const;
    virtual int GetDiskUsed(const SEStreamVersion& rVersion) const;

private:
    static bool msIsStreamRegistered;

private:
    struct Internal;
    static Internal msInternal;
};

template <class T> T* StaticCast(SEObject* pObject);
template <class T> const T* StaticCast(const SEObject* pObject);
template <class T> T* DynamicCast(SEObject* pObject);
template <class T> const T* DynamicCast(const SEObject* pObject);

typedef SESmartPointer<SEObject> SEObjectPtr;

#include "SEObject.inl"

}

#endif
