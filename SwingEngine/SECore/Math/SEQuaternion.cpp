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
#include "SEQuaternion.h"

using namespace Swing;

const SEQuaternionf SEQuaternionf::IDENTITY(1.0f, 0.0f, 0.0f, 0.0f);
const SEQuaternionf SEQuaternionf::ZERO(0.0f, 0.0f, 0.0f, 0.0f);
int SEQuaternionf::m_iNext[3] = { 1, 2, 0 };

//----------------------------------------------------------------------------
SEQuaternionf::SEQuaternionf()
{
}
//----------------------------------------------------------------------------
SEQuaternionf::SEQuaternionf(float fW, float fX, float fY, float fZ)
{
    m_fData[0] = fW;
    m_fData[1] = fX;
    m_fData[2] = fY;
    m_fData[3] = fZ;
}
//----------------------------------------------------------------------------
SEQuaternionf::SEQuaternionf(const SEQuaternionf& rQ)
{
    m_fData[0] = rQ.m_fData[0];
    m_fData[1] = rQ.m_fData[1];
    m_fData[2] = rQ.m_fData[2];
    m_fData[3] = rQ.m_fData[3];
}
//----------------------------------------------------------------------------
SEQuaternionf::SEQuaternionf(const SEMatrix3f& rRotMat)
{
    FromRotationMatrix(rRotMat);
}
//----------------------------------------------------------------------------
SEQuaternionf::SEQuaternionf(const SEVector3f& rAxis, float fAngle)
{
    FromAxisAngle(rAxis, fAngle);
}
//----------------------------------------------------------------------------
SEQuaternionf::operator const float*() const
{
    return &m_fData[0];
}
//----------------------------------------------------------------------------
SEQuaternionf::operator float*()
{
    return &m_fData[0];
}
//----------------------------------------------------------------------------
float SEQuaternionf::operator[](int i) const
{
    SE_ASSERT( 0 <= i && i <= 3 );

    if( i < 0 )
    {
        i = 0;
    }
    else if( i > 3 )
    {
        i = 3;
    }

    return m_fData[i];
}
//----------------------------------------------------------------------------
float& SEQuaternionf::operator[](int i)
{
    SE_ASSERT( 0 <= i && i <= 3 );
    
    if( i < 0 )
    {
        i = 0;
    }
    else if( i > 3 )
    {
        i = 3;
    }

    return m_fData[i];
}
//----------------------------------------------------------------------------
SEQuaternionf& SEQuaternionf::operator=(const SEQuaternionf& rQ)
{
    m_fData[0] = rQ.m_fData[0];
    m_fData[1] = rQ.m_fData[1];
    m_fData[2] = rQ.m_fData[2];
    m_fData[3] = rQ.m_fData[3];

    return *this;
}
//----------------------------------------------------------------------------
bool SEQuaternionf::operator==(const SEQuaternionf& rQ) const
{
    return CompareData(rQ) == 0;
}
//----------------------------------------------------------------------------
bool SEQuaternionf::operator!=(const SEQuaternionf& rQ) const
{
    return CompareData(rQ) != 0;
}
//----------------------------------------------------------------------------
bool SEQuaternionf::operator<(const SEQuaternionf& rQ) const
{
    return CompareData(rQ) < 0;
}
//----------------------------------------------------------------------------
bool SEQuaternionf::operator<=(const SEQuaternionf& rQ) const
{
    return CompareData(rQ) <= 0;
}
//----------------------------------------------------------------------------
bool SEQuaternionf::operator>(const SEQuaternionf& rQ) const
{
    return CompareData(rQ) > 0;
}
//----------------------------------------------------------------------------
bool SEQuaternionf::operator>=(const SEQuaternionf& rQ) const
{
    return CompareData(rQ) >= 0;
}
//----------------------------------------------------------------------------
SEQuaternionf SEQuaternionf::operator+(const SEQuaternionf& rRhsQ) const
{
    SEQuaternionf tempQ;

    tempQ.m_fData[0] = m_fData[0] + rRhsQ.m_fData[0];
    tempQ.m_fData[1] = m_fData[1] + rRhsQ.m_fData[1];
    tempQ.m_fData[2] = m_fData[2] + rRhsQ.m_fData[2];
    tempQ.m_fData[3] = m_fData[3] + rRhsQ.m_fData[3];

    return tempQ;
}
//----------------------------------------------------------------------------
SEQuaternionf SEQuaternionf::operator-(const SEQuaternionf& rRhsQ) const
{
    SEQuaternionf tempQ;

    tempQ.m_fData[0] = m_fData[0] - rRhsQ.m_fData[0];
    tempQ.m_fData[1] = m_fData[1] - rRhsQ.m_fData[1];
    tempQ.m_fData[2] = m_fData[2] - rRhsQ.m_fData[2];
    tempQ.m_fData[3] = m_fData[3] - rRhsQ.m_fData[3];

    return tempQ;
}
//----------------------------------------------------------------------------
SEQuaternionf SEQuaternionf::operator*(const SEQuaternionf& rRhsQ) const
{
    // 注意:quaternion乘法不满足交换律

    SEQuaternionf tempQ;

    tempQ.m_fData[0] =
        m_fData[0]*rRhsQ.m_fData[0] -
        m_fData[1]*rRhsQ.m_fData[1] -
        m_fData[2]*rRhsQ.m_fData[2] -
        m_fData[3]*rRhsQ.m_fData[3];

    tempQ.m_fData[1] =
        m_fData[0]*rRhsQ.m_fData[1] +
        m_fData[1]*rRhsQ.m_fData[0] +
        m_fData[2]*rRhsQ.m_fData[3] -
        m_fData[3]*rRhsQ.m_fData[2];

    tempQ.m_fData[2] =
        m_fData[0]*rRhsQ.m_fData[2] +
        m_fData[2]*rRhsQ.m_fData[0] +
        m_fData[3]*rRhsQ.m_fData[1] -
        m_fData[1]*rRhsQ.m_fData[3];

    tempQ.m_fData[3] =
        m_fData[0]*rRhsQ.m_fData[3] +
        m_fData[3]*rRhsQ.m_fData[0] +
        m_fData[1]*rRhsQ.m_fData[2] -
        m_fData[2]*rRhsQ.m_fData[1];

    return tempQ;
}
//----------------------------------------------------------------------------
SEQuaternionf SEQuaternionf::operator*(float fScalar) const
{
    SEQuaternionf tempQ;

    tempQ.m_fData[0] = fScalar * m_fData[0];
    tempQ.m_fData[1] = fScalar * m_fData[1];
    tempQ.m_fData[2] = fScalar * m_fData[2];
    tempQ.m_fData[3] = fScalar * m_fData[3];

    return tempQ;
}
//----------------------------------------------------------------------------
SEQuaternionf SEQuaternionf::operator/(float fScalar) const
{
    SEQuaternionf tempQ;

    if( fScalar != 0.0f )
    {
        float fInvScalar = 1.0f / fScalar;
        tempQ.m_fData[0] = fInvScalar * m_fData[0];
        tempQ.m_fData[1] = fInvScalar * m_fData[1];
        tempQ.m_fData[2] = fInvScalar * m_fData[2];
        tempQ.m_fData[3] = fInvScalar * m_fData[3];
    }
    else
    {
        tempQ.m_fData[0] = SEMath<float>::MAX_REAL;
        tempQ.m_fData[1] = SEMath<float>::MAX_REAL;
        tempQ.m_fData[2] = SEMath<float>::MAX_REAL;
        tempQ.m_fData[3] = SEMath<float>::MAX_REAL;
    }

    return tempQ;
}
//----------------------------------------------------------------------------
SEQuaternionf SEQuaternionf::operator-() const
{
    SEQuaternionf tempQ;

    tempQ.m_fData[0] = -m_fData[0];
    tempQ.m_fData[1] = -m_fData[1];
    tempQ.m_fData[2] = -m_fData[2];
    tempQ.m_fData[3] = -m_fData[3];

    return tempQ;
}
//----------------------------------------------------------------------------
SEQuaternionf& SEQuaternionf::operator+=(const SEQuaternionf& rRhsQ)
{
    m_fData[0] += rRhsQ.m_fData[0];
    m_fData[1] += rRhsQ.m_fData[1];
    m_fData[2] += rRhsQ.m_fData[2];
    m_fData[3] += rRhsQ.m_fData[3];

    return *this;
}
//----------------------------------------------------------------------------
SEQuaternionf& SEQuaternionf::operator-=(const SEQuaternionf& rRhsQ)
{
    m_fData[0] -= rRhsQ.m_fData[0];
    m_fData[1] -= rRhsQ.m_fData[1];
    m_fData[2] -= rRhsQ.m_fData[2];
    m_fData[3] -= rRhsQ.m_fData[3];

    return *this;
}
//----------------------------------------------------------------------------
SEQuaternionf& SEQuaternionf::operator*=(float fScalar)
{
    m_fData[0] *= fScalar;
    m_fData[1] *= fScalar;
    m_fData[2] *= fScalar;
    m_fData[3] *= fScalar;

    return *this;
}
//----------------------------------------------------------------------------
SEQuaternionf& SEQuaternionf::operator/=(float fScalar)
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
SEVector3f SEQuaternionf::Rotate(const SEVector3f& rSrcVec) const
{
    SEMatrix3f tempRot;
    ToRotationMatrix(tempRot);

    return rSrcVec * tempRot;
}
//----------------------------------------------------------------------------
void SEQuaternionf::FromEulerAngles(const SEVector3f& eulerAngles)
{
    SEVector3f c, s;

    c.X = SEMath<float>::Cos(eulerAngles.X*0.5f);
    c.Y = SEMath<float>::Cos(eulerAngles.Y*0.5f);
    c.Z = SEMath<float>::Cos(eulerAngles.Z*0.5f);
    s.X = SEMath<float>::Sin(eulerAngles.X*0.5f);
    s.Y = SEMath<float>::Sin(eulerAngles.Y*0.5f);
    s.Z = SEMath<float>::Sin(eulerAngles.Z*0.5f);

    W = c.X*c.Y*c.Z + s.X*s.Y*s.Z;
    X = s.X*c.Y*c.Z - c.X*s.Y*s.Z;
    Y = c.X*s.Y*c.Z + s.X*c.Y*s.Z;
    Z = c.X*c.Y*s.Z - s.X*s.Y*c.Z;
}
//----------------------------------------------------------------------------
SEQuaternionf& SEQuaternionf::Slerp(float fT, const SEQuaternionf& rP,
    const SEQuaternionf& rQ)
{
    float fCos = rP.Dot(rQ);
    float fAngle = SEMath<float>::ACos(fCos);

    if( SEMath<float>::FAbs(fAngle) >= SEMath<float>::ZERO_TOLERANCE )
    {
        float fSin = SEMath<float>::Sin(fAngle);
        float fInvSin = 1.0f / fSin;
        float fCoeff0 = SEMath<float>::Sin((1.0f - fT) * fAngle) * fInvSin;
        float fCoeff1 = SEMath<float>::Sin(fT * fAngle) * fInvSin;
        *this = fCoeff0*rP + fCoeff1*rQ;
    }
    else
    {
        *this = rP;
    }

    return *this;
}
//----------------------------------------------------------------------------
SEQuaternionf& SEQuaternionf::Lerp(float fT, const SEQuaternionf& rP, 
    const SEQuaternionf& rQ)
{
    *this = (1.0f - fT)*rP + fT*rQ;
    Normalize();

    return *this;
}
//----------------------------------------------------------------------------
SEQuaternionf& SEQuaternionf::FromRotationMatrix(const SEVector3f aRot[3], 
    bool bIsRow)
{
    SEMatrix3f tempRot;
    if( bIsRow )
    {
        for( int iRow = 0; iRow < 3; iRow++ )
        {
            tempRot(iRow, 0) = aRot[iRow][0];
            tempRot(iRow, 1) = aRot[iRow][1];
            tempRot(iRow, 2) = aRot[iRow][2];
        }
    }
    else
    {
        for( int iCol = 0; iCol < 3; iCol++ )
        {
            tempRot(0, iCol) = aRot[iCol][0];
            tempRot(1, iCol) = aRot[iCol][1];
            tempRot(2, iCol) = aRot[iCol][2];
        }
    }

    return FromRotationMatrix(tempRot);
}
//----------------------------------------------------------------------------
void SEQuaternionf::ToRotationMatrix(SEVector3f aRot[3], bool bIsRow) const
{
    SEMatrix3f tempRot;
    ToRotationMatrix(tempRot);
    if( bIsRow )
    {
        for( int iRow = 0; iRow < 3; iRow++ )
        {
            aRot[iRow][0] = tempRot(iRow, 0);
            aRot[iRow][1] = tempRot(iRow, 1);
            aRot[iRow][2] = tempRot(iRow, 2);
        }
    }
    else
    {
        for( int iCol = 0; iCol < 3; iCol++ )
        {
            aRot[iCol][0] = tempRot(0, iCol);
            aRot[iCol][1] = tempRot(1, iCol);
            aRot[iCol][2] = tempRot(2, iCol);
        }
    }
}
//----------------------------------------------------------------------------
SEQuaternionf Swing::operator*(float fLhsScalar, const SEQuaternionf& rRhsQ)
{
    SEQuaternionf tempQ;

    tempQ[0] = fLhsScalar * rRhsQ[0];
    tempQ[1] = fLhsScalar * rRhsQ[1];
    tempQ[2] = fLhsScalar * rRhsQ[2];
    tempQ[3] = fLhsScalar * rRhsQ[3];

    return tempQ;
}
//----------------------------------------------------------------------------


