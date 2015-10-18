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

#include "SEGeometricToolsPCH.h"
#include "SEHullEdge2.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEHullEdge2f::SEHullEdge2f(int iV0,  int iV1)
{
    V[0] = iV0;
    V[1] = iV1;
    A[0] = 0;
    A[1] = 0;
    Sign = 0;
    Time = -1;
}
//----------------------------------------------------------------------------
int SEHullEdge2f::GetSign(int i,  const SEQuery2f* pQuery)
{
    if( i != Time )
    {
        Time = i;
        Sign = pQuery->ToLine(i,  V[0],  V[1]);
    }

    return Sign;
}
//----------------------------------------------------------------------------
void SEHullEdge2f::Insert(SEHullEdge2f* pAdj0,  SEHullEdge2f* pAdj1)
{
    pAdj0->A[1] = this;
    pAdj1->A[0] = this;
    A[0] = pAdj0;
    A[1] = pAdj1;
}
//----------------------------------------------------------------------------
void SEHullEdge2f::DeleteSelf()
{
    if( A[0] )
    {
        A[0]->A[1] = 0;
    }

    if( A[1] )
    {
        A[1]->A[0] = 0;
    }

    SE_DELETE this;
}
//----------------------------------------------------------------------------
void SEHullEdge2f::DeleteAll()
{
    SEHullEdge2f* pAdj = A[1];
    while( pAdj && pAdj != this )
    {
        SEHullEdge2f* pSave = pAdj->A[1];
        SE_DELETE pAdj;
        pAdj = pSave;
    }

    SE_ASSERT(pAdj == this);

    SE_DELETE this;
}
//----------------------------------------------------------------------------
void SEHullEdge2f::GetIndices(int& riHCount,  int*& raiHIndex)
{
    // Count the number of edge vertices and allocate the index array.
    riHCount = 0;
    SEHullEdge2f* pCurrent = this;
    do
    {
        riHCount++;
        pCurrent = pCurrent->A[1];
    }
    while( pCurrent != this );
    raiHIndex = SE_NEW int[riHCount];

    // Fill the index array.
    riHCount = 0;
    pCurrent = this;
    do
    {
        raiHIndex[riHCount++] = pCurrent->V[0];
        pCurrent = pCurrent->A[1];
    }
    while( pCurrent != this );
}
//----------------------------------------------------------------------------