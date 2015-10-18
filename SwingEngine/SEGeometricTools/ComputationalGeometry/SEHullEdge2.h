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

#ifndef Swing_HullEdge2_H
#define Swing_HullEdge2_H

#include "SEGeometricToolsLIB.h"
#include "SEQuery2.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20081201
//----------------------------------------------------------------------------
class SE_GEOMETRICTOOLS_API SEHullEdge2f
{
public:
    SEHullEdge2f(int iV0,  int iV1);

    int GetSign(int i,  const SEQuery2f* pQuery);

    void Insert(SEHullEdge2f* pAdj0,  SEHullEdge2f* pAdj1);
    void DeleteSelf(void);
    void DeleteAll(void);

    void GetIndices(int& riHCount,  int*& raiHIndex);

    int V[2];
    SEHullEdge2f* A[2];
    int Sign;
    int Time;
};

}

#endif
