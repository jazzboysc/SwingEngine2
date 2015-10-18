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
inline void SECuller::SetCamera(const SECamera* pCamera)
{
    m_pCamera = pCamera;
}
//----------------------------------------------------------------------------
inline const SECamera* SECuller::GetCamera() const
{
    return m_pCamera;
}
//----------------------------------------------------------------------------
inline const float* SECuller::GetFrustum() const
{
    return m_Frustum;
}
//----------------------------------------------------------------------------
inline SEUnculledSet& SECuller::GetVisibleSet()
{
    return m_VisibleSet;
}
//----------------------------------------------------------------------------
inline SEUnculledSet& SECuller::GetAudibleSet()
{
    return m_AudibleSet;
}
//----------------------------------------------------------------------------
inline void SECuller::Insert(SESpatial* pObject, bool bIsAudible)
{
    if( !bIsAudible )
    {
        m_VisibleSet.Insert(pObject);
    }
    else
    {
        m_AudibleSet.Insert(pObject);
    }
}
//----------------------------------------------------------------------------
inline int SECuller::GetPlaneCount() const
{
    return m_iPlaneCount;
}
//----------------------------------------------------------------------------
inline const SEPlane3f* SECuller::GetPlanes() const
{
    return m_aPlane;
}
//----------------------------------------------------------------------------
inline void SECuller::SetPlaneState(unsigned int uiPlaneState)
{
    m_uiPlaneState = uiPlaneState;
}
//----------------------------------------------------------------------------
inline unsigned int SECuller::GetPlaneState() const
{
    return m_uiPlaneState;
}
//----------------------------------------------------------------------------
inline void SECuller::PushPlane(const SEPlane3f& rPlane)
{
    if( m_iPlaneCount < US_MAX_PLANE_COUNT )
    {
        m_aPlane[m_iPlaneCount++] = rPlane;
    }
}
//----------------------------------------------------------------------------
inline void SECuller::PopPlane()
{
    if( m_iPlaneCount > SECamera::VF_COUNT )
    {
        // 截投体平面不能被弹出
        m_iPlaneCount--;
    }
}
//----------------------------------------------------------------------------
