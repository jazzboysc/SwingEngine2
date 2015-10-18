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

#ifndef Swing_DistTriangle3Triangle3_H
#define Swing_DistTriangle3Triangle3_H

#include "SEGeometricToolsLIB.h"
#include "SEDistance.h"
#include "SETriangle3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20090119
//----------------------------------------------------------------------------
class SE_GEOMETRICTOOLS_API SEDistTriangle3Triangle3f : public SEDistance<float, 
    SEVector3f>
{
public:
    SEDistTriangle3Triangle3f(const SETriangle3f& rTriangle0,
        const SETriangle3f& rTriangle1);

    // 对象访问.
    const SETriangle3f& GetTriangle0(void) const;
    const SETriangle3f& GetTriangle1(void) const;

    // static distance查询.
    virtual float Get(void);
    virtual float GetSquared(void);

    // 用于dynamic distance查询的convex function计算.
    virtual float Get(float fT, const SEVector3f& rVelocity0,
        const SEVector3f& rVelocity1);
    virtual float GetSquared(float fT, const SEVector3f& rVelocity0,
        const SEVector3f& rVelocity1);

    // 最近点相关信息.
    float GetTriangleBary0(int i) const;
    float GetTriangleBary1(int i) const;

private:
    const SETriangle3f* m_pTriangle0;
    const SETriangle3f* m_pTriangle1;

    // 最近点相关信息.
    float m_afTriangleBary0[3];  // closest0 = sum_{i=0}^2 bary0[i]*vertex0[i]
    float m_afTriangleBary1[3];  // closest1 = sum_{i=0}^2 bary1[i]*vertex1[i]
};

}

#endif
