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

#ifndef Swing_DistVector2Ray2_H
#define Swing_DistVector2Ray2_H

#include "SEGeometricToolsLIB.h"
#include "SEDistance.h"
#include "SERay2.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20090115
//----------------------------------------------------------------------------
class SE_GEOMETRICTOOLS_API SEDistVector2Ray2f : public SEDistance<float, 
    SEVector2f>
{
public:
    SEDistVector2Ray2f(const SEVector2f& rVector, const SERay2f& rRay);

    // 对象访问.
    const SEVector2f& GetVector(void) const;
    const SERay2f& GetRay(void) const;

    // static distance查询.
    virtual float Get(void);
    virtual float GetSquared(void);

    // 用于dynamic distance查询的convex function计算.
    virtual float Get(float fT, const SEVector2f& rVelocity0,
        const SEVector2f& rVelocity1);
    virtual float GetSquared(float fT, const SEVector2f& rVelocity0,
        const SEVector2f& rVelocity1);

private:
    const SEVector2f* m_pVector;
    const SERay2f* m_pRay;
};

}

#endif
