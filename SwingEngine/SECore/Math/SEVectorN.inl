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
inline void SEVectorNf::SetSize(int iSize)
{
    delete[] m_pData;
    if( iSize > 0 )
    {
        m_iSize = iSize;
        m_pData = new float[m_iSize];
        memset(m_pData, 0, m_iSize*sizeof(float));
    }
    else
    {
        m_iSize = 0;
        m_pData = 0;
    }
}
//----------------------------------------------------------------------------
inline int SEVectorNf::GetSize() const
{
    return m_iSize;
}
//----------------------------------------------------------------------------
inline int SEVectorNf::CompareData(const SEVectorNf& rVec) const
{
    return memcmp(m_pData, rVec.m_pData, m_iSize*sizeof(float));
}
//----------------------------------------------------------------------------
inline float SEVectorNf::GetLength(void) const
{
    float fSqrLen = 0.0f;
    for( int i = 0; i < m_iSize; i++ )
    {
        fSqrLen += m_pData[i] * m_pData[i];
    }

    return SEMath<float>::Sqrt(fSqrLen);
}
//----------------------------------------------------------------------------
inline float SEVectorNf::GetSquaredLength() const
{
    float fSqrLen = 0.0f;
    for( int i = 0; i < m_iSize; i++ )
    {
        fSqrLen += m_pData[i] * m_pData[i];
    }

    return fSqrLen;
}
//----------------------------------------------------------------------------
inline float SEVectorNf::Dot(const SEVectorNf& rRhsVec) const
{
    float fDot = 0.0f;
    for( int i = 0; i < m_iSize; i++ )
    {
        fDot += m_pData[i] * rRhsVec.m_pData[i];
    }

    return fDot;
}
//----------------------------------------------------------------------------
inline float SEVectorNf::Normalize()
{
    float fLength = GetLength();
    int i;

    if( fLength > SEMath<float>::ZERO_TOLERANCE )
    {
        float fInvLength = 1.0f / fLength;
        for( i = 0; i < m_iSize; i++ )
        {
            m_pData[i] *= fInvLength;
        }
    }
    else
    {
        fLength = 0.0f;
        for( i = 0; i < m_iSize; i++ )
        {
            m_pData[i] = 0.0f;
        }
    }

    return fLength;
}
//----------------------------------------------------------------------------
