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
#include "SEMatrix3.h"

using namespace Swing;

//----------------------------------------------------------------------------
// 单精度3阶方阵类
//----------------------------------------------------------------------------

const SEMatrix3f SEMatrix3f::ZERO(0.0f, 0.0f, 0.0f,
                                  0.0f, 0.0f, 0.0f,
                                  0.0f, 0.0f, 0.0f);
const SEMatrix3f SEMatrix3f::IDENTITY(1.0f, 0.0f, 0.0f,
                                      0.0f, 1.0f, 0.0f,
                                      0.0f, 0.0f, 1.0f);

//----------------------------------------------------------------------------
SEMatrix3f::SEMatrix3f()
{
    Zero();
}
//----------------------------------------------------------------------------
SEMatrix3f::SEMatrix3f(const SEMatrix3f& rMat)
{
    memcpy(m_fData, rMat.m_fData, /*12*/9*sizeof(float));
}
//----------------------------------------------------------------------------
SEMatrix3f::SEMatrix3f(float fM11, float fM12, float fM13, 
                   float fM21, float fM22, float fM23, 
                   float fM31, float fM32, float fM33)
{
    m_fData[0][0] = fM11;
    m_fData[0][1] = fM12;
    m_fData[0][2] = fM13;
    //m_fData[0][3] = 0.0f;

    m_fData[1][0] = fM21;
    m_fData[1][1] = fM22;
    m_fData[1][2] = fM23;
    //m_fData[1][3] = 0.0f;

    m_fData[2][0] = fM31;
    m_fData[2][1] = fM32;
    m_fData[2][2] = fM33;
    //m_fData[2][3] = 0.0f;

    //m_fData[3][0] = 0.0f;
    //m_fData[3][1] = 0.0f;
    //m_fData[3][2] = 0.0f;
    //m_fData[3][3] = 0.0f;
}
//----------------------------------------------------------------------------
SEMatrix3f::SEMatrix3f(const SEVector3f& rVecU, const SEVector3f& rVecV, 
    const SEVector3f& rVecW, bool bIsRow)
{
    Zero();

    if( bIsRow )
	{
        m_fData[0][0] = rVecU[0];
        m_fData[0][1] = rVecU[1];
        m_fData[0][2] = rVecU[2];
        m_fData[1][0] = rVecV[0];
        m_fData[1][1] = rVecV[1];
        m_fData[1][2] = rVecV[2];
        m_fData[2][0] = rVecW[0];
        m_fData[2][1] = rVecW[1];
        m_fData[2][2] = rVecW[2];
    }
	else
    {
        m_fData[0][0] = rVecU[0];
        m_fData[0][1] = rVecV[0];
        m_fData[0][2] = rVecW[0];
        m_fData[1][0] = rVecU[1];
        m_fData[1][1] = rVecV[1];
        m_fData[1][2] = rVecW[1];
        m_fData[2][0] = rVecU[2];
        m_fData[2][1] = rVecV[2];
        m_fData[2][2] = rVecW[2];
	}
}
//----------------------------------------------------------------------------
SEMatrix3f::SEMatrix3f(float fM11, float fM22, float fM33)
{
    Zero();

    Diagonal(fM11, fM22, fM33);
}
//----------------------------------------------------------------------------
SEMatrix3f::SEMatrix3f(const SEVector3f& rAxisVec, float fAngle)
{
    Zero();

    FromAxisAngle(rAxisVec, fAngle);
}
//----------------------------------------------------------------------------
SEMatrix3f::SEMatrix3f(const SEVector3f& rVecU, const SEVector3f& rVecV)
{
    Zero();

    TensorProduct(rVecU, rVecV);
}
//----------------------------------------------------------------------------
SEMatrix3f::operator const float*() const
{
    return &m_fData[0][0];
}
//----------------------------------------------------------------------------
SEMatrix3f::operator float*()
{
    return &m_fData[0][0];
}
//----------------------------------------------------------------------------
const float* SEMatrix3f::operator[](int iCurRow) const
{
    return &m_fData[iCurRow][0];
}
//----------------------------------------------------------------------------
float* SEMatrix3f::operator[](int iCurRow)
{
    return &m_fData[iCurRow][0];
}
//----------------------------------------------------------------------------
float SEMatrix3f::operator()(int iCurRow, int iCurCol) const
{
    return m_fData[iCurRow][iCurCol];
}
//----------------------------------------------------------------------------
float& SEMatrix3f::operator()(int iCurRow, int iCurCol)
{
    return m_fData[iCurRow][iCurCol];
}
//----------------------------------------------------------------------------
SEMatrix3f& SEMatrix3f::operator=(const SEMatrix3f& rMat)
{
    memcpy(m_fData, rMat.m_fData, /*12*/9*sizeof(float));

    return *this;
}
//----------------------------------------------------------------------------
bool SEMatrix3f::operator==(const SEMatrix3f& rMat) const
{
    return CompareData(rMat) == 0;
}
//----------------------------------------------------------------------------
bool SEMatrix3f::operator!=(const SEMatrix3f& rMat) const
{
    return CompareData(rMat) != 0;
}
//----------------------------------------------------------------------------
bool SEMatrix3f::operator<(const SEMatrix3f& rMat) const
{
    return CompareData(rMat) < 0;
}
//----------------------------------------------------------------------------
bool SEMatrix3f::operator<=(const SEMatrix3f& rMat) const
{
    return CompareData(rMat) <= 0;
}
//----------------------------------------------------------------------------
bool SEMatrix3f::operator>(const SEMatrix3f& rMat) const
{
    return CompareData(rMat) > 0;
}
//----------------------------------------------------------------------------
bool SEMatrix3f::operator>=(const SEMatrix3f& rMat) const
{
    return CompareData(rMat) >= 0;
}
//----------------------------------------------------------------------------
SEMatrix3f SEMatrix3f::operator+(const SEMatrix3f& rRhsMat) const
{
    SEMatrix3f ResMat;

    ResMat.m_fData[0][0] = m_fData[0][0] + rRhsMat.m_fData[0][0];
    ResMat.m_fData[0][1] = m_fData[0][1] + rRhsMat.m_fData[0][1];
    ResMat.m_fData[0][2] = m_fData[0][2] + rRhsMat.m_fData[0][2];

    ResMat.m_fData[1][0] = m_fData[1][0] + rRhsMat.m_fData[1][0];
    ResMat.m_fData[1][1] = m_fData[1][1] + rRhsMat.m_fData[1][1];
    ResMat.m_fData[1][2] = m_fData[1][2] + rRhsMat.m_fData[1][2];

    ResMat.m_fData[2][0] = m_fData[2][0] + rRhsMat.m_fData[2][0];
    ResMat.m_fData[2][1] = m_fData[2][1] + rRhsMat.m_fData[2][1];
    ResMat.m_fData[2][2] = m_fData[2][2] + rRhsMat.m_fData[2][2];

    return ResMat;
}
//----------------------------------------------------------------------------
SEMatrix3f SEMatrix3f::operator-(const SEMatrix3f& rRhsMat) const
{
    SEMatrix3f ResMat;

    ResMat.m_fData[0][0] = m_fData[0][0] - rRhsMat.m_fData[0][0];
    ResMat.m_fData[0][1] = m_fData[0][1] - rRhsMat.m_fData[0][1];
    ResMat.m_fData[0][2] = m_fData[0][2] - rRhsMat.m_fData[0][2];

    ResMat.m_fData[1][0] = m_fData[1][0] - rRhsMat.m_fData[1][0];
    ResMat.m_fData[1][1] = m_fData[1][1] - rRhsMat.m_fData[1][1];
    ResMat.m_fData[1][2] = m_fData[1][2] - rRhsMat.m_fData[1][2];

    ResMat.m_fData[2][0] = m_fData[2][0] - rRhsMat.m_fData[2][0];
    ResMat.m_fData[2][1] = m_fData[2][1] - rRhsMat.m_fData[2][1];
    ResMat.m_fData[2][2] = m_fData[2][2] - rRhsMat.m_fData[2][2];

    return ResMat;
}
//----------------------------------------------------------------------------
SEMatrix3f SEMatrix3f::operator*(const SEMatrix3f& rRhsMat) const
{
    SEMatrix3f ResMat;

#ifdef _SIMD
    __asm
    {
        mov edi,rRhsMat         // 把右操作数矩阵的首地址指针保存到edi
        movups xmm4,[edi]       // 保存右操作数矩阵的第1行到xmm4
        movups xmm5,[edi+16]    // 保存右操作数矩阵的第2行到xmm5
        movups xmm6,[edi+32]    // 保存右操作数矩阵的第3行到xmm6

        mov esi,this			// 把左操作数矩阵的首地址指针保存到esi

        movss xmm0,[esi]
        // xmm0变为4个重复的左操作数矩阵第1行第1个元素,aaaa
        shufps xmm0,xmm0,0x00
        movss xmm1,[esi+4]
        // xmm1变为4个重复的左操作数矩阵第1行第2个元素,bbbb
        shufps xmm1,xmm1,0x00
        movss xmm2,[esi+8]
        // xmm2变为4个重复的左操作数矩阵第1行第3个元素,cccc
        shufps xmm2,xmm2,0x00

        mulps xmm0,xmm4
        mulps xmm1,xmm5
        mulps xmm2,xmm6

        addps xmm0,xmm1
        addps xmm0,xmm2

        movups ResMat[0],xmm0

        movss xmm0,[esi+16]
        shufps xmm0,xmm0,0x00
        movss xmm1,[esi+20]
        shufps xmm1,xmm1,0x00
        movss xmm2,[esi+24]
        shufps xmm2,xmm2,0x00

        mulps xmm0,xmm4
        mulps xmm1,xmm5
        mulps xmm2,xmm6

        addps xmm0,xmm1
        addps xmm0,xmm2

        movups ResMat[16],xmm0

        movss xmm0,[esi+32]
        shufps xmm0,xmm0,0x00
        movss xmm1,[esi+36]
        shufps xmm1,xmm1,0x00
        movss xmm2,[esi+40]
        shufps xmm2,xmm2,0x00

        mulps xmm0,xmm4
        mulps xmm1,xmm5
        mulps xmm2,xmm6

        addps xmm0,xmm1
        addps xmm0,xmm2

        movups ResMat[32],xmm0
    }
#else
    ResMat.m_fData[0][0] = m_fData[0][0]*rRhsMat.m_fData[0][0] + 
                           m_fData[0][1]*rRhsMat.m_fData[1][0] + 
                           m_fData[0][2]*rRhsMat.m_fData[2][0];
    ResMat.m_fData[0][1] = m_fData[0][0]*rRhsMat.m_fData[0][1] + 
                           m_fData[0][1]*rRhsMat.m_fData[1][1] + 
                           m_fData[0][2]*rRhsMat.m_fData[2][1];
    ResMat.m_fData[0][2] = m_fData[0][0]*rRhsMat.m_fData[0][2] + 
                           m_fData[0][1]*rRhsMat.m_fData[1][2] + 
                           m_fData[0][2]*rRhsMat.m_fData[2][2];

    ResMat.m_fData[1][0] = m_fData[1][0]*rRhsMat.m_fData[0][0] + 
                           m_fData[1][1]*rRhsMat.m_fData[1][0] + 
                           m_fData[1][2]*rRhsMat.m_fData[2][0];
    ResMat.m_fData[1][1] = m_fData[1][0]*rRhsMat.m_fData[0][1] + 
                           m_fData[1][1]*rRhsMat.m_fData[1][1] + 
                           m_fData[1][2]*rRhsMat.m_fData[2][1];
    ResMat.m_fData[1][2] = m_fData[1][0]*rRhsMat.m_fData[0][2] + 
                           m_fData[1][1]*rRhsMat.m_fData[1][2] + 
                           m_fData[1][2]*rRhsMat.m_fData[2][2];

    ResMat.m_fData[2][0] = m_fData[2][0]*rRhsMat.m_fData[0][0] + 
                           m_fData[2][1]*rRhsMat.m_fData[1][0] + 
                           m_fData[2][2]*rRhsMat.m_fData[2][0];
    ResMat.m_fData[2][1] = m_fData[2][0]*rRhsMat.m_fData[0][1] + 
                           m_fData[2][1]*rRhsMat.m_fData[1][1] + 
                           m_fData[2][2]*rRhsMat.m_fData[2][1];
    ResMat.m_fData[2][2] = m_fData[2][0]*rRhsMat.m_fData[0][2] + 
                           m_fData[2][1]*rRhsMat.m_fData[1][2] + 
                           m_fData[2][2]*rRhsMat.m_fData[2][2];
#endif

    return ResMat;
}
//----------------------------------------------------------------------------
SEMatrix3f SEMatrix3f::operator*(float fScalar) const
{
    SEMatrix3f ResMat;

    ResMat.m_fData[0][0] = m_fData[0][0] * fScalar;
    ResMat.m_fData[0][1] = m_fData[0][1] * fScalar;
    ResMat.m_fData[0][2] = m_fData[0][2] * fScalar;

    ResMat.m_fData[1][0] = m_fData[1][0] * fScalar;
    ResMat.m_fData[1][1] = m_fData[1][1] * fScalar;
    ResMat.m_fData[1][2] = m_fData[1][2] * fScalar;

    ResMat.m_fData[2][0] = m_fData[2][0] * fScalar;
    ResMat.m_fData[2][1] = m_fData[2][1] * fScalar;
    ResMat.m_fData[2][2] = m_fData[2][2] * fScalar;

    return ResMat;
}
//----------------------------------------------------------------------------
SEMatrix3f SEMatrix3f::operator/(float fScalar) const
{
    // 注意除数不能为0
    SEMatrix3f ResMat;
    float fInvScalar = 1.0f / fScalar;

    ResMat.m_fData[0][0] = m_fData[0][0] * fInvScalar;
    ResMat.m_fData[0][1] = m_fData[0][1] * fInvScalar;
    ResMat.m_fData[0][2] = m_fData[0][2] * fInvScalar;

    ResMat.m_fData[1][0] = m_fData[1][0] * fInvScalar;
    ResMat.m_fData[1][1] = m_fData[1][1] * fInvScalar;
    ResMat.m_fData[1][2] = m_fData[1][2] * fInvScalar;

    ResMat.m_fData[2][0] = m_fData[2][0] * fInvScalar;
    ResMat.m_fData[2][1] = m_fData[2][1] * fInvScalar;
    ResMat.m_fData[2][2] = m_fData[2][2] * fInvScalar;

    return ResMat;
}
//----------------------------------------------------------------------------
SEMatrix3f SEMatrix3f::operator-() const
{
    SEMatrix3f ResMat;

    ResMat.m_fData[0][0] = -m_fData[0][0];
    ResMat.m_fData[0][1] = -m_fData[0][1];
    ResMat.m_fData[0][2] = -m_fData[0][2];

    ResMat.m_fData[1][0] = -m_fData[1][0];
    ResMat.m_fData[1][1] = -m_fData[1][1];
    ResMat.m_fData[1][2] = -m_fData[1][2];

    ResMat.m_fData[2][0] = -m_fData[2][0];
    ResMat.m_fData[2][1] = -m_fData[2][1];
    ResMat.m_fData[2][2] = -m_fData[2][2];

    return ResMat;
}
//----------------------------------------------------------------------------
SEMatrix3f& SEMatrix3f::operator+=(const SEMatrix3f& rRhsMat)
{
	m_fData[0][0] += rRhsMat.m_fData[0][0];
    m_fData[0][1] += rRhsMat.m_fData[0][1];
    m_fData[0][2] += rRhsMat.m_fData[0][2];

    m_fData[1][0] += rRhsMat.m_fData[1][0];
    m_fData[1][1] += rRhsMat.m_fData[1][1];
    m_fData[1][2] += rRhsMat.m_fData[1][2];

    m_fData[2][0] += rRhsMat.m_fData[2][0];
    m_fData[2][1] += rRhsMat.m_fData[2][1];
    m_fData[2][2] += rRhsMat.m_fData[2][2];

    return *this;
}
//----------------------------------------------------------------------------
SEMatrix3f& SEMatrix3f::operator-=(const SEMatrix3f& rRhsMat)
{
	m_fData[0][0] -= rRhsMat.m_fData[0][0];
    m_fData[0][1] -= rRhsMat.m_fData[0][1];
    m_fData[0][2] -= rRhsMat.m_fData[0][2];

    m_fData[1][0] -= rRhsMat.m_fData[1][0];
    m_fData[1][1] -= rRhsMat.m_fData[1][1];
    m_fData[1][2] -= rRhsMat.m_fData[1][2];

    m_fData[2][0] -= rRhsMat.m_fData[2][0];
    m_fData[2][1] -= rRhsMat.m_fData[2][1];
    m_fData[2][2] -= rRhsMat.m_fData[2][2];

    return *this;
}
//----------------------------------------------------------------------------
SEMatrix3f& SEMatrix3f::operator*=(float fScalar)
{
	m_fData[0][0] *= fScalar;
    m_fData[0][1] *= fScalar;
    m_fData[0][2] *= fScalar;

    m_fData[1][0] *= fScalar;
    m_fData[1][1] *= fScalar;
    m_fData[1][2] *= fScalar;

    m_fData[2][0] *= fScalar;
    m_fData[2][1] *= fScalar;
    m_fData[2][2] *= fScalar;

    return *this;
}
//----------------------------------------------------------------------------
SEMatrix3f& SEMatrix3f::operator/=(float fScalar)
{
    // 注意除数不能为0
    float fInvScalar = 1.0f / fScalar;
	m_fData[0][0] *= fInvScalar;
    m_fData[0][1] *= fInvScalar;
    m_fData[0][2] *= fInvScalar;

    m_fData[1][0] *= fInvScalar;
    m_fData[1][1] *= fInvScalar;
    m_fData[1][2] *= fInvScalar;

    m_fData[2][0] *= fInvScalar;
    m_fData[2][1] *= fInvScalar;
    m_fData[2][2] *= fInvScalar;

    return *this;
}
//----------------------------------------------------------------------------
SEVector3f SEMatrix3f::operator*(const SEVector3f& rRhsVec) const
{
    SEVector3f ResVec;
   
    ResVec[0] = m_fData[0][0]*rRhsVec[0] + m_fData[0][1]*rRhsVec[1] + 
        m_fData[0][2]*rRhsVec[2];
    ResVec[1] = m_fData[1][0]*rRhsVec[0] + m_fData[1][1]*rRhsVec[1] + 
        m_fData[1][2]*rRhsVec[2];
    ResVec[2] = m_fData[2][0]*rRhsVec[0] + m_fData[2][1]*rRhsVec[1] + 
        m_fData[2][2]*rRhsVec[2];

    return ResVec;
}
//----------------------------------------------------------------------------
void SEMatrix3f::Transpose()
{
    SEMatrix3f tempMat;
	GetTranspose(tempMat);
	*this = tempMat;
}
//----------------------------------------------------------------------------
void SEMatrix3f::Inverse()
{
    SEMatrix3f tempMat;
	GetInverse(tempMat);
	*this = tempMat;
}
//----------------------------------------------------------------------------
SEMatrix3f& SEMatrix3f::FromEulerAnglesXYZ(float fYAngle, float fPAngle, 
    float fRAngle)
{
    float fCos, fSin;

    fCos = SEMath<float>::Cos(fYAngle);
    fSin = SEMath<float>::Sin(fYAngle);
    SEMatrix3f RotXMat(
        1.0f,0.0f,0.0f,
        0.0f,fCos,fSin,
        0.0f,-fSin,fCos);

    fCos = SEMath<float>::Cos(fPAngle);
    fSin = SEMath<float>::Sin(fPAngle);
    SEMatrix3f RotYMat(
        fCos,0.0f,-fSin,
        0.0f,1.0f,0.0f,
        fSin,0.0f,fCos);

    fCos = SEMath<float>::Cos(fRAngle);
    fSin = SEMath<float>::Sin(fRAngle);
    SEMatrix3f RotZMat(
        fCos,fSin,0.0f,
        -fSin,fCos,0.0f,
        0.0f,0.0f,1.0f);

    *this = RotXMat*(RotYMat*RotZMat);
    return *this;
}
//----------------------------------------------------------------------------
SEMatrix3f& SEMatrix3f::FromEulerAnglesXZY(float fYAngle, float fPAngle, 
    float fRAngle)
{
    float fCos, fSin;

    fCos = SEMath<float>::Cos(fYAngle);
    fSin = SEMath<float>::Sin(fYAngle);
    SEMatrix3f RotXMat(
        1.0f,0.0f,0.0f,
        0.0f,fCos,fSin,
        0.0f,-fSin,fCos);

    fCos = SEMath<float>::Cos(fPAngle);
    fSin = SEMath<float>::Sin(fPAngle);
    SEMatrix3f RotZMat(
        fCos,fSin,0.0f,
        -fSin,fCos,0.0f,
        0.0f,0.0f,1.0f);

    fCos = SEMath<float>::Cos(fRAngle);
    fSin = SEMath<float>::Sin(fRAngle);
    SEMatrix3f RotYMat(
        fCos,0.0f,-fSin,
        0.0f,1.0f,0.0f,
        fSin,0.0f,fCos);

    *this = RotXMat*(RotZMat*RotYMat);
    return *this;
}
//----------------------------------------------------------------------------
SEMatrix3f& SEMatrix3f::FromEulerAnglesYXZ(float fYAngle, float fPAngle, 
    float fRAngle)
{
    float fCos, fSin;

    fCos = SEMath<float>::Cos(fYAngle);
    fSin = SEMath<float>::Sin(fYAngle);
    SEMatrix3f RotYMat(
        fCos,0.0f,-fSin,
        0.0f,1.0f,0.0f,
        fSin,0.0f,fCos);

    fCos = SEMath<float>::Cos(fPAngle);
    fSin = SEMath<float>::Sin(fPAngle);
    SEMatrix3f RotXMat(
        1.0f,0.0f,0.0f,
        0.0f,fCos,fSin,
        0.0f,-fSin,fCos);

    fCos = SEMath<float>::Cos(fRAngle);
    fSin = SEMath<float>::Sin(fRAngle);
    SEMatrix3f RotZMat(
        fCos,fSin,0.0f,
        -fSin,fCos,0.0f,
        0.0f,0.0f,1.0f);

    *this = RotYMat*(RotXMat*RotZMat);
    return *this;
}
//----------------------------------------------------------------------------
SEMatrix3f& SEMatrix3f::FromEulerAnglesYZX(float fYAngle, float fPAngle, 
    float fRAngle)
{
    float fCos, fSin;

    fCos = SEMath<float>::Cos(fYAngle);
    fSin = SEMath<float>::Sin(fYAngle);
    SEMatrix3f RotYMat(
        fCos,0.0f,-fSin,
        0.0f,1.0f,0.0f,
        fSin,0.0f,fCos);

    fCos = SEMath<float>::Cos(fPAngle);
    fSin = SEMath<float>::Sin(fPAngle);
    SEMatrix3f RotZMat(
        fCos,fSin,0.0f,
        -fSin,fCos,0.0f,
        0.0f,0.0f,1.0f);

    fCos = SEMath<float>::Cos(fRAngle);
    fSin = SEMath<float>::Sin(fRAngle);
    SEMatrix3f RotXMat(
        1.0f,0.0f,0.0f,
        0.0f,fCos,fSin,
        0.0f,-fSin,fCos);

    *this = RotYMat*(RotZMat*RotXMat);
    return *this;
}
//----------------------------------------------------------------------------
SEMatrix3f& SEMatrix3f::FromEulerAnglesZXY(float fYAngle, float fPAngle, 
    float fRAngle)
{
    float fCos, fSin;

    fCos = SEMath<float>::Cos(fYAngle);
    fSin = SEMath<float>::Sin(fYAngle);
    SEMatrix3f RotZMat(
        fCos,fSin,0.0f,
        -fSin,fCos,0.0f,
        0.0f,0.0f,1.0f);

    fCos = SEMath<float>::Cos(fPAngle);
    fSin = SEMath<float>::Sin(fPAngle);
    SEMatrix3f RotXMat(
        1.0f,0.0f,0.0f,
        0.0f,fCos,fSin,
        0.0f,-fSin,fCos);

    fCos = SEMath<float>::Cos(fRAngle);
    fSin = SEMath<float>::Sin(fRAngle);
    SEMatrix3f RotYMat(
        fCos,0.0f,-fSin,
        0.0f,1.0f,0.0f,
        fSin,0.0f,fCos);

    *this = RotZMat*(RotXMat*RotYMat);
    return *this;
}
//----------------------------------------------------------------------------
SEMatrix3f& SEMatrix3f::FromEulerAnglesZYX(float fYAngle, float fPAngle, 
    float fRAngle)
{
    float fCos, fSin;

    fCos = SEMath<float>::Cos(fYAngle);
    fSin = SEMath<float>::Sin(fYAngle);
    SEMatrix3f RotZMat(
        fCos,fSin,0.0f,
        -fSin,fCos,0.0f,
        0.0f,0.0f,1.0f);

    fCos = SEMath<float>::Cos(fPAngle);
    fSin = SEMath<float>::Sin(fPAngle);
    SEMatrix3f RotYMat(
        fCos,0.0f,-fSin,
        0.0f,1.0f,0.0f,
        fSin,0.0f,fCos);

    fCos = SEMath<float>::Cos(fRAngle);
    fSin = SEMath<float>::Sin(fRAngle);
    SEMatrix3f RotXMat(
        1.0f,0.0f,0.0f,
        0.0f,fCos,fSin,
        0.0f,-fSin,fCos);

    *this = RotZMat*(RotYMat*RotXMat);
    return *this;
}
//----------------------------------------------------------------------------
bool SEMatrix3f::ToEulerAnglesXYZ(float& rfXAngle, float& rfYAngle, 
    float& rfZAngle) const
{
    // R = Rx * Ry * Rz
    //   = cy*cz           cy*sz          -sy
    //     cz*sx*sy-cx*sz  cx*cz+sx*sy*sz  sx*cy
    //     cx*cz*sy+sx*sz  cx*sy*sz-cz*sx  cx*cy
    //
    // 如果y = pi/2,则sy = 1,cy = 0
    // R = 0            0           -1
    //     cz*sx-cx*sz  cx*cz+sx*sz  0
    //     cx*cz+sx*sz  cx*sz-cz*sx  0
    //   = 0            0           -1
    //     sin(x-z)     cos(x-z)     0
    //     cos(x-z)     -sin(x-z)    0
    //
    // 如果y = -pi/2,则sy = -1,cy = 0
    // R =  0            0            1
    //     -cz*sx-cx*sz  cx*cz-sx*sz  0
    //     -cx*cz+sx*sz -cx*sz-cz*sx  0
    //   =  0            0            1
    //     -sin(x+z)     cos(x+z)     0
    //     -cos(x+z)    -sin(x+z)     0

    if( m_fData[0][2] > -1.0f )
    {
        if( m_fData[0][2] < 1.0f )
        {
            rfXAngle = SEMath<float>::ATan2(m_fData[1][2], m_fData[2][2]);
            rfYAngle = (float)asin(-(double)m_fData[0][2]);
            rfZAngle = SEMath<float>::ATan2(m_fData[0][1], m_fData[0][0]);

            return true;
        }
        else
        {
            // y = -pi/2,
            // 结果不唯一,
            // x + z = atan2(r10,r11)
            rfXAngle = SEMath<float>::ATan2(-m_fData[1][0], m_fData[1][1]);
            rfYAngle = -SEMath<float>::HALF_PI;
            rfZAngle = 0.0f;

            return false;
        }
    }
    else
    {
        // y = pi/2,
        // 结果不唯一,
        // x - z = atan2(r10,r11)
        rfXAngle = SEMath<float>::ATan2(m_fData[1][0], m_fData[1][1]);
        rfYAngle = SEMath<float>::HALF_PI;
        rfZAngle = 0.0f;

        return false;
    }
}
//----------------------------------------------------------------------------
bool SEMatrix3f::ToEulerAnglesXZY(float& rfXAngle, float& rfZAngle, 
    float& rfYAngle) const
{
    // R = Rx * Rz * Ry
    //   =  cy*cz           sz             -cz*sy
    //     -cx*cy*sz+sx*sy  cx*cz           cx*sy*sz+cy*sx
    //      cy*sx*sz+cx*sy -cz*sx          -sx*sy*sz+cx*cy
    //
    // 如果z = pi/2,则sz = 1,cz = 0
    // R = 0            1            0
    //     sx*sy-cx*cy  0            cx*sy+cy*sx
    //     cy*sx+cx*sy  0            cx*cy-sx*sy
    //   =  0            1           0
    //     -cos(x+y)     0           sin(x+y)
    //      sin(x+y)     0           cos(x+y)
    //
    // 如果z = -pi/2,则sz = -1,cz = 0
    // R =  0           -1            0
    //      cx*cy+sx*sy  0           -cx*sy+cy*sx
    //     -cy*sx+cx*sy  0            sx*sz+cx*cy
    //   =  0           -1            0
    //      cos(x-y)     0            sin(x-y)
    //     -sin(x-y)     0            cos(x-y)

    if( m_fData[0][1] > -1.0f )
    {
        if( m_fData[0][1] < 1.0f )
        {
            rfXAngle = SEMath<float>::ATan2(-m_fData[2][1], m_fData[1][1]);
            rfZAngle = (float)asin((double)m_fData[0][1]);
            rfYAngle = SEMath<float>::ATan2(-m_fData[0][2], m_fData[0][0]);

            return true;
        }
        else
        {
            // z = pi/2,
            // 结果不唯一,
            // x + y = atan2(r20,r22)
            rfXAngle = SEMath<float>::ATan2(m_fData[2][0], m_fData[2][2]);
            rfZAngle = SEMath<float>::HALF_PI;
            rfYAngle = 0.0f;

            return false;
        }
    }
    else
    {
        // z = -pi/2,
        // 结果不唯一,
        // x - y = atan2(-r20,r22)
        rfXAngle = SEMath<float>::ATan2(-m_fData[2][0], m_fData[2][2]);
        rfZAngle = -SEMath<float>::HALF_PI;
        rfYAngle = 0.0f;

        return false;
    }
}
//----------------------------------------------------------------------------
bool SEMatrix3f::ToEulerAnglesYXZ(float& rfYAngle, float& rfXAngle, 
    float& rfZAngle) const
{
    // R = Ry * Rx * Rz
    //   =  cy*cz-sx*sy*sz  cy*sz+cz*sx*sy -cx*sy
    //     -cx*sz           cx*cz           sx
    //      cz*sy+cy*sx*sz  sy*sz-cy*cz*sx  cx*cy
    //
    // 如果x = pi/2,则sx = 1,cx = 0
    // R = cy*cz-sy*sz  cy*sz+cz*sy  0
    //     0            0            1
    //     cz*sy+cy*sz  sy*sz-cy*cz  0
    //   = cos(y+z)     sin(y+z)    0
    //     0            0           1
    //     sin(y+z)    -cos(y+z)    0
    //
    // 如果x = -pi/2,则sx = -1,cx = 0
    // R = cy*cz+sy*sz  cy*sz-cz*sy  0
    //     0            0           -1
    //     cz*sy-cy*sz  sy*sz+cy*cz  0
    //   =  cos(y-z)    -sin(y-z)     0
    //      0            0           -1
    //      sin(y-z)     cos(y-z)     0

    if( m_fData[1][2] > -1.0f )
    {
        if( m_fData[1][2] < 1.0f )
        {
            rfYAngle = SEMath<float>::ATan2(-m_fData[0][2], m_fData[2][2]);
            rfXAngle = (float)asin((double)m_fData[1][2]);
            rfZAngle = SEMath<float>::ATan2(-m_fData[1][0], m_fData[1][1]);

            return true;
        }
        else
        {
            // x = pi/2,
            // 结果不唯一,
            // y + z = atan2(r01,r00)
            rfYAngle = SEMath<float>::ATan2(m_fData[0][1], m_fData[0][0]);
            rfXAngle = SEMath<float>::HALF_PI;
            rfZAngle = 0.0f;

            return false;
        }
    }
    else
    {
        // x = -pi/2,
        // 结果不唯一,
        // y - z = atan2(-r01,r00)
        rfYAngle = SEMath<float>::ATan2(-m_fData[0][1], m_fData[0][0]);
        rfXAngle = -SEMath<float>::HALF_PI;
        rfZAngle = 0.0f;

        return false;
    }
}
//----------------------------------------------------------------------------
bool SEMatrix3f::ToEulerAnglesYZX(float& rfYAngle, float& rfZAngle, 
    float& rfXAngle) const
{
    // R = Ry * Rz * Rx
    //   =  cy*cz           cx*cy*sz+sx*sy  cy*sx*sz-cx*sy
    //     -sz              cx*cz           cz*sx
    //      cz*sy           cx*sy*sz-cy*sx  sx*sy*sz+cx*cy
    //
    // 如果z = pi/2,则sz = 1,cz = 0
    // R =  0            cx*cy+sx*sy  cy*sx-cx*sy
    //     -1            0            0
    //      0            cx*sy-cy*sx  sx*sy+cx*cy
    //   =  0            cos(x-y)    sin(x-y)
    //     -1            0           0
    //      0           -sin(x-y)    cos(x-y)
    //
    // 如果z = -pi/2,则sz = -1,cz = 0
    // R = 0           -cx*cy+sx*sy -cy*sx-cx*sy
    //     1            0            0
    //     0           -cx*sy-cy*sx -sx*sy+cx*cy
    //   =  0           -cos(x+y)    -sin(x+y)
    //      1            0            0
    //      0           -sin(x+y)     cos(x+y)

    if( m_fData[1][0] > -1.0f )
    {
        if( m_fData[1][0] < 1.0f )
        {
            rfYAngle = SEMath<float>::ATan2(m_fData[2][0], m_fData[0][0]);
            rfZAngle = (float)asin(-(double)m_fData[1][0]);
            rfXAngle = SEMath<float>::ATan2(m_fData[1][2], m_fData[1][1]);

            return true;
        }
        else
        {
            // z = -pi/2,
            // 结果不唯一,
            // x + y = atan2(-r21,r22)
            rfYAngle = SEMath<float>::ATan2(-m_fData[2][1], m_fData[2][2]);
            rfZAngle = -SEMath<float>::HALF_PI;
            rfXAngle = 0.0f;

            return false;
        }
    }
    else
    {
        // z = pi/2,
        // 结果不唯一,
        // x - y = atan2(-r21,r22)
        rfYAngle = -SEMath<float>::ATan2(-m_fData[2][1], m_fData[2][2]);
        rfZAngle = SEMath<float>::HALF_PI;
        rfXAngle = 0.0f;

        return false;
    }
}
//----------------------------------------------------------------------------
bool SEMatrix3f::ToEulerAnglesZXY(float& rfZAngle, float& rfXAngle, 
    float& rfYAngle) const
{
    // R = Rz * Rx * Ry
    //   =  cy*cz+sx*sy*sz  cx*sz          -cz*sy+cy*sx*sz
    //     -cy*sz+cz*sx*sy  cx*cz           sy*sz+cy*cz*sx
    //      cx*sy          -sx              cx*cy
    //
    // 如果x = pi/2,则sx = 1,cx = 0
    // R =  cy*cz+sy*sz  0           -cz*sy+cy*sz
    //     -cy*sz+cz*sy  0            sy*sz+cy*cz
    //      0           -1            0
    //   =  cos(y-z)     0          -sin(y-z)
    //      sin(y-z)     0           cos(y-z)
    //      0           -1           0
    //
    // 如果x = -pi/2,则sx = -1,cx = 0
    // R =  cy*cz-sy*sz  0           -cz*sy-cy*sz
    //     -cy*sz-cz*sy  0            sy*sz-cy*cz
    //      0            1            0
    //   =  cos(y+z)     0           -sin(y+z)
    //     -sin(y+z)     0           -cos(y+z)
    //      0            1            0

    if( m_fData[2][1] > -1.0f )
    {
        if( m_fData[2][1] < 1.0f )
        {
            rfZAngle = SEMath<float>::ATan2(m_fData[0][1], m_fData[1][1]);
            rfXAngle = (float)asin(-(double)m_fData[2][1]);
            rfYAngle = SEMath<float>::ATan2(m_fData[2][0], m_fData[2][2]);

            return true;
        }
        else
        {
            // x = -pi/2,
            // 结果不唯一,
            // y + z = atan(-r02,r00)
            rfZAngle = SEMath<float>::ATan2(-m_fData[0][2], m_fData[0][0]);
            rfXAngle = -SEMath<float>::HALF_PI;
            rfYAngle = 0.0f;

            return false;
        }
    }
    else
    {
        // x = pi/2,
        // 结果不唯一,
        // y - z = atan2(-r02,r00)
        rfZAngle = -SEMath<float>::ATan2(-m_fData[0][2], m_fData[0][0]);
        rfXAngle = SEMath<float>::HALF_PI;
        rfYAngle = 0.0f;

        return false;
    }
}
//----------------------------------------------------------------------------
bool SEMatrix3f::ToEulerAnglesZYX(float& rfZAngle, float& rfYAngle, 
    float& rfXAngle) const
{
    // R = Rz * Ry * Rx
    //   =  cy*cz           cx*sz+cz*sx*sy  sx*sz-cx*cz*sy
    //     -cy*sz           cx*cz-sx*sy*sz  cz*sx+cx*sy*sz
    //      sy             -cy*sx           cx*cy
    //
    // 如果y = pi/2,则sy = 1,cy = 0
    // R = 0            cx*sz+cz*sx  sx*sz-cx*cz
    //     0            cx*cz-sx*sz  cz*sx+cx*sz
    //     1            0            0
    //   = 0            sin(x+z)    -cos(x+z)
    //     0            cos(x+z)     sin(x+z)
    //     1            0            0
    //
    // 如果y = -pi/2,则sy = -1,cy = 0
    // R =  0            cx*sz-cz*sx  sx*sz+cx*cz
    //      0            cx*cz+sx*sz  cz*sx-cx*sz
    //     -1            0            0
    //   =  0            sin(z-x)     cos(z-x)
    //      0            cos(z-x)    -sin(z-x)
    //     -1            0            0

    if( m_fData[2][0] > -1.0f )
    {
        if( m_fData[2][0] < 1.0f )
        {
            rfZAngle = SEMath<float>::ATan2(-m_fData[1][0], m_fData[0][0]);
            rfYAngle = (float)asin((double)m_fData[2][0]);
            rfXAngle = SEMath<float>::ATan2(-m_fData[2][1], m_fData[2][2]);

            return true;
        }
        else
        {
            // y = pi/2,
            // 结果不唯一,
            // x + z = atan2(r01,-r02)
            rfZAngle = SEMath<float>::ATan2(m_fData[0][1], -m_fData[0][2]);
            rfYAngle = SEMath<float>::HALF_PI;
            rfXAngle = 0.0f;

            return false;
        }
    }
    else
    {
        // y = -pi/2,
        // 结果不唯一,
        // z - x = atan2(r01,r02)
        rfZAngle = SEMath<float>::ATan2(m_fData[0][1], m_fData[0][2]);
        rfYAngle = -SEMath<float>::HALF_PI;
        rfXAngle = 0.0f;

        return false;
    }
}
//----------------------------------------------------------------------------
SEMatrix3f Swing::operator*(float fLhsScalar, const SEMatrix3f& rRhsMat)
{
    return rRhsMat * fLhsScalar;
}
//----------------------------------------------------------------------------
SEVector3f Swing::operator*(const SEVector3f& rLhsVec, const SEMatrix3f& 
    rRhsMat)
{
    return SEVector3f(
        rLhsVec[0]*rRhsMat[0][0] + rLhsVec[1]*rRhsMat[1][0] + 
        rLhsVec[2]*rRhsMat[2][0],
        rLhsVec[0]*rRhsMat[0][1] + rLhsVec[1]*rRhsMat[1][1] + 
        rLhsVec[2]*rRhsMat[2][1],
        rLhsVec[0]*rRhsMat[0][2] + rLhsVec[1]*rRhsMat[1][2] + 
        rLhsVec[2]*rRhsMat[2][2]);
}
//----------------------------------------------------------------------------
void SEMatrix3f::Orthonormalize()
{
    // 待检查
    // Algorithm uses Gram-Schmidt orthogonalization.  If 'this' matrix is
    // M = [m0|m1|m2], then orthonormal output matrix is Q = [q0|q1|q2],
    //
    //   q0 = m0/|m0|
    //   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
    //   q2 = (m2-(q0*m2)q0-(q1*m2)q1)/|m2-(q0*m2)q0-(q1*m2)q1|
    //
    // where |V| indicates length of vector V and A*B indicates dot
    // product of vectors A and B.

    // compute q0
    float fInvLength = SEMath<float>::InvSqrt(m_fData[0][0]*m_fData[0][0] +
        m_fData[1][0]*m_fData[1][0] + m_fData[2][0]*m_fData[2][0]);

    m_fData[0][0] *= fInvLength;
    m_fData[1][0] *= fInvLength;
    m_fData[2][0] *= fInvLength;

    // compute q1
    float fDot0 = m_fData[0][0]*m_fData[0][1] + m_fData[1][0]*m_fData[1][1] +
        m_fData[2][0]*m_fData[2][1];

    m_fData[0][1] -= fDot0*m_fData[0][0];
    m_fData[1][1] -= fDot0*m_fData[1][0];
    m_fData[2][1] -= fDot0*m_fData[2][0];

    fInvLength = SEMath<float>::InvSqrt(m_fData[0][1]*m_fData[0][1] +
        m_fData[1][1]*m_fData[1][1] + m_fData[2][1]*m_fData[2][1]);

    m_fData[0][1] *= fInvLength;
    m_fData[1][1] *= fInvLength;
    m_fData[2][1] *= fInvLength;

    // compute q2
    float fDot1 = m_fData[0][1]*m_fData[0][2] + m_fData[1][1]*m_fData[1][2] +
        m_fData[2][1]*m_fData[2][2];

    fDot0 = m_fData[0][0]*m_fData[0][2] + m_fData[1][0]*m_fData[1][2] +
        m_fData[2][0]*m_fData[2][2];

    m_fData[0][2] -= fDot0*m_fData[0][0] + fDot1*m_fData[0][1];
    m_fData[1][2] -= fDot0*m_fData[1][0] + fDot1*m_fData[1][1];
    m_fData[2][2] -= fDot0*m_fData[2][0] + fDot1*m_fData[2][1];

    fInvLength = SEMath<float>::InvSqrt(m_fData[0][2]*m_fData[0][2] +
        m_fData[1][2]*m_fData[1][2] + m_fData[2][2]*m_fData[2][2]);

    m_fData[0][2] *= fInvLength;
    m_fData[1][2] *= fInvLength;
    m_fData[2][2] *= fInvLength;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 双精度3阶方阵类
//----------------------------------------------------------------------------

const SEMatrix3d SEMatrix3d::ZERO(0.0, 0.0, 0.0,
                                  0.0, 0.0, 0.0,
                                  0.0, 0.0, 0.0);
const SEMatrix3d SEMatrix3d::IDENTITY(1.0, 0.0, 0.0,
                                      0.0, 1.0, 0.0,
                                      0.0, 0.0, 1.0);

//----------------------------------------------------------------------------
SEMatrix3d::SEMatrix3d()
{
    Zero();
}
//----------------------------------------------------------------------------
SEMatrix3d::SEMatrix3d(const SEMatrix3d& rMat)
{
    memcpy(m_dData, rMat.m_dData, /*12*/9*sizeof(double));
}
//----------------------------------------------------------------------------
SEMatrix3d::SEMatrix3d(double dM11, double dM12, double dM13, 
                   double dM21, double dM22, double dM23, 
                   double dM31, double dM32, double dM33)
{
    m_dData[0][0] = dM11;
    m_dData[0][1] = dM12;
    m_dData[0][2] = dM13;
    //m_dData[0][3] = 0.0;

    m_dData[1][0] = dM21;
    m_dData[1][1] = dM22;
    m_dData[1][2] = dM23;
    //m_dData[1][3] = 0.0;

    m_dData[2][0] = dM31;
    m_dData[2][1] = dM32;
    m_dData[2][2] = dM33;
    //m_dData[2][3] = 0.0;

    //m_dData[3][0] = 0.0;
    //m_dData[3][1] = 0.0;
    //m_dData[3][2] = 0.0;
    //m_dData[3][3] = 0.0;
}
//----------------------------------------------------------------------------
SEMatrix3d::SEMatrix3d(const SEVector3d& rVecU, const SEVector3d& rVecV, 
    const SEVector3d& rVecW, bool bIsRow)
{
    Zero();

    if( bIsRow )
	{
        m_dData[0][0] = rVecU[0];
        m_dData[0][1] = rVecU[1];
        m_dData[0][2] = rVecU[2];
        m_dData[1][0] = rVecV[0];
        m_dData[1][1] = rVecV[1];
        m_dData[1][2] = rVecV[2];
        m_dData[2][0] = rVecW[0];
        m_dData[2][1] = rVecW[1];
        m_dData[2][2] = rVecW[2];
    }
	else
    {
        m_dData[0][0] = rVecU[0];
        m_dData[0][1] = rVecV[0];
        m_dData[0][2] = rVecW[0];
        m_dData[1][0] = rVecU[1];
        m_dData[1][1] = rVecV[1];
        m_dData[1][2] = rVecW[1];
        m_dData[2][0] = rVecU[2];
        m_dData[2][1] = rVecV[2];
        m_dData[2][2] = rVecW[2];
	}
}
//----------------------------------------------------------------------------
SEMatrix3d::SEMatrix3d(double dM11, double dM22, double dM33)
{
    Zero();

    Diagonal(dM11, dM22, dM33);
}
//----------------------------------------------------------------------------
SEMatrix3d::SEMatrix3d(const SEVector3d& rAxisVec, double dAngle)
{
    Zero();

    FromAxisAngle(rAxisVec, dAngle);
}
//----------------------------------------------------------------------------
SEMatrix3d::SEMatrix3d(const SEVector3d& rVecU, const SEVector3d& rVecV)
{
    Zero();

    TensorProduct(rVecU, rVecV);
}
//----------------------------------------------------------------------------
SEMatrix3d::operator const double*() const
{
    return &m_dData[0][0];
}
//----------------------------------------------------------------------------
SEMatrix3d::operator double*()
{
    return &m_dData[0][0];
}
//----------------------------------------------------------------------------
const double* SEMatrix3d::operator[](int iCurRow) const
{
    return &m_dData[iCurRow][0];
}
//----------------------------------------------------------------------------
double* SEMatrix3d::operator[](int iCurRow)
{
    return &m_dData[iCurRow][0];
}
//----------------------------------------------------------------------------
double SEMatrix3d::operator()(int iCurRow, int iCurCol) const
{
    return m_dData[iCurRow][iCurCol];
}
//----------------------------------------------------------------------------
double& SEMatrix3d::operator()(int iCurRow, int iCurCol)
{
    return m_dData[iCurRow][iCurCol];
}
//----------------------------------------------------------------------------
SEMatrix3d& SEMatrix3d::operator=(const SEMatrix3d& rMat)
{
    memcpy(m_dData, rMat.m_dData, /*12*/9*sizeof(double));

    return *this;
}
//----------------------------------------------------------------------------
bool SEMatrix3d::operator==(const SEMatrix3d& rMat) const
{
    return CompareData(rMat) == 0;
}
//----------------------------------------------------------------------------
bool SEMatrix3d::operator!=(const SEMatrix3d& rMat) const
{
    return CompareData(rMat) != 0;
}
//----------------------------------------------------------------------------
bool SEMatrix3d::operator<(const SEMatrix3d& rMat) const
{
    return CompareData(rMat) < 0;
}
//----------------------------------------------------------------------------
bool SEMatrix3d::operator<=(const SEMatrix3d& rMat) const
{
    return CompareData(rMat) <= 0;
}
//----------------------------------------------------------------------------
bool SEMatrix3d::operator>(const SEMatrix3d& rMat) const
{
    return CompareData(rMat) > 0;
}
//----------------------------------------------------------------------------
bool SEMatrix3d::operator>=(const SEMatrix3d& rMat) const
{
    return CompareData(rMat) >= 0;
}
//----------------------------------------------------------------------------
SEMatrix3d SEMatrix3d::operator+(const SEMatrix3d& rRhsMat) const
{
    SEMatrix3d ResMat;

    ResMat.m_dData[0][0] = m_dData[0][0] + rRhsMat.m_dData[0][0];
    ResMat.m_dData[0][1] = m_dData[0][1] + rRhsMat.m_dData[0][1];
    ResMat.m_dData[0][2] = m_dData[0][2] + rRhsMat.m_dData[0][2];

    ResMat.m_dData[1][0] = m_dData[1][0] + rRhsMat.m_dData[1][0];
    ResMat.m_dData[1][1] = m_dData[1][1] + rRhsMat.m_dData[1][1];
    ResMat.m_dData[1][2] = m_dData[1][2] + rRhsMat.m_dData[1][2];

    ResMat.m_dData[2][0] = m_dData[2][0] + rRhsMat.m_dData[2][0];
    ResMat.m_dData[2][1] = m_dData[2][1] + rRhsMat.m_dData[2][1];
    ResMat.m_dData[2][2] = m_dData[2][2] + rRhsMat.m_dData[2][2];

    return ResMat;
}
//----------------------------------------------------------------------------
SEMatrix3d SEMatrix3d::operator-(const SEMatrix3d& rRhsMat) const
{
    SEMatrix3d ResMat;

    ResMat.m_dData[0][0] = m_dData[0][0] - rRhsMat.m_dData[0][0];
    ResMat.m_dData[0][1] = m_dData[0][1] - rRhsMat.m_dData[0][1];
    ResMat.m_dData[0][2] = m_dData[0][2] - rRhsMat.m_dData[0][2];

    ResMat.m_dData[1][0] = m_dData[1][0] - rRhsMat.m_dData[1][0];
    ResMat.m_dData[1][1] = m_dData[1][1] - rRhsMat.m_dData[1][1];
    ResMat.m_dData[1][2] = m_dData[1][2] - rRhsMat.m_dData[1][2];

    ResMat.m_dData[2][0] = m_dData[2][0] - rRhsMat.m_dData[2][0];
    ResMat.m_dData[2][1] = m_dData[2][1] - rRhsMat.m_dData[2][1];
    ResMat.m_dData[2][2] = m_dData[2][2] - rRhsMat.m_dData[2][2];

    return ResMat;
}
//----------------------------------------------------------------------------
SEMatrix3d SEMatrix3d::operator*(const SEMatrix3d& rRhsMat) const
{
    SEMatrix3d ResMat;

#ifdef _SIMD
    // 待实现.
    // 尚未支持SIMD.
    SE_ASSERT( false );
#else
    ResMat.m_dData[0][0] = m_dData[0][0]*rRhsMat.m_dData[0][0] + 
                           m_dData[0][1]*rRhsMat.m_dData[1][0] + 
                           m_dData[0][2]*rRhsMat.m_dData[2][0];
    ResMat.m_dData[0][1] = m_dData[0][0]*rRhsMat.m_dData[0][1] + 
                           m_dData[0][1]*rRhsMat.m_dData[1][1] + 
                           m_dData[0][2]*rRhsMat.m_dData[2][1];
    ResMat.m_dData[0][2] = m_dData[0][0]*rRhsMat.m_dData[0][2] + 
                           m_dData[0][1]*rRhsMat.m_dData[1][2] + 
                           m_dData[0][2]*rRhsMat.m_dData[2][2];

    ResMat.m_dData[1][0] = m_dData[1][0]*rRhsMat.m_dData[0][0] + 
                           m_dData[1][1]*rRhsMat.m_dData[1][0] + 
                           m_dData[1][2]*rRhsMat.m_dData[2][0];
    ResMat.m_dData[1][1] = m_dData[1][0]*rRhsMat.m_dData[0][1] + 
                           m_dData[1][1]*rRhsMat.m_dData[1][1] + 
                           m_dData[1][2]*rRhsMat.m_dData[2][1];
    ResMat.m_dData[1][2] = m_dData[1][0]*rRhsMat.m_dData[0][2] + 
                           m_dData[1][1]*rRhsMat.m_dData[1][2] + 
                           m_dData[1][2]*rRhsMat.m_dData[2][2];

    ResMat.m_dData[2][0] = m_dData[2][0]*rRhsMat.m_dData[0][0] + 
                           m_dData[2][1]*rRhsMat.m_dData[1][0] + 
                           m_dData[2][2]*rRhsMat.m_dData[2][0];
    ResMat.m_dData[2][1] = m_dData[2][0]*rRhsMat.m_dData[0][1] + 
                           m_dData[2][1]*rRhsMat.m_dData[1][1] + 
                           m_dData[2][2]*rRhsMat.m_dData[2][1];
    ResMat.m_dData[2][2] = m_dData[2][0]*rRhsMat.m_dData[0][2] + 
                           m_dData[2][1]*rRhsMat.m_dData[1][2] + 
                           m_dData[2][2]*rRhsMat.m_dData[2][2];
#endif

    return ResMat;
}
//----------------------------------------------------------------------------
SEMatrix3d SEMatrix3d::operator*(double dScalar) const
{
    SEMatrix3d ResMat;

    ResMat.m_dData[0][0] = m_dData[0][0] * dScalar;
    ResMat.m_dData[0][1] = m_dData[0][1] * dScalar;
    ResMat.m_dData[0][2] = m_dData[0][2] * dScalar;

    ResMat.m_dData[1][0] = m_dData[1][0] * dScalar;
    ResMat.m_dData[1][1] = m_dData[1][1] * dScalar;
    ResMat.m_dData[1][2] = m_dData[1][2] * dScalar;

    ResMat.m_dData[2][0] = m_dData[2][0] * dScalar;
    ResMat.m_dData[2][1] = m_dData[2][1] * dScalar;
    ResMat.m_dData[2][2] = m_dData[2][2] * dScalar;

    return ResMat;
}
//----------------------------------------------------------------------------
SEMatrix3d SEMatrix3d::operator/(double dScalar) const
{
    // 注意除数不能为0
    SEMatrix3d ResMat;
    double dInvScalar = 1.0 / dScalar;

    ResMat.m_dData[0][0] = m_dData[0][0] * dInvScalar;
    ResMat.m_dData[0][1] = m_dData[0][1] * dInvScalar;
    ResMat.m_dData[0][2] = m_dData[0][2] * dInvScalar;

    ResMat.m_dData[1][0] = m_dData[1][0] * dInvScalar;
    ResMat.m_dData[1][1] = m_dData[1][1] * dInvScalar;
    ResMat.m_dData[1][2] = m_dData[1][2] * dInvScalar;

    ResMat.m_dData[2][0] = m_dData[2][0] * dInvScalar;
    ResMat.m_dData[2][1] = m_dData[2][1] * dInvScalar;
    ResMat.m_dData[2][2] = m_dData[2][2] * dInvScalar;

    return ResMat;
}
//----------------------------------------------------------------------------
SEMatrix3d SEMatrix3d::operator-() const
{
    SEMatrix3d ResMat;

    ResMat.m_dData[0][0] = -m_dData[0][0];
    ResMat.m_dData[0][1] = -m_dData[0][1];
    ResMat.m_dData[0][2] = -m_dData[0][2];

    ResMat.m_dData[1][0] = -m_dData[1][0];
    ResMat.m_dData[1][1] = -m_dData[1][1];
    ResMat.m_dData[1][2] = -m_dData[1][2];

    ResMat.m_dData[2][0] = -m_dData[2][0];
    ResMat.m_dData[2][1] = -m_dData[2][1];
    ResMat.m_dData[2][2] = -m_dData[2][2];

    return ResMat;
}
//----------------------------------------------------------------------------
SEMatrix3d& SEMatrix3d::operator+=(const SEMatrix3d& rRhsMat)
{
	m_dData[0][0] += rRhsMat.m_dData[0][0];
    m_dData[0][1] += rRhsMat.m_dData[0][1];
    m_dData[0][2] += rRhsMat.m_dData[0][2];

    m_dData[1][0] += rRhsMat.m_dData[1][0];
    m_dData[1][1] += rRhsMat.m_dData[1][1];
    m_dData[1][2] += rRhsMat.m_dData[1][2];

    m_dData[2][0] += rRhsMat.m_dData[2][0];
    m_dData[2][1] += rRhsMat.m_dData[2][1];
    m_dData[2][2] += rRhsMat.m_dData[2][2];

    return *this;
}
//----------------------------------------------------------------------------
SEMatrix3d& SEMatrix3d::operator-=(const SEMatrix3d& rRhsMat)
{
	m_dData[0][0] -= rRhsMat.m_dData[0][0];
    m_dData[0][1] -= rRhsMat.m_dData[0][1];
    m_dData[0][2] -= rRhsMat.m_dData[0][2];

    m_dData[1][0] -= rRhsMat.m_dData[1][0];
    m_dData[1][1] -= rRhsMat.m_dData[1][1];
    m_dData[1][2] -= rRhsMat.m_dData[1][2];

    m_dData[2][0] -= rRhsMat.m_dData[2][0];
    m_dData[2][1] -= rRhsMat.m_dData[2][1];
    m_dData[2][2] -= rRhsMat.m_dData[2][2];

    return *this;
}
//----------------------------------------------------------------------------
SEMatrix3d& SEMatrix3d::operator*=(double dScalar)
{
	m_dData[0][0] *= dScalar;
    m_dData[0][1] *= dScalar;
    m_dData[0][2] *= dScalar;

    m_dData[1][0] *= dScalar;
    m_dData[1][1] *= dScalar;
    m_dData[1][2] *= dScalar;

    m_dData[2][0] *= dScalar;
    m_dData[2][1] *= dScalar;
    m_dData[2][2] *= dScalar;

    return *this;
}
//----------------------------------------------------------------------------
SEMatrix3d& SEMatrix3d::operator/=(double dScalar)
{
    // 注意除数不能为0
    double dInvScalar = 1.0 / dScalar;
	m_dData[0][0] *= dInvScalar;
    m_dData[0][1] *= dInvScalar;
    m_dData[0][2] *= dInvScalar;

    m_dData[1][0] *= dInvScalar;
    m_dData[1][1] *= dInvScalar;
    m_dData[1][2] *= dInvScalar;

    m_dData[2][0] *= dInvScalar;
    m_dData[2][1] *= dInvScalar;
    m_dData[2][2] *= dInvScalar;

    return *this;
}
//----------------------------------------------------------------------------
SEVector3d SEMatrix3d::operator*(const SEVector3d& rRhsVec) const
{
    SEVector3d ResVec;
   
    ResVec[0] = m_dData[0][0]*rRhsVec[0] + m_dData[0][1]*rRhsVec[1] + 
        m_dData[0][2]*rRhsVec[2];
    ResVec[1] = m_dData[1][0]*rRhsVec[0] + m_dData[1][1]*rRhsVec[1] + 
        m_dData[1][2]*rRhsVec[2];
    ResVec[2] = m_dData[2][0]*rRhsVec[0] + m_dData[2][1]*rRhsVec[1] + 
        m_dData[2][2]*rRhsVec[2];

    return ResVec;
}
//----------------------------------------------------------------------------
void SEMatrix3d::Transpose()
{
    SEMatrix3d tempMat;
	GetTranspose(tempMat);
	*this = tempMat;
}
//----------------------------------------------------------------------------
void SEMatrix3d::Inverse()
{
    SEMatrix3d tempMat;
	GetInverse(tempMat);
	*this = tempMat;
}
//----------------------------------------------------------------------------
SEMatrix3d& SEMatrix3d::FromEulerAnglesXYZ(double dYAngle, double dPAngle, 
    double dRAngle)
{
    double dCos, dSin;

    dCos = SEMath<double>::Cos(dYAngle);
    dSin = SEMath<double>::Sin(dYAngle);
    SEMatrix3d RotXMat(
        1.0,0.0,0.0,
        0.0,dCos,dSin,
        0.0,-dSin,dCos);

    dCos = SEMath<double>::Cos(dPAngle);
    dSin = SEMath<double>::Sin(dPAngle);
    SEMatrix3d RotYMat(
        dCos,0.0,-dSin,
        0.0,1.0,0.0,
        dSin,0.0,dCos);

    dCos = SEMath<double>::Cos(dRAngle);
    dSin = SEMath<double>::Sin(dRAngle);
    SEMatrix3d RotZMat(
        dCos,dSin,0.0,
        -dSin,dCos,0.0,
        0.0,0.0,1.0);

    *this = RotXMat*(RotYMat*RotZMat);
    return *this;
}
//----------------------------------------------------------------------------
SEMatrix3d& SEMatrix3d::FromEulerAnglesXZY(double dYAngle, double dPAngle, 
    double dRAngle)
{
    double dCos, dSin;

    dCos = SEMath<double>::Cos(dYAngle);
    dSin = SEMath<double>::Sin(dYAngle);
    SEMatrix3d RotXMat(
        1.0,0.0,0.0,
        0.0,dCos,dSin,
        0.0,-dSin,dCos);

    dCos = SEMath<double>::Cos(dPAngle);
    dSin = SEMath<double>::Sin(dPAngle);
    SEMatrix3d RotZMat(
        dCos,dSin,0.0,
        -dSin,dCos,0.0,
        0.0,0.0,1.0);

    dCos = SEMath<double>::Cos(dRAngle);
    dSin = SEMath<double>::Sin(dRAngle);
    SEMatrix3d RotYMat(
        dCos,0.0,-dSin,
        0.0,1.0,0.0,
        dSin,0.0,dCos);

    *this = RotXMat*(RotZMat*RotYMat);
    return *this;
}
//----------------------------------------------------------------------------
SEMatrix3d& SEMatrix3d::FromEulerAnglesYXZ(double dYAngle, double dPAngle, 
    double dRAngle)
{
    double dCos, dSin;

    dCos = SEMath<double>::Cos(dYAngle);
    dSin = SEMath<double>::Sin(dYAngle);
    SEMatrix3d RotYMat(
        dCos,0.0,-dSin,
        0.0,1.0,0.0,
        dSin,0.0,dCos);

    dCos = SEMath<double>::Cos(dPAngle);
    dSin = SEMath<double>::Sin(dPAngle);
    SEMatrix3d RotXMat(
        1.0,0.0,0.0,
        0.0,dCos,dSin,
        0.0,-dSin,dCos);

    dCos = SEMath<double>::Cos(dRAngle);
    dSin = SEMath<double>::Sin(dRAngle);
    SEMatrix3d RotZMat(
        dCos,dSin,0.0,
        -dSin,dCos,0.0,
        0.0,0.0,1.0);

    *this = RotYMat*(RotXMat*RotZMat);
    return *this;
}
//----------------------------------------------------------------------------
SEMatrix3d& SEMatrix3d::FromEulerAnglesYZX(double dYAngle, double dPAngle, 
    double dRAngle)
{
    double dCos, dSin;

    dCos = SEMath<double>::Cos(dYAngle);
    dSin = SEMath<double>::Sin(dYAngle);
    SEMatrix3d RotYMat(
        dCos,0.0,-dSin,
        0.0,1.0,0.0,
        dSin,0.0,dCos);

    dCos = SEMath<double>::Cos(dPAngle);
    dSin = SEMath<double>::Sin(dPAngle);
    SEMatrix3d RotZMat(
        dCos,dSin,0.0,
        -dSin,dCos,0.0,
        0.0,0.0,1.0);

    dCos = SEMath<double>::Cos(dRAngle);
    dSin = SEMath<double>::Sin(dRAngle);
    SEMatrix3d RotXMat(
        1.0,0.0,0.0,
        0.0,dCos,dSin,
        0.0,-dSin,dCos);

    *this = RotYMat*(RotZMat*RotXMat);
    return *this;
}
//----------------------------------------------------------------------------
SEMatrix3d& SEMatrix3d::FromEulerAnglesZXY(double dYAngle, double dPAngle, 
    double dRAngle)
{
    double dCos, dSin;

    dCos = SEMath<double>::Cos(dYAngle);
    dSin = SEMath<double>::Sin(dYAngle);
    SEMatrix3d RotZMat(
        dCos,dSin,0.0,
        -dSin,dCos,0.0,
        0.0,0.0,1.0);

    dCos = SEMath<double>::Cos(dPAngle);
    dSin = SEMath<double>::Sin(dPAngle);
    SEMatrix3d RotXMat(
        1.0,0.0,0.0,
        0.0,dCos,dSin,
        0.0,-dSin,dCos);

    dCos = SEMath<double>::Cos(dRAngle);
    dSin = SEMath<double>::Sin(dRAngle);
    SEMatrix3d RotYMat(
        dCos,0.0,-dSin,
        0.0,1.0,0.0,
        dSin,0.0,dCos);

    *this = RotZMat*(RotXMat*RotYMat);
    return *this;
}
//----------------------------------------------------------------------------
SEMatrix3d& SEMatrix3d::FromEulerAnglesZYX(double dYAngle, double dPAngle, 
    double dRAngle)
{
    double dCos, dSin;

    dCos = SEMath<double>::Cos(dYAngle);
    dSin = SEMath<double>::Sin(dYAngle);
    SEMatrix3d RotZMat(
        dCos,dSin,0.0,
        -dSin,dCos,0.0,
        0.0,0.0,1.0);

    dCos = SEMath<double>::Cos(dPAngle);
    dSin = SEMath<double>::Sin(dPAngle);
    SEMatrix3d RotYMat(
        dCos,0.0,-dSin,
        0.0,1.0,0.0,
        dSin,0.0,dCos);

    dCos = SEMath<double>::Cos(dRAngle);
    dSin = SEMath<double>::Sin(dRAngle);
    SEMatrix3d RotXMat(
        1.0,0.0,0.0,
        0.0,dCos,dSin,
        0.0,-dSin,dCos);

    *this = RotZMat*(RotYMat*RotXMat);
    return *this;
}
//----------------------------------------------------------------------------
bool SEMatrix3d::ToEulerAnglesXYZ(double& rdXAngle, double& rdYAngle, 
    double& rdZAngle) const
{
    // R = Rx * Ry * Rz
    //   = cy*cz           cy*sz          -sy
    //     cz*sx*sy-cx*sz  cx*cz+sx*sy*sz  sx*cy
    //     cx*cz*sy+sx*sz  cx*sy*sz-cz*sx  cx*cy
    //
    // 如果y = pi/2,则sy = 1,cy = 0
    // R = 0            0           -1
    //     cz*sx-cx*sz  cx*cz+sx*sz  0
    //     cx*cz+sx*sz  cx*sz-cz*sx  0
    //   = 0            0           -1
    //     sin(x-z)     cos(x-z)     0
    //     cos(x-z)     -sin(x-z)    0
    //
    // 如果y = -pi/2,则sy = -1,cy = 0
    // R =  0            0            1
    //     -cz*sx-cx*sz  cx*cz-sx*sz  0
    //     -cx*cz+sx*sz -cx*sz-cz*sx  0
    //   =  0            0            1
    //     -sin(x+z)     cos(x+z)     0
    //     -cos(x+z)    -sin(x+z)     0

    if( m_dData[0][2] > -1.0 )
    {
        if( m_dData[0][2] < 1.0 )
        {
            rdXAngle = SEMath<double>::ATan2(m_dData[1][2], m_dData[2][2]);
            rdYAngle = asin(-m_dData[0][2]);
            rdZAngle = SEMath<double>::ATan2(m_dData[0][1], m_dData[0][0]);

            return true;
        }
        else
        {
            // y = -pi/2,
            // 结果不唯一,
            // x + z = atan2(r10,r11)
            rdXAngle = SEMath<double>::ATan2(-m_dData[1][0], m_dData[1][1]);
            rdYAngle = -SEMath<double>::HALF_PI;
            rdZAngle = 0.0;

            return false;
        }
    }
    else
    {
        // y = pi/2,
        // 结果不唯一,
        // x - z = atan2(r10,r11)
        rdXAngle = SEMath<double>::ATan2(m_dData[1][0], m_dData[1][1]);
        rdYAngle = SEMath<double>::HALF_PI;
        rdZAngle = 0.0;

        return false;
    }
}
//----------------------------------------------------------------------------
bool SEMatrix3d::ToEulerAnglesXZY(double& rdXAngle, double& rdZAngle, 
    double& rdYAngle) const
{
    // R = Rx * Rz * Ry
    //   =  cy*cz           sz             -cz*sy
    //     -cx*cy*sz+sx*sy  cx*cz           cx*sy*sz+cy*sx
    //      cy*sx*sz+cx*sy -cz*sx          -sx*sy*sz+cx*cy
    //
    // 如果z = pi/2,则sz = 1,cz = 0
    // R = 0            1            0
    //     sx*sy-cx*cy  0            cx*sy+cy*sx
    //     cy*sx+cx*sy  0            cx*cy-sx*sy
    //   =  0            1           0
    //     -cos(x+y)     0           sin(x+y)
    //      sin(x+y)     0           cos(x+y)
    //
    // 如果z = -pi/2,则sz = -1,cz = 0
    // R =  0           -1            0
    //      cx*cy+sx*sy  0           -cx*sy+cy*sx
    //     -cy*sx+cx*sy  0            sx*sz+cx*cy
    //   =  0           -1            0
    //      cos(x-y)     0            sin(x-y)
    //     -sin(x-y)     0            cos(x-y)

    if( m_dData[0][1] > -1.0 )
    {
        if( m_dData[0][1] < 1.0 )
        {
            rdXAngle = SEMath<double>::ATan2(-m_dData[2][1], m_dData[1][1]);
            rdZAngle = asin(m_dData[0][1]);
            rdYAngle = SEMath<double>::ATan2(-m_dData[0][2], m_dData[0][0]);

            return true;
        }
        else
        {
            // z = pi/2,
            // 结果不唯一,
            // x + y = atan2(r20,r22)
            rdXAngle = SEMath<double>::ATan2(m_dData[2][0], m_dData[2][2]);
            rdZAngle = SEMath<double>::HALF_PI;
            rdYAngle = 0.0;

            return false;
        }
    }
    else
    {
        // z = -pi/2,
        // 结果不唯一,
        // x - y = atan2(-r20,r22)
        rdXAngle = SEMath<double>::ATan2(-m_dData[2][0], m_dData[2][2]);
        rdZAngle = -SEMath<double>::HALF_PI;
        rdYAngle = 0.0;

        return false;
    }
}
//----------------------------------------------------------------------------
bool SEMatrix3d::ToEulerAnglesYXZ(double& rdYAngle, double& rdXAngle, 
    double& rdZAngle) const
{
    // R = Ry * Rx * Rz
    //   =  cy*cz-sx*sy*sz  cy*sz+cz*sx*sy -cx*sy
    //     -cx*sz           cx*cz           sx
    //      cz*sy+cy*sx*sz  sy*sz-cy*cz*sx  cx*cy
    //
    // 如果x = pi/2,则sx = 1,cx = 0
    // R = cy*cz-sy*sz  cy*sz+cz*sy  0
    //     0            0            1
    //     cz*sy+cy*sz  sy*sz-cy*cz  0
    //   = cos(y+z)     sin(y+z)    0
    //     0            0           1
    //     sin(y+z)    -cos(y+z)    0
    //
    // 如果x = -pi/2,则sx = -1,cx = 0
    // R = cy*cz+sy*sz  cy*sz-cz*sy  0
    //     0            0           -1
    //     cz*sy-cy*sz  sy*sz+cy*cz  0
    //   =  cos(y-z)    -sin(y-z)     0
    //      0            0           -1
    //      sin(y-z)     cos(y-z)     0

    if( m_dData[1][2] > -1.0 )
    {
        if( m_dData[1][2] < 1.0 )
        {
            rdYAngle = SEMath<double>::ATan2(-m_dData[0][2], m_dData[2][2]);
            rdXAngle = asin(m_dData[1][2]);
            rdZAngle = SEMath<double>::ATan2(-m_dData[1][0], m_dData[1][1]);

            return true;
        }
        else
        {
            // x = pi/2,
            // 结果不唯一,
            // y + z = atan2(r01,r00)
            rdYAngle = SEMath<double>::ATan2(m_dData[0][1], m_dData[0][0]);
            rdXAngle = SEMath<double>::HALF_PI;
            rdZAngle = 0.0;

            return false;
        }
    }
    else
    {
        // x = -pi/2,
        // 结果不唯一,
        // y - z = atan2(-r01,r00)
        rdYAngle = SEMath<double>::ATan2(-m_dData[0][1], m_dData[0][0]);
        rdXAngle = -SEMath<double>::HALF_PI;
        rdZAngle = 0.0;

        return false;
    }
}
//----------------------------------------------------------------------------
bool SEMatrix3d::ToEulerAnglesYZX(double& rdYAngle, double& rdZAngle, 
    double& rdXAngle) const
{
    // R = Ry * Rz * Rx
    //   =  cy*cz           cx*cy*sz+sx*sy  cy*sx*sz-cx*sy
    //     -sz              cx*cz           cz*sx
    //      cz*sy           cx*sy*sz-cy*sx  sx*sy*sz+cx*cy
    //
    // 如果z = pi/2,则sz = 1,cz = 0
    // R =  0            cx*cy+sx*sy  cy*sx-cx*sy
    //     -1            0            0
    //      0            cx*sy-cy*sx  sx*sy+cx*cy
    //   =  0            cos(x-y)    sin(x-y)
    //     -1            0           0
    //      0           -sin(x-y)    cos(x-y)
    //
    // 如果z = -pi/2,则sz = -1,cz = 0
    // R = 0           -cx*cy+sx*sy -cy*sx-cx*sy
    //     1            0            0
    //     0           -cx*sy-cy*sx -sx*sy+cx*cy
    //   =  0           -cos(x+y)    -sin(x+y)
    //      1            0            0
    //      0           -sin(x+y)     cos(x+y)

    if( m_dData[1][0] > -1.0 )
    {
        if( m_dData[1][0] < 1.0 )
        {
            rdYAngle = SEMath<double>::ATan2(m_dData[2][0], m_dData[0][0]);
            rdZAngle = asin(-m_dData[1][0]);
            rdXAngle = SEMath<double>::ATan2(m_dData[1][2], m_dData[1][1]);

            return true;
        }
        else
        {
            // z = -pi/2,
            // 结果不唯一,
            // x + y = atan2(-r21,r22)
            rdYAngle = SEMath<double>::ATan2(-m_dData[2][1], m_dData[2][2]);
            rdZAngle = -SEMath<double>::HALF_PI;
            rdXAngle = 0.0;

            return false;
        }
    }
    else
    {
        // z = pi/2,
        // 结果不唯一,
        // x - y = atan2(-r21,r22)
        rdYAngle = -SEMath<double>::ATan2(-m_dData[2][1], m_dData[2][2]);
        rdZAngle = SEMath<double>::HALF_PI;
        rdXAngle = 0.0;

        return false;
    }
}
//----------------------------------------------------------------------------
bool SEMatrix3d::ToEulerAnglesZXY(double& rdZAngle, double& rdXAngle, 
    double& rdYAngle) const
{
    // R = Rz * Rx * Ry
    //   =  cy*cz+sx*sy*sz  cx*sz          -cz*sy+cy*sx*sz
    //     -cy*sz+cz*sx*sy  cx*cz           sy*sz+cy*cz*sx
    //      cx*sy          -sx              cx*cy
    //
    // 如果x = pi/2,则sx = 1,cx = 0
    // R =  cy*cz+sy*sz  0           -cz*sy+cy*sz
    //     -cy*sz+cz*sy  0            sy*sz+cy*cz
    //      0           -1            0
    //   =  cos(y-z)     0          -sin(y-z)
    //      sin(y-z)     0           cos(y-z)
    //      0           -1           0
    //
    // 如果x = -pi/2,则sx = -1,cx = 0
    // R =  cy*cz-sy*sz  0           -cz*sy-cy*sz
    //     -cy*sz-cz*sy  0            sy*sz-cy*cz
    //      0            1            0
    //   =  cos(y+z)     0           -sin(y+z)
    //     -sin(y+z)     0           -cos(y+z)
    //      0            1            0

    if( m_dData[2][1] > -1.0 )
    {
        if( m_dData[2][1] < 1.0 )
        {
            rdZAngle = SEMath<double>::ATan2(m_dData[0][1], m_dData[1][1]);
            rdXAngle = asin(-m_dData[2][1]);
            rdYAngle = SEMath<double>::ATan2(m_dData[2][0], m_dData[2][2]);

            return true;
        }
        else
        {
            // x = -pi/2,
            // 结果不唯一,
            // y + z = atan(-r02,r00)
            rdZAngle = SEMath<double>::ATan2(-m_dData[0][2], m_dData[0][0]);
            rdXAngle = -SEMath<double>::HALF_PI;
            rdYAngle = 0.0;

            return false;
        }
    }
    else
    {
        // x = pi/2,
        // 结果不唯一,
        // y - z = atan2(-r02,r00)
        rdZAngle = -SEMath<double>::ATan2(-m_dData[0][2], m_dData[0][0]);
        rdXAngle = SEMath<double>::HALF_PI;
        rdYAngle = 0.0;

        return false;
    }
}
//----------------------------------------------------------------------------
bool SEMatrix3d::ToEulerAnglesZYX(double& rdZAngle, double& rdYAngle, 
    double& rdXAngle) const
{
    // R = Rz * Ry * Rx
    //   =  cy*cz           cx*sz+cz*sx*sy  sx*sz-cx*cz*sy
    //     -cy*sz           cx*cz-sx*sy*sz  cz*sx+cx*sy*sz
    //      sy             -cy*sx           cx*cy
    //
    // 如果y = pi/2,则sy = 1,cy = 0
    // R = 0            cx*sz+cz*sx  sx*sz-cx*cz
    //     0            cx*cz-sx*sz  cz*sx+cx*sz
    //     1            0            0
    //   = 0            sin(x+z)    -cos(x+z)
    //     0            cos(x+z)     sin(x+z)
    //     1            0            0
    //
    // 如果y = -pi/2,则sy = -1,cy = 0
    // R =  0            cx*sz-cz*sx  sx*sz+cx*cz
    //      0            cx*cz+sx*sz  cz*sx-cx*sz
    //     -1            0            0
    //   =  0            sin(z-x)     cos(z-x)
    //      0            cos(z-x)    -sin(z-x)
    //     -1            0            0

    if( m_dData[2][0] > -1.0 )
    {
        if( m_dData[2][0] < 1.0 )
        {
            rdZAngle = SEMath<double>::ATan2(-m_dData[1][0], m_dData[0][0]);
            rdYAngle = asin(m_dData[2][0]);
            rdXAngle = SEMath<double>::ATan2(-m_dData[2][1], m_dData[2][2]);

            return true;
        }
        else
        {
            // y = pi/2,
            // 结果不唯一,
            // x + z = atan2(r01,-r02)
            rdZAngle = SEMath<double>::ATan2(m_dData[0][1], -m_dData[0][2]);
            rdYAngle = SEMath<double>::HALF_PI;
            rdXAngle = 0.0;

            return false;
        }
    }
    else
    {
        // y = -pi/2,
        // 结果不唯一,
        // z - x = atan2(r01,r02)
        rdZAngle = SEMath<double>::ATan2(m_dData[0][1], m_dData[0][2]);
        rdYAngle = -SEMath<double>::HALF_PI;
        rdXAngle = 0.0;

        return false;
    }
}
//----------------------------------------------------------------------------
SEMatrix3d Swing::operator*(double dLhsScalar, const SEMatrix3d& rRhsMat)
{
    return rRhsMat * dLhsScalar;
}
//----------------------------------------------------------------------------
SEVector3d Swing::operator*(const SEVector3d& rLhsVec, const SEMatrix3d& 
    rRhsMat)
{
    return SEVector3d(
        rLhsVec[0]*rRhsMat[0][0] + rLhsVec[1]*rRhsMat[1][0] + 
        rLhsVec[2]*rRhsMat[2][0],
        rLhsVec[0]*rRhsMat[0][1] + rLhsVec[1]*rRhsMat[1][1] + 
        rLhsVec[2]*rRhsMat[2][1],
        rLhsVec[0]*rRhsMat[0][2] + rLhsVec[1]*rRhsMat[1][2] + 
        rLhsVec[2]*rRhsMat[2][2]);
}
//----------------------------------------------------------------------------
void SEMatrix3d::Orthonormalize()
{
    // 待检查
    // Algorithm uses Gram-Schmidt orthogonalization.  If 'this' matrix is
    // M = [m0|m1|m2], then orthonormal output matrix is Q = [q0|q1|q2],
    //
    //   q0 = m0/|m0|
    //   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
    //   q2 = (m2-(q0*m2)q0-(q1*m2)q1)/|m2-(q0*m2)q0-(q1*m2)q1|
    //
    // where |V| indicates length of vector V and A*B indicates dot
    // product of vectors A and B.

    // compute q0
    double dInvLength = SEMath<double>::InvSqrt(m_dData[0][0]*m_dData[0][0] +
        m_dData[1][0]*m_dData[1][0] + m_dData[2][0]*m_dData[2][0]);

    m_dData[0][0] *= dInvLength;
    m_dData[1][0] *= dInvLength;
    m_dData[2][0] *= dInvLength;

    // compute q1
    double dDot0 = m_dData[0][0]*m_dData[0][1] + m_dData[1][0]*m_dData[1][1] +
        m_dData[2][0]*m_dData[2][1];

    m_dData[0][1] -= dDot0*m_dData[0][0];
    m_dData[1][1] -= dDot0*m_dData[1][0];
    m_dData[2][1] -= dDot0*m_dData[2][0];

    dInvLength = SEMath<double>::InvSqrt(m_dData[0][1]*m_dData[0][1] +
        m_dData[1][1]*m_dData[1][1] + m_dData[2][1]*m_dData[2][1]);

    m_dData[0][1] *= dInvLength;
    m_dData[1][1] *= dInvLength;
    m_dData[2][1] *= dInvLength;

    // compute q2
    double dDot1 = m_dData[0][1]*m_dData[0][2] + m_dData[1][1]*m_dData[1][2] +
        m_dData[2][1]*m_dData[2][2];

    dDot0 = m_dData[0][0]*m_dData[0][2] + m_dData[1][0]*m_dData[1][2] +
        m_dData[2][0]*m_dData[2][2];

    m_dData[0][2] -= dDot0*m_dData[0][0] + dDot1*m_dData[0][1];
    m_dData[1][2] -= dDot0*m_dData[1][0] + dDot1*m_dData[1][1];
    m_dData[2][2] -= dDot0*m_dData[2][0] + dDot1*m_dData[2][1];

    dInvLength = SEMath<double>::InvSqrt(m_dData[0][2]*m_dData[0][2] +
        m_dData[1][2]*m_dData[1][2] + m_dData[2][2]*m_dData[2][2]);

    m_dData[0][2] *= dInvLength;
    m_dData[1][2] *= dInvLength;
    m_dData[2][2] *= dInvLength;
}
//----------------------------------------------------------------------------
