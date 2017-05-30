// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_ISpatialInfo_H
#define Swing_ISpatialInfo_H

#include "SECoreLIB.h"
#include "SEPlatforms.h"
#include "SEMatrix3.h"
#include "SEVector3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20170526
//----------------------------------------------------------------------------
class SE_CORE_API SEISpatialInfo
{
public:
    virtual ~SEISpatialInfo() {}

    virtual SEVector3f GetScale() const = 0;
    virtual SEMatrix3f GetRotation() const = 0;
    virtual SEVector3f GetLocation() const = 0;

protected:
    // Abstract interface.
    SEISpatialInfo() {}
};

}

#endif