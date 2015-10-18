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

#ifndef Swing_Eigen_H
#define Swing_Eigen_H

#include "SEGeometricToolsLIB.h"
#include "SEMatrix2.h"
#include "SEMatrix3.h"
#include "SEMatrix4.h"
#include "SEMatrixMN.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20070706
//----------------------------------------------------------------------------
class SE_GEOMETRICTOOLS_API SEEigen
{
public:
    SEEigen(int iSize);
    SEEigen(const SEMatrix2f& rMat);
    SEEigen(const SEMatrix3f& rMat);
    SEEigen(const SEMatrixMNf& rMat);
    ~SEEigen(void);

public:
    float& operator () (int iRow, int iCol);
    SEEigen& operator = (const SEMatrix2f& rMat);
    SEEigen& operator = (const SEMatrix3f& rMat);
    SEEigen& operator = (const SEMatrixMNf& rMat);

    // 特征向量是特征矩阵的列
    float GetEigenvalue(int i) const;
    const float* GetEigenvalues(void) const;
    void GetEigenvector(int i, SEVector2f& rVec) const;
    void GetEigenvector(int i, SEVector3f& rVec) const;
    SEVectorNf GetEigenvector(int i) const;
    const SEMatrixMNf& GetEigenvectors(void) const;

    void EigenStuff2(void);
    void EigenStuff3(void);
    void EigenStuffN(void);
    void EigenStuff(void);

    void DecrSortEigenStuff2(void);
    void DecrSortEigenStuff3(void);
    void DecrSortEigenStuffN(void);
    void DecrSortEigenStuff (void);

    void IncrSortEigenStuff2(void);
    void IncrSortEigenStuff3(void);
    void IncrSortEigenStuffN(void);
    void IncrSortEigenStuff(void);

private:
    int m_iSize;
    SEMatrixMNf m_Mat;
    float* m_pDiag;
    float* m_pSubd;

    // For odd size matrices, the Householder reduction involves an odd
    // number of reflections.  The product of these is a reflection.  The
    // QL algorithm uses rotations for further reductions.  The final
    // orthogonal matrix whose columns are the eigenvectors is a reflection,
    // so its determinant is -1.  For even size matrices, the Householder
    // reduction involves an even number of reflections whose product is a
    // rotation.  The final orthogonal matrix has determinant +1.  Many
    // algorithms that need an eigendecomposition want a rotation matrix.
    // We want to guarantee this is the case, so m_bRotation keeps track of
    // this.  The DecrSort and IncrSort further complicate the issue since
    // they swap columns of the orthogonal matrix, causing the matrix to
    // toggle between rotation and reflection.  The value m_bRotation must
    // be toggled accordingly.
    bool m_bIsRotation;
    void GuaranteeRotation(void);

    // Householder reduction to tridiagonal form
    void Tridiagonal2(void);
    void Tridiagonal3(void);
    void TridiagonalN(void);

    // QL algorithm with implicit shifting, applies to tridiagonal matrices
    bool QLAlgorithm(void);

    // sort eigenvalues from largest to smallest
    void DecreasingSort(void);

    // sort eigenvalues from smallest to largest
    void IncreasingSort(void);
};

}

#endif
