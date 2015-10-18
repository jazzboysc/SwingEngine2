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

#ifndef Swing_Matrix2_H
#define Swing_Matrix2_H

// ����洢��ʽ���������ȷ�ʽ.
// ע��DirectX���������ȷ�ʽ,OpenGL���������ȷ�ʽ.
// ����,DirectX�и���һ������M,OpenGL����֮��Ӧ�ľ�����M^T

// ������������������ӵķ�ʽ.����,����һ������M��һ������V,�������Ծ�����
// V*M.V������������.��Щͼ��API����M*V����ʽ,��ʱV������������,��ʱ��Mʵ��
// ����V*M���ʱ��M��ת�þ���.һϵ�������ľ���任�ɱ�ʾΪV' = V*M0*M1...Mn,
// ��Щͼ��API�����������ӵķ�ʽ,��ʱV' = Mn...M1*M0*V.
// ע��DirectX�����������ӵķ�ʽ,OpenGL�����������ӵķ�ʽ.
// ����,DirectX�и�������M1,M2,OpenGL�������Ƕ�Ӧ����M1^T,M2^T,
// M1*M2��Ӧ(M1*M2)^T = M2^T*M1^T.

// ��ת��������,�ο�<<Quaternions and rotation sequences by Jack B. Kuipers>>.
// ��������������: A*A^T = I�ľ���,��A^T = A^(-1),��det^2(A) = det^2(A^T) = 1.
// ��ת����ĳ�Ҫ����(iff):
// ��ת��������������,����������ʽ��ֵ��Ϊ+1.
// ������������������ת�þ���.Ϊ��������,���������������ת�þ���ﵽ��任
// ��Ŀ��.
// ʵ����,ת��Ҳ���Ա���,ֻҪ�õȼ۵��������ҳ˸þ���,�ʹﵽ�˲����治ת�ý���
// ��任��Ŀ��.

// ���ʹ����������˽��б任,
// ����涨t > 0ʱΪ��ֽ��(�۲췽��ķ�����)��ʱ����ת,
// ��XYƽ����ת����Ϊ:
//   R =  cos(t)  sin(t)
//       -sin(t)  cos(t)

#include "SECoreLIB.h"
#include "SEVector2.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20070512
//----------------------------------------------------------------------------
class SE_CORE_API SEMatrix2f
{
public:
    union
    {
        struct
        {
            float M11, M12;
            float M21, M22;
        };
        float m_fData[2][2];
    };

public:
    SEMatrix2f(void);
    SEMatrix2f(const SEMatrix2f& rMat);
    SEMatrix2f(float fM11, float fM12, float fM21, float fM22);
	// Ĭ������������������
    SEMatrix2f(const SEVector2f& rVecU, const SEVector2f& rVecV, bool bIsRow = 
        true);
    // �����ԽǾ���
    SEMatrix2f(float fM11, float fM22);
	// ������ת����,������,fAngle > 0Ϊ��ʱ����ת
    SEMatrix2f(float fAngle);
    // ��������U��������V����������������
    SEMatrix2f(const SEVector2f& rVecU, const SEVector2f& rVecV);

    operator const float* (void) const;
    operator float* (void);
    const float* operator [] (int iCurRow) const;
    float* operator [] (int iCurRow);
    float operator () (int iCurRow, int iCurCol) const;
    float& operator () (int iCurRow, int iCurCol);

    inline void SetRow(int iDesRow, const SEVector2f& rSrcVec);
    inline void GetRow(int iSrcRow, SEVector2f& rDesVec) const;
    inline void SetColumn(int iDesCol, const SEVector2f& rSrcVec);
    inline void GetColumn(int SrciCol, SEVector2f& rDesVec) const;

    SEMatrix2f& operator = (const SEMatrix2f& rMat);

    bool operator == (const SEMatrix2f& rMat) const;
    bool operator != (const SEMatrix2f& rMat) const;
    bool operator < (const SEMatrix2f& rMat) const;
    bool operator <= (const SEMatrix2f& rMat) const;
    bool operator > (const SEMatrix2f& rMat) const;
    bool operator >= (const SEMatrix2f& rMat) const;

    SEMatrix2f operator + (const SEMatrix2f& rMat) const;
    SEMatrix2f operator - (const SEMatrix2f& rMat) const;
    SEMatrix2f operator * (const SEMatrix2f& rMat) const;
    SEMatrix2f operator * (float fScalar) const;
    SEMatrix2f operator / (float fScalar) const;
    SEMatrix2f operator - (void) const;

    SEMatrix2f& operator += (const SEMatrix2f& rMat);
    SEMatrix2f& operator -= (const SEMatrix2f& rMat);
    SEMatrix2f& operator *= (float fScalar);
    SEMatrix2f& operator /= (float fScalar);

    SEVector2f operator * (const SEVector2f& rVec) const;

    SE_CORE_API friend SEMatrix2f operator * (float fLhsScalar, 
        const SEMatrix2f& rRhsMat);
    SE_CORE_API friend SEVector2f operator * (const SEVector2f& 
        rLhsVec, const SEMatrix2f& rRhsMat);

