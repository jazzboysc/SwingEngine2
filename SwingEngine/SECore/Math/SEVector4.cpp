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

#include "SECorePCH.h"
#include "SEVector4.h"

using namespace Swing;

//----------------------------------------------------------------------------
// 单精度4维向量类
//----------------------------------------------------------------------------

const SEVector4f SEVector4f::ZERO(0.0f, 0.0f, 0.0f, 0.0f);
const SEVector4f SEVector4f::UNIT_X(1.0f, 0.0f, 0.0f, 0.0f);
const SEVector4f SEVector4f::UNIT_Y(0.0f, 1.0f, 0.0f, 0.0f);
const SEVector4f SEVector4f::UNIT_Z(0.0f, 0.0f, 1.0f, 0.0f);
const SEVector4f SEVector4f::UNIT_W(0.0f, 0.0f, 0.0f, 1.0f);

//----------------------------------------------------------------------------
SEVector4f::SEVector4f()
{
    m_fData[0] = 0.0f;
    m_fData[1] = 0.0f;
    m_fData[2] = 0.0f;
    m_fData[3] = 0.0f;
}
//----------------------------------------------------------------------------
SEVector4f::SEVector4f(float fX, float fY, float fZ, float fW)
{
    m_fData[0] = fX;
    m_fData[1] = fY;
    m_fData[2] = fZ;
    m_fData[3] = fW;
}
//----------------------------------------------------------------------------
SEVector4f::SEVector4f(const SEVector4f& rVec)
{
    m_fData[0] = rVec.m_fData[0];
    m_fData[1] = rVec.m_fData[1];
    m_fData[2] = rVec.m_fData[2];
    m_fData[3] = rVec.m_fData[3];
}
//----------------------------------------------------------------------------
SEVector4f::operator const float*() const
{
    return m_fData;
}
//----------------------------------------------------------------------------
SEVector4f::operator float*()
{
    return m_fData;
}
//----------------------------------------------------------------------------
float SEVector4f::operator[](int i) const
{
    SE_ASSERT( 0 <= i && i <= 3 );
    return m_fData[i];
}
//----------------------------------------------------------------------------
float& SEVector4f::operator[](int i)
{
    SE_ASSERT( 0 <= i && i <= 3 );
    return m_fData[i];
}
//----------------------------------------------------------------------------
SEVector4f& SEVector4f::operator=(const SEVector4f& rVec)
{
    m_fData[0] = rVec.m_fData[0];
    m_fData[1] = rVec.m_fData[1];
    m_fData[2] = rVec.m_fData[2];
    m_fData[3] = rVec.m_fData[3];
    return *this;
}
//----------------------------------------------------------------------------
bool SEVector4f::operator==(const SEVector4f& rVec) const
{
    return CompareData(rVec) == 0;
}
//----------------------------------------------------------------------------
bool SEVector4f::operator!=(const SEVector4f& rVec) const
{
    return CompareData(rVec) != 0;
}
//----------------------------------------------------------------------------
bool SEVector4f::operator<(const SEVector4f& rVec) const
{
    return CompareData(rVec) < 0;
}
//----------------------------------------------------------------------------
bool SEVector4f::operator<=(const SEVector4f& rVec) const
{
    return CompareData(rVec) <= 0;
}
//----------------------------------------------------------------------------
bool SEVector4f::operator>(const SEVector4f& rVec) const
{
    return CompareData(rVec) > 0;
}
//----------------------------------------------------------------------------
bool SEVector4f::operator>=(const SEVector4f& rVec) const
{
    return CompareData(rVec) >= 0;
}
//----------------------------------------------------------------------------
SEVector4f SEVector4f::operator+(const SEVector4f& rRhsVec) const
{
    return SEVector4f(
                   m_fData[0] + rRhsVec.m_fData[0],
                   m_fData[1] + rRhsVec.m_fData[1],
                   m_fData[2] + rRhsVec.m_fData[2],
                   m_fData[3] + rRhsVec.m_fData[3]);
}
//----------------------------------------------------------------------------
SEVector4f SEVector4f::operator-(const SEVector4f& rRhsVec) const
{
    return SEVector4f(
                   m_fData[0] - rRhsVec.m_fData[0],
                   m_fData[1] - rRhsVec.m_fData[1],
                   m_fData[2] - rRhsVec.m_fData[2],
                   m_fData[3] - rRhsVec.m_fData[3]);
}
//----------------------------------------------------------------------------
SEVector4f SEVector4f::operator*(float fScalar) const
{
    return SEVector4f(
                   fScalar * m_fData[0],
                   fScalar * m_fData[1],
                   fScalar * m_fData[2],
                   fScalar * m_fData[3]);
}
//----------------------------------------------------------------------------
SEVector4f SEVector4f::operator/(float fScalar) const
{
    SEVector4f Res;

    if( fScalar != 0.0f )
    {
        float fInvScalar = 1.0f / fScalar;
        Res.m_fData[0] = fInvScalar * m_fData[0];
        Res.m_fData[1] = fInvScalar * m_fData[1];
        Res.m_fData[2] = fInvScalar * m_fData[2];
        Res.m_fData[3] = fInvScalar * m_fData[3];
    }
    else
    {
        Res.m_fData[0] = SEMath<float>::MAX_REAL;
        Res.m_fData[1] = SEMath<float>::MAX_REAL;
        Res.m_fData[2] = SEMath<float>::MAX_REAL;
        Res.m_fData[3] = SEMath<float>::MAX_REAL;
    }

    return Res;
}
//----------------------------------------------------------------------------
SEVector4f SEVector4f::operator-() const
{
    return SEVector4f(
                   -m_fData[0],
                   -m_fData[1],
                   -m_fData[2],
                   -m_fData[3]);
}
//----------------------------------------------------------------------------
SEVector4f Swing::operator*(float fLhsScalar, const SEVector4f& rRhsVec)
{
    return SEVector4f(
                   fLhsScalar * rRhsVec[0],
                   fLhsScalar * rRhsVec[1],
                   fLhsScalar * rRhsVec[2],
                   fLhsScalar * rRhsVec[3]);
}
//----------------------------------------------------------------------------
SEVector4f& SEVector4f::operator+=(const SEVector4f& rRhsVec)
{
    m_fData[0] += rRhsVec.m_fData[0];
    m_fData[1] += rRhsVec.m_fData[1];
    m_fData[2] += rRhsVec.m_fData[2];
    m_fData[3] += rRhsVec.m_fData[3];

    return *this;
}
//----------------------------------------------------------------------------
SEVector4f& SEVector4f::operator+=(float fScalar)
{
    m_fData[0] += fScalar;
    m_fData[1] += fScalar;
    m_fData[2] += fScalar;
    m_fData[3] += fScalar;

    return *this;
}
//----------------------------------------------------------------------------
SEVector4f& SEVector4f::operator-=(const SEVector4f& rRhsVec)
{
    m_fData[0] -= rRhsVec.m_fData[0];
    m_fData[1] -= rRhsVec.m_fData[1];
    m_fData[2] -= rRhsVec.m_fData[2];
    m_fData[3] -= rRhsVec.m_fData[3];

    return *this;
}
//----------------------------------------------------------------------------
SEVector4f& SEVector4f::operator-=(float fScalar)
{
    m_fData[0] -= fScalar;
    m_fData[1] -= fScalar;
    m_fData[2] -= fScalar;
    m_fData[3] -= fScalar;

    return *this;
}
//----------------------------------------------------------------------------
SEVector4f& SEVector4f::operator*=(float fScalar)
{
    m_fData[0] *= fScalar;
    m_fData[1] *= fScalar;
    m_fData[2] *= fScalar;
    m_fData[3] *= fScalar;

    return *this;
}
//----------------------------------------------------------------------------
SEVector4f& SEVector4f::operator/=(float fScalar)
{
    if( fScalar != 0.0f )
    {
        float fInvScalar = 1.0f / fScalar;
        m_fData[0] *= fInvScalar;
        m_fData[1] *= fInvScalar;
        m_fData[2] *= fInvScalar;
        m_fData[3] *= fInvScalar;
    }
    else
    {
        m_fData[0] = SEMath<float>::MAX_REAL;
        m_fData[1] = SEMath<float>::MAX_REAL;
        m_fData[2] = SEMath<float>::MAX_REAL;
        m_fData[3] = SEMath<float>::MAX_REAL;
    }

    return *this;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 双精度4维向量类
//----------------------------------------------------------------------------

const SEVector4d SEVector4d::ZERO(0.0, 0.0, 0.0, 0.0);
const SEVector4d SEVector4d::UNIT_X(1.0, 0.0, 0.0, 0.0);
const SEVector4d SEVector4d::UNIT_Y(0.0, 1.0, 0.0, 0.0);
const SEVector4d SEVector4d::UNIT_Z(0.0, 0.0, 1.0, 0.0);
const SEVector4d SEVector4d::UNIT_W(0.0, 0.0, 0.0, 1.0);

//----------------------------------------------------------------------------
SEVector4d::SEVector4d()
{
    m_dData[0] = 0.0;
    m_dData[1] = 0.0;
    m_dData[2] = 0.0;
    m_dData[3] = 0.0;
}
//----------------------------------------------------------------------------
SEVector4d::SEVector4d(double dX, double dY, double dZ, double dW)
{
    m_dData[0] = dX;
    m_dData[1] = dY;
    m_dData[2] = dZ;
    m_dData[3] = dW;
}
//----------------------------------------------------------------------------
SEVector4d::SEVector4d(const SEVector4d& rVec)
{
    m_dData[0] = rVec.m_dData[0];
    m_dData[1] = rVec.m_dData[1];
    m_dData[2] = rVec.m_dData[2];
    m_dData[3] = rVec.m_dData[3];
}
//----------------------------------------------------------------------------
SEVector4d::operator const double*() const
{
    return m_dData;
}
//----------------------------------------------------------------------------
SEVector4d::operator double*()
{
    return m_dData;
}
//----------------------------------------------------------------------------
double SEVector4d::operator[](int i) const
{
    SE_ASSERT( 0 <= i && i <= 3 );
    return m_dData[i];
}
//----------------------------------------------------------------------------
double& SEVector4d::operator[](int i)
{
    SE_ASSERT( 0 <= i && i <= 3 );
    return m_dData[i];
}
//----------------------------------------------------------------------------
SEVector4d& SEVector4d::operator=(const SEVector4d& rVec)
{
    m_dData[0] = rVec.m_dData[0];
    m_dData[1] = rVec.m_dData[1];
    m_dData[2] = rVec.m_dData[2];
    m_dData[3] = rVec.m_dData[3];
    return *this;
}
//----------------------------------------------------------------------------
bool SEVector4d::operator==(const SEVector4d& rVec) const
{
    return CompareData(rVec) == 0;
}
//----------------------------------------------------------------------------
bool SEVector4d::operator!=(const SEVector4d& rVec) const
{
    return CompareData(rVec) != 0;
}
//----------------------------------------------------------------------------
bool SEVector4d::operator<(const SEVector4d& rVec) const
{
    return CompareData(rVec) < 0;
}
//----------------------------------------------------------------------------
bool SEVector4d::operator<=(const SEVector4d& rVec) const
{
    return CompareData(rVec) <= 0;
}
//----------------------------------------------------------------------------
bool SEVector4d::operator>(const SEVector4d& rVec) const
{
    return CompareData(rVec) > 0;
}
//----------------------------------------------------------------------------
bool SEVector4d::operator>=(const SEVector4d& rVec) const
{
    return CompareData(rVec) >= 0;
}
//----------------------------------------------------------------------------
SEVector4d SEVector4d::operator+(const SEVector4d& rRhsVec) const
{
    return SEVector4d(
                   m_dData[0] + rRhsVec.m_dData[0],
                   m_dData[1] + rRhsVec.m_dData[1],
                   m_dData[2] + rRhsVec.m_dData[2],
                   m_dData[3] + rRhsVec.m_dData[3]);
}
//----------------------------------------------------------------------------
SEVector4d SEVector4d::operator-(const SEVector4d& rRhsVec) const
{
    return SEVector4d(
                   m_dData[0] - rRhsVec.m_dData[0],
                   m_dData[1] - rRhsVec.m_dData[1],
                   m_dData[2] - rRhsVec.m_dData[2],
                   m_dData[3] - rRhsVec.m_dData[3]);
}
//----------------------------------------------------------------------------
SEVector4d SEVector4d::operator*(double dScalar) const
{
    return SEVector4d(
                   dScalar * m_dData[0],
                   dScalar * m_dData[1],
                   dScalar * m_dData[2],
                   dScalar * m_dData[3]);
}
//----------------------------------------------------------------------------
SEVector4d SEVector4d::operator/(double dScalar) const
{
    SEVector4d Res;

    if( dScalar != 0.0 )
    {
        double dInvScalar = 1.0 / dScalar;
        Res.m_dData[0] = dInvScalar * m_dData[0];
        Res.m_dData[1] = dInvScalar * m_dData[1];
        Res.m_dData[2] = dInvScalar * m_dData[2];
        Res.m_dData[3] = dInvScalar * m_dData[3];
    }
    else
    {
        Res.m_dData[0] = SEMath<double>::MAX_REAL;
        Res.m_dData[1] = SEMath<double>::MAX_REAL;
        Res.m_dData[2] = SEMath<double>::MAX_REAL;
        Res.m_dData[3] = SEMath<double>::MAX_REAL;
    }

    return Res;
}
//----------------------------------------------------------------------------
SEVector4d SEVector4d::operator-() const
{
    return SEVector4d(
                   -m_dData[0],
                   -m_dData[1],
                   -m_dData[2],
                   -m_dData[3]);
}
//----------------------------------------------------------------------------
SEVector4d Swing::operator*(double dLhsScalar, const SEVector4d& rRhsVec)
{
    return SEVector4d(
                   dLhsScalar * rRhsVec[0],
                   dLhsScalar * rRhsVec[1],
                   dLhsScalar * rRhsVec[2],
                   dLhsScalar * rRhsVec[3]);
}
//----------------------------------------------------------------------------
SEVector4d& SEVector4d::operator+=(const SEVector4d& rRhsVec)
{
    m_dData[0] += rRhsVec.m_dData[0];
    m_dData[1] += rRhsVec.m_dData[1];
    m_dData[2] += rRhsVec.m_dData[2];
    m_dData[3] += rRhsVec.m_dData[3];

    return *this;
}
//----------------------------------------------------------------------------
SEVector4d& SEVector4d::operator+=(double dScalar)
{
    m_dData[0] += dScalar;
    m_dData[1] += dScalar;
    m_dData[2] += dScalar;
    m_dData[3] += dScalar;

    return *this;
}
//----------------------------------------------------------------------------
SEVector4d& SEVector4d::operator-=(const SEVector4d& rRhsVec)
{
    m_dData[0] -= rRhsVec.m_dData[0];
    m_dData[1] -= rRhsVec.m_dData[1];
    m_dData[2] -= rRhsVec.m_dData[2];
    m_dData[3] -= rRhsVec.m_dData[3];

    return *this;
}
//----------------------------------------------------------------------------
SEVector4d& SEVector4d::operator-=(double dScalar)
{
    m_dData[0] -= dScalar;
    m_dData[1] -= dScalar;
    m_dData[2] -= dScalar;
    m_dData[3] -= dScalar;

    return *this;
}
//----------------------------------------------------------------------------
SEVector4d& SEVector4d::operator*=(double dScalar)
{
    m_dData[0] *= dScalar;
    m_dData[1] *= dScalar;
    m_dData[2] *= dScalar;
    m_dData[3] *= dScalar;

    return *this;
}
//----------------------------------------------------------------------------
SEVector4d& SEVector4d::operator/=(double dScalar)
{
    if( dScalar != 0.0 )
    {
        double dInvScalar = 1.0 / dScalar;
        m_dData[0] *= dInvScalar;
        m_dData[1] *= dInvScalar;
        m_dData[2] *= dInvScalar;
        m_dData[3] *= dInvScalar;
    }
    else
    {
        m_dData[0] = SEMath<double>::MAX_REAL;
        m_dData[1] = SEMath<double>::MAX_REAL;
        m_dData[2] = SEMath<double>::MAX_REAL;
        m_dData[3] = SEMath<double>::MAX_REAL;
    }

    return *this;
}
//----------------------------------------------------------------------------