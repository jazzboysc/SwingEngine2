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

#include "SEGeometricToolsPCH.h"
#include "SEEigen.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEEigen::SEEigen(int iSize)
    : m_Mat(iSize, iSize)
{
    SE_ASSERT( iSize >= 2 );

    m_iSize = iSize;
    m_pDiag = new float[m_iSize];
    m_pSubd = new float[m_iSize];
    m_bIsRotation = false;
}
//----------------------------------------------------------------------------
SEEigen::SEEigen(const SEMatrix2f& rMat)
    : m_Mat(2, 2, (const float*)rMat)
{
    m_iSize = 2;
    m_pDiag = new float[m_iSize];
    m_pSubd = new float[m_iSize];
    m_bIsRotation = false;
}
//----------------------------------------------------------------------------
SEEigen::SEEigen(const SEMatrix3f& rMat)
    : m_Mat(3, 3, (const float*)rMat)
{
    m_iSize = 3;
    m_pDiag = new float[m_iSize];
    m_pSubd = new float[m_iSize];
    m_bIsRotation = false;
}
//----------------------------------------------------------------------------
SEEigen::SEEigen(const SEMatrixMNf& rMat)
    : m_Mat(rMat)
{
    m_iSize = rMat.GetRows();
    SE_ASSERT( m_iSize >= 2 && (rMat.GetCols() == m_iSize) );

    m_pDiag = new float[m_iSize];
    m_pSubd = new float[m_iSize];
    m_bIsRotation = false;
}
//----------------------------------------------------------------------------
SEEigen::~SEEigen()
{
    delete[] m_pSubd;
    delete[] m_pDiag;
}
//----------------------------------------------------------------------------
float& SEEigen::operator()(int iRow, int iCol)
{
    return m_Mat[iRow][iCol];
}
//----------------------------------------------------------------------------
SEEigen& SEEigen::operator=(const SEMatrix2f& rMat)
{
    m_Mat.SetMatrix(2, 2, (const float*)rMat);
    m_iSize = 2;
    delete[] m_pDiag;
    delete[] m_pSubd;
    m_pDiag = new float[m_iSize];
    m_pSubd = new float[m_iSize];

    return *this;
}
//----------------------------------------------------------------------------
SEEigen& SEEigen::operator=(const SEMatrix3f& rMat)
{
    m_Mat.SetMatrix(3, 3, (const float*)rMat);
    m_iSize = 3;
    delete[] m_pDiag;
    delete[] m_pSubd;
    m_pDiag = new float[m_iSize];
    m_pSubd = new float[m_iSize];

    return *this;
}
//----------------------------------------------------------------------------
SEEigen& SEEigen::operator=(const SEMatrixMNf& rMat)
{
    m_Mat = rMat;

    return *this;
}
//----------------------------------------------------------------------------
float SEEigen::GetEigenvalue(int i) const
{
    return m_pDiag[i];
}
//----------------------------------------------------------------------------
const float* SEEigen::GetEigenvalues() const
{
    return m_pDiag;
}
//----------------------------------------------------------------------------
void SEEigen::GetEigenvector(int i, SEVector2f& rVec) const
{
    SE_ASSERT( m_iSize == 2 );

    if( m_iSize == 2 )
    {
        for( int iRow = 0; iRow < m_iSize; iRow++ )
        {
            rVec[iRow] = m_Mat[iRow][i];
        }
    }
    else
    {
        rVec = SEVector2f::ZERO;
    }
}
//----------------------------------------------------------------------------
void SEEigen::GetEigenvector(int i, SEVector3f& rVec) const
{
    SE_ASSERT( m_iSize == 3 );

    if( m_iSize == 3 )
    {
        for( int iRow = 0; iRow < m_iSize; iRow++ )
        {
            rVec[iRow] = m_Mat[iRow][i];
        }
    }
    else
    {
        rVec = SEVector3f::ZERO;
    }
}
//----------------------------------------------------------------------------
SEVectorNf SEEigen::GetEigenvector(int i) const
{
    SEVectorNf ResVec;
	m_Mat.GetCol(i, ResVec);

    return ResVec;
}
//----------------------------------------------------------------------------
const SEMatrixMNf& SEEigen::GetEigenvectors() const
{
    return m_Mat;
}
//----------------------------------------------------------------------------
void SEEigen::Tridiagonal2()
{
    // matrix is already tridiagonal
    m_pDiag[0] = m_Mat[0][0];
    m_pDiag[1] = m_Mat[1][1];
    m_pSubd[0] = m_Mat[0][1];
    m_pSubd[1] = 0.0f;
    m_Mat[0][0] = 1.0f;
    m_Mat[0][1] = 0.0f;
    m_Mat[1][0] = 0.0f;
    m_Mat[1][1] = 1.0f;

    m_bIsRotation = true;
}
//----------------------------------------------------------------------------
void SEEigen::Tridiagonal3()
{
    float fM00 = m_Mat[0][0];
    float fM01 = m_Mat[0][1];
    float fM02 = m_Mat[0][2];
    float fM11 = m_Mat[1][1];
    float fM12 = m_Mat[1][2];
    float fM22 = m_Mat[2][2];

    m_pDiag[0] = fM00;
    m_pSubd[2] = 0.0f;
    if( SEMath<float>::FAbs(fM02) > SEMath<float>::ZERO_TOLERANCE )
    {
        float fLength = SEMath<float>::Sqrt(fM01*fM01 + fM02*fM02);
        float fInvLength = 1.0f / fLength;
        fM01 *= fInvLength;
        fM02 *= fInvLength;
        float fQ = 2.0f*fM01*fM12 + fM02*(fM22 - fM11);
        m_pDiag[1] = fM11 + fM02*fQ;
        m_pDiag[2] = fM22 - fM02*fQ;
        m_pSubd[0] = fLength;
        m_pSubd[1] = fM12 - fM01*fQ;
        m_Mat[0][0] = 1.0f;
        m_Mat[0][1] = 0.0f;
        m_Mat[0][2] = 0.0f;
        m_Mat[1][0] = 0.0f;
        m_Mat[1][1] = fM01;
        m_Mat[1][2] = fM02;
        m_Mat[2][0] = 0.0f;
        m_Mat[2][1] = fM02;
        m_Mat[2][2] = -fM01;

        m_bIsRotation = false;
    }
    else
    {
        m_pDiag[1] = fM11;
        m_pDiag[2] = fM22;
        m_pSubd[0] = fM01;
        m_pSubd[1] = fM12;
        m_Mat[0][0] = 1.0f;
        m_Mat[0][1] = 0.0f;
        m_Mat[0][2] = 0.0f;
        m_Mat[1][0] = 0.0f;
        m_Mat[1][1] = 1.0f;
        m_Mat[1][2] = 0.0f;
        m_Mat[2][0] = 0.0f;
        m_Mat[2][1] = 0.0f;
        m_Mat[2][2] = 1.0f;

        m_bIsRotation = true;
    }
}
//----------------------------------------------------------------------------
void SEEigen::TridiagonalN()
{
    int i0, i1, i2, i3;

    for( i0 = m_iSize - 1, i3 = m_iSize - 2; i0 >= 1; i0--, i3-- )
    {
        float fH = 0.0f, fScale = 0.0f;

        if( i3 > 0 )
        {
            for( i2 = 0; i2 <= i3; i2++ )
            {
                fScale += SEMath<float>::FAbs(m_Mat[i0][i2]);
            }
            if( fScale == 0.0f )
            {
                m_pSubd[i0] = m_Mat[i0][i3];
            }
            else
            {
                float fInvScale = 1.0f / fScale;
                for( i2 = 0; i2 <= i3; i2++ )
                {
                    m_Mat[i0][i2] *= fInvScale;
                    fH += m_Mat[i0][i2] * m_Mat[i0][i2];
                }
                float fF = m_Mat[i0][i3];
                float fG = SEMath<float>::Sqrt(fH);
                if( fF > 0.0f )
                {
                    fG = -fG;
                }
                m_pSubd[i0] = fScale * fG;
                fH -= fF * fG;
                m_Mat[i0][i3] = fF - fG;
                fF = 0.0f;
                float fInvH = 1.0f / fH;
                for( i1 = 0; i1 <= i3; i1++ )
                {
                    m_Mat[i1][i0] = m_Mat[i0][i1] * fInvH;
                    fG = 0.0f;
                    for( i2 = 0; i2 <= i1; i2++ )
                    {
                        fG += m_Mat[i1][i2] * m_Mat[i0][i2];
                    }
                    for( i2 = i1+1; i2 <= i3; i2++ )
                    {
                        fG += m_Mat[i2][i1] * m_Mat[i0][i2];
                    }
                    m_pSubd[i1] = fG * fInvH;
                    fF += m_pSubd[i1] * m_Mat[i0][i1];
                }
                float fHalfFdivH = 0.5f * fF*fInvH;
                for( i1 = 0; i1 <= i3; i1++ )
                {
                    fF = m_Mat[i0][i1];
                    fG = m_pSubd[i1] - fHalfFdivH*fF;
                    m_pSubd[i1] = fG;
                    for( i2 = 0; i2 <= i1; i2++ )
                    {
                        m_Mat[i1][i2] -= fF*m_pSubd[i2] + fG*m_Mat[i0][i2];
                    }
                }
            }
        }
        else
        {
            m_pSubd[i0] = m_Mat[i0][i3];
        }

        m_pDiag[i0] = fH;
    }

    m_pDiag[0] = 0.0f;
    m_pSubd[0] = 0.0f;
    for( i0 = 0, i3 = -1; i0 <= m_iSize-1; i0++, i3++ )
    {
        if( m_pDiag[i0] != 0.0f )
        {
            for( i1 = 0; i1 <= i3; i1++ )
            {
                float fSum = 0.0f;
                for( i2 = 0; i2 <= i3; i2++ )
                {
                    fSum += m_Mat[i0][i2] * m_Mat[i2][i1];
                }
                for( i2 = 0; i2 <= i3; i2++ )
                {
                    m_Mat[i2][i1] -= fSum * m_Mat[i2][i0];
                }
            }
        }
        m_pDiag[i0] = m_Mat[i0][i0];
        m_Mat[i0][i0] = 1.0f;
        for( i1 = 0; i1 <= i3; i1++ )
        {
            m_Mat[i1][i0] = 0.0f;
            m_Mat[i0][i1] = 0.0f;
        }
    }

    // re-ordering if SEEigen::QLAlgorithm is used subsequently
    for( i0 = 1, i3 = 0; i0 < m_iSize; i0++, i3++ )
    {
        m_pSubd[i3] = m_pSubd[i0];
    }
    m_pSubd[m_iSize - 1] = 0.0f;

    m_bIsRotation = ((m_iSize % 2) == 0);
}
//----------------------------------------------------------------------------
bool SEEigen::QLAlgorithm()
{
    const int iMaxIter = 32;

    for( int i0 = 0; i0 < m_iSize; i0++ )
    {
        int i1;
        for( i1 = 0; i1 < iMaxIter; i1++ )
        {
            int i2;
            for( i2 = i0; i2 <= m_iSize - 2; i2++ )
            {
                float fTemp = SEMath<float>::FAbs(m_pDiag[i2]) + 
                    SEMath<float>::FAbs(m_pDiag[i2 + 1]);

                if( SEMath<float>::FAbs(m_pSubd[i2]) + fTemp == fTemp )
                {
                    break;
                }
            }
            if( i2 == i0 )
            {
                break;
            }

            float fG = (m_pDiag[i0 + 1] - m_pDiag[i0]) / (2.0f * m_pSubd[i0]);
            float fR = SEMath<float>::Sqrt(fG*fG + 1.0f);
            if( fG < 0.0f )
            {
                fG = m_pDiag[i2] - m_pDiag[i0] + m_pSubd[i0]/(fG - fR);
            }
            else
            {
                fG = m_pDiag[i2] - m_pDiag[i0] + m_pSubd[i0]/(fG + fR);
            }
            float fSin = 1.0f, fCos = 1.0f, fP = 0.0f;
            for( int i3 = i2 - 1; i3 >= i0; i3-- )
            {
                float fF = fSin * m_pSubd[i3];
                float fB = fCos * m_pSubd[i3];
                if( SEMath<float>::FAbs(fF) >= SEMath<float>::FAbs(fG) )
                {
                    fCos = fG / fF;
                    fR = SEMath<float>::Sqrt(fCos*fCos + 1.0f);
                    m_pSubd[i3 + 1] = fF * fR;
                    fSin = 1.0f / fR;
                    fCos *= fSin;
                }
                else
                {
                    fSin = fF / fG;
                    fR = SEMath<float>::Sqrt(fSin*fSin + 1.0f);
                    m_pSubd[i3 + 1] = fG * fR;
                    fCos = 1.0f / fR;
                    fSin *= fCos;
                }
                fG = m_pDiag[i3 + 1] - fP;
                fR = (m_pDiag[i3] - fG)*fSin + 2.0f*fB*fCos;
                fP = fSin * fR;
                m_pDiag[i3 + 1] = fG + fP;
                fG = fCos*fR - fB;

                for( int i4 = 0; i4 < m_iSize; i4++ )
                {
                    fF = m_Mat[i4][i3 + 1];
                    m_Mat[i4][i3 + 1] = fSin*m_Mat[i4][i3] + fCos*fF;
                    m_Mat[i4][i3] = fCos*m_Mat[i4][i3] - fSin*fF;
                }
            }
            m_pDiag[i0] -= fP;
            m_pSubd[i0] = fG;
            m_pSubd[i2] = 0.0f;
        }
        if( i1 == iMaxIter )
        {
            return false;
        }
    }

    return true;
}
//----------------------------------------------------------------------------
void SEEigen::DecreasingSort()
{
    // sort eigenvalues in decreasing order, e[0] >= ... >= e[iSize-1]
    for( int i0 = 0, i1; i0 <= m_iSize-2; i0++ )
    {
        // locate maximum eigenvalue
        i1 = i0;
        float fMax = m_pDiag[i1];
        int i2;
        for( i2 = i0 + 1; i2 < m_iSize; i2++ )
        {
            if( m_pDiag[i2] > fMax )
            {
                i1 = i2;
                fMax = m_pDiag[i1];
            }
        }

        if( i1 != i0 )
        {
            // swap eigenvalues
            m_pDiag[i1] = m_pDiag[i0];
            m_pDiag[i0] = fMax;

            // swap eigenvectors
            for( i2 = 0; i2 < m_iSize; i2++ )
            {
                float fTemp = m_Mat[i2][i0];
                m_Mat[i2][i0] = m_Mat[i2][i1];
                m_Mat[i2][i1] = fTemp;
                m_bIsRotation = !m_bIsRotation;
            }
        }
    }
}
//----------------------------------------------------------------------------
void SEEigen::IncreasingSort()
{
    // sort eigenvalues in increasing order, e[0] <= ... <= e[iSize-1]
    for(int i0 = 0, i1; i0 <= m_iSize - 2; i0++ )
    {
        // locate minimum eigenvalue
        i1 = i0;
        float fMin = m_pDiag[i1];
        int i2;
        for( i2 = i0 + 1; i2 < m_iSize; i2++ )
        {
            if( m_pDiag[i2] < fMin )
            {
                i1 = i2;
                fMin = m_pDiag[i1];
            }
        }

        if( i1 != i0 )
        {
            // swap eigenvalues
            m_pDiag[i1] = m_pDiag[i0];
            m_pDiag[i0] = fMin;

            // swap eigenvectors
            for( i2 = 0; i2 < m_iSize; i2++ )
            {
                float fTemp = m_Mat[i2][i0];
                m_Mat[i2][i0] = m_Mat[i2][i1];
                m_Mat[i2][i1] = fTemp;
                m_bIsRotation = !m_bIsRotation;
            }
        }
    }
}
//----------------------------------------------------------------------------
void SEEigen::GuaranteeRotation()
{
    if( !m_bIsRotation )
    {
        // change sign on the first column
        for( int iRow = 0; iRow < m_iSize; iRow++ )
        {
            m_Mat[iRow][0] = -m_Mat[iRow][0];
        }
    }
}
//----------------------------------------------------------------------------
void SEEigen::EigenStuff2()
{
    Tridiagonal2();
    QLAlgorithm();
    GuaranteeRotation();
}
//----------------------------------------------------------------------------
void SEEigen::EigenStuff3()
{
    Tridiagonal3();
    QLAlgorithm();
    GuaranteeRotation();
}
//----------------------------------------------------------------------------
void SEEigen::EigenStuffN()
{
    TridiagonalN();
    QLAlgorithm();
    GuaranteeRotation();
}
//----------------------------------------------------------------------------
void SEEigen::EigenStuff()
{
    switch( m_iSize )
    {
    case 2:  Tridiagonal2();  break;
    case 3:  Tridiagonal3();  break;
    default: TridiagonalN();  break;
    }
    QLAlgorithm();
    GuaranteeRotation();
}
//----------------------------------------------------------------------------
void SEEigen::DecrSortEigenStuff2()
{
    Tridiagonal2();
    QLAlgorithm();
    DecreasingSort();
    GuaranteeRotation();
}
//----------------------------------------------------------------------------
void SEEigen::DecrSortEigenStuff3()
{
    Tridiagonal3();
    QLAlgorithm();
    DecreasingSort();
    GuaranteeRotation();
}
//----------------------------------------------------------------------------
void SEEigen::DecrSortEigenStuffN()
{
    TridiagonalN();
    QLAlgorithm();
    DecreasingSort();
    GuaranteeRotation();
}
//----------------------------------------------------------------------------
void SEEigen::DecrSortEigenStuff()
{
    switch( m_iSize )
    {
    case 2:  Tridiagonal2();  break;
    case 3:  Tridiagonal3();  break;
    default: TridiagonalN();  break;
    }
    QLAlgorithm();
    DecreasingSort();
    GuaranteeRotation();
}
//----------------------------------------------------------------------------
void SEEigen::IncrSortEigenStuff2()
{
    Tridiagonal2();
    QLAlgorithm();
    IncreasingSort();
    GuaranteeRotation();
}
//----------------------------------------------------------------------------
void SEEigen::IncrSortEigenStuff3()
{
    Tridiagonal3();
    QLAlgorithm();
    IncreasingSort();
    GuaranteeRotation();
}
//----------------------------------------------------------------------------
void SEEigen::IncrSortEigenStuffN()
{
    TridiagonalN();
    QLAlgorithm();
    IncreasingSort();
    GuaranteeRotation();
}
//----------------------------------------------------------------------------
void SEEigen::IncrSortEigenStuff()
{
    switch( m_iSize )
    {
    case 2:  Tridiagonal2();  break;
    case 3:  Tridiagonal3();  break;
    default: TridiagonalN();  break;
    }
    QLAlgorithm();
    IncreasingSort();
    GuaranteeRotation();
}
//----------------------------------------------------------------------------