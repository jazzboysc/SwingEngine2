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

#ifndef Swing_SmartPointer_H
#define Swing_SmartPointer_H

#include "SESystem.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20070603
//----------------------------------------------------------------------------
template <class T>
class SESmartPointer
{
public:
    SESmartPointer(T* pObject = 0);
    SESmartPointer(const SESmartPointer& rSmartPointer);
    ~SESmartPointer(void);

    operator T* (void) const;  // usage:T* pObject = SmartPoint spObject
    T& operator * (void) const;
    T* operator -> (void) const;

    SESmartPointer& operator = (T* pObject);
    SESmartPointer& operator = (const SESmartPointer& rSmartPointer);

    bool operator == (T* pObject) const;
    bool operator != (T* pObject) const;
    bool operator == (const SESmartPointer& rSmartPointer) const;
    bool operator != (const SESmartPointer& rSmartPointer) const;

protected:
    T* mObject;
};

#include "SESmartPointer.inl"

}

#endif





