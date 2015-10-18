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

#ifndef Swing_Query3Filtered_H
#define Swing_Query3Filtered_H

#include "SEGeometricToolsLIB.h"
#include "SEQuery3TRational.h"
#include "SEVector3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20081202
//----------------------------------------------------------------------------
class SEQuery3Filteredf : public SEQuery3f
{
public:
    // The base class handles floating-point queries.  Each query involves
    // comparing a determinant to zero.  If the determinant is sufficiently
    // close to zero, numerical round-off errors may cause the determinant
    // sign to be misclassified.  To avoid this, the query is repeated with
    // exact rational arithmetic.  You specify the closeness to zero for the
    // switch to rational arithmetic via fUncertainty, a value in the
    // interval [0, 1].  The uncertainty of 0 causes the class to behave
    // as if it were SEQuery3f.  The uncertainty of 1 causes the class to
    // behave as if it were SEQuery3TRationalf.
    SEQuery3Filteredf(int iVCount, const SEVector3f* aVertex, float 
        fUncertainty);
    virtual ~SEQuery3Filteredf(void);

    // run-time type information
    virtual SEQuery::Type GetType(void) const;

    // Queries about the relation of a point to various geometric objects.

    virtual int ToPlane(const SEVector3f& rP, int iV0, int iV1, int iV2)
        const;

    virtual int ToCircumsphere(const SEVector3f& rP, int iV0, int iV1, 
        int iV2, int iV3) const;

private:
    SEQuery3TRationalf m_RQuery;
    float m_fUncertainty;
};

}

#endif
