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

#ifndef Swing_Intersector1_H
#define Swing_Intersector1_H

#include "SEGeometricToolsLIB.h"
#include "SEMath.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20081223
//----------------------------------------------------------------------------
template <class Real>
class SE_GEOMETRICTOOLS_API SEIntersector1
{
public:
    // 用于1维区间[u0,u1]和[v0,v1]的intersection类.
    // 端点顺序必须是: u0 <= u1 且 v0 <= v1.值允许是MAX_REAL和-MAX_REAL,
    // 同时也允许退化的区间: u0 = u1 或 v0 = v1.
    SEIntersector1(Real fU0, Real fU1, Real fV0, Real fV1);
    SEIntersector1(Real afU[2], Real afV[2]);
    virtual ~SEIntersector1(void);

    // 区间访问.
    Real GetU(int i) const;
    Real GetV(int i) const;

    // static intersection查询.
    virtual bool Test(void);
    virtual bool Find(void);

    // dynamic intersection查询.Find查询产生first contact set.
    virtual bool Test(Real fTMax, Real fSpeedU, Real fSpeedV);
    virtual bool Find(Real fTMax, Real fSpeedU, Real fSpeedV);

    // dynamic intersection查询产生的first/last contact time..
    Real GetFirstTime(void) const;
    Real GetLastTime(void) const;

    // 相交集相关信息.
    int GetCount(void) const;
    Real GetOverlap(int i) const;

protected:
    // 待查询区间.
    Real m_afU[2], m_afV[2];

    // 相交集相关信息.
    Real m_fFirstTime, m_fLastTime;
    int m_iCount;
    Real m_afOverlap[2];
};

typedef SEIntersector1<float> SEIntersector1f;
typedef SEIntersector1<double> SEIntersector1d;

}

#endif
