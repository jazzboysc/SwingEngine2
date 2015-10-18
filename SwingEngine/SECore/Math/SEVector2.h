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

#ifndef Swing_Vector2_H
#define Swing_Vector2_H

#include "SECoreLIB.h"
#include "SEMath.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20070512
//----------------------------------------------------------------------------
class SE_CORE_API SEVector2f
{
public:
    union
    {
        struct
        {
            float X;
            float Y;
        };
        float m_fData[2];
    };

public:
    SEVector2f(void);
    SEVector2f(float fX, float fY);
    SEVector2f(const SEVector2f& rVec);

    // 转换为连续指针空间
    operator const float* (void) const;
    operator float* (void);
    // 取第i元素形式
    float operator [] (int i) const;
    float& operator [] (int i);

    SEVector2f& operator = (const SEVector2f& rVec);

    bool operator == (const SEVector2f& rVec) const;
    bool operator != (const SEVector2f& rVec) const;
    bool operator < (const SEVector2f& rVec) const;
    bool operator <= (const SEVector2f& rVec) const;
    bool operator > (const SEVector2f& rVec) const;
    bool operator >= (const SEVector2f& rVec) const;

    SEVector2f operator + (const SEVector2f& rRhsVec) const;
    SEVector2f operator - (const SEVector2f& rRhsVec) const;
    SEVector2f operator * (float fScalar) const;
    SEVector2f operator / (float fScalar) const;
    SEVector2f operator - (void) const;

    SEVector2f& operator += (const SEVector2f& rRhsVec);
    SEVector2f& operator += (float fScalar);
    SEVector2f& operator -= (const SEVector2f& rRhsVec);
    SEVector2f& operator -= (float fScalar);
    SEVector2f& operator *= (float fScalar);
    SEVector2f& operator /= (float fScalar);

    SE_CORE_API friend SEVector2f operator * (float fLhsScalar,
        const SEVector2f& rRhsVec);

    // 求向量长度
    inline float GetLength(void) const;
    // 求向量长度平方
    inline float GetSquaredLength(void) const;
    // 求点积
    inline float Dot(const SEVector2f& rVec) const;
    // 规范化
	inline float Normalize(void);

    // 获取垂直于当前向量的向量(y,-x)
    inline void GetPerp(SEVector2f& rDesVec) const;

    // 获取((x,y),(V.x,V.y)) = x*V.y - y*V.x
    inline float GetDotPerp(const SEVector2f& rVec) const;

	// 对2个线性无关的向量进行Gram-Schmidt规范化正交.
    static void Orthonormalize(SEVector2f& rVecU, SEVector2f& rVecV);

    // 根据V创建一组规范正交基,V必须为非0向量
    static void GetOrthonormalBasis(SEVector2f& rVecU, SEVector2f& rVecV,
        bool bUnitLengthV);

    static const SEVector2f ZERO;
    static const SEVector2f UNIT_X;
    static const SEVector2f UNIT_Y;

private:
    inline int CompareData(const SEVector2f& rVec) const;
};

//----------------------------------------------------------------------------
// Description:
// Date:20090513
//----------------------------------------------------------------------------
class SE_CORE_API SEVector2d
{
public:
    union
    {
        struct
        {
            double X;
            double Y;
        };
        double m_dData[2];
    };

public:
    SEVector2d(void);
    SEVector2d(double dX, double dY);
    SEVector2d(const SEVector2d& rVec);

    // 转换为连续指针空间
    operator const double* (void) const;
    operator double* (void);
    // 取第i元素形式
    double operator [] (int i) const;
    double& operator [] (int i);

    SEVector2d& operator = (const SEVector2d& rVec);

    bool operator == (const SEVector2d& rVec) const;
    bool operator != (const SEVector2d& rVec) const;
    bool operator < (const SEVector2d& rVec) const;
    bool operator <= (const SEVector2d& rVec) const;
    bool operator > (const SEVector2d& rVec) const;
    bool operator >= (const SEVector2d& rVec) const;

    SEVector2d operator + (const SEVector2d& rRhsVec) const;
    SEVector2d operator - (const SEVector2d& rRhsVec) const;
    SEVector2d operator * (double dScalar) const;
    SEVector2d operator / (double dScalar) const;
    SEVector2d operator - (void) const;

    SEVector2d& operator += (const SEVector2d& rRhsVec);
    SEVector2d& operator += (double dScalar);
    SEVector2d& operator -= (const SEVector2d& rRhsVec);
    SEVector2d& operator -= (double dScalar);
    SEVector2d& operator *= (double dScalar);
    SEVector2d& operator /= (double dScalar);

    SE_CORE_API friend SEVector2d operator * (double dLhsScalar, 
        const SEVector2d& rRhsVec);

    // 求向量长度
    inline double GetLength(void) const;
    // 求向量长度平方
    inline double GetSquaredLength(void) const;
    // 求点积
    inline double Dot(const SEVector2d& rVec) const;
    // 规范化
	inline double Normalize(void);

    // 获取垂直于当前向量的向量(y,-x)
    inline void GetPerp(SEVector2d& rDesVec) const;

    // 获取((x,y),(V.x,V.y)) = x*V.y - y*V.x
    inline double GetDotPerp(const SEVector2d& rVec) const;

	// 对2个线性无关的向量进行Gram-Schmidt规范化正交.
    static void Orthonormalize(SEVector2d& rVecU, SEVector2d& rVecV);

    // 根据V创建一组规范正交基,V必须为非0向量
    static void GetOrthonormalBasis(SEVector2d& rVecU, SEVector2d& rVecV,
        bool bUnitLengthV);

    static const SEVector2d ZERO;
    static const SEVector2d UNIT_X;
    static const SEVector2d UNIT_Y;

private:
    inline int CompareData(const SEVector2d& rVec) const;
};

#include "SEVector2.inl"

}

#endif