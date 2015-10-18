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

#ifndef Swing_Triangle3_H
#define Swing_Triangle3_H

#include "SECoreLIB.h"
#include "SEVector3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20080803
//----------------------------------------------------------------------------
class SE_CORE_API SETriangle3f
{
public:
    SETriangle3f(void);  // 未初始化
    SETriangle3f(const SEVector3f& rV0, const SEVector3f& rV1, const 
        SEVector3f& rV2);
    SETriangle3f(const SEVector3f aV[3]);

    // 三角形到点Q的距离.
    float GetDistance(const SEVector3f& rQ) const;

    SEVector3f V[3];
};

}

#endif
