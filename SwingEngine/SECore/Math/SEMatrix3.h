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

#ifndef Swing_Matrix3_H
#define Swing_Matrix3_H

// Matrices in Swing Engine are stored in row-major order.
// CAUTION: By default, DirectX uses row-major order, OpenGL uses column-major
// order. For example, given a matrix M in DirectX, the corresponding matrix
// in OpenGL is M^T.

// Matrix operation is concatenated from left to right. Say, given a matrix
// M and a vector V, V multiply M equals V*M, V is viewed as a row vector. 
// Some graphics APIs express the multiplication as M'*V', in which cases 
// V' equals V but is viewed as a column vector, and M' is the transpose of M.
// So a sequence of transformation should be expressed as V' = V*M0*M1...Mn,
// Others graphics APIs express the operation from right to left, in which 
// cases the sequence of transformation should be expressed as V' = Mn...M1*M0*V.
// CAUTION: DirectX uses the rule of from left to right, OpenGL uses the rule of
// from right to left.
// Say, in DirectX, given M1 and M2, the equivalent items in OpenGL are M1^T, 
// M2^T, ^T means transpose operation.
// M1*M2 is equivalent to (M1*M2)^T = M2^T*M1^T.

// Some natures of rotation matrix are referenced from this book:
// <<Quaternions and rotation sequences>> by Jack B. Kuipers.
// Orthogonal matrix is in the form of the matrix looks like: 
// A*A^T = I, A^T = A^(-1), and det^2(A) = det^2(A^T) = 1.
// iff(if and only if condition) of rotation matrix:
// Rotation matrix is orthogonal matrix, and it's determinant always equals +1.
// The inverse matrix of orthogonal matrix equals it's transpose matrix.
// In order to avoid the inverse operation, we could multiply a row vector on 
// the left side of the transpose matrix.
// But after a more deep looking, we can see that even the transpose itself
// could be avoided too. To achieve this goal, we simply apply a column vector
// which is equivalent to the original row vector on the right side of the 
// original rotation matrix.

// If we apply a row vector on the left side of a rotation matrix,
// If we define that when t > 0, the rotation about an axis is clockwise order,
// then the rotations about X,Y,Z axes are:
//   RX =    1       0       0
//           0     cos(t)  sin(t)
//           0    -sin(t)  cos(t)
//
//   RY =  cos(t)    0    -sin(t)
//           0       1       0
//         sin(t)    0     cos(t)
//
//   RZ =  cos(t)  sin(t)    0
//        -sin(t)  cos(t)    0
//           0       0       1
//
// If we apply a column vector on the right side of a rotation matrix,
// If we define that when t > 0, the rotation about an axis is counter-clockwise 
// order, then the rotations about X,Y,Z axes are:
//   RX' =   1       0       0
//           0     cos(t)  -sin(t)
//           0     sin(t) cos(t)
//
//   RY' = cos(t)    0     sin(t)
//           0       1       0
//        -sin(t)    0     cos(t)
//
//   RZ' = cos(t) -sin(t)    0
//         sin(t)  cos(t)    0
//           0       0       1
//
// Swing Engine uses these rules:
// (1) Left-hand based coordinate system.
// (2) Rotation about an arbitrary axis with an angle theta, the order is 
//     clockwise when theta > 0.
// (3) Direction of cross product of 3D vectors is based on Left-hand rule.

#include "SECoreLIB.h"
#include "SEVector3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Author: Sun Che
// Date: 20070512
//----------------------------------------------------------------------------
class SE_CORE_API SEMatrix3f
{
public:
    inline float M11(void) const;
    inline float& M11(void);
    inline float M12(void) const;
    inline float& M12(void);
    inline float M13(void) const;
    inline float& M13(void);
    inline float M21(void) const;
    inline float& M21(void);
    inline float M22(void) const;
    inline float& M22(void);
    inline float M23(void) const;
    inline float& M23(void);
    inline float M31(void) const;
    inline float& M31(void);
    inline float M32(void) const;
    inline float& M32(void);
    inline float M33(void) const;
    inline float& M33(void);
private:
    float m_fData[3][3];

public:
    SEMatrix3f(void);
    SEMatrix3f(const SEMatrix3f& rMat);
    SEMatrix3f(float fM11, float fM12, float fM13,
             float fM21, float fM22, float fM23,
             float fM31, float fM32, float fM33);
    // By defalut, construct a row-major order matrix.
    SEMatrix3f(const SEVector3f& rVecU, const SEVector3f& rVecV, 
        const SEVector3f& rVecW, bool bIsRow = true);
    // Construct a diagonal matrix.
    SEMatrix3f(float fM11, float fM22, float fM33);
    // 创建绕任意轴旋转矩阵,弧度制,fAngle > 0为顺时针旋转,旋转轴必须为单位向量
    SEMatrix3f(const SEVector3f& rAxisVec, float fAngle);
    // 用列向量U和行向量V的张量积构建矩阵
    SEMatrix3f(const SEVector3f& rVecU, const SEVector3f& rVecV);

