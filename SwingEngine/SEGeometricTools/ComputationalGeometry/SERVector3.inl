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
template <int ISIZE>
SERVector3<ISIZE>::SERVector3()
{
    // the vector is uninitialized
}
//----------------------------------------------------------------------------
template <int ISIZE>
SERVector3<ISIZE>::SERVector3(const SERVector3& rV)
{
    m_aTuple[0] = rV.m_aTuple[0];
    m_aTuple[1] = rV.m_aTuple[1];
    m_aTuple[2] = rV.m_aTuple[2];
}
//----------------------------------------------------------------------------
#ifndef SE_USING_VC70
template <int ISIZE>
SERVector3<ISIZE>::SERVector3(const SETRVector<3,  ISIZE>& rV)
{
    m_aTuple[0] = rV[0];
    m_aTuple[1] = rV[1];
    m_aTuple[2] = rV[2];
}
#endif
//----------------------------------------------------------------------------
template <int ISIZE>
SERVector3<ISIZE>::SERVector3(const SETRational<ISIZE>& rX, 
    const SETRational<ISIZE>& rY,  const SETRational<ISIZE>& rZ)
{
    m_aTuple[0] = rX;
    m_aTuple[1] = rY;
    m_aTuple[2] = rZ;
}
//----------------------------------------------------------------------------
template <int ISIZE>
SERVector3<ISIZE>& SERVector3<ISIZE>::operator=(const SERVector3& rV)
{
    m_aTuple[0] = rV.m_aTuple[0];
    m_aTuple[1] = rV.m_aTuple[1];
    m_aTuple[2] = rV.m_aTuple[2];

    return *this;
}
//----------------------------------------------------------------------------
#ifndef SE_USING_VC70
template <int ISIZE>
SERVector3<ISIZE>& SERVector3<ISIZE>::operator=(const SETRVector<3,  ISIZE>& 
    rV)
{
    m_aTuple[0] = rV[0];
    m_aTuple[1] = rV[1];
    m_aTuple[2] = rV[2];

    return *this;
}
#endif
//----------------------------------------------------------------------------
template <int ISIZE>
SETRational<ISIZE> SERVector3<ISIZE>::X() const
{
    return m_aTuple[0];
}
//----------------------------------------------------------------------------
template <int ISIZE>
SETRational<ISIZE>& SERVector3<ISIZE>::X()
{
    return m_aTuple[0];
}
//----------------------------------------------------------------------------
template <int ISIZE>
SETRational<ISIZE> SERVector3<ISIZE>::Y() const
{
    return m_aTuple[1];
}
//----------------------------------------------------------------------------
template <int ISIZE>
SETRational<ISIZE>& SERVector3<ISIZE>::Y()
{
    return m_aTuple[1];
}
//----------------------------------------------------------------------------
template <int ISIZE>
SETRational<ISIZE> SERVector3<ISIZE>::Z() const
{
    return m_aTuple[2];
}
//----------------------------------------------------------------------------
template <int ISIZE>
SETRational<ISIZE>& SERVector3<ISIZE>::Z()
{
    return m_aTuple[2];
}
//----------------------------------------------------------------------------
template <int ISIZE>
SETRational<ISIZE> SERVector3<ISIZE>::Dot(const SERVector3& rV) const
{
    return m_aTuple[0]*rV.m_aTuple[0] + m_aTuple[1]*rV.m_aTuple[1] +
        m_aTuple[2]*rV.m_aTuple[2];
}
//----------------------------------------------------------------------------
template <int ISIZE>
SERVector3<ISIZE> SERVector3<ISIZE>::Cross(const SERVector3& rV) const
{
    return SERVector3<ISIZE>(
        m_aTuple[1]*rV.m_aTuple[2] - m_aTuple[2]*rV.m_aTuple[1], 
        m_aTuple[2]*rV.m_aTuple[0] - m_aTuple[0]*rV.m_aTuple[2], 
        m_aTuple[0]*rV.m_aTuple[1] - m_aTuple[1]*rV.m_aTuple[0]);
}
//----------------------------------------------------------------------------
template <int ISIZE>
SETRational<ISIZE> SERVector3<ISIZE>::TripleScalar(const SERVector3& rU, 
    const SERVector3& rV) const
{
    return Dot(rU.Cross(rV));
}
//----------------------------------------------------------------------------