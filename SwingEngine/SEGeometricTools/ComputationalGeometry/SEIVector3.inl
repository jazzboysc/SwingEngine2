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
inline SEIVector3::SEIVector3()
{
    // the vector is uninitialized
}
//----------------------------------------------------------------------------
inline SEIVector3::SEIVector3(const SEIVector3& rV)
{
    m_aiTuple[0] = rV.m_aiTuple[0];
    m_aiTuple[1] = rV.m_aiTuple[1];
    m_aiTuple[2] = rV.m_aiTuple[2];
}
//----------------------------------------------------------------------------
inline SEIVector3::SEIVector3(const SETIVector<3>& rV)
{
    m_aiTuple[0] = rV[0];
    m_aiTuple[1] = rV[1];
    m_aiTuple[2] = rV[2];
}
//----------------------------------------------------------------------------
inline SEIVector3::SEIVector3(const SE_Int64& riX,  const SE_Int64& riY, 
    const SE_Int64& riZ)
{
    m_aiTuple[0] = riX;
    m_aiTuple[1] = riY;
    m_aiTuple[2] = riZ;
}
//----------------------------------------------------------------------------
inline SEIVector3& SEIVector3::operator=(const SEIVector3& rV)
{
    m_aiTuple[0] = rV.m_aiTuple[0];
    m_aiTuple[1] = rV.m_aiTuple[1];
    m_aiTuple[2] = rV.m_aiTuple[2];

    return *this;
}
//----------------------------------------------------------------------------
inline SEIVector3& SEIVector3::operator=(const SETIVector<3>& rV)
{
    m_aiTuple[0] = rV[0];
    m_aiTuple[1] = rV[1];
    m_aiTuple[2] = rV[2];

    return *this;
}
//----------------------------------------------------------------------------
inline SE_Int64 SEIVector3::X() const
{
    return m_aiTuple[0];
}
//----------------------------------------------------------------------------
inline SE_Int64& SEIVector3::X()
{
    return m_aiTuple[0];
}
//----------------------------------------------------------------------------
inline SE_Int64 SEIVector3::Y() const
{
    return m_aiTuple[1];
}
//----------------------------------------------------------------------------
inline SE_Int64& SEIVector3::Y()
{
    return m_aiTuple[1];
}
//----------------------------------------------------------------------------
inline SE_Int64 SEIVector3::Z() const
{
    return m_aiTuple[2];
}
//----------------------------------------------------------------------------
inline SE_Int64& SEIVector3::Z()
{
    return m_aiTuple[2];
}
//----------------------------------------------------------------------------
inline SE_Int64 SEIVector3::Dot(const SEIVector3& rV) const
{
    return m_aiTuple[0]*rV.m_aiTuple[0] + m_aiTuple[1]*rV.m_aiTuple[1] +
        m_aiTuple[2]*rV.m_aiTuple[2];
}
//----------------------------------------------------------------------------
inline SEIVector3 SEIVector3::Cross(const SEIVector3& rV) const
{
    return SEIVector3(
        m_aiTuple[1]*rV.m_aiTuple[2] - m_aiTuple[2]*rV.m_aiTuple[1], 
        m_aiTuple[2]*rV.m_aiTuple[0] - m_aiTuple[0]*rV.m_aiTuple[2], 
        m_aiTuple[0]*rV.m_aiTuple[1] - m_aiTuple[1]*rV.m_aiTuple[0]);
}
//----------------------------------------------------------------------------
inline SE_Int64 SEIVector3::TripleScalar(const SEIVector3& rU, 
    const SEIVector3& rV) const
{
    return Dot(rU.Cross(rV));
}
//----------------------------------------------------------------------------