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

#ifndef Swing_Tuple_H
#define Swing_Tuple_H

#include "SESystem.h"

// The class TYPE is either native data or is class data that has the
// following member functions:
//   TYPE::TYPE()
//   TYPE::TYPE(const TYPE&);
//   TYPE& TYPE::operator=(const TYPE&)

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20100623
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
class SETuple
{
public:
    // Construction and destruction.  The default constructor does not
    // initialize the tuple elements for native elements. The tuple elements
    // are initialized for class data whenever TYPE initializes during its
    // default construction.
    SETuple(void);
    SETuple(const SETuple& rTuple);
    ~SETuple(void);

    // Coordinate access.
    inline operator const TYPE* (void) const;
    inline operator TYPE* (void);
    inline TYPE operator [] (int i) const;
    inline TYPE& operator [] (int i);

    // Assignment.
    SETuple& operator = (const SETuple& rTuple);

    // Comparison. The inequalities make the comparisons using memcmp, thus
    // treating the tuple as an array of unsigned bytes.
    bool operator == (const SETuple& rTuple) const;
    bool operator != (const SETuple& rTuple) const;
    bool operator <  (const SETuple& rTuple) const;
    bool operator <= (const SETuple& rTuple) const;
    bool operator >  (const SETuple& rTuple) const;
    bool operator >= (const SETuple& rTuple) const;

protected:
    TYPE m_Tuple[DIMENSION];
};

#include "SETuple.inl"

}

#endif
