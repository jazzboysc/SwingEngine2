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
inline int SESphereBV::GetBVType() const
{
    return SEBoundingVolume::BV_SPHERE;
}
//----------------------------------------------------------------------------
inline void SESphereBV::SetCenter(const SEVector3f& rCenter)
{
    m_Sphere.Center = rCenter;
}
//----------------------------------------------------------------------------
inline void SESphereBV::SetRadius(float fRadius)
{
    m_Sphere.Radius = fRadius;
}
//----------------------------------------------------------------------------
inline SEVector3f SESphereBV::GetCenter() const
{
    return m_Sphere.Center;
}
//----------------------------------------------------------------------------
inline float SESphereBV::GetRadius() const
{
    return m_Sphere.Radius;
}
//----------------------------------------------------------------------------
inline SESphere3f& SESphereBV::Sphere()
{
    return m_Sphere;
}
//----------------------------------------------------------------------------
inline const SESphere3f& SESphereBV::GetSphere() const
{
    return m_Sphere;
}
//----------------------------------------------------------------------------
