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

//----------------------------------------------------------------------------
inline void SEMatrixMNf::GetTranspose(SEMatrixMNf& rDesMat) const
{
	SE_ASSERT( rDesMat.m_iRows == m_iCols && rDesMat.m_iCols == m_iRows );

    for( int iRow = 0; iRow < m_iRows; iRow++ )
    {
        for( int iCol = 0; iCol < m_iCols; iCol++ )
        {
            rDesMat.m_ppData[iCol][iRow] = m_ppData[iRow][iCol];
        }
    }
}
//----------------------------------------------------------------------------
inline void SEMatrixMNf::Allocate(bool bSetToZero)
{
    // 之前要正确初始化m_iRows,m_iCols,m_iCount

    m_pData = new float[m_iCount];
    if( bSetToZero )
    {
        memset(m_pData, 0, m_iCount*sizeof(float));
    }

    m_ppData = new float*[m_iRows];
    for( int iRow = 0; iRow < m_iRows; iRow++ )
    {
        m_ppData[iRow] = &m_pData[iRow * m_iCols];
    }
}
//----------------------------------------------------------------------------
inline void SEMatrixMNf::Deallocate()
{
    delete[] m_pData;
    delete[] m_ppData;
}
//----------------------------------------------------------------------------
inline void SEMatrixMNf::SetSize(int iRows, int iCols)
{
    Deallocate();

    if( iRows > 0 && iCols > 0 )
    {
        m_iRows = iRows;
        m_iCols = iCols;
        m_iCount = m_iRows * m_iCols;

        Allocate(true);
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
inline void SEMatrixMNf::GetSize(int& riRows, int& riCols) const
{
    riRows = m_iRows;
    riCols = m_iCols;
}
//----------------------------------------------------------------------------
inline int SEMatrixMNf::GetRows() const
{
    return m_iRows;
}
//----------------------------------------------------------------------------
inline int SEMatrixMNf::GetCols() const
{
    return m_iCols;
}
//----------------------------------------------------------------------------
inline int SEMatrixMNf::GetCount() const
{
    return m_iCount;
}
//----------------------------------------------------------------------------
inline int SEMatrixMNf::CompareData(const SEMatrixMNf& rMat) const
{
    return memcmp(m_pData, rMat.m_pData, m_iCount*sizeof(float));
}
//----------------------------------------------------------------------------
inline void SEMatrixMNf::SetRow(int iDesRow, const SEVectorNf& rSrcVec)
{
    SE_ASSERT( 0 <= iDesRow && iDesRow < m_iRows );
    SE_ASSERT( rSrcVec.GetSize() == m_iCols );

    for( int iCol = 0; iCol < m_iCols; iCol++ )
    {
        m_ppData[iDesRow][iCol] = rSrcVec[iCol];
    }
}
//----------------------------------------------------------------------------
inline void SEMatrixMNf::GetRow(int iSrcRow, SEVectorNf& rDesVec) const
{
    SE_ASSERT( 0 <= iSrcRow && iSrcRow < m_iRows );
    SE_ASSERT( rDesVec.GetSize() == m_iRows );

    for( int iCol = 0; iCol < m_iCols; iCol++ )
    {
        rDesVec[iCol] = m_ppData[iSrcRow][iCol];
    }
}
//----------------------------------------------------------------------------
inline void SEMatrixMNf::SetCol(int iDesCol, const SEVectorNf& rSrcVec)
{
    SE_ASSERT( 0 <= iDesCol && iDesCol < m_iCols );
    SE_ASSERT( rSrcVec.GetSize() == m_iRows );

    for( int iRow = 0; iRow < m_iRows; iRow++ )
    {
        m_ppData[iRow][iDesCol] = rSrcVec[iRow];
    }
}
//----------------------------------------------------------------------------
inline void SEMatrixMNf::GetCol(int iSrcCol, SEVectorNf& rDesVec) const
{
    SE_ASSERT( 0 <= iSrcCol && iSrcCol < m_iCols );
	SE_ASSERT( rDesVec.GetSize() == m_iCols );

    for( int iRow = 0; iRow < m_iRows; iRow++ )
    {
        rDesVec[iRow] = m_ppData[iRow][iSrcCol];
    }
}
//----------------------------------------------------------------------------