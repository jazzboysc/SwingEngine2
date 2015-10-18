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

#ifndef Swing_Segment3_H
#define Swing_Segment3_H

#include "SECoreLIB.h"
#include "SEVector3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20090113
//----------------------------------------------------------------------------
class SE_CORE_API SESegment3f
{
public:
    // 线段表示为参数方程形式:P + t*D,其中P是线段原点,D是单位长度方向向量,
    // |t| <= e,其中e是线段延展长度.
    // 用户必须确保D为单位向量.
    // 线段端点是P - e*D和P + e*D.
    // 线段的表示方法类似于OBB.P是中点,D是单位轴向量,e是轴向延展长度.

    SESegment3f(void);
    SESegment3f(const SEVector3f& rOrigin, const SEVector3f& rDirection,
        float fExtent);

    // 端点访问.
    SEVector3f GetPosEnd(void) const;  // P + e*D
    SEVector3f GetNegEnd(void) const;  // P - e*D

    SEVector3f Origin, Direction;
    float Extent;
};

}

#endif
