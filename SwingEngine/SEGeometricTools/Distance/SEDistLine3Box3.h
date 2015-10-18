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

#ifndef Swing_DistLine3Box3_H
#define Swing_DistLine3Box3_H

#include "SEGeometricToolsLIB.h"
#include "SEDistance.h"
#include "SELine3.h"
#include "SEBox3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20090113
//----------------------------------------------------------------------------
class SE_GEOMETRICTOOLS_API SEDistLine3Box3f : public SEDistance<float, 
    SEVector3f>
{
public:
    SEDistLine3Box3f(const SELine3f& rLine, const SEBox3f& rBox);

    // 对象访问.
    const SELine3f& GetLine(void) const;
    const SEBox3f& GetBox(void) const;

    // static distance查询.
    virtual float Get(void);
    virtual float GetSquared(void);

    // 用于dynamic distance查询的convex function计算.
    virtual float Get(float fT, const SEVector3f& rVelocity0,
        const SEVector3f& rVelocity1);
    virtual float GetSquared(float fT, const SEVector3f& rVelocity0,
        const SEVector3f& rVelocity1);

    // 访问最近点处的直线参数.
    // ray-box和segment-box的distance计算也使用这个函数.
    float GetLineParameter(void) const;

private:
    void Face(int i0, int i1, int i2, SEVector3f& rPnt,
        const SEVector3f& rDir, const SEVector3f& rPmE,
        float& rfSqrDistance);

    void CaseNoZeros(SEVector3f& rPnt, const SEVector3f& rDir,
        float& rfSqrDistance);

    void Case0(int i0, int i1, int i2, SEVector3f& rPnt,
        const SEVector3f& rDir, float& rfSqrDistance);

    void Case00(int i0, int i1, int i2, SEVector3f& rPnt,
        const SEVector3f& rDir, float& rfSqrDistance);

    void Case000(SEVector3f& rPnt, float& rfSqrDistance);

    const SELine3f* m_pLine;
    const SEBox3f* m_pBox;
    float m_fLParam;
};

}

#endif
