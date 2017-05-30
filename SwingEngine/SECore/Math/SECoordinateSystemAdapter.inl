// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

//----------------------------------------------------------------------------
template <class Matrix3, class Vector3>
void SECoordinateSystemAdapter::SEToZUpRHColumnMajorOrder(const SEMatrix3f& srcMatrix, const SEVector3f& srcOffset, Matrix3& dstMatrix, Vector3& dstOffset)
{
    dstMatrix[0][0] = srcMatrix[0][0];
    dstMatrix[0][1] = srcMatrix[0][2];
    dstMatrix[0][2] = srcMatrix[0][1];
    dstMatrix[1][0] = srcMatrix[2][0];
    dstMatrix[1][1] = srcMatrix[2][2];
    dstMatrix[1][2] = srcMatrix[2][1];
    dstMatrix[2][0] = srcMatrix[1][0];
    dstMatrix[2][1] = srcMatrix[1][2];
    dstMatrix[2][2] = srcMatrix[1][1];
    dstOffset[0] = srcOffset[0];
    dstOffset[1] = srcOffset[2];
    dstOffset[2] = srcOffset[1];
}
//----------------------------------------------------------------------------
template <class Matrix3>
void SECoordinateSystemAdapter::SEToZUpRHColumnMajorOrder(const SEMatrix3f& srcMatrix, Matrix3& dstMatrix)
{
    dstMatrix[0][0] = srcMatrix[0][0];
    dstMatrix[0][1] = srcMatrix[0][2];
    dstMatrix[0][2] = srcMatrix[0][1];
    dstMatrix[1][0] = srcMatrix[2][0];
    dstMatrix[1][1] = srcMatrix[2][2];
    dstMatrix[1][2] = srcMatrix[2][1];
    dstMatrix[2][0] = srcMatrix[1][0];
    dstMatrix[2][1] = srcMatrix[1][2];
    dstMatrix[2][2] = srcMatrix[1][1];
}
//----------------------------------------------------------------------------
template <class Vector3>
void SECoordinateSystemAdapter::SEToZUpRHColumnMajorOrder(const SEVector3f& srcOffset, Vector3& dstOffset)
{
    dstOffset[0] = srcOffset[0];
    dstOffset[1] = srcOffset[2];
    dstOffset[2] = srcOffset[1];
}
//----------------------------------------------------------------------------