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

#ifndef Swing_AxisAlignedBox3_H
#define Swing_AxisAlignedBox3_H

#include "SECoreLIB.h"
#include "SEVector3.h"
#include "SERay3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20100121
//----------------------------------------------------------------------------
class SE_CORE_API SEAxisAlignedBox3f
{
public:
    // Min is assigned with SEMathf::MAX_REAL, Max is assigned with 
    // -(SEMathf::MAX_REAL). This is useful when a default created box is 
    // combined with another box.
    SEAxisAlignedBox3f(void);

    // The caller must ensure that fXMin <= fXMax and fYMin <= fYMax. This
    // function will not check for validity of the input.
    SEAxisAlignedBox3f(float fXMin, float fXMax, float fYMin, float fYMax,
        float fZMin, float fZMax);

    SEAxisAlignedBox3f(const SEVector3f& rP1, const SEVector3f& rP2);
    SEAxisAlignedBox3f(const SEVector3f& rP);

    SEAxisAlignedBox3f& operator = (const SEVector3f& rP);
    bool operator == (const SEAxisAlignedBox3f& rBox) const;
    bool operator != (const SEAxisAlignedBox3f& rBox) const;

    // Overlap testing is in the strict sense. If the two boxes are just
    // touching along a common edge, the boxes are reported as overlapping.
    bool HasXOverlap(const SEAxisAlignedBox3f& rBox) const;
    bool HasYOverlap(const SEAxisAlignedBox3f& rBox) const;
    bool HasZOverlap(const SEAxisAlignedBox3f& rBox) const;
    bool TestIntersection(const SEAxisAlignedBox3f& rBox) const;

    // The return value is 'true' if there is overlap. In this case the
    // intersection is stored in rIntr. If the return value is 'false',
    // there is no overlap. In this case rIntr is undefined.
    bool FindIntersection(const SEAxisAlignedBox3f& rBox,
        SEAxisAlignedBox3f& rIntr) const;

    // Ray-box intersection test.
    bool FindIntersection(const SERay3f& rRay, float* fHitT0 = 0, 
        float* fHitT1 = 0) const;

    // Test if the given point is inside the box.
    bool IsInside(const SEVector3f& rP) const;

    // Expand the box with a given delta value.
    void Expand(float fDelta);

    // Get various geometric information.
    float GetSurfaceArea() const;
    float GetVolume() const;
    int GetMaximumExtent() const;
    SEVector3f GetDelta() const;
    SEVector3f GetBoxCenter() const;
    SEVector3f GetExtension() const;

    void Merge(const SEAxisAlignedBox3f& box);
    void ExpandToCube();

public:
    float Min[3], Max[3];
};

//----------------------------------------------------------------------------
SE_CORE_API SEAxisAlignedBox3f Union(const SEAxisAlignedBox3f& rBox, 
    const SEVector3f& rP);
//----------------------------------------------------------------------------
SE_CORE_API SEAxisAlignedBox3f Union(const SEAxisAlignedBox3f& rBox1, 
    const SEAxisAlignedBox3f& rBox2);
//----------------------------------------------------------------------------

}

#endif
