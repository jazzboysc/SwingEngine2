// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

//----------------------------------------------------------------------------
template <class Matrix3, class Vector3>
void SECoordinateSystemAdapter::SEToZUpRHColumnMajorOrder(const SEMatrix3f& srcRotation, const SEVector3f& srcLocation, Matrix3& dstRotation, Vector3& dstLocation)
{
    dstRotation[0][0] = srcRotation[0][0];
    dstRotation[0][1] = srcRotation[0][2];
    dstRotation[0][2] = srcRotation[0][1];
    dstRotation[1][0] = srcRotation[2][0];
    dstRotation[1][1] = srcRotation[2][2];
    dstRotation[1][2] = srcRotation[2][1];
    dstRotation[2][0] = srcRotation[1][0];
    dstRotation[2][1] = srcRotation[1][2];
    dstRotation[2][2] = srcRotation[1][1];
    dstLocation[0] = srcLocation[0];
    dstLocation[1] = srcLocation[2];
    dstLocation[2] = srcLocation[1];
}
//----------------------------------------------------------------------------