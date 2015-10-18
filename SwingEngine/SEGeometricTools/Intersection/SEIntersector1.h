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
    // ����1ά����[u0,u1]��[v0,v1]��intersection��.
    // �˵�˳�������: u0 <= u1 �� v0 <= v1.ֵ������MAX_REAL��-MAX_REAL,
    // ͬʱҲ�����˻�������: u0 = u1 �� v0 = v1.
    SEIntersector1(Real fU0, Real fU1, Real fV0, Real fV1);
    SEIntersector1(Real afU[2], Real afV[2]);
    virtual ~SEIntersector1(void);

    // �������.
    Real GetU(int i) const;
    Real GetV(int i) const;

    // static intersection��ѯ.
    virtual bool Test(void);
    virtual bool Find(void);

    // dynamic intersection��ѯ.Find��ѯ����first contact set.
    virtual bool Test(Real fTMax, Real fSpeedU, Real fSpeedV);
    virtual bool Find(Real fTMax, Real fSpeedU, Real fSpeedV);

    // dynamic intersection��ѯ������first/last contact time..
    Real GetFirstTime(void) const;
    Real GetLastTime(void) const;

    // �ཻ�������Ϣ.
    int GetCount(void) const;
    Real GetOverlap(int i) const;

protected:
    // ����ѯ����.
    Real m_afU[2], m_afV[2];

    // �ཻ�������Ϣ.
    Real m_fFirstTime, m_fLastTime;
    int m_iCount;
    Real m_afOverlap[2];
};

typedef SEIntersector1<float> SEIntersector1f;
typedef SEIntersector1<double> SEIntersector1d;

}

#endif
