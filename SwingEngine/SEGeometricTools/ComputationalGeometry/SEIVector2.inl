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
inline SEIVector2::SEIVector2()
{
    // the vector is uninitialized
}
//----------------------------------------------------------------------------
inline SEIVector2::SEIVector2(const SEIVector2& rV)
{
    m_aiTuple[0] = rV.m_aiTuple[0];
    m_aiTuple[1] = rV.m_aiTuple[1];
}
//----------------------------------------------------------------------------
inline SEIVector2::SEIVector2(const SETIVector<2>& rV)
{
    m_aiTuple[0] = rV[0];
    m_aiTuple[1] = rV[1];
}
//----------------------------------------------------------------------------
inline SEIVector2::SEIVector2(const SE_Int64& riX,  const SE_Int64& riY)
{
    m_aiTuple[0] = riX;
    m_aiTuple[1] = riY;
}
//----------------------------------------------------------------------------
inline SEIVector2& SEIVector2::operator=(const SEIVector2& rV)
{
    m_aiTuple[0] = rV.m_aiTuple[0];
    m_aiTuple[1] = rV.m_aiTuple[1];

    return *this;
}
//----------------------------------------------------------------------------
inline SEIVector2& SEIVector2::operator=(const SETIVector<2>& rV)
{
    m_aiTuple[0] = rV[0];
    m_aiTuple[1] = rV[1];

    return *this;
}
//----------------------------------------------------------------------------
inline SE_Int64 SEIVector2::X() const
{
    return m_aiTuple[0];
}
//----------------------------------------------------------------------------
inline SE_Int64& SEIVector2::X()
{
    return m_aiTuple[0];
}
//----------------------------------------------------------------------------
inline SE_Int64 SEIVector2::Y() const
{
    return m_aiTuple[1];
}
//----------------------------------------------------------------------------
inline SE_Int64& SEIVector2::Y()
{
    return m_aiTuple[1];
}
//----------------------------------------------------------------------------
inline SE_Int64 SEIVector2::Dot(const SEIVector2& rV) const
{
    return m_aiTuple[0]*rV.m_aiTuple[0] + m_aiTuple[1]*rV.m_aiTuple[1];
}
//----------------------------------------------------------------------------
inline SEIVector2 SEIVector2::Perp() const
{
    return SEIVector2(m_aiTuple[1],  -m_aiTuple[0]);
}
//----------------------------------------------------------------------------
inline SE_Int64 SEIVector2::DotPerp(const SEIVector2& rV) const
{
    return m_aiTuple[0]*rV.m_aiTuple[1] - m_aiTuple[1]*rV.m_aiTuple[0];
}
//----------------------------------------------------------------------------