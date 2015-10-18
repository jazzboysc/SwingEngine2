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

#ifndef Swing_DistLine3Segment3_H
#define Swing_DistLine3Segment3_H

#include "SEGeometricToolsLIB.h"
#include "SEDistance.h"
#include "SELine3.h"
#include "SESegment3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20090113
//----------------------------------------------------------------------------
class SE_GEOMETRICTOOLS_API SEDistLine3Segment3f : public SEDistance<float, 
    SEVector3f>
{
public:
    SEDistLine3Segment3f(const SELine3f& rLine, const SESegment3f& rSegment);

    // 对象访问.
    const SELine3f& GetLine(void) const;
    const SESegment3f& GetSegment(void) const;

    // static distance查询.
    virtual float Get(void);
    virtual float GetSquared(void);

    // 用于dynamic distance查询的convex function计算.
    virtual float Get(float fT, const SEVector3f& rVelocity0,
        const SEVector3f& rVelocity1);
    virtual float GetSquared(float fT, const SEVector3f& rVelocity0,
        const SEVector3f& rVelocity1);

    // 最近点相关信息.
    float GetLineParameter(void) const;
    float GetSegmentParameter(void) const;

private:
    const SELine3f* m_pLine;
    const SESegment3f* m_pSegment;

    // 最近点相关信息.
    float m_fLineParameter;  // closest0 = line.origin+param*line.direction
    float m_fSegmentParameter;  // closest1 = seg.origin+param*seg.direction
};

}

#endif
