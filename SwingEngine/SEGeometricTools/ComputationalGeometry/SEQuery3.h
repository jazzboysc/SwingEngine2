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

#ifndef Swing_Query3_H
#define Swing_Query3_H

#include "SEGeometricToolsLIB.h"
#include "SEQuery.h"
#include "SEVector3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20081202
//----------------------------------------------------------------------------
class SEQuery3f : public SEQuery
{
public:
    // The base class handles floating-point queries.
    SEQuery3f(int iVCount, const SEVector3f* aVertex);
    virtual ~SEQuery3f(void);

    // run-time type information
    virtual SEQuery::Type GetType(void) const;

    // member access
    int GetCount(void) const;
    const SEVector3f* GetVertices(void) const;

    // Queries about the relation of a point to various geometric objects.

    // returns
    //   +1, on positive side of plane
    //   -1, on negative side of line
    //    0, on the plane
    virtual int ToPlane(int i, int iV0, int iV1, int iV2) const;
    virtual int ToPlane(const SEVector3f& rP, int iV0, int iV1, int iV2)
        const;

    // returns
    //   +1, outside tetrahedron
    //   -1, inside tetrahedron
    //    0, on tetrahedron
    virtual int ToTetrahedron(int i, int iV0, int iV1, int iV2, int iV3)
        const;
    virtual int ToTetrahedron(const SEVector3f& rP, int iV0, int iV1, 
        int iV2, int iV3) const;

    // returns
    //   +1, outside circumsphere of tetrahedron
    //   -1, inside circumsphere of tetrahedron
    //    0, on circumsphere of tetrahedron
    virtual int ToCircumsphere(int i, int iV0, int iV1, int iV2, int iV3)
        const;
    virtual int ToCircumsphere(const SEVector3f& rP, int iV0, int iV1, 
        int iV2, int iV3) const;

protected:
    // input points
    int m_iVCount;
    const SEVector3f* m_aVertex;

    static float Dot(float fX0, float fY0, float fZ0, float fX1, float fY1, 
        float fZ1);

    static float Det3(float fX0, float fY0, float fZ0, float fX1, float fY1, 
        float fZ1, float fX2, float fY2, float fZ2);

    static float Det4(float fX0, float fY0, float fZ0, float fW0, float fX1, 
        float fY1, float fZ1, float fW1, float fX2, float fY2, float fZ2, 
        float fW2, float fX3, float fY3, float fZ3, float fW3);
};

}

#endif
