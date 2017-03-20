// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_Referencable_H
#define Swing_Referencable_H

#include "SEAtomicOperation.h"
#include "SESmartPointer.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date: 11/13/2014
//----------------------------------------------------------------------------
class SE_CORE_API SEReferencable
{
public:
    virtual ~SEReferencable();

// Smart pointer system.
public:
    inline void IncrementReferenceCount();
    void DecrementReferenceCount();
    inline int GetReferenceCount() const;

protected:
    // Abstract base class.
    SEReferencable();

private:
    SE_AtomicInt32 mReferenceCount;
};

typedef SESmartPointer<SEReferencable> SEReferencablePtr;

#include "SEReferencable.inl"

}

#endif