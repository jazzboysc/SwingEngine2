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

#include "SECorePCH.h"
#include "SEPlane3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEPlane3f::SEPlane3f()
{
}
//----------------------------------------------------------------------------
SEPlane3f::SEPlane3f(const SEPlane3f& rPlane)
    : Normal(rPlane.Normal)
{
    Constant = rPlane.Constant;
}
//----------------------------------------------------------------------------
SEPlane3f::SEPlane3f(const SEVector3f& rNormal, float fConstant)
    : Normal(rNormal)
{
    Constant = fConstant;
}
//----------------------------------------------------------------------------
SEPlane3f::SEPlane3f(const SEVector3f& rNormal, const SEVector3f& rP0)
    : Normal(rNormal)
{
    Constant = rNormal.Dot(rP0);
}
//----------------------------------------------------------------------------
SEPlane3f::SEPlane3f(const SEVector3f& rP0, const SEVector3f& rP1, const 
    SEVector3f& rP2)
{
    SEVector3f Vec1 = rP1 - rP0;
    SEVector3f Vec2 = rP2 - rP0;
    Normal = Vec1.Cross(Vec2);
    Normal.Normalize();
    Constant = Normal.Dot(rP0);
}
//----------------------------------------------------------------------------
SEPlane3f& SEPlane3f::operator=(const SEPlane3f& rPlane)
{
    Normal = rPlane.Normal;
    Constant = rPlane.Constant;
    
    return *this;
}
//----------------------------------------------------------------------------