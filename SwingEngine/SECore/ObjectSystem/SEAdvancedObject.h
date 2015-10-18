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

#ifndef Swing_AdvancedObject_H
#define Swing_AdvancedObject_H

#include "SECoreLIB.h"
#include "SEObject.h"

namespace Swing
{

class SEController;

//----------------------------------------------------------------------------
// Description:
// Date:20100531
//----------------------------------------------------------------------------
class SE_CORE_API SEAdvancedObject : public SEObject
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    ~SEAdvancedObject(void);

protected:
    // Abstract base class.
    SEAdvancedObject(void);

// Controller system.
public:
    inline int GetControllerCount(void) const;
    inline SEController* GetController(int i) const;
    void AttachController(SEController* pController);
    void DetachController(SEController* pController);
    void DetachAllControllers(void);
    bool UpdateControllers(double dAppTime);

private:
    std::vector<SEObjectPtr> m_Controllers;
};

typedef SESmartPointer<SEAdvancedObject> SEAdvancedObjectPtr;

#include "SEAdvancedObject.inl"

}

#endif