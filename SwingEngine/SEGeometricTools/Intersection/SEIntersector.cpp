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

#include "SEGeometricToolsPCH.h"
#include "SEIntersector.h"

namespace Swing
{
//----------------------------------------------------------------------------
template <class Real, class TVector>
SEIntersector<Real, TVector>::SEIntersector()
{
    m_fContactTime = (Real)0.0;
    m_iIntersectionType = IT_EMPTY;
}
//----------------------------------------------------------------------------
template <class Real, class TVector>
SEIntersector<Real, TVector>::~SEIntersector()
{
}
//----------------------------------------------------------------------------
template <class Real, class TVector>
Real SEIntersector<Real, TVector>::GetContactTime() const
{
    return m_fContactTime;
}
//----------------------------------------------------------------------------
template <class Real, class TVector>
int SEIntersector<Real, TVector>::GetIntersectionType() const
{
    return m_iIntersectionType;
}
//----------------------------------------------------------------------------
template <class Real, class TVector>
bool SEIntersector<Real, TVector>::Test()
{
    // 由派生类负责实现.
    SE_ASSERT( false );

    return false;
}
//----------------------------------------------------------------------------
template <class Real, class TVector>
bool SEIntersector<Real, TVector>::Find()
{
    // 由派生类负责实现.
    SE_ASSERT( false );

    return false;
}
//----------------------------------------------------------------------------
template <class Real, class TVector>
bool SEIntersector<Real, TVector>::Test(Real, const TVector&, const TVector&)
{
    // 由派生类负责实现.
    SE_ASSERT( false );

    return false;
}
//----------------------------------------------------------------------------
template <class Real, class TVector>
bool SEIntersector<Real, TVector>::Find(Real, const TVector&, const TVector&)
{
    // 由派生类负责实现.
    SE_ASSERT( false );

    return false;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// explicit instantiation
//----------------------------------------------------------------------------
template SE_GEOMETRICTOOLS_API
class SEIntersector<float, SEVector2f>;

template SE_GEOMETRICTOOLS_API
class SEIntersector<float, SEVector3f>;

//template SE_GEOMETRICTOOLS_API
//class SEIntersector<double, SEVector2d>;
//
//template SE_GEOMETRICTOOLS_API
//class SEIntersector<double, SEVector3d>;
//----------------------------------------------------------------------------
}
