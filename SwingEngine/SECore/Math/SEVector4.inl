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
// 单精度4维向量类
//----------------------------------------------------------------------------
inline int SEVector4f::CompareData(const SEVector4f& rVec) const
{
    return memcmp(m_fData, rVec.m_fData, 4*sizeof(float));
}
//----------------------------------------------------------------------------
inline float SEVector4f::GetLength() const
{
    return SEMath<float>::Sqrt(
                               m_fData[0] * m_fData[0] +
                               m_fData[1] * m_fData[1] +
                               m_fData[2] * m_fData[2] +
                               m_fData[3] * m_fData[3]);
}
//----------------------------------------------------------------------------
inline float SEVector4f::GetSquaredLength() const
{
    return
            m_fData[0] * m_fData[0] +
            m_fData[1] * m_fData[1] +
            m_fData[2] * m_fData[2] +
            m_fData[3] * m_fData[3];
}
//----------------------------------------------------------------------------
inline float SEVector4f::Dot(const SEVector4f& rRhsVec) const
{
    return
            m_fData[0] * rRhsVec.m_fData[0] +
            m_fData[1] * rRhsVec.m_fData[1] +
            m_fData[2] * rRhsVec.m_fData[2] +
            m_fData[3] * rRhsVec.m_fData[3];
}
//----------------------------------------------------------------------------
inline float SEVector4f::Normalize()
{
    float fLength = GetLength();

    if( fLength > SEMath<float>::ZERO_TOLERANCE )
    {
        float fInvLength = 1.0f / fLength;
        m_fData[0] *= fInvLength;
        m_fData[1] *= fInvLength;
        m_fData[2] *= fInvLength;
        m_fData[3] *= fInvLength;
    }
    else
    {
        m_fData[0] = 0.0f;
        m_fData[1] = 0.0f;
        m_fData[2] = 0.0f;
        m_fData[3] = 0.0f;
    }

    return fLength;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 双精度4维向量类
//----------------------------------------------------------------------------
inline int SEVector4d::CompareData(const SEVector4d& rVec) const
{
    return memcmp(m_dData, rVec.m_dData, 4*sizeof(double));
}
//----------------------------------------------------------------------------
inline double SEVector4d::GetLength() const
{
    return SEMath<double>::Sqrt(
                               m_dData[0] * m_dData[0] +
                               m_dData[1] * m_dData[1] +
                               m_dData[2] * m_dData[2] +
                               m_dData[3] * m_dData[3]);
}
//----------------------------------------------------------------------------
inline double SEVector4d::GetSquaredLength() const
{
    return
            m_dData[0] * m_dData[0] +
            m_dData[1] * m_dData[1] +
            m_dData[2] * m_dData[2] +
            m_dData[3] * m_dData[3];
}
//----------------------------------------------------------------------------
inline double SEVector4d::Dot(const SEVector4d& rRhsVec) const
{
    return
            m_dData[0] * rRhsVec.m_dData[0] +
            m_dData[1] * rRhsVec.m_dData[1] +
            m_dData[2] * rRhsVec.m_dData[2] +
            m_dData[3] * rRhsVec.m_dData[3];
}
//----------------------------------------------------------------------------
inline double SEVector4d::Normalize()
{
    double dLength = GetLength();

    if( dLength > SEMath<double>::ZERO_TOLERANCE )
    {
        double fInvLength = 1.0 / dLength;
        m_dData[0] *= fInvLength;
        m_dData[1] *= fInvLength;
        m_dData[2] *= fInvLength;
        m_dData[3] *= fInvLength;
    }
    else
    {
        m_dData[0] = 0.0;
        m_dData[1] = 0.0;
        m_dData[2] = 0.0;
        m_dData[3] = 0.0;
    }

    return dLength;
}
//----------------------------------------------------------------------------