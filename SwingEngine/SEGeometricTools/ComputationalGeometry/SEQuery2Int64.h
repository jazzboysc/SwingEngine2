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

#ifndef Swing_Query2Int64_H
#define Swing_Query2Int64_H

#include "SEGeometricToolsLIB.h"
#include "SEQuery2.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20081202
//----------------------------------------------------------------------------
class SEQuery2Int64f : public SEQuery2f
{
public:
    // The components of the input vertices are truncated to 64-bit integer
    // values, so you should guarantee that the vertices are sufficiently
    // large to give a good distribution of numbers.
    SEQuery2Int64f(int iVCount, const SEVector2f* aVertex);

    // run-time type information
    virtual SEQuery::Type GetType(void) const;

    // Queries about the relation of a point to various geometric objects.

    virtual int ToLine(const SEVector2f& rP, int iV0, int iV1) const;

    virtual int ToCircumcircle(const SEVector2f& rP, int iV0, int iV1, 
        int iV2) const;

private:
    static SE_Int64 Dot(SE_Int64 iX0, SE_Int64 iY0, SE_Int64 iX1, 
        SE_Int64 iY1);

    static SE_Int64 Det2(SE_Int64 iX0, SE_Int64 iY0, SE_Int64 iX1, 
        SE_Int64 iY1);

    static SE_Int64 Det3(SE_Int64 iX0, SE_Int64 iY0, SE_Int64 iZ0, 
        SE_Int64 iX1, SE_Int64 iY1, SE_Int64 iZ1, 
        SE_Int64 iX2, SE_Int64 iY2, SE_Int64 iZ2);
};

}

#endif
