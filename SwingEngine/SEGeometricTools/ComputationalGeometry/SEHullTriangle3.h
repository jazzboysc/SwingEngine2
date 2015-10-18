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

#ifndef Swing_HullTriangle3_H
#define Swing_HullTriangle3_H

#include "SEGeometricToolsLIB.h"
#include "SEQuery3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20081201
//----------------------------------------------------------------------------
class SE_GEOMETRICTOOLS_API SEHullTriangle3f
{
public:
    SEHullTriangle3f(int iV0,  int iV1,  int iV2);

    int GetSign(int i,  const SEQuery3f* pQuery);

    void AttachTo(SEHullTriangle3f* pAdj0,  SEHullTriangle3f* pAdj1, 
        SEHullTriangle3f* pAdj2);

    int DetachFrom(int iAdj,  SEHullTriangle3f* pAdj);

    int V[3];
    SEHullTriangle3f* A[3];
    int Sign;
    int Time;
    bool OnStack;
};

}

#endif
