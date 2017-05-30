// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_CoordinateSystemAdapter_H
#define Swing_CoordinateSystemAdapter_H

#include "SECoreLIB.h"
#include "SEMath.h"
#include "SEVector3.h"
#include "SEMatrix3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20170506
//----------------------------------------------------------------------------
class SE_CORE_API SECoordinateSystemAdapter
{
public:
    template <class Matrix3, class Vector3>
    static void SEToZUpRHColumnMajorOrder(const SEMatrix3f& srcMatrix, const SEVector3f& srcOffset, Matrix3& dstMatrix, Vector3& dstOffset);

    template <class Matrix3>
    static void SEToZUpRHColumnMajorOrder(const SEMatrix3f& srcMatrix, Matrix3& dstMatrix);

    template <class Vector3>
    static void SEToZUpRHColumnMajorOrder(const SEVector3f& srcOffset, Vector3& dstOffset);
};

#include "SECoordinateSystemAdapter.inl"

}

#endif