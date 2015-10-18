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

    // static distance查询.
    virtual Real Get(void) = 0;     // distance
    virtual Real GetSquared(void) = 0;  // squared distance

    // 用于dynamic distance查询的convex function计算.
    virtual Real Get(Real fT, const TVector& rVelocity0,
        const TVector& rVelocity1) = 0;
    virtual Real GetSquared(Real fT, const TVector& rVelocity0,
        const TVector& rVelocity1) = 0;

    // 用于dynamic distance查询的导数计算.
    // 默认使用有限差分估算导数.
    // f'(t) = (f(t+h)-f(t-h))/(2*h).
    // 其中h = DifferenceStep.  派生类可以重载这些函数,
    // 从而实现一个不需要h的精确导数计算方法.
    virtual Real GetDerivative(Real fT, const TVector& rVelocity0,
        const TVector& rVelocity1);
    // g(t) = f(t)^2, g'(t) = 2*f(t)*f'(t).
    virtual Real GetDerivativeSquared(Real fT, const TVector& rVelocity0,
        const TVector& rVelocity1);

    // Dynamic distance查询.
    // 函数计算出在给定时间区间[tmin,tmax]上,两对象间的最小距离.
    virtual Real Get(Real fTMin, Real fTMax, const TVector& rVelocity0,
        const TVector& rVelocity1);
    virtual Real GetSquared(Real fTMin, Real fTMax,
        const TVector& rVelocity0, const TVector& rVelocity1);

    // 用于Newton's method和inverse parabolic interpolation.
    int MaximumIterations;  // default = 8
    Real ZeroThreshold;     // default = SEMath<Real>::ZERO_TOLERANCE

    // 用于导数估算.
    void SetDifferenceStep(Real fDifferenceStep);  // default = 1e-03
    Real GetDifferenceStep(void) const;

    // dynamic distance查询时,达到最小距离的时刻.
    Real GetContactTime(void) const;

    // 两对象上的最近点.对于static或dynmaic查询都有效.
    // 一个对象上的最近点可以是一个集合,此时,布尔成员函数返回为true.
    // 派生类可以实现支持完整最近点集合的查询.
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
