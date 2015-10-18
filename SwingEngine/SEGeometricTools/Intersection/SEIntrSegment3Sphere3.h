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

#ifndef Swing_IntrSegment3Sphere3_H
#define Swing_IntrSegment3Sphere3_H

#include "SEGeometricToolsLIB.h"
#include "SEIntersector.h"
#include "SESegment3.h"
#include "SESphere3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20081220
//----------------------------------------------------------------------------
class SE_GEOMETRICTOOLS_API SEIntrSegment3Sphere3f : public SEIntersector<float, 
    SEVector3f>
{
public:
    SEIntrSegment3Sphere3f(const SESegment3f& rSegment, const SESphere3f& 
        rSphere);

    // 对象访问.
    const SESegment3f& GetSegment(void) const;
    const SESphere3f& GetSphere(void) const;

    // test-intersection查询.
    virtual bool Test(void);

    // find-intersection查询.
    virtual bool Find(void);
    int GetCount(void) const;
    const SEVector3f& GetPoint(int i) const;
    float GetSegmentT(int i) const;

    // dynamic test-intersection查询.
    virtual bool Test(float fTMax, const SEVector3f& rVelocity0,
        const SEVector3f& rVelocity1);

    // dynamic find-intersection查询.
    virtual bool Find(float fTMax, const SEVector3f& rVelocity0,
        const SEVector3f& rVelocity1);

    float ZeroThreshold;  // default = SEMath<float>::ZERO_TOLERANCE

private:
    // 待检查对象.
    const SESegment3f* m_pSegment;
    const SESphere3f* m_pSphere;

    // 相交集相关信息.
    int m_iCount;
    SEVector3f m_aPoint[2];
    float m_afSegmentT[2];
};

}

#endif
