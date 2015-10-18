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

#ifndef Swing_IVector2_H
#define Swing_IVector2_H

#include "SEGeometricToolsLIB.h"
#include "SETIVector.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20081202
//----------------------------------------------------------------------------
class SEIVector2 : public SETIVector<2>
{
public:
    // construction
    SEIVector2(void);
    SEIVector2(const SEIVector2& rV);
    SEIVector2(const SETIVector<2>& rV);
    SEIVector2(const SE_Int64& riX,  const SE_Int64& riY);

    // member access
    SE_Int64 X(void) const;
    SE_Int64& X(void);
    SE_Int64 Y(void) const;
    SE_Int64& Y(void);

    // assignment
    SEIVector2& operator = (const SEIVector2& rV);
    SEIVector2& operator = (const SETIVector<2>& rV);

    // returns Dot(this, V)
    SE_Int64 Dot(const SEIVector2& rV) const;

    // returns (y, -x)
    SEIVector2 Perp(void) const;

    // returns Cross((x, y, 0), (V.x, V.y, 0)) = x*V.y - y*V.x
    SE_Int64 DotPerp(const SEIVector2& rV) const;

protected:
    using SETIVector<2>::m_aiTuple;
};

#include "SEIVector2.inl"

}

#endif
