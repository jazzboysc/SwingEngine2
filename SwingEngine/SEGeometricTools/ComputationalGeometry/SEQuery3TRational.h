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

#ifndef Swing_Query3TRational_H
#define Swing_Query3TRational_H

#include "SEGeometricToolsLIB.h"
#include "SEQuery3.h"
#include "SERVector3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20081202
//----------------------------------------------------------------------------
class SEQuery3TRationalf : public SEQuery3f
{
public:
    // The components of the input vertices are represented exactly as
    // rational values.
    SEQuery3TRationalf(int iVCount, const SEVector3f* aVertex);
    virtual ~SEQuery3TRationalf(void);

    // run-time type information
    virtual SEQuery::Type GetType(void) const;

    // Queries about the relation of a point to various geometric objects.

    virtual int ToPlane(int i, int iV0, int iV1, int iV2) const;
    virtual int ToPlane(const SEVector3f& rP, int iV0, int iV1, int iV2)
        const;

    virtual int ToTetrahedron(int i, int iV0, int iV1, int iV2, int iV3)
        const;
    virtual int ToTetrahedron(const SEVector3f& rP, int iV0, int iV1, 
        int iV2, int iV3) const;

    virtual int ToCircumsphere(int i, int iV0, int iV1, int iV2, int iV3)
        const;
    virtual int ToCircumsphere(const SEVector3f& rP, int iV0, int iV1, 
        int iV2, int iV3) const;

private:
    // Caching for rational representations of the input. The conversion of
    // floating-point numbers to SETRational form is slow, so it is better to
    // keep track of which values have been converted.
    typedef SETRational<8*sizeof(float)> Rational;
    typedef SERVector3<8*sizeof(float)> RVector;
    mutable RVector* m_aRVertex;
    mutable bool* m_abEvaluated;

    void Convert(int iCount, int* aiIndex) const;

    int ToPlane(const RVector& rRP, int iV0, int iV1, int iV2) const;
    int ToTetrahedron(const RVector& rRP, int iV0, int iV1, int iV2, 
        int iV3) const;
    int ToCircumsphere(const RVector& rRP, int iV0, int iV1, int iV2, 
        int iV3) const;

    static Rational Dot(Rational& rX0, Rational& rY0, Rational& rZ0, 
        Rational& rX1, Rational& rY1, Rational& rZ1);

    static Rational Det3(Rational& rX0, Rational& rY0, Rational& rZ0, 
        Rational& rX1, Rational& rY1, Rational& rZ1, Rational& rX2, 
        Rational& rY2, Rational& rZ2);

    static Rational Det4(Rational& rX0, Rational& rY0, Rational& rZ0, 
        Rational& rW0, Rational& rX1, Rational& rY1, Rational& rZ1, 
        Rational& rW1, Rational& rX2, Rational& rY2, Rational& rZ2, 
        Rational& rW2, Rational& rX3, Rational& rY3, Rational& rZ3, 
        Rational& rW3);
};

}

#endif
