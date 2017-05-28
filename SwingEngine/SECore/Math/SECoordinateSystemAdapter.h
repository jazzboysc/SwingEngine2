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
    static void SEToZUpRHColumnMajorOrder(const SEMatrix3f& srcRotation, const SEVector3f& srcLocation, Matrix3& dstRotation, Vector3& dstLocation);

    template <class Matrix3>
    static void SEToZUpRHColumnMajorOrder(const SEMatrix3f& srcRotation, Matrix3& dstRotation);

    template <class Vector3>
    static void SEToZUpRHColumnMajorOrder(const SEVector3f& srcLocation, Vector3& dstLocation);
};

#include "SECoordinateSystemAdapter.inl"

}

#endif