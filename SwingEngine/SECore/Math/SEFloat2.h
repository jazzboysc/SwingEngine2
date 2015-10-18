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

#ifndef Swing_Float2_H
#define Swing_Float2_H

#include "SECoreLIB.h"
#include "SETuple.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20100623
//----------------------------------------------------------------------------
class SE_CORE_API SEFloat2 : public SETuple<2, float>
{
public:
    SEFloat2(void);   // uninitialized
    SEFloat2(float f0, float f1);
    SEFloat2(const SEFloat2& rTuple);
    ~SEFloat2(void);  // hides ~SETuple<2, float>

    // Assignment.
    SEFloat2& operator = (const SEFloat2& rTuple);
};

}

#endif
