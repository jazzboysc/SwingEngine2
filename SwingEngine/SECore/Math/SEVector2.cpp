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
#include "SEVector2.h"

using namespace Swing;

//----------------------------------------------------------------------------
// 单精度2维向量类
//----------------------------------------------------------------------------

const SEVector2f SEVector2f::ZERO(0.0f, 0.0f);
const SEVector2f SEVector2f::UNIT_X(1.0f, 0.0f);
const SEVector2f SEVector2f::UNIT_Y(0.0f, 1.0f);

//----------------------------------------------------------------------------
SEVector2f::SEVector2f()
{
    m_fData[0] = 0.0f;
    m_fData[1] = 0.0f;
}
//----------------------------------------------------------------------------
SEVector2f::SEVector2f(float fX, float fY)
{
    m_fData[0] = fX;
    m_fData[1] = fY;
}
//----------------------------------------------------------------------------
SEVector2f::SEVector2f(const SEVector2f& rVec)
{
    m_fData[0] = rVec.m_fData[0];
    m_fData[1] = rVec.m_fData[1];
}
//----------------------------------------------------------------------------
SEVector2f::operator const float*() const
{
    return m_fData;
}
//----------------------------------------------------------------------------
SEVector2f::operator float*()
{
    return m_fData;
}
//----------------------------------------------------------------------------
float SEVector2f::operator[](int i) const
{
    SE_ASSERT( 0 <= i && i <= 1 );

    return m_fData[i];
}
//----------------------------------------------------------------------------
float& SEVector2f::operator[](int i)
{
    SE_ASSERT( 0 <= i && i <= 1 );

    return m_fData[i];
}
//----------------------------------------------------------------------------
SEVector2f& SEVector2f::operator=(const SEVector2f& rVec)
{
    m_fData[0] = rVec.m_fData[0];
    m_fData[1] = rVec.m_fData[1];

    return *this;
}
//----------------------------------------------------------------------------
bool SEVector2f::operator==(const SEVector2f& rVec) const
{
    return CompareData(rVec) == 0;
}
//----------------------------------------------------------------------------
bool SEVector2f::operator!=(const SEVector2f& rVec) const
{
    return CompareData(rVec) != 0;
}
//----------------------------------------------------------------------------
bool SEVector2f::operator<(const SEVector2f& rVec) const
{
    return CompareData(rVec) < 0;
}
//----------------------------------------------------------------------------
bool SEVector2f::operator<=(const SEVector2f& rVec) const
{
    return CompareData(rVec) <= 0;
}
//----------------------------------------------------------------------------
bool SEVector2f::operator>(const SEVector2f& rVec) const
{
    return CompareData(rVec) > 0;
}
//----------------------------------------------------------------------------
bool SEVector2f::operator>=(const SEVector2f& rVec) const
{
    return CompareData(rVec) >= 0;
}
//----------------------------------------------------------------------------
SEVector2f SEVector2f::operator+(const SEVector2f& rRhsVec) const
{
    return SEVector2f(
                   m_fData[0] + rRhsVec.m_fData[0],
                   m_fData[1] + rRhsVec.m_fData[1]);
}
//----------------------------------------------------------------------------
SEVector2f SEVector2f::operator-(const SEVector2f& rRhsVec) const
{
    return SEVector2f(
                   m_fData[0] - rRhsVec.m_fData[0],
                   m_fData[1] - rRhsVec.m_fData[1]);
}
//----------------------------------------------------------------------------
SEVector2f SEVector2f::operator*(float fScalar) const
{
    return SEVector2f(
                   fScalar * m_fData[0],
                   fScalar * m_fData[1]);
}
//----------------------------------------------------------------------------
SEVector2f SEVector2f::operator/(float fScalar) const
{
    SEVector2f Res;

    if( fScalar != 0.0f )
    {
        float fInvScalar = (1.0f) / fScalar;
        Res.m_fData[0] = fInvScalar * m_fData[0];
        Res.m_fData[1] = fInvScalar * m_fData[1];
    }
    else
    {
        Res.m_fData[0] = SEMath<float>::MAX_REAL;
        Res.m_fData[1] = SEMath<float>::MAX_REAL;
    }

    return Res;
}
//----------------------------------------------------------------------------
SEVector2f SEVector2f::operator-() const
{
    return SEVector2f(
                   -m_fData[0],
                   -m_fData[1]);
}
//----------------------------------------------------------------------------
SEVector2f Swing::operator*(float fLhsScalar, const SEVector2f& rRhsVec)
{
    return SEVector2f(
                   fLhsScalar * rRhsVec[0],
                   fLhsScalar * rRhsVec[1]);
}
//----------------------------------------------------------------------------
SEVector2f& SEVector2f::operator+=(const SEVector2f& rRhsVec)
{
    m_fData[0] += rRhsVec.m_fData[0];
    m_fData[1] += rRhsVec.m_fData[1];

    return *this;
}
//----------------------------------------------------------------------------
SEVector2f& SEVector2f::operator+=(float fScalar)
{
    m_fData[0] += fScalar;
    m_fData[1] += fScalar;

    return *this;
}
//----------------------------------------------------------------------------
SEVector2f& SEVector2f::operator-=(const SEVector2f& rRhsVec)
{
    m_fData[0] -= rRhsVec.m_fData[0];
    m_fData[1] -= rRhsVec.m_fData[1];

    return *this;
}
//----------------------------------------------------------------------------
SEVector2f& SEVector2f::operator-=(float fScalar)
{
    m_fData[0] -= fScalar;
    m_fData[1] -= fScalar;

    return *this;
}
//----------------------------------------------------------------------------
SEVector2f& SEVector2f::operator*=(float fScalar)
{
    m_fData[0] *= fScalar;
    m_fData[1] *= fScalar;

    return *this;
}
//----------------------------------------------------------------------------
SEVector2f& SEVector2f::operator/=(float fScalar)
{
    if( fScalar != 0.0f )
    {
        float fInvScalar = (1.0f) / fScalar;
        m_fData[0] *= fInvScalar;
        m_fData[1] *= fInvScalar;
    }
    else
    {
        m_fData[0] = SEMath<float>::MAX_REAL;
        m_fData[1] = SEMath<float>::MAX_REAL;
    }

    return *this;
}
//----------------------------------------------------------------------------
void SEVector2f::Orthonormalize(SEVector2f& rVecU, SEVector2f& rVecV)
{
    rVecU.Normalize();
    // V在U上的投影长度
    float fDot0 = rVecU.Dot(rVecV);
    // V = V减去V在U上的投影向量
    rVecV -= rVecU*fDot0;
    rVecV.Normalize();
}
//----------------------------------------------------------------------------
void SEVector2f::GetOrthonormalBasis(SEVector2f& rVecU, SEVector2f& rVecV, 
    bool bUnitLengthV)
{
    if( !bUnitLengthV )
        rVecV.Normalize();

    rVecV.GetPerp(rVecU);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 双精度2维向量类
//----------------------------------------------------------------------------

const SEVector2d SEVector2d::ZERO(0.0, 0.0);
const SEVector2d SEVector2d::UNIT_X(1.0, 0.0);
const SEVector2d SEVector2d::UNIT_Y(0.0, 1.0);

//----------------------------------------------------------------------------
SEVector2d::SEVector2d()
{
    m_dData[0] = 0.0;
    m_dData[1] = 0.0;
}
//----------------------------------------------------------------------------
SEVector2d::SEVector2d(double dX, double dY)
{
    m_dData[0] = dX;
    m_dData[1] = dY;
}
//----------------------------------------------------------------------------
SEVector2d::SEVector2d(const SEVector2d& rVec)
{
    m_dData[0] = rVec.m_dData[0];
    m_dData[1] = rVec.m_dData[1];
}
//----------------------------------------------------------------------------
SEVector2d::operator const double*() const
{
    return m_dData;
}
//----------------------------------------------------------------------------
SEVector2d::operator double*()
{
    return m_dData;
}
//----------------------------------------------------------------------------
double SEVector2d::operator[](int i) const
{
    SE_ASSERT( 0 <= i && i <= 1 );

    return m_dData[i];
}
//----------------------------------------------------------------------------
double& SEVector2d::operator[](int i)
{
    SE_ASSERT( 0 <= i && i <= 1 );

    return m_dData[i];
}
//----------------------------------------------------------------------------
SEVector2d& SEVector2d::operator=(const SEVector2d& rVec)
{
    m_dData[0] = rVec.m_dData[0];
    m_dData[1] = rVec.m_dData[1];

    return *this;
}
//----------------------------------------------------------------------------
bool SEVector2d::operator==(const SEVector2d& rVec) const
{
    return CompareData(rVec) == 0;
}
//----------------------------------------------------------------------------
bool SEVector2d::operator!=(const SEVector2d& rVec) const
{
    return CompareData(rVec) != 0;
}
//----------------------------------------------------------------------------
bool SEVector2d::operator<(const SEVector2d& rVec) const
{
    return CompareData(rVec) < 0;
}
//----------------------------------------------------------------------------
bool SEVector2d::operator<=(const SEVector2d& rVec) const
{
    return CompareData(rVec) <= 0;
}
//----------------------------------------------------------------------------
bool SEVector2d::operator>(const SEVector2d& rVec) const
{
    return CompareData(rVec) > 0;
}
//----------------------------------------------------------------------------
bool SEVector2d::operator>=(const SEVector2d& rVec) const
{
    return CompareData(rVec) >= 0;
}
//----------------------------------------------------------------------------
SEVector2d SEVector2d::operator+(const SEVector2d& rRhsVec) const
{
    return SEVector2d(
                   m_dData[0] + rRhsVec.m_dData[0],
                   m_dData[1] + rRhsVec.m_dData[1]);
}
//----------------------------------------------------------------------------
SEVector2d SEVector2d::operator-(const SEVector2d& rRhsVec) const
{
    return SEVector2d(
                   m_dData[0] - rRhsVec.m_dData[0],
                   m_dData[1] - rRhsVec.m_dData[1]);
}
//----------------------------------------------------------------------------
SEVector2d SEVector2d::operator*(double dScalar) const
{
    return SEVector2d(
                   dScalar * m_dData[0],
                   dScalar * m_dData[1]);
}
//----------------------------------------------------------------------------
SEVector2d SEVector2d::operator/(double dScalar) const
{
    SEVector2d Res;

    if( dScalar != 0.0 )
    {
        double dInvScalar = 1.0 / dScalar;
        Res.m_dData[0] = dInvScalar * m_dData[0];
        Res.m_dData[1] = dInvScalar * m_dData[1];
    }
    else
    {
        Res.m_dData[0] = SEMath<double>::MAX_REAL;
        Res.m_dData[1] = SEMath<double>::MAX_REAL;
    }

    return Res;
}
//----------------------------------------------------------------------------
SEVector2d SEVector2d::operator-() const
{
    return SEVector2d(
                   -m_dData[0],
                   -m_dData[1]);
}
//----------------------------------------------------------------------------
SEVector2d Swing::operator*(double dLhsScalar, const SEVector2d& rRhsVec)
{
    return SEVector2d(
                   dLhsScalar * rRhsVec[0],
                   dLhsScalar * rRhsVec[1]);
}
//----------------------------------------------------------------------------
SEVector2d& SEVector2d::operator+=(const SEVector2d& rRhsVec)
{
    m_dData[0] += rRhsVec.m_dData[0];
    m_dData[1] += rRhsVec.m_dData[1];

    return *this;
}
//----------------------------------------------------------------------------
SEVector2d& SEVector2d::operator+=(double dScalar)
{
    m_dData[0] += dScalar;
    m_dData[1] += dScalar;

    return *this;
}
//----------------------------------------------------------------------------
SEVector2d& SEVector2d::operator-=(const SEVector2d& rRhsVec)
{
    m_dData[0] -= rRhsVec.m_dData[0];
    m_dData[1] -= rRhsVec.m_dData[1];

    return *this;
}
//----------------------------------------------------------------------------
SEVector2d& SEVector2d::operator-=(double dScalar)
{
    m_dData[0] -= dScalar;
    m_dData[1] -= dScalar;

    return *this;
}
//----------------------------------------------------------------------------
SEVector2d& SEVector2d::operator*=(double dScalar)
{
    m_dData[0] *= dScalar;
    m_dData[1] *= dScalar;

    return *this;
}
//----------------------------------------------------------------------------
SEVector2d& SEVector2d::operator/=(double dScalar)
{
    if( dScalar != 0.0 )
    {
        double dInvScalar = 1.0 / dScalar;
        m_dData[0] *= dInvScalar;
        m_dData[1] *= dInvScalar;
    }
    else
    {
        m_dData[0] = SEMath<double>::MAX_REAL;
        m_dData[1] = SEMath<double>::MAX_REAL;
    }

    return *this;
}
//----------------------------------------------------------------------------
void SEVector2d::Orthonormalize(SEVector2d& rVecU, SEVector2d& rVecV)
{
    rVecU.Normalize();
    // V在U上的投影长度
    double dDot0 = rVecU.Dot(rVecV);
    // V = V减去V在U上的投影向量
    rVecV -= rVecU*dDot0;
    rVecV.Normalize();
}
//----------------------------------------------------------------------------
void SEVector2d::GetOrthonormalBasis(SEVector2d& rVecU, SEVector2d& rVecV, 
    bool bUnitLengthV)
{
    if( !bUnitLengthV )
        rVecV.Normalize();

    rVecV.GetPerp(rVecU);
}
//----------------------------------------------------------------------------