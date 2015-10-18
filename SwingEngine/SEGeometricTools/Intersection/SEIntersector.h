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

#ifndef Swing_Intersector_H
#define Swing_Intersector_H

#include "SEGeometricToolsLIB.h"
#include "SEPlatforms.h"
#include "SELinComp.h"
#include "SEVector2.h"
#include "SEVector3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20081219
//----------------------------------------------------------------------------
template <class Real, class TVector>
class SE_GEOMETRICTOOLS_API SEIntersector
{
public:
    // Abstract base class.
    virtual ~SEIntersector(void);

    // ��̬�ཻ��ѯ.Ĭ��ʵ�ַ���false.
    // Find��ѯ����һ���ཻ��.�������������ṩ���ʸü��ϵĹ���,
    // ��Ϊ�ü��ϵ������ɾ���Ķ�������������.
    virtual bool Test(void);
    virtual bool Find(void);

    // ��̬�ཻ��ѯ.Ĭ��ʵ�ַ���false.
    // Find��ѯ����һ��first contact set.�������������ṩ���ʸü��ϵĹ���,
    // ��Ϊ�ü��ϵ������ɾ���Ķ�������������.
    virtual bool Test(Real fTMax, const TVector& rVelocity0,
        const TVector& rVelocity1);
    virtual bool Find(Real fTMax, const TVector& rVelocity0,
        const TVector& rVelocity1);

    // ��̬�ཻ��ѯʱ,���������һ�νӴ���ʱ��.
    Real GetContactTime(void) const;

    // �ཻ������Ϣ.
    enum
    {
        IT_EMPTY = SELinComp<Real>::CT_EMPTY,
        IT_POINT = SELinComp<Real>::CT_POINT,
        IT_SEGMENT = SELinComp<Real>::CT_SEGMENT,
        IT_RAY = SELinComp<Real>::CT_RAY,
        IT_LINE = SELinComp<Real>::CT_LINE,
        IT_POLYGON,
        IT_PLANE,
        IT_POLYHEDRON,
        IT_OTHER
    };
    int GetIntersectionType(void) const;

protected:
    SEIntersector(void);

    Real m_fContactTime;
    int m_iIntersectionType;
};

typedef SEIntersector<float, SEVector2f> SEIntersector2f;
typedef SEIntersector<float, SEVector3f> SEIntersector3f;
typedef SEIntersector<double, SEVector2d> SEIntersector2d;
typedef SEIntersector<double, SEVector3d> SEIntersector3d;

}

#endif
