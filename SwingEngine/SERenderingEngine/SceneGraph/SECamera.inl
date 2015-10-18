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
inline SEVector3f SECamera::GetLocation() const
{
    return m_Location;
}
//----------------------------------------------------------------------------
inline SEVector3f SECamera::GetRVector() const
{
    return m_RVector;
}
//----------------------------------------------------------------------------
inline SEVector3f SECamera::GetUVector() const
{
    return m_UVector;
}
//----------------------------------------------------------------------------
inline SEVector3f SECamera::GetDVector() const
{
    return m_DVector;
}
//----------------------------------------------------------------------------
inline const float* SECamera::GetFrustum() const
{
    return m_Frustum;
}
//----------------------------------------------------------------------------
inline float SECamera::GetDMin() const
{
    return m_Frustum[VF_DMIN];
}
//----------------------------------------------------------------------------
inline float SECamera::GetDMax() const
{
    return m_Frustum[VF_DMAX];
}
//----------------------------------------------------------------------------
inline float SECamera::GetUMin() const
{
    return m_Frustum[VF_UMIN];
}
//----------------------------------------------------------------------------
inline float SECamera::GetUMax() const
{
    return m_Frustum[VF_UMAX];
}
//----------------------------------------------------------------------------
inline float SECamera::GetRMin() const
{
    return m_Frustum[VF_RMIN];
}
//----------------------------------------------------------------------------
inline float SECamera::GetRMax() const
{
    return m_Frustum[VF_RMAX];
}
//----------------------------------------------------------------------------
inline bool SECamera::IsPerspective() const
{
    return m_bPerspective;
}
//----------------------------------------------------------------------------