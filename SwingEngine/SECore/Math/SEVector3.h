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

#ifndef Swing_Vector3_H
#define Swing_Vector3_H

#include "SECoreLIB.h"
#include "SEMath.h"

// 向量叉积使用左手原则

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20070512
//----------------------------------------------------------------------------
class SE_CORE_API SEVector3f
{
public:
    union
    {
        struct
        {
            float X;
            float Y;
            float Z;
        };
        float m_fData[3];
    };

public:
    SEVector3f(void);
    SEVector3f(float fX, float fY, float fZ);
    SEVector3f(const SEVector3f& rVec);

    // 转换为连续指针空间
    operator const float* (void) const;
    operator float* (void);
    // 取第i元素形式
    float operator [] (int i) const;
    float& operator [] (int i);

    SEVector3f& operator = (const SEVector3f& rVec);

    bool operator == (const SEVector3f& rVec) const;
    bool operator != (const SEVector3f& rVec) const;
    bool operator < (const SEVector3f& rVec) const;
    bool operator <= (const SEVector3f& rVec) const;
    bool operator > (const SEVector3f& rVec) const;
    bool operator >= (const SEVector3f& rVec) const;

    SEVector3f operator + (const SEVector3f& rRhsVec) const;
    SEVector3f operator - (const SEVector3f& rRhsVec) const;
    SEVector3f operator * (float fScalar) const;
    SEVector3f operator / (float fScalar) const;
    SEVector3f operator - (void) const;

    SEVector3f& operator += (const SEVector3f& rRhsVec);
    SEVector3f& operator += (float fScalar);
    SEVector3f& operator -= (const SEVector3f& rRhsVec);
    SEVector3f& operator -= (float fScalar);
    SEVector3f& operator *= (float fScalar);
    SEVector3f& operator /= (float fScalar);

    SE_CORE_API friend SEVector3f operator * (float fLhsScalar, 
        const SEVector3f& rRhsVec);

    // 求向量长度
    inline float GetLength(void) const;
    // 求向量长度平方
    inline float GetSquaredLength(void) const;
    // 求点积
	inline float Dot(const SEVector3f& rRhsVec) const;
    // 规范化
	inline float Normalize(void);

    // 注意叉积运算使用左手原则,适用于DirectX的左手坐标系,
    // OpenGL中叉积使用右手原则
    inline SEVector3f Cross(const SEVector3f& rRhsVec) const;
    inline SEVector3f UnitCross(const SEVector3f& rRhsVec) const;

    // 对线性无关的U,V,W向量进行Gram-Schmidt规范化正交
    static void Orthonormalize(SEVector3f& rVecU, SEVector3f& rVecV, 
        SEVector3f& rVecW);
    static void Orthonormalize(SEVector3f* pVec);

    // 根据W创建一组规范正交基,W必须为非0向量,
    // U,V,W将构成左手坐标系
    static void GetOrthonormalBasis(SEVector3f& rVecU, SEVector3f& rVecV, 
        SEVector3f& rVecW, bool bUnitLengthW);

    // 根据输入的若干顶点,计算其空间分布最小值和最大值
	static void ComputeExtremes(int iCount, const SEVector3f* pPoints, 
        SEVector3f& rMin, SEVector3f& rMax);

    // Get a flipped vector based on 'rOriginal' vector so that it lies in 
    // the same hemisphere as 'rReference' vector.
    static void Faceforward(const SEVector3f& rOriginal, 
        const SEVector3f& rReference, SEVector3f& rFlipped);

    static const SEVector3f ZERO;
    static const SEVector3f UNIT_X;
    static const SEVector3f UNIT_Y;
    static const SEVector3f UNIT_Z;

private:
    inline int CompareData(const SEVector3f& rVec) const;
};

//----------------------------------------------------------------------------
// Description:
// Date:20090514
//----------------------------------------------------------------------------
class SE_CORE_API SEVector3d
{
public:
    union
    {
        struct
        {
            double X;
            double Y;
            double Z;
        };
        double m_dData[3];
    };

public:
    SEVector3d(void);
    SEVector3d(double fX, double fY, double fZ);
    SEVector3d(const SEVector3d& rVec);

    // 转换为连续指针空间
    operator const double* (void) const;
    operator double* (void);
    // 取第i元素形式
    double operator [] (int i) const;
    double& operator [] (int i);

    SEVector3d& operator = (const SEVector3d& rVec);

    bool operator == (const SEVector3d& rVec) const;
    bool operator != (const SEVector3d& rVec) const;
    bool operator < (const SEVector3d& rVec) const;
    bool operator <= (const SEVector3d& rVec) const;
    bool operator > (const SEVector3d& rVec) const;
    bool operator >= (const SEVector3d& rVec) const;

    SEVector3d operator + (const SEVector3d& rRhsVec) const;
    SEVector3d operator - (const SEVector3d& rRhsVec) const;
    SEVector3d operator * (double dScalar) const;
    SEVector3d operator / (double dScalar) const;
    SEVector3d operator - (void) const;

    SEVector3d& operator += (const SEVector3d& rRhsVec);
    SEVector3d& operator += (double dScalar);
    SEVector3d& operator -= (const SEVector3d& rRhsVec);
    SEVector3d& operator -= (double dScalar);
    SEVector3d& operator *= (double dScalar);
    SEVector3d& operator /= (double dScalar);

    SE_CORE_API friend SEVector3d operator * (double fLhsScalar, 
        const SEVector3d& rRhsVec);

    // 求向量长度
    inline double GetLength(void) const;
    // 求向量长度平方
    inline double GetSquaredLength(void) const;
    // 求点积
	inline double Dot(const SEVector3d& rRhsVec) const;
    // 规范化
	inline double Normalize(void);

    // 注意叉积运算使用左手原则,适用于DirectX的左手坐标系,
    // OpenGL中叉积使用右手原则
    inline SEVector3d Cross(const SEVector3d& rRhsVec) const;
    inline SEVector3d UnitCross(const SEVector3d& rRhsVec) const;

    // 对线性无关的U,V,W向量进行Gram-Schmidt规范化正交
    static void Orthonormalize(SEVector3d& rVecU, SEVector3d& rVecV, 
        SEVector3d& rVecW);
    static void Orthonormalize(SEVector3d* pVec);

    // 根据W创建一组规范正交基,W必须为非0向量,
    // U,V,W将构成左手坐标系
    static void GetOrthonormalBasis(SEVector3d& rVecU, SEVector3d& rVecV, 
        SEVector3d& rVecW, bool bUnitLengthW);

    // 根据输入的若干顶点,计算其空间分布最小值和最大值
	static void ComputeExtremes(int iCount, const SEVector3d* pPoints, 
        SEVector3d& rMin, SEVector3d& rMax);

    static const SEVector3d ZERO;
    static const SEVector3d UNIT_X;
    static const SEVector3d UNIT_Y;
    static const SEVector3d UNIT_Z;

private:
    inline int CompareData(const SEVector3d& rVec) const;
};

#include "SEVector3.inl"

}

#endif