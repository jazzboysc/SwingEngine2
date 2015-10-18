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

#ifndef Swing_AxisAlignedBox2_H
#define Swing_AxisAlignedBox2_H

#include "SECoreLIB.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20100121
//----------------------------------------------------------------------------
class SE_CORE_API SEAxisAlignedBox2f
{
public:
    SEAxisAlignedBox2f(void);

    // The caller must ensure that fXMin <= fXMax and fYMin <= fYMax. The
    // class will not check for validity of the input.
    SEAxisAlignedBox2f(float fXMin, float fXMax, float fYMin, float fYMax);

    // Overlap testing is in the strict sense. If the two boxes are just
    // touching along a common edge, the boxes are reported as overlapping.
    bool HasXOverlap(const SEAxisAlignedBox2f& rBox) const;
    bool HasYOverlap(const SEAxisAlignedBox2f& rBox) const;
    bool TestIntersection(const SEAxisAlignedBox2f& rBox) const;

    // The return value is 'true' if there is overlap. In this case the
    // intersection is stored in rIntr. If the return value is 'false',
    // if there is no overlap. In this case rIntr is undefined.
    bool FindIntersection(const SEAxisAlignedBox2f& rBox,
        SEAxisAlignedBox2f& rIntr) const;

    float Min[2], Max[2];
};

}

#endif
