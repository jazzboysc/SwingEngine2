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

// �������ʹ������ԭ��

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

    // ת��Ϊ����ָ��ռ�
    operator const float* (void) const;
    operator float* (void);
    // ȡ��iԪ����ʽ
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

    // ����������
    inline float GetLength(void) const;
    // ����������ƽ��
    inline float GetSquaredLength(void) const;
    // ����
	inline float Dot(const SEVector3f& rRhsVec) const;
    // �淶��
	inline float Normalize(void);

    // ע��������ʹ������ԭ��,������DirectX����������ϵ,
    // OpenGL�в��ʹ������ԭ��
    inline SEVector3f Cross(const SEVector3f& rRhsVec) const;
    inline SEVector3f UnitCross(const SEVector3f& rRhsVec) const;

    // �������޹ص�U,V,W��������Gram-Schmidt�淶������
    static void Orthonormalize(SEVector3f& rVecU, SEVector3f& rVecV, 
        SEVector3f& rVecW);
    static void Orthonormalize(SEVector3f* pVec);

    // ����W����һ��淶������,W����Ϊ��0����,
    // U,V,W��������������ϵ
    static void GetOrthonormalBasis(SEVector3f& rVecU, SEVector3f& rVecV, 
        SEVector3f& rVecW, bool bUnitLengthW);

    // ������������ɶ���,������ռ�ֲ���Сֵ�����ֵ
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

    // ת��Ϊ����ָ��ռ�
    operator const double* (void) const;
    operator double* (void);
    // ȡ��iԪ����ʽ
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

    // ����������
    inline double GetLength(void) const;
    // ����������ƽ��
    inline double GetSquaredLength(void) const;
    // ����
	inline double Dot(const SEVector3d& rRhsVec) const;
    // �淶��
	inline double Normalize(void);

    // ע��������ʹ������ԭ��,������DirectX����������ϵ,
    // OpenGL�в��ʹ������ԭ��
    inline SEVector3d Cross(const SEVector3d& rRhsVec) const;
    inline SEVector3d UnitCross(const SEVector3d& rRhsVec) const;

    // �������޹ص�U,V,W��������Gram-Schmidt�淶������
    static void Orthonormalize(SEVector3d& rVecU, SEVector3d& rVecV, 
        SEVector3d& rVecW);
    static void Orthonormalize(SEVector3d* pVec);

    // ����W����һ��淶������,W����Ϊ��0����,
    // U,V,W��������������ϵ
    static void GetOrthonormalBasis(SEVector3d& rVecU, SEVector3d& rVecV, 
        SEVector3d& rVecW, bool bUnitLengthW);

    // ������������ɶ���,������ռ�ֲ���Сֵ�����ֵ
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