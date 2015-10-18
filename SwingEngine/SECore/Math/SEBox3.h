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

#ifndef Swing_Box3_H
#define Swing_Box3_H

#include "SECoreLIB.h"
#include "SEVector3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:oriented bounding box
// Date:20070705
//----------------------------------------------------------------------------
class SE_CORE_API SEBox3f
{
public:
    SEBox3f(void);
    SEBox3f(const SEVector3f& rCenter, const SEVector3f* aAxis, 
        const float* afExtent);
    SEBox3f(const SEVector3f& rCenter, const SEVector3f& rAxis0, 
        const SEVector3f& rAxis1, const SEVector3f& rAxis2, float fExtent0, 
        float fExtent1, float fExtent2);

    void ComputeVertices(SEVector3f aVertex[8]) const;

    SEVector3f Center;  // 盒子中心
    SEVector3f Axis[3];  // 规范正交基
    float Extent[3];  // 半长,半宽,半高,均非负
};

}

#endif