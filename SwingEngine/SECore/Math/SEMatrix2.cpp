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
#include "SEMatrix2.h"

using namespace Swing;

//----------------------------------------------------------------------------
// 单精度2阶方阵类
//----------------------------------------------------------------------------

const SEMatrix2f SEMatrix2f::ZERO(
    0.0f, 0.0f, 
    0.0f, 0.0f);
const SEMatrix2f SEMatrix2f::IDENTITY(
    1.0f, 0.0f, 
    0.0f, 1.0f);

//----------------------------------------------------------------------------
SEMatrix2f::SEMatrix2f()
{
    Zero();
}
//----------------------------------------------------------------------------
SEMatrix2f::SEMatrix2f(const SEMatrix2f& rMat)
{
    memcpy(m_fData, rMat.m_fData, 4*sizeof(float));
}
//----------------------------------------------------------------------------
SEMatrix2f::SEMatrix2f(float fM11, float fM12, float fM21, float fM22)
{
    m_fData[0][0] = fM11;
    m_fData[0][1] = fM12;
    m_fData[1][0] = fM21;
    m_fData[1][1] = fM22;
}
//----------------------------------------------------------------------------
SEMatrix2f::SEMatrix2f(const SEVector2f& rVecU, const SEVector2f& rVecV, bool 
    bIsRow)
{
    if( bIsRow )
	{
        m_fData[0][0] = rVecU[0];
        m_fData[0][1] = rVecU[1];
        m_fData[1][0] = rVecV[0];
        m_fData[1][1] = rVecV[1];
    }
	else
    {
        m_fData[0][0] = rVecU[0];
        m_fData[0][1] = rVecV[0];
        m_fData[1][0] = rVecU[1];
        m_fData[1][1] = rVecV[1];
	}
}
//----------------------------------------------------------------------------
SEMatrix2f::SEMatrix2f(float fM11, float fM22)
{
    Diagonal(fM11, fM22);
}
//----------------------------------------------------------------------------
SEMatrix2f::SEMatrix2f(float fAngle)
{
    FromAngle(fAngle);
}
//----------------------------------------------------------------------------
SEMatrix2f::SEMatrix2f(const SEVector2f& rVecU, const SEVector2f& rVecV)
{
    TensorProduct(rVecU, rVecV);
}
//----------------------------------------------------------------------------
SEMatrix2f::operator const float*() const
{
    return &m_fData[0][0];
}
//----------------------------------------------------------------------------
SEMatrix2f::operator float*()
{
    return &m_fData[0][0];
}
//----------------------------------------------------------------------------
const float* SEMatrix2f::operator[](int iCurRow) const
{
    return &m_fData[iCurRow][0];
}
//----------------------------------------------------------------------------
float* SEMatrix2f::operator[](int iCurRow)
{
    return &m_fData[iCurRow][0];
}
//----------------------------------------------------------------------------
float SEMatrix2f::operator()(int iCurRow, int iCurCol) const
{
    return m_fData[iCurRow][iCurCol];
}
//----------------------------------------------------------------------------
float& SEMatrix2f::operator()(int iCurRow, int iCurCol)
{
    return m_fData[iCurRow][iCurCol];
}
//----------------------------------------------------------------------------
SEMatrix2f& SEMatrix2f::operator=(const SEMatrix2f& rMat)
{
    memcpy(m_fData, rMat.m_fData, 4*sizeof(float));

    return *this;
}
//----------------------------------------------------------------------------
bool SEMatrix2f::operator==(const SEMatrix2f& rMat) const
{
    return CompareData(rMat) == 0;
}
//----------------------------------------------------------------------------
bool SEMatrix2f::operator!=(const SEMatrix2f& rMat) const
{
    return CompareData(rMat) != 0;
}
//----------------------------------------------------------------------------
bool SEMatrix2f::operator<(const SEMatrix2f& rMat) const
{
    return CompareData(rMat) < 0;
}
//----------------------------------------------------------------------------
bool SEMatrix2f::operator<=(const SEMatrix2f& rMat) const
{
    return CompareData(rMat) <= 0;
}
//----------------------------------------------------------------------------
bool SEMatrix2f::operator>(const SEMatrix2f& rMat) const
{
    return CompareData(rMat) > 0;
}
//----------------------------------------------------------------------------
bool SEMatrix2f::operator>=(const SEMatrix2f& rMat) const
{
    return CompareData(rMat) >= 0;
}
//----------------------------------------------------------------------------
SEMatrix2f SEMatrix2f::operator+(const SEMatrix2f& rRhsMat) const
{
    SEMatrix2f ResMat;

    ResMat.M11 = M11 + rRhsMat.M11;
    ResMat.M12 = M12 + rRhsMat.M12;

    ResMat.M21 = M21 + rRhsMat.M21;
    ResMat.M22 = M22 + rRhsMat.M22;

    return ResMat;
}
//----------------------------------------------------------------------------
SEMatrix2f SEMatrix2f::operator-(const SEMatrix2f& rRhsMat) const
{
    SEMatrix2f ResMat;

    ResMat.M11 = M11 - rRhsMat.M11;
    ResMat.M12 = M12 - rRhsMat.M12;

    ResMat.M21 = M21 - rRhsMat.M21;
    ResMat.M22 = M22 - rRhsMat.M22;

    return ResMat;
}
//----------------------------------------------------------------------------
SEMatrix2f SEMatrix2f::operator*(const SEMatrix2f& rRhsMat) const
{
    SEMatrix2f ResMat;

    ResMat.M11 = M11*rRhsMat.M11 + M12*rRhsMat.M21;
    ResMat.M12 = M11*rRhsMat.M12 + M12*rRhsMat.M22;

    ResMat.M21 = M21*rRhsMat.M11 + M22*rRhsMat.M21;
    ResMat.M22 = M21*rRhsMat.M12 + M22*rRhsMat.M22;

    return ResMat;
}
//----------------------------------------------------------------------------
SEMatrix2f SEMatrix2f::operator*(float fScalar) const
{
    SEMatrix2f ResMat;

    ResMat.M11 = M11 * fScalar;
    ResMat.M12 = M12 * fScalar;

    ResMat.M21 = M21 * fScalar;
    ResMat.M22 = M22 * fScalar;

    return ResMat;
}
//----------------------------------------------------------------------------
SEMatrix2f SEMatrix2f::operator/(float fScalar) const
{
    // 注意除数不能为0
    SEMatrix2f ResMat;
    float fInvScalar = 1.0f / fScalar;

    ResMat.M11 = M11 * fInvScalar;
    ResMat.M12 = M12 * fInvScalar;

    ResMat.M21 = M21 * fInvScalar;
    ResMat.M22 = M22 * fInvScalar;

    return ResMat;
}
//----------------------------------------------------------------------------
SEMatrix2f SEMatrix2f::operator-() const
{
    SEMatrix2f ResMat;

    ResMat.M11 = -M11;
    ResMat.M12 = -M12;

    ResMat.M21 = -M21;
    ResMat.M22 = -M22;

    return ResMat;
}
//----------------------------------------------------------------------------
SEMatrix2f& SEMatrix2f::operator+=(const SEMatrix2f& rRhsMat)
{
	M11 += rRhsMat.M11;
    M12 += rRhsMat.M12;

    M21 += rRhsMat.M21;
    M22 += rRhsMat.M22;

    return *this;
}
//----------------------------------------------------------------------------
SEMatrix2f& SEMatrix2f::operator-=(const SEMatrix2f& rRhsMat)
{
	M11 -= rRhsMat.M11;
    M12 -= rRhsMat.M12;

    M21 -= rRhsMat.M21;
    M22 -= rRhsMat.M22;

    return *this;
}
//----------------------------------------------------------------------------
SEMatrix2f& SEMatrix2f::operator*=(float fScalar)
{
	M11 *= fScalar;
    M12 *= fScalar;

    M21 *= fScalar;
    M22 *= fScalar;

    return *this;
}
//----------------------------------------------------------------------------
SEMatrix2f& SEMatrix2f::operator/=(float fScalar)
{
    // 注意除数不能为0
    float fInvScalar = 1.0f / fScalar;
	M11 *= fInvScalar;
    M12 *= fInvScalar;

    M21 *= fInvScalar;
    M22 *= fInvScalar;

    return *this;
}
//----------------------------------------------------------------------------
SEVector2f SEMatrix2f::operator*(const SEVector2f& rRhsVec) const
{
    SEVector2f ResVec;
    
    ResVec[0] = M11*rRhsVec[0] + M12*rRhsVec[1];
    ResVec[1] = M21*rRhsVec[0] + M22*rRhsVec[1];

    return ResVec;
}
//----------------------------------------------------------------------------
void SEMatrix2f::Transpose()
{
    SEMatrix2f tempMat;
	GetTranspose(tempMat);
	*this = tempMat;
}
//----------------------------------------------------------------------------
void SEMatrix2f::Inverse()
{
    SEMatrix2f tempMat;
	GetInverse(tempMat);
	*this = tempMat;
}
//----------------------------------------------------------------------------
SEMatrix2f Swing::operator*(float fLhsScalar, const SEMatrix2f& rRhsMat)
{
    return rRhsMat * fLhsScalar;
}
//----------------------------------------------------------------------------
SEVector2f Swing::operator*(const SEVector2f& rVecV, const SEMatrix2f& rMat)
{
    return SEVector2f(
                   rVecV[0]*rMat[0][0] + rVecV[1]*rMat[1][0],
                   rVecV[0]*rMat[0][1] + rVecV[1]*rMat[1][1]);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 双精度2阶方阵类
//----------------------------------------------------------------------------

const SEMatrix2d SEMatrix2d::ZERO(
    0.0, 0.0, 
    0.0, 0.0);
const SEMatrix2d SEMatrix2d::IDENTITY(
    1.0, 0.0, 
    0.0, 1.0);

//----------------------------------------------------------------------------
SEMatrix2d::SEMatrix2d()
{
    Zero();
}
//----------------------------------------------------------------------------
SEMatrix2d::SEMatrix2d(const SEMatrix2d& rMat)
{
    memcpy(m_dData, rMat.m_dData, 4*sizeof(double));
}
//----------------------------------------------------------------------------
SEMatrix2d::SEMatrix2d(double dM11, double dM12, double dM21, double dM22)
{
    m_dData[0][0] = dM11;
    m_dData[0][1] = dM12;
    m_dData[1][0] = dM21;
    m_dData[1][1] = dM22;
}
//----------------------------------------------------------------------------
SEMatrix2d::SEMatrix2d(const SEVector2d& rVecU, const SEVector2d& rVecV, bool 
    bIsRow)
{
    if( bIsRow )
	{
        m_dData[0][0] = rVecU[0];
        m_dData[0][1] = rVecU[1];
        m_dData[1][0] = rVecV[0];
        m_dData[1][1] = rVecV[1];
    }
	else
    {
        m_dData[0][0] = rVecU[0];
        m_dData[0][1] = rVecV[0];
        m_dData[1][0] = rVecU[1];
        m_dData[1][1] = rVecV[1];
	}
}
//----------------------------------------------------------------------------
SEMatrix2d::SEMatrix2d(double dM11, double dM22)
{
    Diagonal(dM11, dM22);
}
//----------------------------------------------------------------------------
SEMatrix2d::SEMatrix2d(double dAngle)
{
    FromAngle(dAngle);
}
//----------------------------------------------------------------------------
SEMatrix2d::SEMatrix2d(const SEVector2d& rVecU, const SEVector2d& rVecV)
{
    TensorProduct(rVecU, rVecV);
}
//----------------------------------------------------------------------------
SEMatrix2d::operator const double*() const
{
    return &m_dData[0][0];
}
//----------------------------------------------------------------------------
SEMatrix2d::operator double*()
{
    return &m_dData[0][0];
}
//----------------------------------------------------------------------------
const double* SEMatrix2d::operator[](int iCurRow) const
{
    return &m_dData[iCurRow][0];
}
//----------------------------------------------------------------------------
double* SEMatrix2d::operator[](int iCurRow)
{
    return &m_dData[iCurRow][0];
}
//----------------------------------------------------------------------------
double SEMatrix2d::operator()(int iCurRow, int iCurCol) const
{
    return m_dData[iCurRow][iCurCol];
}
//----------------------------------------------------------------------------
double& SEMatrix2d::operator()(int iCurRow, int iCurCol)
{
    return m_dData[iCurRow][iCurCol];
}
//----------------------------------------------------------------------------
SEMatrix2d& SEMatrix2d::operator=(const SEMatrix2d& rMat)
{
    memcpy(m_dData, rMat.m_dData, 4*sizeof(double));

    return *this;
}
//----------------------------------------------------------------------------
bool SEMatrix2d::operator==(const SEMatrix2d& rMat) const
{
    return CompareData(rMat) == 0;
}
//----------------------------------------------------------------------------
bool SEMatrix2d::operator!=(const SEMatrix2d& rMat) const
{
    return CompareData(rMat) != 0;
}
//----------------------------------------------------------------------------
bool SEMatrix2d::operator<(const SEMatrix2d& rMat) const
{
    return CompareData(rMat) < 0;
}
//----------------------------------------------------------------------------
bool SEMatrix2d::operator<=(const SEMatrix2d& rMat) const
{
    return CompareData(rMat) <= 0;
}
//----------------------------------------------------------------------------
bool SEMatrix2d::operator>(const SEMatrix2d& rMat) const
{
    return CompareData(rMat) > 0;
}
//----------------------------------------------------------------------------
bool SEMatrix2d::operator>=(const SEMatrix2d& rMat) const
{
    return CompareData(rMat) >= 0;
}
//----------------------------------------------------------------------------
SEMatrix2d SEMatrix2d::operator+(const SEMatrix2d& rRhsMat) const
{
    SEMatrix2d ResMat;

    ResMat.M11 = M11 + rRhsMat.M11;
    ResMat.M12 = M12 + rRhsMat.M12;

    ResMat.M21 = M21 + rRhsMat.M21;
    ResMat.M22 = M22 + rRhsMat.M22;

    return ResMat;
}
//----------------------------------------------------------------------------
SEMatrix2d SEMatrix2d::operator-(const SEMatrix2d& rRhsMat) const
{
    SEMatrix2d ResMat;

    ResMat.M11 = M11 - rRhsMat.M11;
    ResMat.M12 = M12 - rRhsMat.M12;

    ResMat.M21 = M21 - rRhsMat.M21;
    ResMat.M22 = M22 - rRhsMat.M22;

    return ResMat;
}
//----------------------------------------------------------------------------
SEMatrix2d SEMatrix2d::operator*(const SEMatrix2d& rRhsMat) const
{
    SEMatrix2d ResMat;

    ResMat.M11 = M11*rRhsMat.M11 + M12*rRhsMat.M21;
    ResMat.M12 = M11*rRhsMat.M12 + M12*rRhsMat.M22;

    ResMat.M21 = M21*rRhsMat.M11 + M22*rRhsMat.M21;
    ResMat.M22 = M21*rRhsMat.M12 + M22*rRhsMat.M22;

    return ResMat;
}
//----------------------------------------------------------------------------
SEMatrix2d SEMatrix2d::operator*(double dScalar) const
{
    SEMatrix2d ResMat;

    ResMat.M11 = M11 * dScalar;
    ResMat.M12 = M12 * dScalar;

    ResMat.M21 = M21 * dScalar;
    ResMat.M22 = M22 * dScalar;

    return ResMat;
}
//----------------------------------------------------------------------------
SEMatrix2d SEMatrix2d::operator/(double dScalar) const
{
    // 注意除数不能为0
    SEMatrix2d ResMat;
    double dInvScalar = 1.0 / dScalar;

    ResMat.M11 = M11 * dInvScalar;
    ResMat.M12 = M12 * dInvScalar;

    ResMat.M21 = M21 * dInvScalar;
    ResMat.M22 = M22 * dInvScalar;

    return ResMat;
}
//----------------------------------------------------------------------------
SEMatrix2d SEMatrix2d::operator-() const
{
    SEMatrix2d ResMat;

    ResMat.M11 = -M11;
    ResMat.M12 = -M12;

    ResMat.M21 = -M21;
    ResMat.M22 = -M22;

    return ResMat;
}
//----------------------------------------------------------------------------
SEMatrix2d& SEMatrix2d::operator+=(const SEMatrix2d& rRhsMat)
{
	M11 += rRhsMat.M11;
    M12 += rRhsMat.M12;

    M21 += rRhsMat.M21;
    M22 += rRhsMat.M22;

    return *this;
}
//----------------------------------------------------------------------------
SEMatrix2d& SEMatrix2d::operator-=(const SEMatrix2d& rRhsMat)
{
	M11 -= rRhsMat.M11;
    M12 -= rRhsMat.M12;

    M21 -= rRhsMat.M21;
    M22 -= rRhsMat.M22;

    return *this;
}
//----------------------------------------------------------------------------
SEMatrix2d& SEMatrix2d::operator*=(double dScalar)
{
	M11 *= dScalar;
    M12 *= dScalar;

    M21 *= dScalar;
    M22 *= dScalar;

    return *this;
}
//----------------------------------------------------------------------------
SEMatrix2d& SEMatrix2d::operator/=(double dScalar)
{
    // 注意除数不能为0
    double dInvScalar = 1.0 / dScalar;
	M11 *= dInvScalar;
    M12 *= dInvScalar;

    M21 *= dInvScalar;
    M22 *= dInvScalar;

    return *this;
}
//----------------------------------------------------------------------------
SEVector2d SEMatrix2d::operator*(const SEVector2d& rRhsVec) const
{
    SEVector2d ResVec;
    
    ResVec[0] = M11*rRhsVec[0] + M12*rRhsVec[1];
    ResVec[1] = M21*rRhsVec[0] + M22*rRhsVec[1];

    return ResVec;
}
//----------------------------------------------------------------------------
void SEMatrix2d::Transpose()
{
    SEMatrix2d tempMat;
	GetTranspose(tempMat);
	*this = tempMat;
}
//----------------------------------------------------------------------------
void SEMatrix2d::Inverse()
{
    SEMatrix2d tempMat;
	GetInverse(tempMat);
	*this = tempMat;
}
//----------------------------------------------------------------------------
SEMatrix2d Swing::operator*(double dLhsScalar, const SEMatrix2d& rRhsMat)
{
    return rRhsMat * dLhsScalar;
}
//----------------------------------------------------------------------------
SEVector2d Swing::operator*(const SEVector2d& rVecV, const SEMatrix2d& rMat)
{
    return SEVector2d(
                   rVecV[0]*rMat[0][0] + rVecV[1]*rMat[1][0],
                   rVecV[0]*rMat[0][1] + rVecV[1]*rMat[1][1]);
}
//----------------------------------------------------------------------------