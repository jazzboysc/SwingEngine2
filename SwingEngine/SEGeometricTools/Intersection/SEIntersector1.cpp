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
#include "SEIntersector1.h"

namespace Swing
{
//----------------------------------------------------------------------------
template <class Real>
SEIntersector1<Real>::SEIntersector1(Real fU0, Real fU1, Real fV0, Real fV1)
{
    SE_ASSERT( fU0 <= fU1 && fV0 <= fV1 );

    m_afU[0] = fU0;
    m_afU[1] = fU1;
    m_afV[0] = fV0;
    m_afV[1] = fV1;
    m_fFirstTime = (Real)0.0;
    m_fLastTime = (Real)0.0;
    m_iCount = 0;
}
//----------------------------------------------------------------------------
template <class Real>
SEIntersector1<Real>::SEIntersector1(Real afU[2], Real afV[2])
{
    SE_ASSERT( afU[0] <= afU[1] && afV[0] <= afV[1] );

    for( int i = 0; i < 2; i++ )
    {
        m_afU[i] = afU[i];
        m_afV[i] = afV[i];
    }
    m_fFirstTime = (Real)0.0;
    m_fLastTime = (Real)0.0;
    m_iCount = 0;
}
//----------------------------------------------------------------------------
template <class Real>
SEIntersector1<Real>::~SEIntersector1()
{
}
//----------------------------------------------------------------------------
template <class Real>
Real SEIntersector1<Real>::GetU(int i) const
{
    SE_ASSERT( 0 <= i && i < 2 );

    return m_afU[i];
}
//----------------------------------------------------------------------------
template <class Real>
Real SEIntersector1<Real>::GetV(int i) const
{
    SE_ASSERT( 0 <= i && i < 2 );

    return m_afV[i];
}
//----------------------------------------------------------------------------
template <class Real>
bool SEIntersector1<Real>::Test()
{
    return m_afU[0] <= m_afV[1] && m_afU[1] >= m_afV[0];
}
//----------------------------------------------------------------------------
template <class Real>
bool SEIntersector1<Real>::Find()
{
    if( m_afU[1] < m_afV[0] || m_afU[0] > m_afV[1] )
    {
        m_iCount = 0;
    }
    else if( m_afU[1] > m_afV[0] )
    {
        if( m_afU[0] < m_afV[1] )
        {
            m_iCount = 2;
            m_afOverlap[0] = (m_afU[0] < m_afV[0] ? m_afV[0] : m_afU[0]);
            m_afOverlap[1] = (m_afU[1] > m_afV[1] ? m_afV[1] : m_afU[1]);
            if( m_afOverlap[0] == m_afOverlap[1] )
            {
                m_iCount = 1;
            }
        }
        else  // m_afU[0] == m_afV[1]
        {
            m_iCount = 1;
            m_afOverlap[0] = m_afU[0];
        }
    }
    else  // m_afU[1] == m_afV[0]
    {
        m_iCount = 1;
        m_afOverlap[0] = m_afU[1];
    }

    return m_iCount > 0;
}
//----------------------------------------------------------------------------
template <class Real>
bool SEIntersector1<Real>::Test(Real fTMax, Real fSpeedU, Real fSpeedV)
{
    Real fDiffSpeed, fInvDiffSpeed, fDiffPos;

    if( m_afU[1] < m_afV[0] )
    {
        // [u0,u1]初始在[v0,v1]左边.
        fDiffSpeed = fSpeedU - fSpeedV;
        if( fDiffSpeed > (Real)0.0 )
        {
            // 两区间在移动过程中必然交错.
            fDiffPos = m_afV[0] - m_afU[1];
            if( fDiffPos <= fTMax*fDiffSpeed )
            {
                // 两区间在指定时间内相交.
                fInvDiffSpeed = ((Real)1.0)/fDiffSpeed;
                m_fFirstTime = fDiffPos*fInvDiffSpeed;
                m_fLastTime = (m_afV[1] - m_afU[0])*fInvDiffSpeed;

                return true;
            }
        }
    }
    else if( m_afU[0] > m_afV[1] )
    {
        // [u0,u1]初始在[v0,v1]右边.
        fDiffSpeed = fSpeedV - fSpeedU;
        if( fDiffSpeed > (Real)0.0 )
        {
            // 两区间在移动过程中必然交错.
            fDiffPos = m_afU[0] - m_afV[1];
            if( fDiffPos <= fTMax*fDiffSpeed )
            {
                // 两区间在指定时间内相交.
                fInvDiffSpeed = ((Real)1.0)/fDiffSpeed;
                m_fFirstTime = fDiffPos*fInvDiffSpeed;
                m_fLastTime = (m_afU[1] - m_afV[0])*fInvDiffSpeed;

                return true;
            }
        }
    }
    else
    {
        // 两区间初始时相交.
        m_fFirstTime = 0.0f;
        if( fSpeedV > fSpeedU )
        {
            m_fLastTime = (m_afU[1] - m_afV[0])/(fSpeedV - fSpeedU);
        }
        else if( fSpeedV < fSpeedU )
        {
            m_fLastTime = (m_afV[1] - m_afU[0])/(fSpeedU - fSpeedV);
        }
        else
        {
            m_fLastTime = SEMath<Real>::MAX_REAL;
        }

        return true;
    }

    return false;
}
//----------------------------------------------------------------------------
template <class Real>
bool SEIntersector1<Real>::Find(Real fTMax, Real fSpeedU, Real fSpeedV)
{
    Real fDiffSpeed, fInvDiffSpeed, fDiffPos;

    if( m_afU[1] < m_afV[0] )
    {
        // [u0,u1]初始在[v0,v1]左边.
        fDiffSpeed = fSpeedU - fSpeedV;
        if( fDiffSpeed > (Real)0.0 )
        {
            // 两区间在移动过程中必然交错.
            fDiffPos = m_afV[0] - m_afU[1];
            if( fDiffPos <= fTMax*fDiffSpeed )
            {
                // 两区间在指定时间内相交.
                fInvDiffSpeed = ((Real)1.0)/fDiffSpeed;
                m_fFirstTime = fDiffPos*fInvDiffSpeed;
                m_fLastTime = (m_afV[1] - m_afU[0])*fInvDiffSpeed;
                m_iCount = 1;
                m_afOverlap[0] = m_afU[0] + m_fFirstTime*fSpeedU;

                return true;
            }
        }
    }
    else if( m_afU[0] > m_afV[1] )
    {
        // [u0,u1]初始在[v0,v1]右边.
        fDiffSpeed = fSpeedV - fSpeedU;
        if( fDiffSpeed > (Real)0.0 )
        {
            // 两区间在移动过程中必然交错.
            fDiffPos = m_afU[0] - m_afV[1];
            if( fDiffPos <= fTMax*fDiffSpeed )
            {
                // 两区间在指定时间内相交.
                fInvDiffSpeed = ((Real)1.0)/fDiffSpeed;
                m_fFirstTime = fDiffPos*fInvDiffSpeed;
                m_fLastTime = (m_afU[1] - m_afV[0])*fInvDiffSpeed;
                m_iCount = 1;
                m_afOverlap[0] = m_afV[1] + m_fFirstTime*fSpeedV;

                return true;
            }
        }
    }
    else
    {
        // 两区间初始时相交.
        m_fFirstTime = 0.0f;
        if( fSpeedV > fSpeedU )
        {
            m_fLastTime = (m_afU[1] - m_afV[0])/(fSpeedV - fSpeedU);
        }
        else if( fSpeedV < fSpeedU )
        {
            m_fLastTime = (m_afV[1] - m_afU[0])/(fSpeedU - fSpeedV);
        }
        else
        {
            m_fLastTime = SEMath<Real>::MAX_REAL;
        }

        if( m_afU[1] > m_afV[0] )
        {
            if( m_afU[0] < m_afV[1] )
            {
                m_iCount = 2;
                m_afOverlap[0] = (m_afU[0] < m_afV[0] ? m_afV[0] : m_afU[0]);
                m_afOverlap[1] = (m_afU[1] > m_afV[1] ? m_afV[1] : m_afU[1]);
            }
            else  // m_afU[0] == m_afV[1]
            {
                m_iCount = 1;
                m_afOverlap[0] = m_afU[0];
            }
        }
        else  // m_afU[1] == m_afV[0]
        {
            m_iCount = 1;
            m_afOverlap[0] = m_afU[1];
        }

        return true;
    }

    m_iCount = 0;

    return false;
}
//----------------------------------------------------------------------------
template <class Real>
Real SEIntersector1<Real>::GetFirstTime() const
{
    return m_fFirstTime;
}
//----------------------------------------------------------------------------
template <class Real>
Real SEIntersector1<Real>::GetLastTime() const
{
    return m_fLastTime;
}
//----------------------------------------------------------------------------
template <class Real>
int SEIntersector1<Real>::GetCount() const
{
    return m_iCount;
}
//----------------------------------------------------------------------------
template <class Real>
Real SEIntersector1<Real>::GetOverlap(int i) const
{
    SE_ASSERT( 0 <= i && i < m_iCount );

    return m_afOverlap[i];
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// explicit instantiation
//----------------------------------------------------------------------------
template SE_GEOMETRICTOOLS_API
class SEIntersector1<float>;

template SE_GEOMETRICTOOLS_API
class SEIntersector1<double>;
//----------------------------------------------------------------------------
}
