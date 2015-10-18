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

#ifndef Swing_Box2_H
#define Swing_Box2_H

#include "SECoreLIB.h"
#include "SEVector2.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:oriented bounding box
// Date:20090113
//----------------------------------------------------------------------------
class SE_CORE_API SEBox2f
{
public:
    SEBox2f(void);
    SEBox2f(const SEVector2f& rCenter, const SEVector2f* aAxis,
        const float* afExtent);
    SEBox2f(const SEVector2f& rCenter, const SEVector2f& rAxis0,
        const SEVector2f& rAxis1, float fExtent0, float fExtent1);

    void ComputeVertices(SEVector2f aVertex[4]) const;

    SEVector2f Center;  // 盒子中心
    SEVector2f Axis[2];  // 规范正交基
    float Extent[2];  // 半长,半宽,均非负
};

}

#endif
