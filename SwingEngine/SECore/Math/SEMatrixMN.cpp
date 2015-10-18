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

#include "SECorePCH.h"
#include "SEMatrixMN.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEMatrixMNf::SEMatrixMNf(int iRows, int iCols)
{
    m_pData = 0;
    m_ppData = 0;

    SetSize(iRows, iCols);
}
//----------------------------------------------------------------------------
SEMatrixMNf::SEMatrixMNf(int iRows, int iCols, const float* pData)
{
    m_pData = 0;
    m_ppData = 0;

    SetMatrix(iRows, iCols, pData);
}
//----------------------------------------------------------------------------
SEMatrixMNf::SEMatrixMNf(int iRows, int iCols, const float** ppData)
{
    m_pData = 0;
    m_ppData = 0;

    SetMatrix(iRows, iCols, ppData);
}
//----------------------------------------------------------------------------
SEMatrixMNf::SEMatrixMNf(const SEMatrixMNf& rMat)
{
    m_iRows = 0;
    m_iCols = 0;
    m_iCount = 0;
    m_pData = 0;
    m_ppData = 0;
    *this = rMat;
}
//----------------------------------------------------------------------------
SEMatrixMNf::~SEMatrixMNf()
{
    Deallocate();
}
//----------------------------------------------------------------------------
SEMatrixMNf::operator float*()
{
    return m_pData;
}
//----------------------------------------------------------------------------
SEMatrixMNf::operator const float*() const
{
    return m_pData;
}
//----------------------------------------------------------------------------
float* SEMatrixMNf::operator[](int iRow)
{
    SE_ASSERT( 0 <= iRow && iRow < m_iRows );

    return m_ppData[iRow];
}
//----------------------------------------------------------------------------
const float* SEMatrixMNf::operator[](int iRow) const
{
    SE_ASSERT( 0 <= iRow && iRow < m_iRows );

    return m_ppData[iRow];
}
//----------------------------------------------------------------------------
float SEMatrixMNf::operator()(int iRow, int iCol) const
{
    return m_ppData[iRow][iCol];
}
//----------------------------------------------------------------------------
float& SEMatrixMNf::operator()(int iRow, int iCol)
{
    SE_ASSERT( 0 <= iRow && iRow < m_iRows && 0 <= iCol && iCol <= m_iCols );

    return m_ppData[iRow][iCol];
}
//----------------------------------------------------------------------------
void SEMatrixMNf::SwapRows(int iRow0, int iRow1)
{
    SE_ASSERT( 0 <= iRow0 && iRow0 < m_iRows && 0 <= iRow1 && iRow1 < 
        m_iRows );

    float* pTemp = m_ppData[iRow0];
    m_ppData[iRow0] = m_ppData[iRow1];
    m_ppData[iRow1] = pTemp;
}
//----------------------------------------------------------------------------
void SEMatrixMNf::SetMatrix(int iRows, int iCols, const float* pData)
{
    Deallocate();

    if( iRows > 0 && iCols > 0 )
    {
        m_iRows = iRows;
        m_iCols = iCols;
        m_iCount = m_iRows * m_iCols;

        Allocate(false);

        size_t uiSize = m_iCount * sizeof(float);
        memcpy(m_pData, pData, uiSize);
    }
    else
    {
        m_iRows = 0;
        m_iCols = 0;
        m_iCount = 0;
        m_pData = 0;
        m_ppData = 0;
    }
}
//----------------------------------------------------------------------------
void SEMatrixMNf::SetMatrix(int iRows, int iCols, const float** ppData)
{
    Deallocate();

    if( iRows > 0 && iCols > 0 )
    {
        m_iRows = iRows;
        m_iCols = iCols;
        m_iCount = m_iRows * m_iCols;

        Allocate(false);

        for( int iRow = 0; iRow < m_iRows; iRow++ )
        {
            for( int iCol = 0; iCol < m_iCols; iCol++ )
            {
                m_ppData[iRow][iCol] = ppData[iRow][iCol];
            }
        }
    }
    else
    {
        m_iRows = 0;
        m_iCols = 0;
        m_iCount = 0;
        m_pData = 0;
        m_ppData = 0;
    }
}
//----------------------------------------------------------------------------
SEMatrixMNf& SEMatrixMNf::operator=(const SEMatrixMNf& rMat)
{
    if( rMat.m_iCount > 0 )
    {
        if( m_iRows != rMat.m_iRows || m_iCols != rMat.m_iCols )
        {
            Deallocate();

            m_iRows = rMat.m_iRows;
            m_iCols = rMat.m_iCols;
            m_iCount = rMat.m_iCount;
            
            Allocate(false);
        }
        for( int iRow = 0; iRow < m_iRows; iRow++ )
        {
            for( int iCol = 0; iCol < m_iCols; iCol++ )
            {
                m_ppData[iRow][iCol] = rMat.m_ppData[iRow][iCol];
            }
        }
    }
    else
    {
        Deallocate();

        m_iRows = 0;
        m_iCols = 0;
        m_iCount = 0;
        m_pData = 0;
        m_ppData = 0;
    }

    return *this;
}
//----------------------------------------------------------------------------
bool SEMatrixMNf::operator==(const SEMatrixMNf& rMat) const
{
    return CompareData(rMat) == 0;
}
//----------------------------------------------------------------------------
bool SEMatrixMNf::operator!=(const SEMatrixMNf& rMat) const
{
    return CompareData(rMat) != 0;
}
//----------------------------------------------------------------------------
bool SEMatrixMNf::operator<(const SEMatrixMNf& rMat) const
{
    return CompareData(rMat) < 0;
}
//----------------------------------------------------------------------------
bool SEMatrixMNf::operator<=(const SEMatrixMNf& rMat) const
{
    return CompareData(rMat) <= 0;
}
//----------------------------------------------------------------------------
bool SEMatrixMNf::operator>(const SEMatrixMNf& rMat) const
{
    return CompareData(rMat) > 0;
}
//----------------------------------------------------------------------------
bool SEMatrixMNf::operator>=(const SEMatrixMNf& rMat) const
{
    return CompareData(rMat) >= 0;
}
//----------------------------------------------------------------------------
SEMatrixMNf SEMatrixMNf::operator+(const SEMatrixMNf& rRhsMat) const
{
    SE_ASSERT( rRhsMat.m_iCols == m_iCols && rRhsMat.m_iRows == m_iRows );

    SEMatrixMNf ResMat(rRhsMat.m_iRows, rRhsMat.m_iCols);
    for( int i = 0; i < m_iCount; i++ )
    {
        ResMat.m_pData[i] = m_pData[i] + rRhsMat.m_pData[i];
    }

    return ResMat;
}
//----------------------------------------------------------------------------
SEMatrixMNf SEMatrixMNf::operator-(const SEMatrixMNf& rRhsMat) const
{
    SE_ASSERT( rRhsMat.m_iCols == m_iCols && rRhsMat.m_iRows == m_iRows );

    SEMatrixMNf ResMat(rRhsMat.m_iRows, rRhsMat.m_iCols);
    for( int i = 0; i < m_iCount; i++ )
    {
        ResMat.m_pData[i] = m_pData[i] - rRhsMat.m_pData[i];
    }

    return ResMat;
}
//----------------------------------------------------------------------------
SEMatrixMNf SEMatrixMNf::operator*(const SEMatrixMNf& rRhsMat) const
{
    SE_ASSERT( m_iCols == rRhsMat.m_iRows );

    SEMatrixMNf ResMat(m_iRows, rRhsMat.m_iCols);
    for( int iRow = 0; iRow < ResMat.m_iRows; iRow++ )
    {
        for( int iCol = 0; iCol < ResMat.m_iCols; iCol++ )
        {
            for( int iMid = 0; iMid < m_iCols; iMid++ )
            {
                ResMat.m_ppData[iRow][iCol] += m_ppData[iRow][iMid] * 
                    rRhsMat.m_ppData[iMid][iCol];
            }
        }
    }

    return ResMat;
}
//----------------------------------------------------------------------------
SEMatrixMNf SEMatrixMNf::operator*(float fScalar) const
{
    SEMatrixMNf ResMat(m_iRows, m_iCols);
    for( int i = 0; i < m_iCount; i++ )
    {
        ResMat.m_pData[i] = fScalar * m_pData[i];
    }

    return ResMat;
}
//----------------------------------------------------------------------------
SEMatrixMNf SEMatrixMNf::operator/(float fScalar) const
{
    SEMatrixMNf ResMat(m_iRows, m_iCols);
    int i;

    if( fScalar != 0.0f )
    {
        float fInvScalar = 1.0f / fScalar;
        for( i = 0; i < m_iCount; i++ )
        {
            ResMat.m_pData[i] = fInvScalar * m_pData[i];
        }
    }
    else
    {
        for( i = 0; i < m_iCount; i++ )
        {
            ResMat.m_pData[i] = SEMath<float>::MAX_REAL;
        }
    }

    return ResMat;
}
//----------------------------------------------------------------------------
SEMatrixMNf SEMatrixMNf::operator-() const
{
    SEMatrixMNf ResMat(m_iRows, m_iCols);
    for( int i = 0; i < m_iCount; i++ )
    {
        ResMat.m_pData[i] = -m_pData[i];
    }

    return ResMat;
}
//----------------------------------------------------------------------------
SEMatrixMNf& SEMatrixMNf::operator+=(const SEMatrixMNf& rRhsMat)
{
    for( int i = 0; i < m_iCount; i++ )
    {
        m_pData[i] += rRhsMat.m_pData[i];
    }

    return *this;
}
//----------------------------------------------------------------------------
SEMatrixMNf& SEMatrixMNf::operator-=(const SEMatrixMNf& rRhsMat)
{
    for( int i = 0; i < m_iCount; i++ )
    {
        m_pData[i] -= rRhsMat.m_pData[i];
    }

    return *this;
}
//----------------------------------------------------------------------------
SEMatrixMNf& SEMatrixMNf::operator*=(float fScalar)
{
    for( int i = 0; i < m_iCount; i++ )
    {
        m_pData[i] *= fScalar;
    }

    return *this;
}
//----------------------------------------------------------------------------
SEMatrixMNf& SEMatrixMNf::operator/=(float fScalar)
{
    int i;

    if( fScalar != 0.0f )
    {
        float fInvScalar = 1.0f / fScalar;
        for( i = 0; i < m_iCount; i++ )
        {
            m_pData[i] *= fInvScalar;
        }
    }
    else
    {
        for( i = 0; i < m_iCount; i++ )
        {
            m_pData[i] = SEMath<float>::MAX_REAL;
        }
    }

    return *this;
}
//----------------------------------------------------------------------------
SEVectorNf SEMatrixMNf::operator*(const SEVectorNf& rRhsVec) const
{
    SE_ASSERT( rRhsVec.GetSize() == m_iCols );

    SEVectorNf ResVec(m_iRows);
    for( int iRow = 0; iRow < m_iRows; iRow++ )
    {
        for( int iCol = 0; iCol < m_iCols; iCol++ )
        {
            ResVec[iRow] += m_ppData[iRow][iCol] * rRhsVec[iCol];
        }
            
    }
    return ResVec;
}
//----------------------------------------------------------------------------
bool SEMatrixMNf::GetInverse(SEMatrixMNf& rDesMat) const
{
    // 只有N阶方阵才可逆
    if( GetRows() > 0 && GetRows() != GetCols() )
    {
        return false;
    }

    int iSize = GetRows();
    rDesMat = *this;

    int* pColIndex = new int[iSize];
    int* pRowIndex = new int[iSize];
    bool* pPivoted = new bool[iSize];
    memset(pPivoted, 0, iSize*sizeof(bool));

    int i1, i2, iRow = 0, iCol = 0;
    float fTempData;

    // 主元消去法
    for( int i0 = 0; i0 < iSize; i0++ )
    {
        // 查找最大矩阵元素(主元行之外)
        float fMax = 0.0f;
        for( i1 = 0; i1 < iSize; i1++ )
        {
            if( !pPivoted[i1] )
            {
                // 排除主元行

                for( i2 = 0; i2 < iSize; i2++ )
                {
                    if( !pPivoted[i2] )
                    {
                        // 排除主元列

                        float fAbs = SEMath<float>::FAbs(rDesMat[i1][i2]);
                        if( fAbs > fMax )
                        {
                            fMax = fAbs;
                            iRow = i1;
                            iCol = i2;
                        }
                    }
                }
            }
        }

        if( fMax == 0.0f )
        {
            // 当前可选矩阵元素都为0,因此矩阵不可逆
            delete[] pColIndex;
            delete[] pRowIndex;
            delete[] pPivoted;

            return false;
        }

        pPivoted[iCol] = true;

        // 交换两行,使主元在对角线A[iCol][iCol]上
        if( iRow != iCol )
        {
            rDesMat.SwapRows(iRow, iCol);
        }

        // 跟踪行位置的排列变换情况
        pRowIndex[i0] = iRow;
        pColIndex[i0] = iCol;

        // 缩放主元行,使主元为1
        float fInv = 1.0f / rDesMat[iCol][iCol];
        rDesMat[iCol][iCol] = 1.0f;
        for( i2 = 0; i2 < iSize; i2++ )
        {
            rDesMat[iCol][i2] *= fInv;
        }

        for( i1 = 0; i1 < iSize; i1++ )
        {
            if( i1 != iCol )
            {
                fTempData = rDesMat[i1][iCol];
                rDesMat[i1][iCol] = 0.0f;
                for( i2 = 0; i2 < iSize; i2++ )
                {
                    // i1是正在进行消去的行,iCol是主元行
                    rDesMat[i1][i2] -= rDesMat[iCol][i2] * fTempData;
                }
            }
        }
    }

    // 重新排列各行,逆矩阵由此产生
    for( i1 = iSize - 1; i1 >= 0; i1-- )
    {
        if( pRowIndex[i1] != pColIndex[i1] )
        {
            for( i2 = 0; i2 < iSize; i2++ )
            {
                fTempData = rDesMat[i2][pRowIndex[i1]];
                rDesMat[i2][pRowIndex[i1]] = rDesMat[i2][pColIndex[i1]];
                rDesMat[i2][pColIndex[i1]] = fTempData;
            }
        }
    }

    delete[] pColIndex;
    delete[] pRowIndex;
    delete[] pPivoted;

    return true;
}
//----------------------------------------------------------------------------
SEVectorNf Swing::operator*(const SEVectorNf& rLhsVec, const SEMatrixMNf& 
    rRhsMat)
{
    int iRows = rRhsMat.GetRows();
    int iCols = rRhsMat.GetCols();

    SE_ASSERT( rLhsVec.GetSize() == iRows );

    SEVectorNf ResVec(iCols);
    float* pVecData = ResVec;

    for( int iCol = 0; iCol < iCols; iCol++ )
    {
        for( int iRow = 0; iRow < iRows; iRow++ )
        {
            pVecData[iCol] += rLhsVec[iRow] * rRhsMat[iRow][iCol];
        }
    }

    return ResVec;
}
//----------------------------------------------------------------------------
SEMatrixMNf Swing::operator*(float fLhsScalar, const SEMatrixMNf& rRhsMat)
{
    SEMatrixMNf ResMat(rRhsMat.GetRows(), rRhsMat.GetCols());
    const float* pRhsMatData = rRhsMat;
    float* pResMatData = ResMat;

    for( int i = 0; i < rRhsMat.GetCount(); i++ )
    {
        pResMatData[i] = fLhsScalar * pRhsMatData[i];
    }

    return ResMat;
}
//----------------------------------------------------------------------------