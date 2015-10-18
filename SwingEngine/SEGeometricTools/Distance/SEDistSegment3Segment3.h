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

#ifndef Swing_DistSegment3Segment3_H
#define Swing_DistSegment3Segment3_H

#include "SEGeometricToolsLIB.h"
#include "SEDistance.h"
#include "SESegment3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20090119
//----------------------------------------------------------------------------
class SE_GEOMETRICTOOLS_API SEDistSegment3Segment3f : public SEDistance<float, 
    SEVector3f>
{
public:
    SEDistSegment3Segment3f(const SESegment3f& rSegment0,
        const SESegment3f& rSegment1);

    // 对象访问.
    const SESegment3f& GetSegment0(void) const;
    const SESegment3f& GetSegment1(void) const;

    // static distance查询.
    virtual float Get(void);
    virtual float GetSquared(void);

    // 用于dynamic distance查询的convex function计算.
    virtual float Get(float fT, const SEVector3f& rVelocity0,
        const SEVector3f& rVelocity1);
    virtual float GetSquared(float fT, const SEVector3f& rVelocity0,
        const SEVector3f& rVelocity1);

    // 最近点相关信息.
    float GetSegment0Parameter(void) const;
    float GetSegment1Parameter(void) const;

private:
    const SESegment3f* m_pSegment0;
    const SESegment3f* m_pSegment1;

    // 最近点相关信息.
    float m_fSegment0Parameter;  // closest0 = seg0.origin+param*seg0.direction
    float m_fSegment1Parameter;  // closest1 = seg1.origin+param*seg1.direction
};

}

#endif
