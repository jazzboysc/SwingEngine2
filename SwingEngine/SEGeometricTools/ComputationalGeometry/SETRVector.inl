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
template <int VSIZE,  int ISIZE>
SETRVector<VSIZE,  ISIZE>::SETRVector()
{
    // For efficiency in construction of large arrays of vectors,  the
    // default constructor does not initialize the vector.
}
//----------------------------------------------------------------------------
template <int VSIZE,  int ISIZE>
SETRVector<VSIZE,  ISIZE>::SETRVector(const SETRVector& rV)
{
    for( int i = 0; i < VSIZE; i++ )
    {
        m_aTuple[i] = rV.m_aTuple[i];
    }
}
//----------------------------------------------------------------------------
template <int VSIZE,  int ISIZE>
SETRVector<VSIZE,  ISIZE>::operator const SETRational<ISIZE>*() const
{
    return m_aTuple;
}
//----------------------------------------------------------------------------
template <int VSIZE,  int ISIZE>
SETRVector<VSIZE,  ISIZE>::operator SETRational<ISIZE>*()
{
    return m_aTuple;
}
//----------------------------------------------------------------------------
template <int VSIZE,  int ISIZE>
SETRational<ISIZE> SETRVector<VSIZE,  ISIZE>::operator[](int i) const
{
    SE_ASSERT( 0 <= i && i < VSIZE );

    return m_aTuple[i];
}
//----------------------------------------------------------------------------
template <int VSIZE,  int ISIZE>
SETRational<ISIZE>& SETRVector<VSIZE,  ISIZE>::operator[](int i)
{
    SE_ASSERT( 0 <= i && i < VSIZE );

    return m_aTuple[i];
}
//----------------------------------------------------------------------------
template <int VSIZE,  int ISIZE>
SETRVector<VSIZE,  ISIZE>& SETRVector<VSIZE,  ISIZE>::operator=(const 
    SETRVector& rV)
{
    for( int i = 0; i < VSIZE; i++ )
    {
        m_aTuple[i] = rV.m_aTuple[i];
    }

    return *this;
}
//----------------------------------------------------------------------------
template <int VSIZE,  int ISIZE>
bool SETRVector<VSIZE,  ISIZE>::operator==(const SETRVector& rV) const
{
    for( int i = 0; i < VSIZE; i++ )
    {
        if( m_aTuple[i] != rV.m_aTuple[i] )
        {
            return false;
        }
    }

    return true;
}
//----------------------------------------------------------------------------
template <int VSIZE,  int ISIZE>
bool SETRVector<VSIZE,  ISIZE>::operator!=(const SETRVector& rV) const
{
    return !operator==(rV);
}
//----------------------------------------------------------------------------
template <int VSIZE,  int ISIZE>
int SETRVector<VSIZE,  ISIZE>::CompareArrays(const SETRVector& rV) const
{
    for( int i = 0; i < VSIZE; i++ )
    {
        if( m_aTuple[i] < rV.m_aTuple[i] )
        {
            return -1;
        }
        if( m_aTuple[i] > rV.m_aTuple[i] )
        {
            return +1;
        }
    }

    return 0;
}
//----------------------------------------------------------------------------
template <int VSIZE,  int ISIZE>
bool SETRVector<VSIZE,  ISIZE>::operator<(const SETRVector& rV) const
{
    return CompareArrays(rV) < 0;
}
//----------------------------------------------------------------------------
template <int VSIZE,  int ISIZE>
bool SETRVector<VSIZE,  ISIZE>::operator<=(const SETRVector& rV) const
{
    return CompareArrays(rV) <= 0;
}
//----------------------------------------------------------------------------
template <int VSIZE,  int ISIZE>
bool SETRVector<VSIZE,  ISIZE>::operator>(const SETRVector& rV) const
{
    return CompareArrays(rV) > 0;
}
//----------------------------------------------------------------------------
template <int VSIZE,  int ISIZE>
bool SETRVector<VSIZE,  ISIZE>::operator>=(const SETRVector& rV) const
{
    return CompareArrays(rV) >= 0;
}
//----------------------------------------------------------------------------
template <int VSIZE,  int ISIZE>
SETRVector<VSIZE,  ISIZE> SETRVector<VSIZE,  ISIZE>::operator+(const 
    SETRVector& rV) const
{
    SETRVector<VSIZE,  ISIZE> tempSum;
    for( int i = 0; i < VSIZE; i++ )
    {
        tempSum.m_aTuple[i] = m_aTuple[i] + rV.m_aTuple[i];
    }

    return tempSum;
}
//----------------------------------------------------------------------------
template <int VSIZE,  int ISIZE>
SETRVector<VSIZE,  ISIZE> SETRVector<VSIZE,  ISIZE>::operator-(const 
    SETRVector& rV) const
{
    SETRVector<VSIZE,  ISIZE> tempDiff;
    for( int i = 0; i < VSIZE; i++ )
    {
        tempDiff.m_aTuple[i] = m_aTuple[i] - rV.m_aTuple[i];
    }

    return tempDiff;
}
//----------------------------------------------------------------------------
template <int VSIZE,  int ISIZE>
SETRVector<VSIZE,  ISIZE> SETRVector<VSIZE,  ISIZE>::operator*
    (const SETRational<ISIZE>& rR) const
{
    SETRVector<VSIZE,  ISIZE> tempProd;
    for( int i = 0; i < VSIZE; i++ )
    {
        tempProd.m_aTuple[i] = rR * m_aTuple[i];
    }

    return tempProd;
}
//----------------------------------------------------------------------------
template <int VSIZE,  int ISIZE>
SETRVector<VSIZE,  ISIZE> SETRVector<VSIZE,  ISIZE>::operator/
    (const SETRational<ISIZE>& rR) const
{
    SE_ASSERT( rR != 0 );

    SETRVector<VSIZE,  ISIZE> tempProd;
    for( int i = 0; i < VSIZE; i++ )
    {
        tempProd.m_aTuple[i] = m_aTuple[i] / rR;
    }

    return tempProd;
}
//----------------------------------------------------------------------------
template <int VSIZE,  int ISIZE>
SETRVector<VSIZE,  ISIZE> SETRVector<VSIZE,  ISIZE>::operator-() const
{
    SETRVector<VSIZE,  ISIZE> tempNeg;
    for( int i = 0; i < VSIZE; i++ )
    {
        tempNeg.m_aTuple[i] = -m_aTuple[i];
    }

    return tempNeg;
}
//----------------------------------------------------------------------------
template <int VSIZE,  int ISIZE>
SETRVector<VSIZE,  ISIZE> operator*(const SETRational<ISIZE>& rR, 
    const SETRVector<VSIZE,  ISIZE>& rV)
{
    SETRVector<VSIZE,  ISIZE> tempProd;
    for( int i = 0; i < VSIZE; i++ )
    {
        tempProd[i] = rR * rV[i];
    }

    return tempProd;
}
//----------------------------------------------------------------------------
template <int VSIZE,  int ISIZE>
SETRVector<VSIZE,  ISIZE>& SETRVector<VSIZE,  ISIZE>::operator+=(const 
    SETRVector& rV)
{
    for( int i = 0; i < VSIZE; i++ )
    {
        m_aTuple[i] += rV.m_aTuple[i];
    }

    return *this;
}
//----------------------------------------------------------------------------
template <int VSIZE,  int ISIZE>
SETRVector<VSIZE,  ISIZE>& SETRVector<VSIZE,  ISIZE>::operator-=(const 
    SETRVector& rV)
{
    for( int i = 0; i < VSIZE; i++ )
    {
        m_aTuple[i] -= rV.m_aTuple[i];
    }

    return *this;
}
//----------------------------------------------------------------------------
template <int VSIZE,  int ISIZE>
SETRVector<VSIZE,  ISIZE>& SETRVector<VSIZE,  ISIZE>::operator*=
    (const SETRational<ISIZE>& rR)
{
    for( int i = 0; i < VSIZE; i++ )
    {
        m_aTuple[i] *= rR;
    }

    return *this;
}
//----------------------------------------------------------------------------
template <int VSIZE,  int ISIZE>
SETRVector<VSIZE,  ISIZE>& SETRVector<VSIZE,  ISIZE>::operator/=
    (const SETRational<ISIZE>& rR)
{
    SE_ASSERT( rR != 0 );

    for( int i = 0; i < VSIZE; i++ )
    {
        m_aTuple[i] /= rR;
    }

    return *this;
}
//----------------------------------------------------------------------------
template <int VSIZE,  int ISIZE>
SETRational<ISIZE> SETRVector<VSIZE,  ISIZE>::GetSquaredLength() const
{
    SETRational<ISIZE> tempSqrLen = 0;
    for( int i = 0; i < VSIZE; i++ )
    {
        tempSqrLen += m_aTuple[i] * m_aTuple[i];
    }

    return tempSqrLen;
}
//----------------------------------------------------------------------------
template <int VSIZE,  int ISIZE>
SETRational<ISIZE> SETRVector<VSIZE,  ISIZE>::Dot(const SETRVector& rV) const
{
    SETRational<ISIZE> tempDot = 0;
    for( int i = 0; i < VSIZE; i++ )
    {
        tempDot += m_aTuple[i] * rV.m_aTuple[i];
    }

    return tempDot;
}
//----------------------------------------------------------------------------
