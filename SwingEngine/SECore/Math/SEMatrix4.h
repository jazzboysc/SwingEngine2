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

#ifndef Swing_Matrix4_H
#define Swing_Matrix4_H

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

#include "SECoreLIB.h"
#include "SEVector4.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20070512
//----------------------------------------------------------------------------
class SE_CORE_API SEMatrix4f
{
public:
    union
    {
        struct
        {
            float M11, M12, M13, M14;
            float M21, M22, M23, M24;
            float M31, M32, M33, M34;
            float M41, M42, M43, M44;
        };
        float m_fData[4][4];
    };

public:
    SEMatrix4f(void);
    SEMatrix4f(const SEMatrix4f& rMat);
    SEMatrix4f(float fM11, float fM12, float fM13, float fM14,
             float fM21, float fM22, float fM23, float fM24,
             float fM31, float fM32, float fM33, float fM34,
             float fM41, float fM42, float fM43, float fM44);

public:    
    operator const float* (void) const;
    operator float* (void);
    const float* operator [] (int iCurRow) const;
    float* operator [] (int iCurRow);
    float operator () (int iCurRow, int iCurCol) const;
    float& operator () (int iCurRow, int iCurCol);

    inline void SetRow(int iDesRow, const SEVector4f& rSrcVec);
    inline void GetRow(int iSrcRow, SEVector4f& rDesVec) const;
    inline void SetColumn(int iDesCol, const SEVector4f& rSrcVec);
    inline void GetColumn(int iSrcCol, SEVector4f& rDesVec) const;

    SEMatrix4f& operator = (const SEMatrix4f& rMat);

    bool operator == (const SEMatrix4f& rMat) const;
    bool operator != (const SEMatrix4f& rMat) const;
    bool operator <  (const SEMatrix4f& rMat) const;
    bool operator <= (const SEMatrix4f& rMat) const;
    bool operator >  (const SEMatrix4f& rMat) const;
    bool operator >= (const SEMatrix4f& rMat) const;

    SEMatrix4f operator + (const SEMatrix4f& rRhsMat) const;
    SEMatrix4f operator - (const SEMatrix4f& rRhsMat) const;
    SEMatrix4f operator * (const SEMatrix4f& rRhsMat) const;
    SEMatrix4f operator * (float fScalar) const;
    SEMatrix4f operator / (float fScalar) const;
    SEMatrix4f operator - (void) const;

    SEMatrix4f& operator += (const SEMatrix4f& rRhsMat);
    SEMatrix4f& operator -= (const SEMatrix4f& rRhsMat);
    SEMatrix4f& operator *= (float fScalar);
    SEMatrix4f& operator /= (float fScalar);

    SEVector4f operator * (const SEVector4f& rRhsVec) const;

    SE_CORE_API friend SEMatrix4f operator * (float fLhsScalar, 
        const SEMatrix4f& rRhsMat);
    SE_CORE_API friend SEVector4f operator * (const SEVector4f& rLhsVec, 
        const SEMatrix4f& rRhsMat);

    inline void Zero(void);
    inline void Identity(void);
    void Transpose(void);
    void Inverse(void);
    inline void GetTranspose(SEMatrix4f& rDesMat) const;
    inline void GetInverse(SEMatrix4f& rDesMat) const;
    inline void GetAdjoint(SEMatrix4f& rDesMat) const;
    inline float GetDeterminant(void) const;
    // this^T * M
    inline void GetTransposeTimes(const SEMatrix4f& rRhsMat, 
        SEMatrix4f& rDesMat) const;
    // this * M^T
    inline void GetTimesTranspose(const SEMatrix4f& rRhsMat, 
        SEMatrix4f& rDesMat) const;

    inline void FromAxisAngle(const SEVector3f& rAxisVec, float fAngle);
    SEMatrix4f& FromEulerAnglesXYZ(float fYAngle, float fPAngle,
        float fRAngle);

