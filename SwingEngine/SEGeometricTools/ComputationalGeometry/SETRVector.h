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

#ifndef Swing_TRVector_H
#define Swing_TRVector_H

#include "SEGeometricToolsLIB.h"
#include "SETRational.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20081202
//----------------------------------------------------------------------------
template <int VSIZE,  int ISIZE>
class SETRVector
{
public:
    // construction
    SETRVector(void);
    SETRVector(const SETRVector& rV);

    // coordinate access
    operator const SETRational<ISIZE>* (void) const;
    operator SETRational<ISIZE>* (void);
    SETRational<ISIZE> operator [] (int i) const;
    SETRational<ISIZE>& operator [] (int i);

    // assignment
    SETRVector& operator = (const SETRVector& rV);

    // comparison
    bool operator == (const SETRVector& rV) const;
    bool operator != (const SETRVector& rV) const;
    bool operator <  (const SETRVector& rV) const;
    bool operator <= (const SETRVector& rV) const;
    bool operator >  (const SETRVector& rV) const;
    bool operator >= (const SETRVector& rV) const;

    // arithmetic operations
    SETRVector operator + (const SETRVector& rV) const;
    SETRVector operator - (const SETRVector& rV) const;
    SETRVector operator * (const SETRational<ISIZE>& rR) const;
    SETRVector operator / (const SETRational<ISIZE>& rR) const;
    SETRVector operator - (void) const;

    // arithmetic updates
    SETRVector& operator += (const SETRVector& rV);
    SETRVector& operator -= (const SETRVector& rV);
    SETRVector& operator *= (const SETRational<ISIZE>& rR);
    SETRVector& operator /= (const SETRational<ISIZE>& rR);

    // vector operations
    SETRational<ISIZE> GetSquaredLength(void) const;
    SETRational<ISIZE> Dot(const SETRVector& rV) const;

protected:
    // support for comparisons
    int CompareArrays(const SETRVector& rV) const;

    SETRational<ISIZE> m_aTuple[VSIZE];
};

template <int VSIZE,  int ISIZE>
SETRVector<VSIZE,  ISIZE> operator * (const SETRational<ISIZE>& rR, 
    const SETRVector<VSIZE,  ISIZE>& rV);

#include "SETRVector.inl"

}

#endif
