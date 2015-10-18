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

#ifndef Swing_Query2_H
#define Swing_Query2_H

#include "SEGeometricToolsLIB.h"
#include "SEQuery.h"
#include "SEVector2.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20081202
//----------------------------------------------------------------------------
class SEQuery2f : public SEQuery
{
public:
    // The base class handles floating-point queries.
    SEQuery2f(int iVCount, const SEVector2f* aVertex);
    virtual ~SEQuery2f(void);

    // run-time type information
    virtual SEQuery::Type GetType(void) const;

    // member access
    int GetCount(void) const;
    const SEVector2f* GetVertices(void) const;

    // Queries about the relation of a point to various geometric objects.

    // returns
    //   +1, on right of line
    //   -1, on left of line
    //    0, on the line
    virtual int ToLine(int i, int iV0, int iV1) const;
    virtual int ToLine(const SEVector2f& rP, int iV0, int iV1) const;

    // returns
    //   +1, outside triangle
    //   -1, inside triangle
    //    0, on triangle
    virtual int ToTriangle(int i, int iV0, int iV1, int iV2) const;
    virtual int ToTriangle(const SEVector2f& rP, int iV0, int iV1,
        int iV2) const;

    // returns
    //   +1, outside circumcircle of triangle
    //   -1, inside circumcircle of triangle
    //    0, on circumcircle of triangle
    virtual int ToCircumcircle(int i, int iV0, int iV1, int iV2) const;
    virtual int ToCircumcircle(const SEVector2f& rP, int iV0, int iV1,
        int iV2) const;

protected:
    // input points
    int m_iVCount;
    const SEVector2f* m_aVertex;

    static float Dot(float fX0, float fY0, float fX1, float fY1);

    static float Det2(float fX0, float fY0, float fX1, float fY1);

    static float Det3(float iX0, float iY0, float iZ0, float iX1, float iY1,
        float iZ1, float iX2, float iY2, float iZ2);
};

}

#endif
