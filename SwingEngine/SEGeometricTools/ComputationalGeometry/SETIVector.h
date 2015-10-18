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

#ifndef Swing_TIVector_H
#define Swing_TIVector_H

#include "SEGeometricToolsLIB.h"
#include "SESystem.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20081201
//----------------------------------------------------------------------------
template <int VSIZE>
class SETIVector
{
public:
    // construction
    SETIVector(void);
    SETIVector(const SETIVector& rV);

    // coordinate access
    operator const SE_Int64* (void) const;
    operator SE_Int64* (void);
    SE_Int64 operator [] (int i) const;
    SE_Int64& operator [] (int i);

    // assignment
    SETIVector& operator = (const SETIVector& rV);

    // comparison
    bool operator == (const SETIVector& rV) const;
    bool operator != (const SETIVector& rV) const;
    bool operator <  (const SETIVector& rV) const;
    bool operator <= (const SETIVector& rV) const;
    bool operator >  (const SETIVector& rV) const;
    bool operator >= (const SETIVector& rV) const;

    // arithmetic operations
    SETIVector operator + (const SETIVector& rV) const;
    SETIVector operator - (const SETIVector& rV) const;
    SETIVector operator * (const SE_Int64& riI) const;
    SETIVector operator / (const SE_Int64& riI) const;
    SETIVector operator - (void) const;

    // arithmetic updates
    SETIVector& operator += (const SETIVector& rV);
    SETIVector& operator -= (const SETIVector& rV);
    SETIVector& operator *= (const SE_Int64& riI);
    SETIVector& operator /= (const SE_Int64& riI);

    // vector operations
    SE_Int64 GetSquaredLength(void) const;
    SE_Int64 Dot(const SETIVector& rV) const;

protected:
    // support for comparisons
    int CompareArrays(const SETIVector& rV) const;

    SE_Int64 m_aiTuple[VSIZE];
};

template <int VSIZE>
SETIVector<VSIZE> operator * (const SE_Int64& riI,  const SETIVector<VSIZE>& 
    rV);

#include "SETIVector.inl"

}

#endif
