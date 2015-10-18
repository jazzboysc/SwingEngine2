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
#include "SERay3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SERay3f::SERay3f()
    :
    Origin(SEVector3f::ZERO),
    Direction(SEVector3f::ZERO),
    MinT(0.0f), MaxT(SEMathf::MAX_REAL),
    Time(0.0f),
    Depth(0)
{
}
//----------------------------------------------------------------------------
SERay3f::SERay3f(const SEVector3f& rOrigin, const SEVector3f& rDirection)
    :
    Origin(rOrigin),
    Direction(rDirection),
    MinT(0.0f), MaxT(SEMathf::MAX_REAL),
    Time(0.0f),
    Depth(0)
{
}
//----------------------------------------------------------------------------
SERay3f::SERay3f(const SEVector3f& rOrigin, const SEVector3f& rDirection, 
    float fMinT, float fMaxT, float fTime, int iDepth)
    :
    Origin(rOrigin),
    Direction(rDirection),
    MinT(fMinT), MaxT(fMaxT),
    Time(fTime),
    Depth(iDepth)
{
}
//----------------------------------------------------------------------------
SERay3f::SERay3f(const SEVector3f& rOrigin, const SEVector3f& rDirection, 
    const SERay3f& rParent, float fMinT, float fMaxT)
    :
    Origin(rOrigin),
    Direction(rDirection),
    MinT(fMinT), MaxT(fMaxT),
    Time(rParent.Time),
    Depth(rParent.Depth + 1)
{
}
//----------------------------------------------------------------------------
SEVector3f SERay3f::operator()(float fT) const
{
    return Origin + Direction*fT;
}
//----------------------------------------------------------------------------