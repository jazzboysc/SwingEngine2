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
#include "SEAxisAlignedBox2.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEAxisAlignedBox2f::SEAxisAlignedBox2f()
{
}
//----------------------------------------------------------------------------
SEAxisAlignedBox2f::SEAxisAlignedBox2f(float fXMin, float fXMax, float fYMin,
    float fYMax)
{
    Min[0] = fXMin;
    Max[0] = fXMax;
    Min[1] = fYMin;
    Max[1] = fYMax;
}
//----------------------------------------------------------------------------
bool SEAxisAlignedBox2f::HasXOverlap(const SEAxisAlignedBox2f& rBox) const
{
    return Max[0] >= rBox.Min[0] && Min[0] <= rBox.Max[0];
}
//----------------------------------------------------------------------------
bool SEAxisAlignedBox2f::HasYOverlap(const SEAxisAlignedBox2f& rBox) const
{
    return Max[1] >= rBox.Min[1] && Min[1] <= rBox.Max[1];
}
//----------------------------------------------------------------------------
bool SEAxisAlignedBox2f::TestIntersection(const SEAxisAlignedBox2f& rBox) 
    const
{
    for( int i = 0; i < 2; i++ )
    {
        if( Max[i] < rBox.Min[i] || Min[i] > rBox.Max[i] )
        {
            return false;
        }
    }

    return true;
}
//----------------------------------------------------------------------------
bool SEAxisAlignedBox2f::FindIntersection(const SEAxisAlignedBox2f& rBox,
    SEAxisAlignedBox2f& rIntr) const
{
    int i;
    for( i = 0; i < 2; i++ )
    {
        if( Max[i] < rBox.Min[i] || Min[i] > rBox.Max[i] )
        {
            return false;
        }
    }

    for( i = 0; i < 2; i++ )
    {
        if( Max[i] <= rBox.Max[i] )
        {
            rIntr.Max[i] = Max[i];
        }
        else
        {
            rIntr.Max[i] = rBox.Max[i];
        }

        if( Min[i] <= rBox.Min[i] )
        {
            rIntr.Min[i] = rBox.Min[i];
        }
        else
        {
            rIntr.Min[i] = Min[i];
        }
    }

    return true;
}
//----------------------------------------------------------------------------
