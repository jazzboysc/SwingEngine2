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

#ifndef Swing_Query2TInteger_H
#define Swing_Query2TInteger_H

#include "SEGeometricToolsLIB.h"
#include "SEQuery2.h"
#include "SETInteger.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20081202
//----------------------------------------------------------------------------
class SEQuery2TIntegerf : public SEQuery2f
{
public:
    // The components of the input vertices are truncated to 32-bit integer
    // values, so you should guarantee that the vertices are sufficiently
    // large to give a good distribution of numbers.  The value N in
    // SETInteger<N> is chosen large enough so that the exact arithmetic is
    // correct for the functions.
    SEQuery2TIntegerf(int iVCount, const SEVector2f* aVertex);

    // run-time type information
    virtual SEQuery::Type GetType(void) const;

    // Queries about the relation of a point to various geometric objects.

    virtual int ToLine(const SEVector2f& rP, int iV0, int iV1) const;

    virtual int ToCircumcircle(const SEVector2f& rP, int iV0, int iV1, 
        int iV2) const;

private:
    static SETInteger<2> Dot(SETInteger<2>& rX0, SETInteger<2>& rY0, 
        SETInteger<2>& rX1, SETInteger<2>& rY1);

    static SETInteger<2> Det2(SETInteger<2>& rX0, SETInteger<2>& rY0, 
        SETInteger<2>& rX1, SETInteger<2>& rY1);

    static SETInteger<4> Det3(SETInteger<4>& rX0, SETInteger<4>& rY0, 
        SETInteger<4>& rZ0, SETInteger<4>& rX1, SETInteger<4>& rY1, 
        SETInteger<4>& rZ1, SETInteger<4>& rX2, SETInteger<4>& rY2, 
        SETInteger<4>& rZ2);
};

}

#endif
