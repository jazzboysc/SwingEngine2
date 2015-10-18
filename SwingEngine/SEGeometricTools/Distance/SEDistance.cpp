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

#include "SEGeometricToolsPCH.h"
#include "SEDistance.h"

namespace Swing
{
//----------------------------------------------------------------------------
template <class Real, class TVector>
SEDistance<Real, TVector>::SEDistance()
{
    MaximumIterations = 8;
    ZeroThreshold = SEMath<Real>::ZERO_TOLERANCE;
    SetDifferenceStep((Real)1e-03);

    m_fContactTime = SEMath<Real>::MAX_REAL;
    m_bHasMultipleClosestPoints0 = false;
    m_bHasMultipleClosestPoints1 = false;
}
//----------------------------------------------------------------------------
template <class Real, class TVector>
SEDistance<Real, TVector>::~SEDistance()
{
}
//----------------------------------------------------------------------------
template <class Real, class TVector>
Real SEDistance<Real, TVector>::GetDifferenceStep() const
{
    return m_fDifferenceStep;
}
//----------------------------------------------------------------------------
template <class Real, class TVector>
Real SEDistance<Real, TVector>::GetContactTime() const
{
    return m_fContactTime;
}
//----------------------------------------------------------------------------
template <class Real, class TVector>
const TVector& SEDistance<Real, TVector>::GetClosestPoint0() const
{
    return m_ClosestPoint0;
}
//----------------------------------------------------------------------------
template <class Real, class TVector>
const TVector& SEDistance<Real, TVector>::GetClosestPoint1() const
{
    return m_ClosestPoint1;
}
//----------------------------------------------------------------------------
template <class Real, class TVector>
bool SEDistance<Real, TVector>::HasMultipleClosestPoints0() const
{
    return m_bHasMultipleClosestPoints0;
}
//----------------------------------------------------------------------------
template <class Real, class TVector>
bool SEDistance<Real, TVector>::HasMultipleClosestPoints1() const
{
    return m_bHasMultipleClosestPoints1;
}
//----------------------------------------------------------------------------
template <class Real, class TVector>
void SEDistance<Real, TVector>::SetDifferenceStep(Real fDifferenceStep)
{
    SE_ASSERT( fDifferenceStep > (Real)0.0 );

    if( fDifferenceStep > (Real)0.0 )
    {
        m_fDifferenceStep = fDifferenceStep;
    }
    else
    {
        m_fDifferenceStep = (Real)1e-03;
    }

    m_fInvTwoDifferenceStep = ((Real)0.5)/m_fDifferenceStep;
}
//----------------------------------------------------------------------------
template <class Real, class TVector>
Real SEDistance<Real, TVector>::GetDerivative(Real fT, 
    const TVector& rVelocity0, const TVector& rVelocity1)
{
    // �����޲�ֹ��㵼��: f'(t) = (f(t + h) - f(t - h))/(2 * h).
    Real fFp = Get(fT + m_fDifferenceStep, rVelocity0, rVelocity1);
    Real fFm = Get(fT - m_fDifferenceStep, rVelocity0, rVelocity1);
    Real fDFApprox = m_fInvTwoDifferenceStep*(fFp - fFm);

    return fDFApprox;
}
//----------------------------------------------------------------------------
template <class Real, class TVector>
Real SEDistance<Real, TVector>::GetDerivativeSquared(Real fT, 
    const TVector& rVelocity0, const TVector& rVelocity1)
{
    // ����������и���ķ����������ƽ���ĵ���,��Ӧ�����������.
    Real fDistance = Get(fT, rVelocity0, rVelocity1);
    Real fDerivative = GetDerivative(fT, rVelocity0, rVelocity1);

    return ((Real)2.0)*fDistance*fDerivative;
}
//----------------------------------------------------------------------------
template <class Real, class TVector>
Real SEDistance<Real, TVector>::Get(Real fTMin, Real fTMax, 
    const TVector& rVelocity0, const TVector& rVelocity1)
{
    // ������ں���f(t)��convex function��ǰ����.
    // ���f'(tmin) >= 0,����Сֵ��tmin��.
    // ���f'(tmax) <= 0,����Сֵ��tmax��.
    // ����,f'(tmin) < 0��f'(tmax) > 0,��Сֵ��(tmin,tmax)������.

    Real fT0 = fTMin;
    Real fF0 = Get(fT0, rVelocity0, rVelocity1);
    if( fF0 <= ZeroThreshold )
    {
        // ���뼸��Ϊ��.
        // �����ʼʱ�̽Ӵ�.
        m_fContactTime = fT0;

        return (Real)0.0;
    }
    Real fDF0 = GetDerivative(fT0, rVelocity0, rVelocity1);
    if( fDF0 >= (Real)0.0 )
    {
        // ������[tmin, tmax]�ϳ�������.
        m_fContactTime = fT0;

        return fF0;
    }

    Real fT1 = fTMax;
    Real fF1 = Get(fT1, rVelocity0, rVelocity1);
    if( fF1 <= ZeroThreshold )
    {
        // ���뼸��Ϊ��.
        m_fContactTime = fT1;

        return (Real)0.0;
    }
    Real fDF1 = GetDerivative(fT1, rVelocity0, rVelocity1);
    if( fDF1 <= (Real)0.0 )
    {
        // ������[tmin, tmax]�ϳ�����С.
        m_fContactTime = fT1;

        return fF1;
    }

    // ��ţ�ٷ����㺯��ֵΪ0ʱ���Ա���tֵ.
    // �ڸù�����,������벻�ܴﵽ0ֵ,���л���һ��numerical minimizer.
    int i;
    for( i = 0; i < MaximumIterations; i++ )
    {
        // ������һ��ţ�ٵ���λ��.
        Real fT = fT0 - fF0/fDF0;
        if( fT >= fTMax )
        {
            // ����ͼ�ε�͹��ȷ���������������ʱ,����������ֵ.
            // �л���һ��numerical minimizer.
            break;
        }

        Real fF = Get(fT, rVelocity0, rVelocity1);
        if( fF <= ZeroThreshold )
        {
            // ���뼸��Ϊ��.
            m_fContactTime = fT;

            return (Real)0.0;
        }

        Real fDF = GetDerivative(fT, rVelocity0, rVelocity1);
        if( fDF >= (Real)0.0 )
        {
            // ����ͼ�ε�͹��ȷ���������������ʱ,����������ֵ.
            // �л���һ��numerical minimizer.
            break;
        }

        fT0 = fT;
        fF0 = fF;
        fDF0 = fDF;
    }

    if( i == MaximumIterations )
    {
        // ��ָ�����������ں���û��������0ֵ����.
        // ��������ʱ,�����ĵ����ض���Ϊ��ֵ,
        // ��˰����һ�ε���ʱ��t�ĺ���ֵ��Ϊ����ֵ����.
        m_fContactTime = fT0;

        return fF0;
    }

    // ����������ֵ.
    // �ö��ַ����ҵ������ĸ�.
    Real fTm = fT0;
    for( i = 0; i < MaximumIterations; i++ )
    {
        fTm = ((Real)0.5)*(fT0 + fT1);
        Real fDFm = GetDerivative(fTm, rVelocity0, rVelocity1);
        Real fProduct = fDFm*fDF0;
        if( fProduct < -ZeroThreshold )
        {
            fT1 = fTm;
            fDF1 = fDFm;
        }
        else if( fProduct > ZeroThreshold )
        {
            fT0 = fTm;
            fDF0 = fDFm;
        }
        else
        {
            break;
        }
    }

    // ���Ǻ���������Сֵ��tֵ,������contact time.
    // �洢��ֵ����debugging.
    m_fContactTime = fTm;
    Real fFm = Get(fTm, rVelocity0, rVelocity1);

    return fFm;
}
//----------------------------------------------------------------------------
template <class Real, class TVector>
Real SEDistance<Real, TVector>::GetSquared(Real fTMin, Real fTMax, 
    const TVector& rVelocity0, const TVector& rVelocity1)
{
    // ������ں���f(t)��convex function��ǰ����.
    // ���f'(tmin) >= 0,����Сֵ��tmin��.
    // ���f'(tmax) <= 0,����Сֵ��tmax��.
    // ����,f'(tmin) < 0��f'(tmax) > 0,��Сֵ��(tmin,tmax)������.

    Real fT0 = fTMin;
    Real fF0 = GetSquared(fT0, rVelocity0, rVelocity1);
    if( fF0 <= ZeroThreshold )
    {
        // ���뼸��Ϊ��.
        // �����ʼʱ�̽Ӵ�.
        m_fContactTime = fT0;

        return (Real)0.0;
    }
    Real fDF0 = GetDerivativeSquared(fT0, rVelocity0, rVelocity1);
    if( fDF0 >= (Real)0.0 )
    {
        // ������[tmin, tmax]�ϳ�������.
        m_fContactTime = fT0;

        return fF0;
    }

    Real fT1 = fTMax;
    Real fF1 = GetSquared(fT1, rVelocity0, rVelocity1);
    if( fF1 <= ZeroThreshold )
    {
        // ���뼸��Ϊ��.
        m_fContactTime = fT1;

        return (Real)0.0;
    }
    Real fDF1 = GetDerivativeSquared(fT1, rVelocity0, rVelocity1);
    if( fDF1 <= (Real)0.0 )
    {
        // ������[tmin, tmax]�ϳ�����С.
        m_fContactTime = fT1;

        return fF1;
    }

    // ��ţ�ٷ����㺯��ֵΪ0ʱ���Ա���tֵ.
    // �ڸù�����,������벻�ܴﵽ0ֵ,���л���һ��numerical minimizer.
    int i;
    for( i = 0; i < MaximumIterations; i++ )
    {
        // ������һ��ţ�ٵ���λ��.
        Real fT = fT0 - fF0/fDF0;
        if( fT >= fTMax )
        {
            // ����ͼ�ε�͹��ȷ���������������ʱ,����������ֵ.
            // �л���һ��numerical minimizer.
            break;
        }

        Real fF = GetSquared(fT, rVelocity0, rVelocity1);
        if( fF <= ZeroThreshold )
        {
            // ���뼸��Ϊ��.
            m_fContactTime = fT;

            return (Real)0.0;
        }

        Real fDF = GetDerivativeSquared(fT, rVelocity0, rVelocity1);
        if( fDF >= (Real)0.0 )
        {
            // ����ͼ�ε�͹��ȷ���������������ʱ,����������ֵ.
            // �л���һ��numerical minimizer.
            break;
        }

        fT0 = fT;
        fF0 = fF;
        fDF0 = fDF;
    }

    if( i == MaximumIterations )
    {
        // ��ָ�����������ں���û��������0ֵ����.
        // ��������ʱ,�����ĵ����ض���Ϊ��ֵ,
        // ��˰����һ�ε���ʱ��t�ĺ���ֵ��Ϊ����ֵ����.
        m_fContactTime = fT0;

        return fF0;
    }

    // ����������ֵ.
    // �ö��ַ����ҵ������ĸ�.
    Real fTm = fT0;
    for( i = 0; i < MaximumIterations; i++ )
    {
        fTm = ((Real)0.5)*(fT0 + fT1);
        Real fDFm = GetDerivativeSquared(fTm, rVelocity0, rVelocity1);
        Real fProduct = fDFm*fDF0;
        if( fProduct < -ZeroThreshold )
        {
            fT1 = fTm;
            fDF1 = fDFm;
        }
        else if( fProduct > ZeroThreshold )
        {
            fT0 = fTm;
            fDF0 = fDFm;
        }
        else
        {
            break;
        }
    }

    // ���Ǻ���������Сֵ��tֵ,������contact time.
    // �洢��ֵ����debugging.
    m_fContactTime = fTm;
    Real fFm = GetSquared(fTm, rVelocity0, rVelocity1);

    return fFm;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// explicit instantiation
//----------------------------------------------------------------------------
template class SEDistance<float, SEVector2f>;
template class SEDistance<float, SEVector3f>;
template class SEDistance<double, SEVector2d>;
template class SEDistance<double, SEVector3d>;
//----------------------------------------------------------------------------
}
