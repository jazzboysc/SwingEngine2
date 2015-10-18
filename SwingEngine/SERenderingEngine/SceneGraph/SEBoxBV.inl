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
inline int SEBoxBV::GetBVType() const
{
    return SEBoundingVolume::BV_BOX;
}
//----------------------------------------------------------------------------
inline void SEBoxBV::SetCenter(const SEVector3f& rCenter)
{
    m_Box.Center = rCenter;
}
//----------------------------------------------------------------------------
inline SEVector3f SEBoxBV::GetCenter() const
{
    return m_Box.Center;
}
//----------------------------------------------------------------------------
inline SEBox3f& SEBoxBV::Box()
{
    return m_Box;
}
//----------------------------------------------------------------------------
inline const SEBox3f& SEBoxBV::GetBox() const
{
    return m_Box;
}
//----------------------------------------------------------------------------