    operator const float* (void) const;
    operator float* (void);
    const float* operator [] (int iCurRow) const;
    float* operator [] (int iCurRow);
    float operator () (int iCurRow, int iCurCol) const;
    float& operator () (int iCurRow, int iCurCol);

    inline void SetRow(int iDesRow, const SEVector3f& rSrcVec);
    inline void GetRow(int iSrcRow, SEVector3f& rDesVec) const;
    inline void SetColumn(int iDesCol, const SEVector3f& rSrcVec);
    inline void GetColumn(int iSrcCol, SEVector3f& rDesVec) const;

    SEMatrix3f& operator = (const SEMatrix3f& rMat);

    bool operator == (const SEMatrix3f& rMat) const;
    bool operator != (const SEMatrix3f& rMat) const;
    bool operator < (const SEMatrix3f& rMat) const;
    bool operator <= (const SEMatrix3f& rMat) const;
    bool operator > (const SEMatrix3f& rMat) const;
    bool operator >= (const SEMatrix3f& rMat) const;

    SEMatrix3f operator + (const SEMatrix3f& rRhsMat) const;
    SEMatrix3f operator - (const SEMatrix3f& rRhsMat) const;
    SEMatrix3f operator * (const SEMatrix3f& rRhsMat) const;
    SEMatrix3f operator * (float fScalar) const;
    SEMatrix3f operator / (float fScalar) const;
    SEMatrix3f operator - (void) const;

    SEMatrix3f& operator += (const SEMatrix3f& rRhsMat);
    SEMatrix3f& operator -= (const SEMatrix3f& rRhsMat);
    SEMatrix3f& operator *= (float fScalar);
    SEMatrix3f& operator /= (float fScalar);

    SEVector3f operator * (const SEVector3f& rRhsVec) const;

    SE_CORE_API friend SEMatrix3f operator * (float fLhsScalar, 
        const SEMatrix3f& rRhsMat);
    SE_CORE_API friend SEVector3f operator * (const SEVector3f& rLhsVec, 
        const SEMatrix3f& rRhsMat);

    inline void Zero(void);
    inline void Identity(void);
    inline void Diagonal(float fM11, float fM22, float fM33);
    inline void TensorProduct(const SEVector3f& rVecU, const SEVector3f& 
        rVecV);
    void Transpose(void);
    inline void GetTranspose(SEMatrix3f& rDesMat) const;
    void Inverse(void);
    inline void GetInverse(SEMatrix3f& rDesMat) const;
    inline void GetAdjoint(SEMatrix3f& rDesMat) const;
    inline float GetDeterminant(void) const;
    inline void GetTimesDiagonal(const SEVector3f& rDiag, 
        SEMatrix3f& rDesMat) const;
    inline void GetDiagonalTimes(const SEVector3f& rDiag, 
        SEMatrix3f& rDesMat) const;
    // this^T * M
    inline void GetTransposeTimes(const SEMatrix3f& rRhsMat, 
        SEMatrix3f& rDesMat) const;
    // this * M^T
    inline void GetTimesTranspose(const SEMatrix3f& rRhsMat, 
        SEMatrix3f& rDesMat) const;

    inline void FromAxisAngle(const SEVector3f& rAxisVec, float fAngle);
    inline void ToAxisAngle(SEVector3f& rAxisVec, float& rfAngle) const;
    // Gram-Schmidt normalization.
    void Orthonormalize(void);

    SEMatrix3f& FromEulerAnglesXYZ(float fYAngle, float fPAngle, 
        float fRAngle);
    SEMatrix3f& FromEulerAnglesXZY(float fYAngle, float fPAngle, 
        float fRAngle);
    SEMatrix3f& FromEulerAnglesYXZ(float fYAngle, float fPAngle, 
        float fRAngle);
    SEMatrix3f& FromEulerAnglesYZX(float fYAngle, float fPAngle, 
        float fRAngle);
    SEMatrix3f& FromEulerAnglesZXY(float fYAngle, float fPAngle, 
        float fRAngle);
    SEMatrix3f& FromEulerAnglesZYX(float fYAngle, float fPAngle, 
        float fRAngle);

