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
// 单精度2阶方阵类
//----------------------------------------------------------------------------
inline int SEMatrix2f::CompareData(const SEMatrix2f& rMat) const
{
    return memcmp(m_fData, rMat.m_fData, 4*sizeof(float));
}
//----------------------------------------------------------------------------
inline void SEMatrix2f::Zero()
{
    memset(m_fData, 0, 4*sizeof(float));
}
//----------------------------------------------------------------------------
inline void SEMatrix2f::Identity()
{
    m_fData[0][0] = 1.0f;
    m_fData[0][1] = 0.0f;
    m_fData[1][0] = 0.0f;
    m_fData[1][1] = 1.0f;
}
//----------------------------------------------------------------------------
inline void SEMatrix2f::Diagonal(float fM11, float fM22)
{
    m_fData[0][0] = fM11;
    m_fData[0][1] = 0.0f;
    m_fData[1][0] = 0.0f;
    m_fData[1][1] = fM22;
}
//----------------------------------------------------------------------------
inline void SEMatrix2f::FromAngle(float fAngle)
{
    m_fData[0][0] = SEMath<float>::Cos(fAngle);
    m_fData[1][0] = -SEMath<float>::Sin(fAngle);
    m_fData[0][1] = -m_fData[1][0];
    m_fData[1][1] =  m_fData[0][0];
}
//----------------------------------------------------------------------------
inline void SEMatrix2f::TensorProduct(const SEVector2f& rVecU, const 
    SEVector2f& rVecV)
{
    m_fData[0][0] = rVecU[0] * rVecV[0];
    m_fData[0][1] = rVecU[0] * rVecV[1];
    m_fData[1][0] = rVecU[1] * rVecV[0];
    m_fData[1][1] = rVecU[1] * rVecV[1];
}
//----------------------------------------------------------------------------
inline void SEMatrix2f::SetRow(int iDesRow, const SEVector2f& rSrcVec)
{
    SE_ASSERT( 0 <= iDesRow && iDesRow < 2 );

	m_fData[iDesRow][0] = rSrcVec.m_fData[0];
    m_fData[iDesRow][1] = rSrcVec.m_fData[1];
}
//----------------------------------------------------------------------------
inline void SEMatrix2f::GetRow(int iSrcRow, SEVector2f& rDesVec) const
{
    SE_ASSERT( 0 <= iSrcRow && iSrcRow < 2 );

    rDesVec.m_fData[0] = m_fData[iSrcRow][0];
    rDesVec.m_fData[1] = m_fData[iSrcRow][1];
}
//----------------------------------------------------------------------------
inline void SEMatrix2f::SetColumn(int iDesCol, const SEVector2f& rSrcVec)
{
    SE_ASSERT( 0 <= iDesCol && iDesCol < 2 );

	m_fData[0][iDesCol] = rSrcVec.m_fData[0];
	m_fData[1][iDesCol] = rSrcVec.m_fData[1];
}
//----------------------------------------------------------------------------
inline void SEMatrix2f::GetColumn(int iSrcCol, SEVector2f& rDesVec) const
{
    SE_ASSERT( 0 <= iSrcCol && iSrcCol < 2 );

	rDesVec.m_fData[0] = m_fData[0][iSrcCol];
    rDesVec.m_fData[1] = m_fData[1][iSrcCol];
}
//----------------------------------------------------------------------------
inline void SEMatrix2f::GetTranspose(SEMatrix2f& rDesMat) const
{
    rDesMat.M11 = M11;
    rDesMat.M12 = M21;

    rDesMat.M21 = M12;
    rDesMat.M22 = M22;
}
//----------------------------------------------------------------------------
inline void SEMatrix2f::GetInverse(SEMatrix2f& rDesMat) const
{
    float fDet = m_fData[0][0]*m_fData[1][1] - m_fData[0][1]*m_fData[1][0];

    if( SEMath<float>::FAbs(fDet) > SEMath<float>::ZERO_TOLERANCE )
    {
        float fInvDet = 1.0f / fDet;

        rDesMat[0][0] =  m_fData[1][1] * fInvDet;
        rDesMat[0][1] = -m_fData[0][1] * fInvDet;
        rDesMat[1][0] = -m_fData[1][0] * fInvDet;
        rDesMat[1][1] =  m_fData[0][0] * fInvDet;
    }
    else
    {
        rDesMat.Zero();
    }
}
//----------------------------------------------------------------------------
inline void SEMatrix2f::GetAdjoint(SEMatrix2f& rDesMat) const
{
    rDesMat[0][0] = m_fData[1][1];
    rDesMat[0][1] = -m_fData[0][1];
    rDesMat[1][0] = -m_fData[1][0];
    rDesMat[1][1] = m_fData[0][0];
}
//----------------------------------------------------------------------------
inline float SEMatrix2f::GetDeterminant() const
{
    return m_fData[0][0]*m_fData[1][1] - m_fData[0][1]*m_fData[1][0];
}
//----------------------------------------------------------------------------
inline void SEMatrix2f::ToAngle(float& rfAngle) const
{
    rfAngle = SEMath<float>::ATan2(m_fData[1][0], m_fData[0][0]);
}
//----------------------------------------------------------------------------
inline void SEMatrix2f::GetTransposeTimes(const SEMatrix2f& rRhsMat, 
    SEMatrix2f& rDesMat) const
{
    // C = A^T * B
    rDesMat.M11 = M11*rRhsMat.M11 + M21*rRhsMat.M21;
    rDesMat.M12 = M11*rRhsMat.M12 + M21*rRhsMat.M22;

    rDesMat.M21 = M12*rRhsMat.M11 + M22*rRhsMat.M21;
    rDesMat.M22 = M12*rRhsMat.M12 + M22*rRhsMat.M22;
}
//----------------------------------------------------------------------------
inline void SEMatrix2f::GetTimesTranspose(const SEMatrix2f& rRhsMat, 
    SEMatrix2f& rDesMat) const
{
    // C = A * B^T
    rDesMat.M11 = M11*rRhsMat.M11 + M12*rRhsMat.M12;
    rDesMat.M12 = M11*rRhsMat.M21 + M12*rRhsMat.M22;

    rDesMat.M21 = M21*rRhsMat.M11 + M22*rRhsMat.M12;
    rDesMat.M22 = M21*rRhsMat.M21 + M22*rRhsMat.M22;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 双精度2阶方阵类
//----------------------------------------------------------------------------
inline int SEMatrix2d::CompareData(const SEMatrix2d& rMat) const
{
    return memcmp(m_dData, rMat.m_dData, 4*sizeof(double));
}
//----------------------------------------------------------------------------
inline void SEMatrix2d::Zero()
{
    memset(m_dData, 0, 4*sizeof(double));
}
//----------------------------------------------------------------------------
inline void SEMatrix2d::Identity()
{
    m_dData[0][0] = 1.0;
    m_dData[0][1] = 0.0;
    m_dData[1][0] = 0.0;
    m_dData[1][1] = 1.0;
}
//----------------------------------------------------------------------------
inline void SEMatrix2d::Diagonal(double dM11, double dM22)
{
    m_dData[0][0] = dM11;
    m_dData[0][1] = 0.0;
    m_dData[1][0] = 0.0;
    m_dData[1][1] = dM22;
}
//----------------------------------------------------------------------------
inline void SEMatrix2d::FromAngle(double dAngle)
{
    m_dData[0][0] = SEMath<double>::Cos(dAngle);
    m_dData[1][0] = -SEMath<double>::Sin(dAngle);
    m_dData[0][1] = -m_dData[1][0];
    m_dData[1][1] =  m_dData[0][0];
}
//----------------------------------------------------------------------------
inline void SEMatrix2d::TensorProduct(const SEVector2d& rVecU, 
    const SEVector2d& rVecV)
{
    m_dData[0][0] = rVecU[0] * rVecV[0];
    m_dData[0][1] = rVecU[0] * rVecV[1];
    m_dData[1][0] = rVecU[1] * rVecV[0];
    m_dData[1][1] = rVecU[1] * rVecV[1];
}
//----------------------------------------------------------------------------
inline void SEMatrix2d::SetRow(int iDesRow, const SEVector2d& rSrcVec)
{
    SE_ASSERT( 0 <= iDesRow && iDesRow < 2 );

	m_dData[iDesRow][0] = rSrcVec.m_dData[0];
    m_dData[iDesRow][1] = rSrcVec.m_dData[1];
}
//----------------------------------------------------------------------------
inline void SEMatrix2d::GetRow(int iSrcRow, SEVector2d& rDesVec) const
{
    SE_ASSERT( 0 <= iSrcRow && iSrcRow < 2 );

    rDesVec.m_dData[0] = m_dData[iSrcRow][0];
    rDesVec.m_dData[1] = m_dData[iSrcRow][1];
}
//----------------------------------------------------------------------------
inline void SEMatrix2d::SetColumn(int iDesCol, const SEVector2d& rSrcVec)
{
    SE_ASSERT( 0 <= iDesCol && iDesCol < 2 );

	m_dData[0][iDesCol] = rSrcVec.m_dData[0];
	m_dData[1][iDesCol] = rSrcVec.m_dData[1];
}
//----------------------------------------------------------------------------
inline void SEMatrix2d::GetColumn(int iSrcCol, SEVector2d& rDesVec) const
{
    SE_ASSERT( 0 <= iSrcCol && iSrcCol < 2 );

	rDesVec.m_dData[0] = m_dData[0][iSrcCol];
    rDesVec.m_dData[1] = m_dData[1][iSrcCol];
}
//----------------------------------------------------------------------------
inline void SEMatrix2d::GetTranspose(SEMatrix2d& rDesMat) const
{
    rDesMat.M11 = M11;
    rDesMat.M12 = M21;

    rDesMat.M21 = M12;
    rDesMat.M22 = M22;
}
//----------------------------------------------------------------------------
inline void SEMatrix2d::GetInverse(SEMatrix2d& rDesMat) const
{
    double dDet = m_dData[0][0]*m_dData[1][1] - m_dData[0][1]*m_dData[1][0];

    if( SEMath<double>::FAbs(dDet) > SEMath<double>::ZERO_TOLERANCE )
    {
        double dInvDet = 1.0 / dDet;

        rDesMat[0][0] =  m_dData[1][1] * dInvDet;
        rDesMat[0][1] = -m_dData[0][1] * dInvDet;
        rDesMat[1][0] = -m_dData[1][0] * dInvDet;
        rDesMat[1][1] =  m_dData[0][0] * dInvDet;
    }
    else
    {
        rDesMat.Zero();
    }
}
//----------------------------------------------------------------------------
inline void SEMatrix2d::GetAdjoint(SEMatrix2d& rDesMat) const
{
    rDesMat[0][0] = m_dData[1][1];
    rDesMat[0][1] = -m_dData[0][1];
    rDesMat[1][0] = -m_dData[1][0];
    rDesMat[1][1] = m_dData[0][0];
}
//----------------------------------------------------------------------------
inline double SEMatrix2d::GetDeterminant() const
{
    return m_dData[0][0]*m_dData[1][1] - m_dData[0][1]*m_dData[1][0];
}
//----------------------------------------------------------------------------
inline void SEMatrix2d::ToAngle(double& rdAngle) const
{
    rdAngle = SEMath<double>::ATan2(m_dData[1][0], m_dData[0][0]);
}
//----------------------------------------------------------------------------
inline void SEMatrix2d::GetTransposeTimes(const SEMatrix2d& rRhsMat, 
    SEMatrix2d& rDesMat) const
{
    // C = A^T * B
    rDesMat.M11 = M11*rRhsMat.M11 + M21*rRhsMat.M21;
    rDesMat.M12 = M11*rRhsMat.M12 + M21*rRhsMat.M22;

    rDesMat.M21 = M12*rRhsMat.M11 + M22*rRhsMat.M21;
    rDesMat.M22 = M12*rRhsMat.M12 + M22*rRhsMat.M22;
}
//----------------------------------------------------------------------------
inline void SEMatrix2d::GetTimesTranspose(const SEMatrix2d& rRhsMat, 
    SEMatrix2d& rDesMat) const
{
    // C = A * B^T
    rDesMat.M11 = M11*rRhsMat.M11 + M12*rRhsMat.M12;
    rDesMat.M12 = M11*rRhsMat.M21 + M12*rRhsMat.M22;

    rDesMat.M21 = M21*rRhsMat.M11 + M22*rRhsMat.M12;
    rDesMat.M22 = M21*rRhsMat.M21 + M22*rRhsMat.M22;
}
//----------------------------------------------------------------------------