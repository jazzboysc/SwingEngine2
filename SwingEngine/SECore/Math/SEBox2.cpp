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
#include "SEBox2.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEBox2f::SEBox2f()
{
}
//----------------------------------------------------------------------------
SEBox2f::SEBox2f(const SEVector2f& rCenter, const SEVector2f* aAxis,
    const float* afExtent)
    :
    Center(rCenter)
{
    for( int i = 0; i < 2; i++ )
    {
        Axis[i] = aAxis[i];
        Extent[i] = afExtent[i];
    }
}
//----------------------------------------------------------------------------
SEBox2f::SEBox2f(const SEVector2f& rCenter, const SEVector2f& rAxis0,
    const SEVector2f& rAxis1, float fExtent0, float fExtent1)
    :
    Center(rCenter)
{
    Axis[0] = rAxis0;
    Axis[1] = rAxis1;
    Extent[0] = fExtent0;
    Extent[1] = fExtent1;
}
//----------------------------------------------------------------------------
void SEBox2f::ComputeVertices(SEVector2f aVertex[4]) const
{
    SEVector2f aEAxis[2] =
    {
        Axis[0] * Extent[0],
        Axis[1] * Extent[1]
    };

    aVertex[0] = Center - aEAxis[0] - aEAxis[1];
    aVertex[1] = Center + aEAxis[0] - aEAxis[1];
    aVertex[2] = Center + aEAxis[0] + aEAxis[1];
    aVertex[3] = Center - aEAxis[0] + aEAxis[1];
}
//----------------------------------------------------------------------------
