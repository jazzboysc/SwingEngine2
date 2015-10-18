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

//----------------------------------------------------------------------------
template <class Real>
SELinComp<Real>::SELinComp()
{
    m_iType = CT_EMPTY;
    m_fMin = SEMath<Real>::MAX_REAL;
    m_fMax = -SEMath<Real>::MAX_REAL;
}
//----------------------------------------------------------------------------
template <class Real>
SELinComp<Real>::~SELinComp()
{
}
//----------------------------------------------------------------------------
template <class Real>
SELinComp<Real>& SELinComp<Real>::operator=(const SELinComp& rComponent)
{
    m_iType = rComponent.m_iType;
    m_fMin = rComponent.m_fMin;
    m_fMax = rComponent.m_fMax;

    return *this;
}
//----------------------------------------------------------------------------
template <class Real>
int SELinComp<Real>::GetType() const
{
    return m_iType;
}
//----------------------------------------------------------------------------
template <class Real>
Real SELinComp<Real>::GetMin() const
{
    return m_fMin;
}
//----------------------------------------------------------------------------
template <class Real>
Real SELinComp<Real>::GetMax() const
{
    return m_fMax;
}
//----------------------------------------------------------------------------
template <class Real>
bool SELinComp<Real>::Contains(Real fParam) const
{
    return m_fMin <= fParam && fParam <= m_fMax;
}
//----------------------------------------------------------------------------
template <class Real>
void SELinComp<Real>::SetInterval(Real fMin, Real fMax)
{
    m_iType = GetTypeFromInterval(fMin, fMax);
    m_fMin = fMin;
    m_fMax = fMax;
}
//----------------------------------------------------------------------------
template <class Real>
int SELinComp<Real>::GetTypeFromInterval(Real fMin, Real fMax)
{
    if( fMin < fMax )
    {
        if( fMax == SEMath<Real>::MAX_REAL )
        {
            if( fMin == -SEMath<Real>::MAX_REAL )
            {
                return CT_LINE;
            }
            else
            {
                return CT_RAY;
            }
        }
        else
        {
            if( fMin == -SEMath<Real>::MAX_REAL )
            {
                return CT_RAY;
            }
            else
            {
                return CT_SEGMENT;
            }
        }
    }
    else if( fMin == fMax )
    {
        if( fMin != -SEMath<Real>::MAX_REAL && fMax != SEMath<Real>::MAX_REAL )
        {
            return CT_POINT;
        }
    }

    return CT_EMPTY;
}
//----------------------------------------------------------------------------
template <class Real>
bool SELinComp<Real>::IsCanonical() const
{
    if( m_iType == CT_RAY )
    {
        return m_fMin == (Real)0.0 && m_fMax == SEMath<Real>::MAX_REAL;
    }

    if( m_iType == CT_SEGMENT )
    {
        return m_fMin == -m_fMax;
    }

    if( m_iType == CT_POINT )
    {
        return m_fMin == (Real)0.0; 
    }

    if( m_iType == CT_EMPTY )
    {
        return m_fMin == SEMath<Real>::MAX_REAL
            && m_fMax == -SEMath<Real>::MAX_REAL;
    }

    // m_iType == CT_LINE
    return true;
}
//----------------------------------------------------------------------------