    // ��Ϊ0����
    inline void Zero(void);
    // ��Ϊ��λ����
    inline void Identity(void);
    // ��Ϊ�ԽǾ���
    inline void Diagonal(float fM11, float fM22);
    // ������������������������������
    inline void TensorProduct(const SEVector2f& rVecU, const SEVector2f& 
        rVecV);
    // ���Լ�ת��
    void Transpose(void);
    // ��ȡת�þ���
    inline void GetTranspose(SEMatrix2f& rDesMat) const;
    // ���Լ�����
    void Inverse(void);
    // ��ȡ�����
    inline void GetInverse(SEMatrix2f& rDesMat) const;
    // ��ȡ�������
    inline void GetAdjoint(SEMatrix2f& rDesMat) const;
    // ��ȡ����ʽ
    inline float GetDeterminant(void) const;
    // this^T * M
    inline void GetTransposeTimes(const SEMatrix2f& rRhsMat, 
        SEMatrix2f& rDesMat) const;
    // this * M^T
    inline void GetTimesTranspose(const SEMatrix2f& rRhsMat, 
        SEMatrix2f& rDesMat) const;

    // ������ת����
    inline void FromAngle(float fAngle);
    // ����Ϊ��ת����,��������ת�Ƕ�
    inline void ToAngle(float& rfAngle) const;

    static const SEMatrix2f ZERO;
    static const SEMatrix2f IDENTITY;

private:
    inline int CompareData(const SEMatrix2f& rMat) const;
};

//----------------------------------------------------------------------------
// Description:
// Date:20090516
//----------------------------------------------------------------------------
class SE_CORE_API SEMatrix2d
{
public:
    union
    {
        struct
        {
            double M11, M12;
            double M21, M22;
        };
        double m_dData[2][2];
    };

public:
    SEMatrix2d(void);
    SEMatrix2d(const SEMatrix2d& rMat);
    SEMatrix2d(double dM11, double dM12, double dM21, double dM22);
	// Ĭ������������������
    SEMatrix2d(const SEVector2d& rVecU, const SEVector2d& rVecV, bool bIsRow = 
        true);
    // �����ԽǾ���
    SEMatrix2d(double dM11, double dM22);
	// ������ת����,������,dAngle > 0Ϊ��ʱ����ת
    SEMatrix2d(double dAngle);
    // ��������U��������V����������������
    SEMatrix2d(const SEVector2d& rVecU, const SEVector2d& rVecV);

    operator const double* (void) const;
    operator double* (void);
    const double* operator [] (int iCurRow) const;
    double* operator [] (int iCurRow);
    double operator () (int iCurRow, int iCurCol) const;
    double& operator () (int iCurRow, int iCurCol);

    inline void SetRow(int iDesRow, const SEVector2d& rSrcVec);
    inline void GetRow(int iSrcRow, SEVector2d& rDesVec) const;
    inline void SetColumn(int iDesCol, const SEVector2d& rSrcVec);
    inline void GetColumn(int SrciCol, SEVector2d& rDesVec) const;

    SEMatrix2d& operator = (const SEMatrix2d& rMat);

    bool operator == (const SEMatrix2d& rMat) const;
    bool operator != (const SEMatrix2d& rMat) const;
    bool operator < (const SEMatrix2d& rMat) const;
    bool operator <= (const SEMatrix2d& rMat) const;
    bool operator > (const SEMatrix2d& rMat) const;
    bool operator >= (const SEMatrix2d& rMat) const;

    SEMatrix2d operator + (const SEMatrix2d& rMat) const;
    SEMatrix2d operator - (const SEMatrix2d& rMat) const;
    SEMatrix2d operator * (const SEMatrix2d& rMat) const;
    SEMatrix2d operator * (double dScalar) const;
    SEMatrix2d operator / (double dScalar) const;
    SEMatrix2d operator - (void) const;

    SEMatrix2d& operator += (const SEMatrix2d& rMat);
    SEMatrix2d& operator -= (const SEMatrix2d& rMat);
    SEMatrix2d& operator *= (double dScalar);
    SEMatrix2d& operator /= (double dScalar);

    SEVector2d operator * (const SEVector2d& rVec) const;

    SE_CORE_API friend SEMatrix2d operator * (double dLhsScalar, 
        const SEMatrix2d& rRhsMat);
    SE_CORE_API friend SEVector2d operator * (const SEVector2d& rLhsVec, 
        const SEMatrix2d& rRhsMat);

    // ��Ϊ0����
    inline void Zero(void);
    // ��Ϊ��λ����
    inline void Identity(void);
    // ��Ϊ�ԽǾ���
    inline void Diagonal(double dM11, double dM22);
    // ������������������������������
    inline void TensorProduct(const SEVector2d& rVecU, const SEVector2d& 
        rVecV);
    // ���Լ�ת��
    void Transpose(void);
    // ��ȡת�þ���
    inline void GetTranspose(SEMatrix2d& rDesMat) const;
    // ���Լ�����
    void Inverse(void);
    // ��ȡ�����
    inline void GetInverse(SEMatrix2d& rDesMat) const;
    // ��ȡ�������
    inline void GetAdjoint(SEMatrix2d& rDesMat) const;
    // ��ȡ����ʽ
    inline double GetDeterminant(void) const;
    // this^T * M
    inline void GetTransposeTimes(const SEMatrix2d& rRhsMat, 
        SEMatrix2d& rDesMat) const;
    // this * M^T
    inline void GetTimesTranspose(const SEMatrix2d& rRhsMat, 
        SEMatrix2d& rDesMat) const;

    // ������ת����
    inline void FromAngle(double dAngle);
    // ����Ϊ��ת����,��������ת�Ƕ�
    inline void ToAngle(double& rdAngle) const;

    static const SEMatrix2d ZERO;
    static const SEMatrix2d IDENTITY;

private:
    inline int CompareData(const SEMatrix2d& rMat) const;
};

#include "SEMatrix2.inl"

}

#endif



