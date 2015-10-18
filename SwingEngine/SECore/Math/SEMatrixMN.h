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

#ifndef Swing_MatrixMN_H
#define Swing_MatrixMN_H

#include "SECoreLIB.h"
#include "SEVectorN.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20070706
//----------------------------------------------------------------------------
class SE_CORE_API SEMatrixMNf
{
public:
    SEMatrixMNf(int iRows = 0, int iCols = 0);
    SEMatrixMNf(int iRows, int iCols, const float* afData);
    SEMatrixMNf(int iRows, int iCols, const float** aafEntry);
    SEMatrixMNf(const SEMatrixMNf& rMat);
    ~SEMatrixMNf(void);

public:
    inline void SetSize(int iRows, int iCols);
    inline void GetSize(int& riRows, int& riCols) const;
    inline int GetRows(void) const;
    inline int GetCols(void) const;
    inline int GetCount(void) const;

    operator float* (void);
    operator const float* (void) const;
    float* operator [] (int iRow);
    const float* operator [] (int iRow) const;
    void SwapRows(int iRow0, int iRow1);
    float operator () (int iRow, int iCol) const;
    float& operator () (int iRow, int iCol);
    
    inline void SetRow(int iDesRow, const SEVectorNf& rSrcVec);
    inline void GetRow(int iSrcRow, SEVectorNf& rDesVec) const;
    inline void SetCol(int iDesCol, const SEVectorNf& rSrcVec);
    inline void GetCol(int iSrcCol, SEVectorNf& rDesVec) const;

    void SetMatrix(int iRows, int iCols, const float* pData);
    void SetMatrix(int iRows, int iCols, const float** ppData);

    SEMatrixMNf& operator = (const SEMatrixMNf& rMat);

    bool operator == (const SEMatrixMNf& rMat) const;
    bool operator != (const SEMatrixMNf& rMat) const;
    bool operator <  (const SEMatrixMNf& rMat) const;
    bool operator <= (const SEMatrixMNf& rMat) const;
    bool operator >  (const SEMatrixMNf& rMat) const;
    bool operator >= (const SEMatrixMNf& rMat) const;

    SEMatrixMNf operator + (const SEMatrixMNf& rRhsMat) const;
    SEMatrixMNf operator - (const SEMatrixMNf& rRhsMat) const;
    SEMatrixMNf operator * (const SEMatrixMNf& rRhsMat) const;
    SEMatrixMNf operator * (float fScalar) const;
    SEMatrixMNf operator / (float fScalar) const;
    SEMatrixMNf operator - (void) const;

    SEMatrixMNf& operator += (const SEMatrixMNf& rRhsMat);
    SEMatrixMNf& operator -= (const SEMatrixMNf& rRhsMat);
    SEMatrixMNf& operator *= (float fScalar);
    SEMatrixMNf& operator /= (float fScalar);

    // 获取转置矩阵
    inline void GetTranspose(SEMatrixMNf& rDesMat) const;
    // 获取逆矩阵,矩阵必须是方阵,可逆方阵返回true
    bool GetInverse(SEMatrixMNf& rDesMat) const;

    // 乘以列向量
    SEVectorNf operator * (const SEVectorNf& rRhsVec) const;

    SE_CORE_API friend SEMatrixMNf operator * (float fLhsScalar,
        const SEMatrixMNf& rRhsMat);
    SE_CORE_API friend SEVectorNf operator * (const SEVectorNf& rLhsVec,
        const SEMatrixMNf& rMRhsat);

private:
    inline void Allocate(bool bSetToZero);
    inline void Deallocate(void);

    inline int CompareData(const SEMatrixMNf& rMat) const;

    int m_iRows, m_iCols, m_iCount;

    float* m_pData;

    // 每个指针指向矩阵数据的一行,用于解线性方程组时的行变换
    float** m_ppData;
};

#include "SEMatrixMN.inl"

}

#endif