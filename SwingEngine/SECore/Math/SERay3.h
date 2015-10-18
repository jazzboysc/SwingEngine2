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

#ifndef Swing_Ray3_H
#define Swing_Ray3_H

#include "SECoreLIB.h"
#include "SEVector3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20080312
//----------------------------------------------------------------------------
class SE_CORE_API SERay3f
{
public:
    // 直线表示为参数方程形式:P + t*D,其中P是直线上一点,D是单位长度方向向量,
    // 用户必须确保D为单位向量.

    SERay3f(void);
    SERay3f(const SEVector3f& rOrigin, const SEVector3f& rDirection);
    SERay3f(const SEVector3f& rOrigin, const SEVector3f& rDirection, 
        float fMinT, float fMaxT = SEMathf::MAX_REAL, float fTime = 0.0f, 
        int iDepth = 0);
    SERay3f(const SEVector3f& rOrigin, const SEVector3f& rDirection, 
        const SERay3f& rParent, float fMinT, float fMaxT = SEMathf::MAX_REAL);

    SEVector3f operator()(float fT) const;

public:
    SEVector3f Origin, Direction;
    mutable float MinT, MaxT;
    float Time;
    int Depth;
};

}

#endif
