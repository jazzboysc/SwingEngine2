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

#ifndef Swing_IVector3_H
#define Swing_IVector3_H

#include "SEGeometricToolsLIB.h"
#include "SETIVector.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20081202
//----------------------------------------------------------------------------
class SEIVector3 : public SETIVector<3>
{
public:
    // construction
    SEIVector3(void);
    SEIVector3(const SEIVector3& rV);
    SEIVector3(const SETIVector<3>& rV);
    SEIVector3(const SE_Int64& riX,  const SE_Int64& riY, 
        const SE_Int64& riZ);

    // member access
    SE_Int64 X(void) const;
    SE_Int64& X(void);
    SE_Int64 Y(void) const;
    SE_Int64& Y(void);
    SE_Int64 Z(void) const;
    SE_Int64& Z(void);

    // assignment
    SEIVector3& operator = (const SEIVector3& rV);
    SEIVector3& operator = (const SETIVector<3>& rV);

    // returns Dot(this, V)
    SE_Int64 Dot(const SEIVector3& rV) const;

    // returns Cross(this, V)
    SEIVector3 Cross(const SEIVector3& rV) const;

    // returns Dot(this, Cross(U, V))
    SE_Int64 TripleScalar(const SEIVector3& rU,  const SEIVector3& rV) const;

protected:
    using SETIVector<3>::m_aiTuple;
};

#include "SEIVector3.inl"

}

#endif
