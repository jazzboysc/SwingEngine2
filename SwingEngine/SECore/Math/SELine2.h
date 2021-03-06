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

#ifndef Swing_Line2_H
#define Swing_Line2_H

#include "SECoreLIB.h"
#include "SEVector2.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20090105
//----------------------------------------------------------------------------
class SE_CORE_API SELine2f
{
public:
    // 直线表示为参数方程形式:P + t*D,其中P是直线上一点,D是单位长度方向向量,
    // 必须确保D为单位向量.

    SELine2f(void);
    SELine2f(const SEVector2f& rOrigin, const SEVector2f& rDirection);

    SEVector2f Origin, Direction;
};

}

#endif
