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

#ifndef Swing_Triangle2_H
#define Swing_Triangle2_H

#include "SECoreLIB.h"
#include "SEVector2.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20081223
//----------------------------------------------------------------------------
class SE_CORE_API SETriangle2f
{
public:
    SETriangle2f(void);  // 未初始化
    SETriangle2f(const SEVector2f& rV0, const SEVector2f& rV1, const 
        SEVector2f& rV2);
    SETriangle2f(const SEVector2f aV[3]);

    // 三角形到点Q的距离.
    float GetDistance(const SEVector2f& rQ) const;

    SEVector2f V[3];
};

}

#endif