const SEQuaterniond SEQuaterniond::IDENTITY(1.0, 0.0, 0.0, 0.0);
const SEQuaterniond SEQuaterniond::ZERO(0.0, 0.0, 0.0, 0.0);
int SEQuaterniond::m_iNext[3] = { 1, 2, 0 };

//----------------------------------------------------------------------------
SEQuaterniond::SEQuaterniond()
{
}
//----------------------------------------------------------------------------
SEQuaterniond::SEQuaterniond(double dW, double dX, double dY, double dZ)
{
    m_dData[0] = dW;
    m_dData[1] = dX;
    m_dData[2] = dY;
    m_dData[3] = dZ;
}
//----------------------------------------------------------------------------
SEQuaterniond::SEQuaterniond(const SEQuaterniond& rQ)
{
    m_dData[0] = rQ.m_dData[0];
    m_dData[1] = rQ.m_dData[1];
    m_dData[2] = rQ.m_dData[2];
    m_dData[3] = rQ.m_dData[3];
}
//----------------------------------------------------------------------------
SEQuaterniond::SEQuaterniond(const SEMatrix3d& rRotMat)
{
    FromRotationMatrix(rRotMat);
}
//----------------------------------------------------------------------------
SEQuaterniond::SEQuaterniond(const SEVector3d& rAxis, double dAngle)
{
    FromAxisAngle(rAxis, dAngle);
}
//----------------------------------------------------------------------------
SEQuaterniond::operator const double*() const
{
    return &m_dData[0];
}
//----------------------------------------------------------------------------
SEQuaterniond::operator double*()
{
    return &m_dData[0];
}
//----------------------------------------------------------------------------
double SEQuaterniond::operator[](int i) const
{
    SE_ASSERT( 0 <= i && i <= 3 );

    if( i < 0 )
    {
        i = 0;
    }
    else if( i > 3 )
    {
        i = 3;
    }

    return m_dData[i];
}
//----------------------------------------------------------------------------
double& SEQuaterniond::operator[](int i)
{
    SE_ASSERT( 0 <= i && i <= 3 );
    
    if( i < 0 )
    {
        i = 0;
    }
    else if( i > 3 )
    {
        i = 3;
    }

    return m_dData[i];
}
//----------------------------------------------------------------------------
SEQuaterniond& SEQuaterniond::operator=(const SEQuaterniond& rQ)
{
    m_dData[0] = rQ.m_dData[0];
    m_dData[1] = rQ.m_dData[1];
    m_dData[2] = rQ.m_dData[2];
    m_dData[3] = rQ.m_dData[3];

    return *this;
}
//----------------------------------------------------------------------------
bool SEQuaterniond::operator==(const SEQuaterniond& rQ) const
{
    return CompareData(rQ) == 0;
}
//----------------------------------------------------------------------------
bool SEQuaterniond::operator!=(const SEQuaterniond& rQ) const
{
    return CompareData(rQ) != 0;
}
//----------------------------------------------------------------------------
bool SEQuaterniond::operator<(const SEQuaterniond& rQ) const
{
    return CompareData(rQ) < 0;
}
//----------------------------------------------------------------------------
bool SEQuaterniond::operator<=(const SEQuaterniond& rQ) const
{
    return CompareData(rQ) <= 0;
}
//----------------------------------------------------------------------------
bool SEQuaterniond::operator>(const SEQuaterniond& rQ) const
{
    return CompareData(rQ) > 0;
}
//----------------------------------------------------------------------------
bool SEQuaterniond::operator>=(const SEQuaterniond& rQ) const
{
    return CompareData(rQ) >= 0;
}
//----------------------------------------------------------------------------
SEQuaterniond SEQuaterniond::operator+(const SEQuaterniond& rRhsQ) const
{
    SEQuaterniond tempQ;

    tempQ.m_dData[0] = m_dData[0] + rRhsQ.m_dData[0];
    tempQ.m_dData[1] = m_dData[1] + rRhsQ.m_dData[1];
    tempQ.m_dData[2] = m_dData[2] + rRhsQ.m_dData[2];
    tempQ.m_dData[3] = m_dData[3] + rRhsQ.m_dData[3];

    return tempQ;
}
//----------------------------------------------------------------------------
SEQuaterniond SEQuaterniond::operator-(const SEQuaterniond& rRhsQ) const
{
    SEQuaterniond tempQ;

    tempQ.m_dData[0] = m_dData[0] - rRhsQ.m_dData[0];
    tempQ.m_dData[1] = m_dData[1] - rRhsQ.m_dData[1];
    tempQ.m_dData[2] = m_dData[2] - rRhsQ.m_dData[2];
    tempQ.m_dData[3] = m_dData[3] - rRhsQ.m_dData[3];

    return tempQ;
}
//----------------------------------------------------------------------------
SEQuaterniond SEQuaterniond::operator*(const SEQuaterniond& rRhsQ) const
{
    // 注意:quaternion乘法不满足交换律

    SEQuaterniond tempQ;

    tempQ.m_dData[0] =
        m_dData[0]*rRhsQ.m_dData[0] -
        m_dData[1]*rRhsQ.m_dData[1] -
        m_dData[2]*rRhsQ.m_dData[2] -
        m_dData[3]*rRhsQ.m_dData[3];

    tempQ.m_dData[1] =
        m_dData[0]*rRhsQ.m_dData[1] +
        m_dData[1]*rRhsQ.m_dData[0] +
        m_dData[2]*rRhsQ.m_dData[3] -
        m_dData[3]*rRhsQ.m_dData[2];

    tempQ.m_dData[2] =
        m_dData[0]*rRhsQ.m_dData[2] +
        m_dData[2]*rRhsQ.m_dData[0] +
        m_dData[3]*rRhsQ.m_dData[1] -
        m_dData[1]*rRhsQ.m_dData[3];

    tempQ.m_dData[3] =
        m_dData[0]*rRhsQ.m_dData[3] +
        m_dData[3]*rRhsQ.m_dData[0] +
        m_dData[1]*rRhsQ.m_dData[2] -
        m_dData[2]*rRhsQ.m_dData[1];

    return tempQ;
}
//----------------------------------------------------------------------------
SEQuaterniond SEQuaterniond::operator*(double dScalar) const
{
    SEQuaterniond tempQ;

    tempQ.m_dData[0] = dScalar * m_dData[0];
    tempQ.m_dData[1] = dScalar * m_dData[1];
    tempQ.m_dData[2] = dScalar * m_dData[2];
    tempQ.m_dData[3] = dScalar * m_dData[3];

    return tempQ;
}
//----------------------------------------------------------------------------
SEQuaterniond SEQuaterniond::operator/(double dScalar) const
{
    SEQuaterniond tempQ;

    if( dScalar != 0.0 )
    {
        double dInvScalar = 1.0 / dScalar;
        tempQ.m_dData[0] = dInvScalar * m_dData[0];
        tempQ.m_dData[1] = dInvScalar * m_dData[1];
        tempQ.m_dData[2] = dInvScalar * m_dData[2];
        tempQ.m_dData[3] = dInvScalar * m_dData[3];
    }
    else
    {
        tempQ.m_dData[0] = SEMath<double>::MAX_REAL;
        tempQ.m_dData[1] = SEMath<double>::MAX_REAL;
        tempQ.m_dData[2] = SEMath<double>::MAX_REAL;
        tempQ.m_dData[3] = SEMath<double>::MAX_REAL;
    }

    return tempQ;
}
//----------------------------------------------------------------------------
SEQuaterniond SEQuaterniond::operator-() const
{
    SEQuaterniond tempQ;

    tempQ.m_dData[0] = -m_dData[0];
    tempQ.m_dData[1] = -m_dData[1];
    tempQ.m_dData[2] = -m_dData[2];
    tempQ.m_dData[3] = -m_dData[3];

    return tempQ;
}
//----------------------------------------------------------------------------
SEQuaterniond& SEQuaterniond::operator+=(const SEQuaterniond& rRhsQ)
{
    m_dData[0] += rRhsQ.m_dData[0];
    m_dData[1] += rRhsQ.m_dData[1];
    m_dData[2] += rRhsQ.m_dData[2];
    m_dData[3] += rRhsQ.m_dData[3];

    return *this;
}
//----------------------------------------------------------------------------
SEQuaterniond& SEQuaterniond::operator-=(const SEQuaterniond& rRhsQ)
{
    m_dData[0] -= rRhsQ.m_dData[0];
    m_dData[1] -= rRhsQ.m_dData[1];
    m_dData[2] -= rRhsQ.m_dData[2];
    m_dData[3] -= rRhsQ.m_dData[3];

    return *this;
}
//----------------------------------------------------------------------------
SEQuaterniond& SEQuaterniond::operator*=(double dScalar)
{
    m_dData[0] *= dScalar;
    m_dData[1] *= dScalar;
    m_dData[2] *= dScalar;
    m_dData[3] *= dScalar;

    return *this;
}
//----------------------------------------------------------------------------
SEQuaterniond& SEQuaterniond::operator/=(double dScalar)
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
        m_dData[0] = SEMath<float>::MAX_REAL;
        m_dData[1] = SEMath<float>::MAX_REAL;
        m_dData[2] = SEMath<float>::MAX_REAL;
        m_dData[3] = SEMath<float>::MAX_REAL;
    }

    return *this;
}
//----------------------------------------------------------------------------
SEVector3d SEQuaterniond::Rotate(const SEVector3d& rSrcVec) const
{
    SEMatrix3d tempRot;
    ToRotationMatrix(tempRot);

    return rSrcVec * tempRot;
}
//----------------------------------------------------------------------------
SEQuaterniond& SEQuaterniond::Slerp(double dT, const SEQuaterniond& rP,
    const SEQuaterniond& rQ)
{
    double dCos = rP.Dot(rQ);
    double dAngle = SEMath<double>::ACos(dCos);

    if( SEMath<double>::FAbs(dAngle) >= SEMath<double>::ZERO_TOLERANCE )
    {
        double dSin = SEMath<double>::Sin(dAngle);
        double dInvSin = 1.0 / dSin;
        double dCoeff0 = SEMath<double>::Sin((1.0 - dT) * dAngle) * dInvSin;
        double dCoeff1 = SEMath<double>::Sin(dT * dAngle) * dInvSin;
        *this = dCoeff0*rP + dCoeff1*rQ;
    }
    else
    {
        *this = rP;
    }

    return *this;
}
//----------------------------------------------------------------------------
SEQuaterniond& SEQuaterniond::Lerp(double dT, const SEQuaterniond& rP, 
    const SEQuaterniond& rQ)
{
    *this = (1.0 - dT)*rP + dT*rQ;
    Normalize();

    return *this;
}
//----------------------------------------------------------------------------
SEQuaterniond& SEQuaterniond::FromRotationMatrix(const SEVector3d aRot[3], 
    bool bIsRow)
{
    SEMatrix3d tempRot;
    if( bIsRow )
    {
        for( int iRow = 0; iRow < 3; iRow++ )
        {
            tempRot(iRow, 0) = aRot[iRow][0];
            tempRot(iRow, 1) = aRot[iRow][1];
            tempRot(iRow, 2) = aRot[iRow][2];
        }
    }
    else
    {
        for( int iCol = 0; iCol < 3; iCol++ )
        {
            tempRot(0, iCol) = aRot[iCol][0];
            tempRot(1, iCol) = aRot[iCol][1];
            tempRot(2, iCol) = aRot[iCol][2];
        }
    }

    return FromRotationMatrix(tempRot);
}
//----------------------------------------------------------------------------
void SEQuaterniond::ToRotationMatrix(SEVector3d aRot[3], bool bIsRow) const
{
    SEMatrix3d tempRot;
    ToRotationMatrix(tempRot);
    if( bIsRow )
    {
        for( int iRow = 0; iRow < 3; iRow++ )
        {
            aRot[iRow][0] = tempRot(iRow, 0);
            aRot[iRow][1] = tempRot(iRow, 1);
            aRot[iRow][2] = tempRot(iRow, 2);
        }
    }
    else
    {
        for( int iCol = 0; iCol < 3; iCol++ )
        {
            aRot[iCol][0] = tempRot(0, iCol);
            aRot[iCol][1] = tempRot(1, iCol);
            aRot[iCol][2] = tempRot(2, iCol);
        }
    }
}
//----------------------------------------------------------------------------
SEQuaterniond Swing::operator*(double dLhsScalar, const SEQuaterniond& rRhsQ)
{
    SEQuaterniond tempQ;

    tempQ[0] = dLhsScalar * rRhsQ[0];
    tempQ[1] = dLhsScalar * rRhsQ[1];
    tempQ[2] = dLhsScalar * rRhsQ[2];
    tempQ[3] = dLhsScalar * rRhsQ[3];

    return tempQ;
}
//----------------------------------------------------------------------------