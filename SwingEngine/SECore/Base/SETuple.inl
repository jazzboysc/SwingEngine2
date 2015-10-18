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
template <int DIMENSION, typename TYPE>
SETuple<DIMENSION, TYPE>::SETuple()
{
    // Uninitialized for native data. Initialized for class data as long as
    // TYPE's default constructor initializes its own data.
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
SETuple<DIMENSION, TYPE>::SETuple(const SETuple& rTuple)
{
    for( int i = 0; i < DIMENSION; ++i )
    {
        m_Tuple[i] = rTuple.m_Tuple[i];
    }
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
SETuple<DIMENSION, TYPE>::~SETuple()
{
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
inline SETuple<DIMENSION, TYPE>::operator const TYPE*() const
{
    return m_Tuple;
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
inline SETuple<DIMENSION, TYPE>::operator TYPE*()
{
    return m_Tuple;
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
inline TYPE SETuple<DIMENSION, TYPE>::operator[](int i) const
{
    return m_Tuple[i];
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
inline TYPE& SETuple<DIMENSION, TYPE>::operator[](int i)
{
    return m_Tuple[i];
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
SETuple<DIMENSION, TYPE>& SETuple<DIMENSION, TYPE>::operator=(const SETuple& 
    rTuple)
{
    for( int i = 0; i < DIMENSION; ++i )
    {
        m_Tuple[i] = rTuple.m_Tuple[i];
    }
    return *this;
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
bool SETuple<DIMENSION, TYPE>::operator==(const SETuple& rTuple) const
{
    return memcmp(m_Tuple, rTuple.m_Tuple, DIMENSION*sizeof(TYPE)) == 0;
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
bool SETuple<DIMENSION, TYPE>::operator!=(const SETuple& rTuple) const
{
    return memcmp(m_Tuple, rTuple.m_Tuple, DIMENSION*sizeof(TYPE)) != 0;
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
bool SETuple<DIMENSION, TYPE>::operator<(const SETuple& rTuple) const
{
    return memcmp(m_Tuple, rTuple.m_Tuple, DIMENSION*sizeof(TYPE)) < 0;
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
bool SETuple<DIMENSION, TYPE>::operator<=(const SETuple& rTuple) const
{
    return memcmp(m_Tuple, rTuple.m_Tuple, DIMENSION*sizeof(TYPE)) <= 0;
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
bool SETuple<DIMENSION, TYPE>::operator>(const SETuple& rTuple) const
{
    return memcmp(m_Tuple, rTuple.m_Tuple, DIMENSION*sizeof(TYPE)) > 0;
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
bool SETuple<DIMENSION, TYPE>::operator>=(const SETuple& rTuple) const
{
    return memcmp(m_Tuple, rTuple.m_Tuple, DIMENSION*sizeof(TYPE)) >= 0;
}
//----------------------------------------------------------------------------
