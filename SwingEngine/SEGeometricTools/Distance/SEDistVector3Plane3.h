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

#ifndef Swing_DistVector3Plane3_H
#define Swing_DistVector3Plane3_H

#include "SEGeometricToolsLIB.h"
#include "SEDistance.h"
#include "SEPlane3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20090116
//----------------------------------------------------------------------------
class SE_GEOMETRICTOOLS_API SEDistVector3Plane3f : public SEDistance<float, 
    SEVector3f>
{
public:
    SEDistVector3Plane3f(const SEVector3f& rVector, const SEPlane3f& rPlane);

    // 对象访问.
    const SEVector3f& GetVector(void) const;
    const SEPlane3f& GetPlane(void) const;

    // static distance查询.
    virtual float Get(void);
    virtual float GetSquared(void);

    // 用于dynamic distance查询的convex function计算.
    virtual float Get(float fT, const SEVector3f& rVelocity0,
        const SEVector3f& rVelocity1);
    virtual float GetSquared(float fT, const SEVector3f& rVelocity0,
        const SEVector3f& rVelocity1);

private:
    const SEVector3f* m_pVector;
    const SEPlane3f* m_pPlane;
};

}

#endif
