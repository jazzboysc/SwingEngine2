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

#ifndef Swing_Rectangle3_H
#define Swing_Rectangle3_H

#include "SECoreLIB.h"
#include "SEVector3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20090113
//----------------------------------------------------------------------------
class SE_CORE_API SERectangle3f
{
public:
    // 矩形上的点表示为:R(s,t) = C + s0*U0 + s1*U1,其中C是矩形中心点,
    // U0和U1是互相垂直的单位轴向量.
    // 参数s0和s1被限制在:|s0| <= e0且|s1| <= e1,
    // 其中e0 > 0,e1 > 0,称为矩形轴向延展长度.

    SERectangle3f(void);
    SERectangle3f(const SEVector3f& rCenter, const SEVector3f* aAxis,
        const float* afExtent);
    SERectangle3f(const SEVector3f& rCenter, const SEVector3f& rAxis0,
        const SEVector3f& rAxis1, float fExtent0, float fExtent1);

    void ComputeVertices(SEVector3f aVertex[4]) const;

    // 角顶点访问.
    SEVector3f GetPPCorner(void) const;  // C + e0*A0 + e1*A1
    SEVector3f GetPMCorner(void) const;  // C + e0*A0 - e1*A1
    SEVector3f GetMPCorner(void) const;  // C - e0*A0 + e1*A1
    SEVector3f GetMMCorner(void) const;  // C - e0*A0 - e1*A1

    SEVector3f Center;
    SEVector3f Axis[2];
    float Extent[2];
};

}

#endif
