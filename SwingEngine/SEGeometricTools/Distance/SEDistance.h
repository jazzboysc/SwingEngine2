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

#ifndef Swing_Distance_H
#define Swing_Distance_H

#include "SEGeometricToolsLIB.h"
#include "SEVector2.h"
#include "SEVector3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20090104
//----------------------------------------------------------------------------
template <class Real, class TVector>
class SE_GEOMETRICTOOLS_API SEDistance
{
public:
    // Abstract base class.
    virtual ~SEDistance(void);

    // static distance��ѯ.
    virtual Real Get(void) = 0;     // distance
    virtual Real GetSquared(void) = 0;  // squared distance

    // ����dynamic distance��ѯ��convex function����.
    virtual Real Get(Real fT, const TVector& rVelocity0,
        const TVector& rVelocity1) = 0;
    virtual Real GetSquared(Real fT, const TVector& rVelocity0,
        const TVector& rVelocity1) = 0;

    // ����dynamic distance��ѯ�ĵ�������.
    // Ĭ��ʹ�����޲�ֹ��㵼��.
    // f'(t) = (f(t+h)-f(t-h))/(2*h).
    // ����h = DifferenceStep.  ���������������Щ����,
    // �Ӷ�ʵ��һ������Ҫh�ľ�ȷ�������㷽��.
    virtual Real GetDerivative(Real fT, const TVector& rVelocity0,
        const TVector& rVelocity1);
    // g(t) = f(t)^2, g'(t) = 2*f(t)*f'(t).
    virtual Real GetDerivativeSquared(Real fT, const TVector& rVelocity0,
        const TVector& rVelocity1);

    // Dynamic distance��ѯ.
    // ����������ڸ���ʱ������[tmin,tmax]��,����������С����.
    virtual Real Get(Real fTMin, Real fTMax, const TVector& rVelocity0,
        const TVector& rVelocity1);
    virtual Real GetSquared(Real fTMin, Real fTMax,
        const TVector& rVelocity0, const TVector& rVelocity1);

    // ����Newton's method��inverse parabolic interpolation.
    int MaximumIterations;  // default = 8
    Real ZeroThreshold;     // default = SEMath<Real>::ZERO_TOLERANCE

    // ���ڵ�������.
    void SetDifferenceStep(Real fDifferenceStep);  // default = 1e-03
    Real GetDifferenceStep(void) const;

    // dynamic distance��ѯʱ,�ﵽ��С�����ʱ��.
    Real GetContactTime(void) const;

    // �������ϵ������.����static��dynmaic��ѯ����Ч.
    // һ�������ϵ�����������һ������,��ʱ,������Ա��������Ϊtrue.
    // ���������ʵ��֧����������㼯�ϵĲ�ѯ.
    const TVector& GetClosestPoint0(void) const;
    const TVector& GetClosestPoint1(void) const;
    bool HasMultipleClosestPoints0(void) const;
    bool HasMultipleClosestPoints1(void) const;

protected:
    SEDistance(void);

    Real m_fContactTime;
    TVector m_ClosestPoint0;
    TVector m_ClosestPoint1;
    bool m_bHasMultipleClosestPoints0;
    bool m_bHasMultipleClosestPoints1;
    Real m_fDifferenceStep, m_fInvTwoDifferenceStep;
};

typedef SEDistance<float, SEVector2f> SEDistance2f;
typedef SEDistance<float, SEVector3f> SEDistance3f;
typedef SEDistance<double, SEVector2d> SEDistance2d;
typedef SEDistance<double, SEVector3d> SEDistance3d;

}

#endif