    static const SEMatrix4f ZERO;
    static const SEMatrix4f IDENTITY;

private:
    inline int CompareData(const SEMatrix4f& rMat) const;

};

//----------------------------------------------------------------------------
// Description:
// Date:20090518
//----------------------------------------------------------------------------
class SE_CORE_API SEMatrix4d
{
public:
    union
    {
        struct
        {
            double M11, M12, M13, M14;
            double M21, M22, M23, M24;
            double M31, M32, M33, M34;
            double M41, M42, M43, M44;
        };
        double m_dData[4][4];
    };

public:
    SEMatrix4d(void);
    SEMatrix4d(const SEMatrix4d& rMat);
    SEMatrix4d(double dM11, double dM12, double dM13, double dM14,
             double dM21, double dM22, double dM23, double dM24,
             double dM31, double dM32, double dM33, double dM34,
             double dM41, double dM42, double dM43, double dM44);

public:    
    operator const double* (void) const;
    operator double* (void);
    const double* operator [] (int iCurRow) const;
    double* operator [] (int iCurRow);
    double operator () (int iCurRow, int iCurCol) const;
    double& operator () (int iCurRow, int iCurCol);

    inline void SetRow(int iDesRow, const SEVector4d& rSrcVec);
    inline void GetRow(int iSrcRow, SEVector4d& rDesVec) const;
    inline void SetColumn(int iDesCol, const SEVector4d& rSrcVec);
    inline void GetColumn(int iSrcCol, SEVector4d& rDesVec) const;

    SEMatrix4d& operator = (const SEMatrix4d& rMat);

    bool operator == (const SEMatrix4d& rMat) const;
    bool operator != (const SEMatrix4d& rMat) const;
    bool operator <  (const SEMatrix4d& rMat) const;
    bool operator <= (const SEMatrix4d& rMat) const;
    bool operator >  (const SEMatrix4d& rMat) const;
    bool operator >= (const SEMatrix4d& rMat) const;

    SEMatrix4d operator + (const SEMatrix4d& rRhsMat) const;
    SEMatrix4d operator - (const SEMatrix4d& rRhsMat) const;
    SEMatrix4d operator * (const SEMatrix4d& rRhsMat) const;
    SEMatrix4d operator * (double dScalar) const;
    SEMatrix4d operator / (double dScalar) const;
    SEMatrix4d operator - (void) const;

    SEMatrix4d& operator += (const SEMatrix4d& rRhsMat);
    SEMatrix4d& operator -= (const SEMatrix4d& rRhsMat);
    SEMatrix4d& operator *= (double dScalar);
    SEMatrix4d& operator /= (double dScalar);

    SEVector4d operator * (const SEVector4d& rRhsVec) const;

    SE_CORE_API friend SEMatrix4d operator * (double dLhsScalar, 
        const SEMatrix4d& rRhsMat);
    SE_CORE_API friend SEVector4d operator * (const SEVector4d& rLhsVec, 
        const SEMatrix4d& rRhsMat);

    inline void Zero(void);
    inline void Identity(void);
    void Transpose(void);
    void Inverse(void);
    inline void GetTranspose(SEMatrix4d& rDesMat) const;
    inline void GetInverse(SEMatrix4d& rDesMat) const;
    inline void GetAdjoint(SEMatrix4d& rDesMat) const;
    inline double GetDeterminant(void) const;
    // this^T * M
    inline void GetTransposeTimes(const SEMatrix4d& rRhsMat, 
        SEMatrix4d& rDesMat) const;
    // this * M^T
    inline void GetTimesTranspose(const SEMatrix4d& rRhsMat, 
        SEMatrix4d& rDesMat) const;

    static const SEMatrix4d ZERO;
    static const SEMatrix4d IDENTITY;

private:
    inline int CompareData(const SEMatrix4d& rMat) const;

};

typedef SE_ALIGN16 SEMatrix4f SEMatrix4fA16;
typedef SE_ALIGN16 SEMatrix4d SEMatrix4dA16;

#include "SEMatrix4.inl"

}

#endif



