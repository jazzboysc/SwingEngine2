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
#include "SEVector3.h"

using namespace Swing;

//----------------------------------------------------------------------------
// 单精度3维向量类
//----------------------------------------------------------------------------

const SEVector3f SEVector3f::ZERO(0.0f, 0.0f, 0.0f);
const SEVector3f SEVector3f::UNIT_X(1.0f, 0.0f, 0.0f);
const SEVector3f SEVector3f::UNIT_Y(0.0f, 1.0f, 0.0f);
const SEVector3f SEVector3f::UNIT_Z(0.0f, 0.0f, 1.0f);

//----------------------------------------------------------------------------
SEVector3f::SEVector3f()
{
    m_fData[0] = 0.0f;
    m_fData[1] = 0.0f;
    m_fData[2] = 0.0f;
}
//----------------------------------------------------------------------------
SEVector3f::SEVector3f(float fX, float fY, float fZ)
{
    m_fData[0] = fX;
    m_fData[1] = fY;
    m_fData[2] = fZ;
}
//----------------------------------------------------------------------------
SEVector3f::SEVector3f(const SEVector3f& rVecV)
{
    m_fData[0] = rVecV.m_fData[0];
    m_fData[1] = rVecV.m_fData[1];
    m_fData[2] = rVecV.m_fData[2];
}
//----------------------------------------------------------------------------
SEVector3f::operator const float*() const
{
    return m_fData;
}
//----------------------------------------------------------------------------
SEVector3f::operator float*()
{
    return m_fData;
}
//----------------------------------------------------------------------------
float SEVector3f::operator[](int i) const
{
    SE_ASSERT( 0 <= i && i <= 2 );

    return m_fData[i];
}
//----------------------------------------------------------------------------
float& SEVector3f::operator[](int i)
{
    SE_ASSERT( 0 <= i && i <= 2 );

    return m_fData[i];
}
//----------------------------------------------------------------------------
SEVector3f& SEVector3f::operator=(const SEVector3f& rVecV)
{
    m_fData[0] = rVecV.m_fData[0];
    m_fData[1] = rVecV.m_fData[1];
    m_fData[2] = rVecV.m_fData[2];

    return *this;
}
//----------------------------------------------------------------------------
bool SEVector3f::operator==(const SEVector3f& rVecV) const
{
    return CompareData(rVecV) == 0;
}
//----------------------------------------------------------------------------
bool SEVector3f::operator!=(const SEVector3f& rVecV) const
{
    return CompareData(rVecV) != 0;
}
//----------------------------------------------------------------------------
bool SEVector3f::operator<(const SEVector3f& rVecV) const
{
    return CompareData(rVecV) < 0;
}
//----------------------------------------------------------------------------
bool SEVector3f::operator<=(const SEVector3f& rVecV) const
{
    return CompareData(rVecV) <= 0;
}
//----------------------------------------------------------------------------
bool SEVector3f::operator>(const SEVector3f& rVecV) const
{
    return CompareData(rVecV) > 0;
}
//----------------------------------------------------------------------------
bool SEVector3f::operator>=(const SEVector3f& rVecV) const
{
    return CompareData(rVecV) >= 0;
}
//----------------------------------------------------------------------------
SEVector3f SEVector3f::operator+(const SEVector3f& rRhsVec) const
{
    return SEVector3f(
                   m_fData[0] + rRhsVec.m_fData[0],
                   m_fData[1] + rRhsVec.m_fData[1],
                   m_fData[2] + rRhsVec.m_fData[2]);
}
//----------------------------------------------------------------------------
SEVector3f SEVector3f::operator-(const SEVector3f& rRhsVec) const
{
    return SEVector3f(
                   m_fData[0] - rRhsVec.m_fData[0],
                   m_fData[1] - rRhsVec.m_fData[1],
                   m_fData[2] - rRhsVec.m_fData[2]);
}
//----------------------------------------------------------------------------
SEVector3f SEVector3f::operator*(float fScalar) const
{
    return SEVector3f(
                   fScalar * m_fData[0],
                   fScalar * m_fData[1],
                   fScalar * m_fData[2]);
}
//----------------------------------------------------------------------------
SEVector3f SEVector3f::operator/(float fScalar) const
{
    SEVector3f Res;

    if( fScalar != 0.0f )
    {
        float fInvScalar = 1.0f/fScalar;
        Res.m_fData[0] = fInvScalar * m_fData[0];
        Res.m_fData[1] = fInvScalar * m_fData[1];
        Res.m_fData[2] = fInvScalar * m_fData[2];
    }
    else
    {
        Res.m_fData[0] = SEMath<float>::MAX_REAL;
        Res.m_fData[1] = SEMath<float>::MAX_REAL;
        Res.m_fData[2] = SEMath<float>::MAX_REAL;
    }

    return Res;
}
//----------------------------------------------------------------------------
SEVector3f SEVector3f::operator-() const
{
    return SEVector3f(
                   -m_fData[0],
                   -m_fData[1],
                   -m_fData[2]);
}
//----------------------------------------------------------------------------
SEVector3f& SEVector3f::operator+=(const SEVector3f& rRhsVec)
{
    m_fData[0] += rRhsVec.m_fData[0];
    m_fData[1] += rRhsVec.m_fData[1];
    m_fData[2] += rRhsVec.m_fData[2];

    return *this;
}
//----------------------------------------------------------------------------
SEVector3f& SEVector3f::operator+=(float fScalar)
{
    m_fData[0] += fScalar;
    m_fData[1] += fScalar;
    m_fData[2] += fScalar;

    return *this;
}
//----------------------------------------------------------------------------
SEVector3f& SEVector3f::operator-=(const SEVector3f& rRhsVec)
{
    m_fData[0] -= rRhsVec.m_fData[0];
    m_fData[1] -= rRhsVec.m_fData[1];
    m_fData[2] -= rRhsVec.m_fData[2];

    return *this;
}
//----------------------------------------------------------------------------
SEVector3f& SEVector3f::operator-=(float fScalar)
{
    m_fData[0] -= fScalar;
    m_fData[1] -= fScalar;
    m_fData[2] -= fScalar;

    return *this;
}
//----------------------------------------------------------------------------
SEVector3f& SEVector3f::operator*=(float fScalar)
{
    m_fData[0] *= fScalar;
    m_fData[1] *= fScalar;
    m_fData[2] *= fScalar;

    return *this;
}
//----------------------------------------------------------------------------
SEVector3f& SEVector3f::operator/=(float fScalar)
{
    if( fScalar != 0.0f )
    {
        float fInvScalar = 1.0f / fScalar;
        m_fData[0] *= fInvScalar;
        m_fData[1] *= fInvScalar;
        m_fData[2] *= fInvScalar;
    }
    else
    {
        m_fData[0] = SEMath<float>::MAX_REAL;
        m_fData[1] = SEMath<float>::MAX_REAL;
        m_fData[2] = SEMath<float>::MAX_REAL;
    }

    return *this;
}
//----------------------------------------------------------------------------
void SEVector3f::Orthonormalize(SEVector3f& rVecU, SEVector3f& rVecV, 
    SEVector3f& rVecW)
{
    rVecU.Normalize();
    // V在U上的投影长度
    float fDot0 = rVecU.Dot(rVecV);
    // V = V减去V在U上的投影向量
    rVecV -= fDot0 * rVecU;
    rVecV.Normalize();
    // W在V上的投影长度
    float fDot1 = rVecV.Dot(rVecW);
	// W在U上的投影长度
    fDot0 = rVecU.Dot(rVecW);
	// W = W减去W在U上的投影向量再减去W在V上的投影向量
    rVecW -= fDot0*rVecU + fDot1*rVecV;
    rVecW.Normalize();
}
//----------------------------------------------------------------------------
void SEVector3f::Orthonormalize(SEVector3f* pVec)
{
	SE_ASSERT( pVec );

    Orthonormalize(pVec[0], pVec[1], pVec[2]);
}
//----------------------------------------------------------------------------
void SEVector3f::GetOrthonormalBasis(SEVector3f& rVecU, SEVector3f& rVecV, 
    SEVector3f& rVecW, bool bUnitLengthW)
{
    if( !bUnitLengthW )
        rVecW.Normalize();

    float fInvLength;

    if( SEMath<float>::FAbs(rVecW.m_fData[0]) >= 
        SEMath<float>::FAbs(rVecW.m_fData[1]) )
    {
        // W.X或W.Z是最大分量
        fInvLength = SEMath<float>::InvSqrt(rVecW.m_fData[0]*rVecW.m_fData[0] + 
            rVecW.m_fData[2]*rVecW.m_fData[2]);

        // 在左手坐标系中,
        // 对于向量(a,b,c),我们想获得与其垂直的顺时针一侧向量为(c,0,-a),
        // 与其垂直的逆时针一侧向量为(-c,0,a)
        rVecU.m_fData[0] = rVecW.m_fData[2] * fInvLength;
        rVecU.m_fData[1] = 0.0f;
        rVecU.m_fData[2] = -rVecW.m_fData[0] * fInvLength;

        // 叉积生成V
        rVecV.m_fData[0] = rVecW.m_fData[1] * rVecU.m_fData[2];
        rVecV.m_fData[1] = rVecW.m_fData[2]*rVecU.m_fData[0] - 
            rVecW.m_fData[0]*rVecU.m_fData[2];
        rVecV.m_fData[2] = -rVecW.m_fData[1] * rVecU.m_fData[0];
    }
    else
    {
        // W.Y或W.Z是最大分量
        fInvLength = SEMath<float>::InvSqrt(rVecW.m_fData[1]*rVecW.m_fData[1] + 
            rVecW.m_fData[2]*rVecW.m_fData[2]);

        rVecU.m_fData[0] = 0.0f;
        rVecU.m_fData[1] = +rVecW.m_fData[2] * fInvLength;
        rVecU.m_fData[2] = -rVecW.m_fData[1] * fInvLength;
        
        // 叉积生成V
		rVecV.m_fData[0] = rVecW.m_fData[1]*rVecU.m_fData[2] - 
            rVecW.m_fData[2]*rVecU.m_fData[1];
        rVecV.m_fData[1] = -rVecW.m_fData[0] * rVecU.m_fData[2];
        rVecV.m_fData[2] = rVecW.m_fData[0] * rVecU.m_fData[1];
    }
}
//----------------------------------------------------------------------------
void SEVector3f::ComputeExtremes(int iCount, const SEVector3f* pPoints, 
    SEVector3f& rMin, SEVector3f& rMax)
{
    SE_ASSERT( iCount > 0 && pPoints );

    rMin = pPoints[0];
    rMax = rMin;
    for( int i = 1; i < iCount; i++ )
    {
        const SEVector3f& rPoint = pPoints[i];
        for( int j = 0; j < 3; j++ )
        {
            if( rPoint[j] < rMin[j] )
            {
                rMin[j] = rPoint[j];
            }
            else if( rPoint[j] > rMax[j] )
            {
                rMax[j] = rPoint[j];
            }
        }
    }
}
//----------------------------------------------------------------------------
void SEVector3f::Faceforward(const SEVector3f& rOriginal, 
    const SEVector3f& rReference, SEVector3f& rFlipped)
{
    float fD = rOriginal.Dot(rReference);
    if( fD < 0.0f )
    {
        rFlipped = -rOriginal;
    }
    else
    {
        rFlipped = rOriginal;
    }
}
//----------------------------------------------------------------------------
SEVector3f Swing::operator*(float fLhsScalar, const SEVector3f& rRhsVec)
{
    return SEVector3f(
                   fLhsScalar * rRhsVec[0],
                   fLhsScalar * rRhsVec[1],
                   fLhsScalar * rRhsVec[2]);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 双精度3维向量类
//----------------------------------------------------------------------------

const SEVector3d SEVector3d::ZERO(0.0, 0.0, 0.0);
const SEVector3d SEVector3d::UNIT_X(1.0, 0.0, 0.0);
const SEVector3d SEVector3d::UNIT_Y(0.0, 1.0, 0.0);
const SEVector3d SEVector3d::UNIT_Z(0.0, 0.0, 1.0);

//----------------------------------------------------------------------------
SEVector3d::SEVector3d()
{
    m_dData[0] = 0.0;
    m_dData[1] = 0.0;
    m_dData[2] = 0.0;
}
//----------------------------------------------------------------------------
SEVector3d::SEVector3d(double dX, double dY, double dZ)
{
    m_dData[0] = dX;
    m_dData[1] = dY;
    m_dData[2] = dZ;
}
//----------------------------------------------------------------------------
SEVector3d::SEVector3d(const SEVector3d& rVecV)
{
    m_dData[0] = rVecV.m_dData[0];
    m_dData[1] = rVecV.m_dData[1];
    m_dData[2] = rVecV.m_dData[2];
}
//----------------------------------------------------------------------------
SEVector3d::operator const double*() const
{
    return m_dData;
}
//----------------------------------------------------------------------------
SEVector3d::operator double*()
{
    return m_dData;
}
//----------------------------------------------------------------------------
double SEVector3d::operator[](int i) const
{
    SE_ASSERT( 0 <= i && i <= 2 );

    return m_dData[i];
}
//----------------------------------------------------------------------------
double& SEVector3d::operator[](int i)
{
    SE_ASSERT( 0 <= i && i <= 2 );

    return m_dData[i];
}
//----------------------------------------------------------------------------
SEVector3d& SEVector3d::operator=(const SEVector3d& rVecV)
{
    m_dData[0] = rVecV.m_dData[0];
    m_dData[1] = rVecV.m_dData[1];
    m_dData[2] = rVecV.m_dData[2];

    return *this;
}
//----------------------------------------------------------------------------
bool SEVector3d::operator==(const SEVector3d& rVecV) const
{
    return CompareData(rVecV) == 0;
}
//----------------------------------------------------------------------------
bool SEVector3d::operator!=(const SEVector3d& rVecV) const
{
    return CompareData(rVecV) != 0;
}
//----------------------------------------------------------------------------
bool SEVector3d::operator<(const SEVector3d& rVecV) const
{
    return CompareData(rVecV) < 0;
}
//----------------------------------------------------------------------------
bool SEVector3d::operator<=(const SEVector3d& rVecV) const
{
    return CompareData(rVecV) <= 0;
}
//----------------------------------------------------------------------------
bool SEVector3d::operator>(const SEVector3d& rVecV) const
{
    return CompareData(rVecV) > 0;
}
//----------------------------------------------------------------------------
bool SEVector3d::operator>=(const SEVector3d& rVecV) const
{
    return CompareData(rVecV) >= 0;
}
//----------------------------------------------------------------------------
SEVector3d SEVector3d::operator+(const SEVector3d& rRhsVec) const
{
    return SEVector3d(
                   m_dData[0] + rRhsVec.m_dData[0],
                   m_dData[1] + rRhsVec.m_dData[1],
                   m_dData[2] + rRhsVec.m_dData[2]);
}
//----------------------------------------------------------------------------
SEVector3d SEVector3d::operator-(const SEVector3d& rRhsVec) const
{
    return SEVector3d(
                   m_dData[0] - rRhsVec.m_dData[0],
                   m_dData[1] - rRhsVec.m_dData[1],
                   m_dData[2] - rRhsVec.m_dData[2]);
}
//----------------------------------------------------------------------------
SEVector3d SEVector3d::operator*(double dScalar) const
{
    return SEVector3d(
                   dScalar * m_dData[0],
                   dScalar * m_dData[1],
                   dScalar * m_dData[2]);
}
//----------------------------------------------------------------------------
SEVector3d SEVector3d::operator/(double dScalar) const
{
    SEVector3d Res;

    if( dScalar != 0.0 )
    {
        double fInvScalar = 1.0/dScalar;
        Res.m_dData[0] = fInvScalar * m_dData[0];
        Res.m_dData[1] = fInvScalar * m_dData[1];
        Res.m_dData[2] = fInvScalar * m_dData[2];
    }
    else
    {
        Res.m_dData[0] = SEMath<double>::MAX_REAL;
        Res.m_dData[1] = SEMath<double>::MAX_REAL;
        Res.m_dData[2] = SEMath<double>::MAX_REAL;
    }

    return Res;
}
//----------------------------------------------------------------------------
SEVector3d SEVector3d::operator-() const
{
    return SEVector3d(
                   -m_dData[0],
                   -m_dData[1],
                   -m_dData[2]);
}
//----------------------------------------------------------------------------
SEVector3d& SEVector3d::operator+=(const SEVector3d& rRhsVec)
{
    m_dData[0] += rRhsVec.m_dData[0];
    m_dData[1] += rRhsVec.m_dData[1];
    m_dData[2] += rRhsVec.m_dData[2];

    return *this;
}
//----------------------------------------------------------------------------
SEVector3d& SEVector3d::operator+=(double dScalar)
{
    m_dData[0] += dScalar;
    m_dData[1] += dScalar;
    m_dData[2] += dScalar;

    return *this;
}
//----------------------------------------------------------------------------
SEVector3d& SEVector3d::operator-=(const SEVector3d& rRhsVec)
{
    m_dData[0] -= rRhsVec.m_dData[0];
    m_dData[1] -= rRhsVec.m_dData[1];
    m_dData[2] -= rRhsVec.m_dData[2];

    return *this;
}
//----------------------------------------------------------------------------
SEVector3d& SEVector3d::operator-=(double dScalar)
{
    m_dData[0] -= dScalar;
    m_dData[1] -= dScalar;
    m_dData[2] -= dScalar;

    return *this;
}
//----------------------------------------------------------------------------
SEVector3d& SEVector3d::operator*=(double dScalar)
{
    m_dData[0] *= dScalar;
    m_dData[1] *= dScalar;
    m_dData[2] *= dScalar;

    return *this;
}
//----------------------------------------------------------------------------
SEVector3d& SEVector3d::operator/=(double dScalar)
{
    if( dScalar != 0.0 )
    {
        double dInvScalar = 1.0 / dScalar;
        m_dData[0] *= dInvScalar;
        m_dData[1] *= dInvScalar;
        m_dData[2] *= dInvScalar;
    }
    else
    {
        m_dData[0] = SEMath<double>::MAX_REAL;
        m_dData[1] = SEMath<double>::MAX_REAL;
        m_dData[2] = SEMath<double>::MAX_REAL;
    }

    return *this;
}
//----------------------------------------------------------------------------
void SEVector3d::Orthonormalize(SEVector3d& rVecU, SEVector3d& rVecV, 
    SEVector3d& rVecW)
{
    rVecU.Normalize();
    // V在U上的投影长度
    double dDot0 = rVecU.Dot(rVecV);
    // V = V减去V在U上的投影向量
    rVecV -= dDot0 * rVecU;
    rVecV.Normalize();
    // W在V上的投影长度
    double dDot1 = rVecV.Dot(rVecW);
	// W在U上的投影长度
    dDot0 = rVecU.Dot(rVecW);
	// W = W减去W在U上的投影向量再减去W在V上的投影向量
    rVecW -= dDot0*rVecU + dDot1*rVecV;
    rVecW.Normalize();
}
//----------------------------------------------------------------------------
void SEVector3d::Orthonormalize(SEVector3d* pVec)
{
	SE_ASSERT( pVec );

    Orthonormalize(pVec[0], pVec[1], pVec[2]);
}
//----------------------------------------------------------------------------
void SEVector3d::GetOrthonormalBasis(SEVector3d& rVecU, SEVector3d& rVecV, 
    SEVector3d& rVecW, bool bUnitLengthW)
{
    if( !bUnitLengthW )
        rVecW.Normalize();

    double dInvLength;

    if( SEMath<double>::FAbs(rVecW.m_dData[0]) >= 
        SEMath<double>::FAbs(rVecW.m_dData[1]) )
    {
        // W.X或W.Z是最大分量
        dInvLength = SEMath<double>::InvSqrt(rVecW.m_dData[0]*rVecW.m_dData[0] + 
            rVecW.m_dData[2]*rVecW.m_dData[2]);

        // 在左手坐标系中,
        // 对于向量(a,b,c),我们想获得与其垂直的顺时针一侧向量为(c,0,-a),
        // 与其垂直的逆时针一侧向量为(-c,0,a)
        rVecU.m_dData[0] = rVecW.m_dData[2] * dInvLength;
        rVecU.m_dData[1] = 0.0;
        rVecU.m_dData[2] = -rVecW.m_dData[0] * dInvLength;

        // 叉积生成V
        rVecV.m_dData[0] = rVecW.m_dData[1] * rVecU.m_dData[2];
        rVecV.m_dData[1] = rVecW.m_dData[2]*rVecU.m_dData[0] - 
            rVecW.m_dData[0]*rVecU.m_dData[2];
        rVecV.m_dData[2] = -rVecW.m_dData[1] * rVecU.m_dData[0];
    }
    else
    {
        // W.Y或W.Z是最大分量
        dInvLength = SEMath<double>::InvSqrt(rVecW.m_dData[1]*rVecW.m_dData[1] + 
            rVecW.m_dData[2]*rVecW.m_dData[2]);

        rVecU.m_dData[0] = 0.0;
        rVecU.m_dData[1] = +rVecW.m_dData[2] * dInvLength;
        rVecU.m_dData[2] = -rVecW.m_dData[1] * dInvLength;
        
        // 叉积生成V
		rVecV.m_dData[0] = rVecW.m_dData[1]*rVecU.m_dData[2] - 
            rVecW.m_dData[2]*rVecU.m_dData[1];
        rVecV.m_dData[1] = -rVecW.m_dData[0] * rVecU.m_dData[2];
        rVecV.m_dData[2] = rVecW.m_dData[0] * rVecU.m_dData[1];
    }
}
//----------------------------------------------------------------------------
void SEVector3d::ComputeExtremes(int iCount, const SEVector3d* pPoints, 
    SEVector3d& rMin, SEVector3d& rMax)
{
    SE_ASSERT( iCount > 0 && pPoints );

    rMin = pPoints[0];
    rMax = rMin;
    for( int i = 1; i < iCount; i++ )
    {
        const SEVector3d& rPoint = pPoints[i];
        for( int j = 0; j < 3; j++ )
        {
            if( rPoint[j] < rMin[j] )
            {
                rMin[j] = rPoint[j];
            }
            else if( rPoint[j] > rMax[j] )
            {
                rMax[j] = rPoint[j];
            }
        }
    }
}
//----------------------------------------------------------------------------
SEVector3d Swing::operator*(double dLhsScalar, const SEVector3d& rRhsVec)
{
    return SEVector3d(
                   dLhsScalar * rRhsVec[0],
                   dLhsScalar * rRhsVec[1],
                   dLhsScalar * rRhsVec[2]);
}
//----------------------------------------------------------------------------