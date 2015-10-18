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

#ifndef Swing_IntrLine3Sphere3_H
#define Swing_IntrLine3Sphere3_H

#include "SEGeometricToolsLIB.h"
#include "SEIntersector.h"
#include "SELine3.h"
#include "SESphere3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20090203
//----------------------------------------------------------------------------
class SE_GEOMETRICTOOLS_API SEIntrLine3Sphere3f : public SEIntersector<float, 
    SEVector3f>
{
public:
    SEIntrLine3Sphere3f(const SELine3f& rLine, const SESphere3f& rSphere);

    // 对象访问.
    const SELine3f& GetLine(void) const;
    const SESphere3f& GetSphere(void) const;

    // test-intersection查询.
    virtual bool Test(void);

    // find-intersection查询.
    virtual bool Find(void);
    int GetCount(void) const;
    const SEVector3f& GetPoint(int i) const;
    float GetLineT(int i) const;

private:
    // 待检查对象.
    const SELine3f* m_pLine;
    const SESphere3f* m_pSphere;

    // 相交集相关信息.
    int m_iCount;
    SEVector3f m_aPoint[2];
    float m_afLineT[2];
};

}

#endif