    bool ToEulerAnglesXYZ(float& rfYAngle, float& rfPAngle, 
        float& rfRAngle) const;
    bool ToEulerAnglesXZY(float& rfYAngle, float& rfPAngle, 
        float& rfRAngle) const;
    bool ToEulerAnglesYXZ(float& rfYAngle, float& rfPAngle, 
        float& rfRAngle) const;
    bool ToEulerAnglesYZX(float& rfYAngle, float& rfPAngle, 
        float& rfRAngle) const;
    bool ToEulerAnglesZXY(float& rfYAngle, float& rfPAngle, 
        float& rfRAngle) const;
    bool ToEulerAnglesZYX(float& rfYAngle, float& rfPAngle, 
        float& rfRAngle) const;

    static const SEMatrix3f ZERO;
    static const SEMatrix3f IDENTITY;

private:
    inline int CompareData(const SEMatrix3f& rMat) const;
};

//----------------------------------------------------------------------------
// Description:
// Author: Sun Che
// Date: 20090517
//----------------------------------------------------------------------------
class SE_CORE_API SEMatrix3d
{
public:
    inline double M11(void) const;
    inline double& M11(void);
    inline double M12(void) const;
    inline double& M12(void);
    inline double M13(void) const;
    inline double& M13(void);
    inline double M21(void) const;
    inline double& M21(void);
    inline double M22(void) const;
    inline double& M22(void);
    inline double M23(void) const;
    inline double& M23(void);
    inline double M31(void) const;
    inline double& M31(void);
    inline double M32(void) const;
    inline double& M32(void);
    inline double M33(void) const;
    inline double& M33(void);
private:
    double m_dData[3][3];

public:
    SEMatrix3d(void);
    SEMatrix3d(const SEMatrix3d& rMat);
    SEMatrix3d(double dM11, double dM12, double dM13,
            double dM21, double dM22, double dM23,
            double dM31, double dM32, double dM33);
    // 默认用行向量构建矩阵
    SEMatrix3d(const SEVector3d& rVecU, const SEVector3d& rVecV, 
        const SEVector3d& rVecW, bool bIsRow = true);
    // 创建对角矩阵
    SEMatrix3d(double dM11, double dM22, double dM33);
    // 创建绕任意轴旋转矩阵,弧度制,fAngle > 0为顺时针旋转,旋转轴必须为单位向量
    SEMatrix3d(const SEVector3d& rAxisVec, double dAngle);
    // 用列向量U和行向量V的张量积构建矩阵
    SEMatrix3d(const SEVector3d& rVecU, const SEVector3d& rVecV);

    operator const double* (void) const;
    operator double* (void);
    const double* operator [] (int iCurRow) const;
    double* operator [] (int iCurRow);
    double operator () (int iCurRow, int iCurCol) const;
    double& operator () (int iCurRow, int iCurCol);

    inline void SetRow(int iDesRow, const SEVector3d& rSrcVec);
    inline void GetRow(int iSrcRow, SEVector3d& rDesVec) const;
    inline void SetColumn(int iDesCol, const SEVector3d& rSrcVec);
    inline void GetColumn(int iSrcCol, SEVector3d& rDesVec) const;

    SEMatrix3d& operator = (const SEMatrix3d& rMat);

    bool operator == (const SEMatrix3d& rMat) const;
    bool operator != (const SEMatrix3d& rMat) const;
    bool operator < (const SEMatrix3d& rMat) const;
    bool operator <= (const SEMatrix3d& rMat) const;
    bool operator > (const SEMatrix3d& rMat) const;
    bool operator >= (const SEMatrix3d& rMat) const;

    SEMatrix3d operator + (const SEMatrix3d& rRhsMat) const;
    SEMatrix3d operator - (const SEMatrix3d& rRhsMat) const;
    SEMatrix3d operator * (const SEMatrix3d& rRhsMat) const;
    SEMatrix3d operator * (double dScalar) const;
    SEMatrix3d operator / (double dScalar) const;
    SEMatrix3d operator - (void) const;

