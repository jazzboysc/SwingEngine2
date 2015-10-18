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

#ifndef Swing_Vector4_H
#define Swing_Vector4_H

#include "SECoreLIB.h"
#include "SEMath.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20070512
//----------------------------------------------------------------------------
class SE_CORE_API SEVector4f
{
public:
    union
    {
        struct
        {
            float X;
            float Y;
            float Z;
            float W;
        };
        float m_fData[4];
    };

public:
    SEVector4f(void);
    SEVector4f(float fX, float fY, float fZ, float fW);
    SEVector4f(const SEVector4f& rVec);

	// 转换为连续指针空间
    operator const float* (void) const;
    operator float* (void);
	// 取第i元素形式
    float operator [] (int i) const;
    float& operator [] (int i);

    SEVector4f& operator = (const SEVector4f& rVec);

    bool operator == (const SEVector4f& rVec) const;
    bool operator != (const SEVector4f& rVec) const;
    bool operator <  (const SEVector4f& rVec) const;
    bool operator <= (const SEVector4f& rVec) const;
    bool operator >  (const SEVector4f& rVec) const;
    bool operator >= (const SEVector4f& rVec) const;

    SEVector4f operator + (const SEVector4f& rRhsVec) const;
    SEVector4f operator - (const SEVector4f& rRhsVec) const;
    SEVector4f operator * (float fScalar) const;
    SEVector4f operator / (float fScalar) const;
    SEVector4f operator - (void) const;

    SEVector4f& operator += (const SEVector4f& rRhsVec);
    SEVector4f& operator += (float fScalar);
    SEVector4f& operator -= (const SEVector4f& rRhsVec);
    SEVector4f& operator -= (float fScalar);
    SEVector4f& operator *= (float fScalar);
    SEVector4f& operator /= (float fScalar);

    SE_CORE_API friend SEVector4f operator * (float fLhsScalar, 
        const SEVector4f& rRhsVec);
	
    // 求向量长度
    inline float GetLength(void) const;
    // 求向量长度平方
    inline float GetSquaredLength(void) const;
    // 求点积
    inline float Dot(const SEVector4f& rRhsVec) const;
	// 规范化
	inline float Normalize(void);

    static const SEVector4f ZERO;
    static const SEVector4f UNIT_X;
    static const SEVector4f UNIT_Y;
    static const SEVector4f UNIT_Z;
    static const SEVector4f UNIT_W;

private:
    inline int CompareData(const SEVector4f& rVec) const;
};

//----------------------------------------------------------------------------
// Description:
// Date:20090515
//----------------------------------------------------------------------------
class SE_CORE_API SEVector4d
{
public:
    union
    {
        struct
        {
            double X;
            double Y;
            double Z;
            double W;
        };
        double m_dData[4];
    };

public:
    SEVector4d(void);
    SEVector4d(double dX, double dY, double dZ, double dW);
    SEVector4d(const SEVector4d& rVec);

	// 转换为连续指针空间
    operator const double* (void) const;
    operator double* (void);
	// 取第i元素形式
    double operator [] (int i) const;
    double& operator [] (int i);

    SEVector4d& operator = (const SEVector4d& rVec);

    bool operator == (const SEVector4d& rVec) const;
    bool operator != (const SEVector4d& rVec) const;
    bool operator <  (const SEVector4d& rVec) const;
    bool operator <= (const SEVector4d& rVec) const;
    bool operator >  (const SEVector4d& rVec) const;
    bool operator >= (const SEVector4d& rVec) const;

    SEVector4d operator + (const SEVector4d& rRhsVec) const;
    SEVector4d operator - (const SEVector4d& rRhsVec) const;
    SEVector4d operator * (double dScalar) const;
    SEVector4d operator / (double dScalar) const;
    SEVector4d operator - (void) const;

    SEVector4d& operator += (const SEVector4d& rRhsVec);
    SEVector4d& operator += (double dScalar);
    SEVector4d& operator -= (const SEVector4d& rRhsVec);
    SEVector4d& operator -= (double dScalar);
    SEVector4d& operator *= (double dScalar);
    SEVector4d& operator /= (double dScalar);

    SE_CORE_API friend SEVector4d operator * (double dLhsScalar, 
        const SEVector4d& rRhsVec);
	
    // 求向量长度
    inline double GetLength(void) const;
    // 求向量长度平方
    inline double GetSquaredLength(void) const;
    // 求点积
    inline double Dot(const SEVector4d& rRhsVec) const;
	// 规范化
	inline double Normalize(void);

    static const SEVector4d ZERO;
    static const SEVector4d UNIT_X;
    static const SEVector4d UNIT_Y;
    static const SEVector4d UNIT_Z;
    static const SEVector4d UNIT_W;

private:
    inline int CompareData(const SEVector4d& rVec) const;
};

typedef SE_ALIGN16 SEVector4f SEVector4fA16;
typedef SE_ALIGN16 SEVector4d SEVector4dA16;

#include "SEVector4.inl"

}

#endif