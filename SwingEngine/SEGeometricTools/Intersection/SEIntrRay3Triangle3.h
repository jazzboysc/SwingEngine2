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

#ifndef Swing_IntrRay3Triangle3_H
#define Swing_IntrRay3Triangle3_H

#include "SEGeometricToolsLIB.h"
#include "SEPlatforms.h"
#include "SEIntersector.h"
#include "SERay3.h"
#include "SETriangle3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20081220
//----------------------------------------------------------------------------
class SE_GEOMETRICTOOLS_API SEIntrRay3Triangle3f : public SEIntersector<float, 
    SEVector3f>
{
public:
    SEIntrRay3Triangle3f(const SERay3f& rRay, const SETriangle3f& rTriangle);

    // 对象访问.
    const SERay3f& GetRay(void) const;
    const SETriangle3f& GetTriangle(void) const;

    // test-intersection查询.
    virtual bool Test(void);

    // Find-intersection查询.相交点为
    //   P = origin + t*direction = b0*V0 + b1*V1 + b2*V2
    virtual bool Find(void);
    float GetRayT(void) const;
    float GetTriB0(void) const;
    float GetTriB1(void) const;
    float GetTriB2(void) const;

private:
    // 待检查是否相交的对象.
    const SERay3f* m_pRay;
    const SETriangle3f* m_pTriangle;

    // 相交集相关信息.
    float m_fRayT, m_fTriB0, m_fTriB1, m_fTriB2;
};

}

#endif