    SEMatrix3d& operator += (const SEMatrix3d& rRhsMat);
    SEMatrix3d& operator -= (const SEMatrix3d& rRhsMat);
    SEMatrix3d& operator *= (double dScalar);
    SEMatrix3d& operator /= (double dScalar);

    // 乘以列向量
    SEVector3d operator * (const SEVector3d& rRhsVec) const;

    SE_CORE_API friend SEMatrix3d operator * (double dLhsScalar, 
        const SEMatrix3d& rRhsMat);
    SE_CORE_API friend SEVector3d operator * (const SEVector3d& rLhsVec, 
        const SEMatrix3d& rRhsMat);

    // 置为0矩阵
    inline void Zero(void);
    // 置为单位矩阵
    inline void Identity(void);
    // 置为对角矩阵
    inline void Diagonal(double dM11, double dM22, double dM33);
    // 列向量与行向量的张量积构建矩阵
    inline void TensorProduct(const SEVector3d& rVecU, const SEVector3d& 
        rVecV);
    // 将自己转置
    void Transpose(void);
    // 获取转置矩阵
    inline void GetTranspose(SEMatrix3d& rDesMat) const;
    // 将自己求逆
    void Inverse(void);
    // 获取逆矩阵
    inline void GetInverse(SEMatrix3d& rDesMat) const;
    // 获取伴随矩阵
    inline void GetAdjoint(SEMatrix3d& rDesMat) const;
    // 获取行列式
    inline double GetDeterminant(void) const;
    // 获取当前矩阵乘以对角矩阵,对角矩阵只需传入对角线向量
    inline void GetTimesDiagonal(const SEVector3d& rDiag, 
        SEMatrix3d& rDesMat) const;
    // 获取对角矩阵乘以当前矩阵,对角矩阵只需传入对角线向量
    inline void GetDiagonalTimes(const SEVector3d& rDiag, 
        SEMatrix3d& rDesMat) const;
    // this^T * M
    inline void GetTransposeTimes(const SEMatrix3d& rRhsMat, 
        SEMatrix3d& rDesMat) const;
    // this * M^T
    inline void GetTimesTranspose(const SEMatrix3d& rRhsMat, 
        SEMatrix3d& rDesMat) const;

    // 构建任意轴旋转矩阵,旋转轴向量必须为单位向量
    inline void FromAxisAngle(const SEVector3d& rAxisVec, double dAngle);
    // 必须为旋转矩阵,解析出旋转轴和旋转角度
    inline void ToAxisAngle(SEVector3d& rAxisVec, double& rdAngle) const;
    // 必须为旋转矩阵,Gram-Schmidt规范化正交使坐标基成为规范坐标基.
    void Orthonormalize(void);

    SEMatrix3d& FromEulerAnglesXYZ(double dYAngle, double dPAngle, 
        double dRAngle);
    SEMatrix3d& FromEulerAnglesXZY(double dYAngle, double dPAngle, 
        double dRAngle);
    SEMatrix3d& FromEulerAnglesYXZ(double dYAngle, double dPAngle, 
        double dRAngle);
    SEMatrix3d& FromEulerAnglesYZX(double dYAngle, double dPAngle, 
        double dRAngle);
    SEMatrix3d& FromEulerAnglesZXY(double dYAngle, double dPAngle, 
        double dRAngle);
    SEMatrix3d& FromEulerAnglesZYX(double dYAngle, double dPAngle, 
        double dRAngle);

    bool ToEulerAnglesXYZ(double& rdYAngle, double& rdPAngle, 
        double& rdRAngle) const;
    bool ToEulerAnglesXZY(double& rdYAngle, double& rdPAngle, 
        double& rdRAngle) const;
    bool ToEulerAnglesYXZ(double& rdYAngle, double& rdPAngle, 
        double& rdRAngle) const;
    bool ToEulerAnglesYZX(double& rdYAngle, double& rdPAngle, 
        double& rdRAngle) const;
    bool ToEulerAnglesZXY(double& rdYAngle, double& rdPAngle, 
        double& rdRAngle) const;
    bool ToEulerAnglesZYX(double& rdYAngle, double& rdPAngle, 
        double& rdRAngle) const;

    static const SEMatrix3d ZERO;
    static const SEMatrix3d IDENTITY;

private:
    inline int CompareData(const SEMatrix3d& rMat) const;
};

typedef SE_ALIGN16 SEMatrix3f SEMatrix3fA16;
typedef SE_ALIGN16 SEMatrix3d SEMatrix3dA16;

#include "SEMatrix3.inl"

}

#endif



