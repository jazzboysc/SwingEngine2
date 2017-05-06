// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_ICamera_H
#define Swing_ICamera_H

#include "SECoreLIB.h"
#include "SEMatrix3.h"
#include "SEVector3.h"
#include "SEMatrix4.h"

namespace Swing
{
//----------------------------------------------------------------------------
// Description:
// Date:20170506
//----------------------------------------------------------------------------
class SE_CORE_API SEICamera
{
    // SwingEngine Camera frame
    //
    //       U
    //       |    D 
    //       |  / 
    //       | / 
    //       |/_________R
    //       E

    // Default location  E = (0,0,0)
    // Default direction R = (1,0,0)
    // Default up        U = (0,1,0)
    // Default right     D = (0,0,1)

public:
    virtual ~SEICamera() {}

    // MT form transformation interface.
    virtual SEMatrix3f GetRotation() const = 0;
    virtual SEVector3f GetLocation() const = 0;

    // View Matrix interface.
    virtual SEMatrix4f GetViewTransform() const = 0;

    // RUD interface.
    virtual SEVector3f GetRight() const = 0;
    virtual SEVector3f GetUp() const = 0;
    virtual SEVector3f GetDirection() const = 0;

protected:
    // Abstract interface.
    SEICamera() {}
};


}

#endif