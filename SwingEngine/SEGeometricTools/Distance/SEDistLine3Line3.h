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

#ifndef Swing_DistLine3Line3_H
#define Swing_DistLine3Line3_H

#include "SEGeometricToolsLIB.h"
#include "SEDistance.h"
#include "SELine3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20090113
//----------------------------------------------------------------------------
class SE_GEOMETRICTOOLS_API SEDistLine3Line3f : public SEDistance<float, 
    SEVector3f>
{
public:
    SEDistLine3Line3f(const SELine3f& rLine0, const SELine3f& rLine1);

    // 对象访问.
    const SELine3f& GetLine0(void) const;
    const SELine3f& GetLine1(void) const;

    // static distance查询.
    virtual float Get(void);
    virtual float GetSquared(void);

    // 用于dynamic distance查询的convex function计算.
    virtual float Get(float fT, const SEVector3f& rVelocity0,
        const SEVector3f& rVelocity1);
    virtual float GetSquared(float fT, const SEVector3f& rVelocity0,
        const SEVector3f& rVelocity1);

    // 最近点相关信息.
    float GetLine0Parameter(void) const;
    float GetLine1Parameter(void) const;

private:
    const SELine3f* m_pLine0;
    const SELine3f* m_pLine1;

    // 最近点相关信息.
    float m_fLine0Parameter;  // closest0 = line0.origin+param*line0.direction
    float m_fLine1Parameter;  // closest1 = line1.origin+param*line1.direction
};

}

#endif
