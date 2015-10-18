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

#ifndef Swing_ConvexHull2_H
#define Swing_ConvexHull2_H

#include "SEGeometricToolsLIB.h"
#include "SEConvexHull1.h"
#include "SEHullEdge2.h"
#include "SEQuery2.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20081201
//----------------------------------------------------------------------------
class SE_GEOMETRICTOOLS_API SEConvexHull2f : public SEConvexHullf
{
public:
    // The input to the constructor is the array of vertices whose convex hull
    // is required. If you want SEConvexHull2f to delete the vertices during
    // destruction, set bOwner to 'true'. Otherwise, you own the vertices and
    // must delete them yourself.
    //
    // You have a choice of speed versus accuracy. The fastest choice is
    // SEQuery::QT_INT64, but it gives up a lot of precision, scaling the 
    // points to [0, 2^{20}]^3. The choice SEQuery::QT_INTEGER gives up less 
    // precision, scaling the points to [0, 2^{24}]^3. The choice 
    // SEQuery::QT_RATIONAL uses exact arithmetic, but is the slowest choice. 
    // The choice SEQuery::QT_REAL uses floating-point arithmetic, but is not 
    // robust in all cases.

    SEConvexHull2f(int iVertexCount,  SEVector2f* aVertex,  float fEpsilon, 
        bool bOwner,  SEQuery::Type eQueryType);
    virtual ~SEConvexHull2f(void);

    // If GetDimension() returns 1, then the points lie on a line.  You must
    // create a SEConvexHull1f object using the function provided.
    const SEVector2f& GetLineOrigin(void) const;
    const SEVector2f& GetLineDirection(void) const;
    SEConvexHull1f* GetConvexHull1(void) const;

private:
    // Support for streaming to/from disk.
    SEConvexHull2f(const char* acFilename);
    bool Load(const char* acFilename);
    bool Save(const char* acFilename) const;

    bool Update(SEHullEdge2f*& rpHull,  int i);

    // The input points.
    SEVector2f* m_aVertex;

    // Support for robust queries.
    SEVector2f* m_aSVertex;
    SEQuery2f* m_pQuery;

    // The line of containment if the dimension is 1.
    SEVector2f m_LineOrigin,  m_LineDirection;
};

}

#endif
