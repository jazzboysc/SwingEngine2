// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

//----------------------------------------------------------------------------
SE_Int32 SEObjMetaMesh::GetVertexCount() const
{
    return mVertexCount;
}
//----------------------------------------------------------------------------
SE_Int32 SEObjMetaMesh::GetTCoordCount() const
{
	return mTCoordCount;
}
//----------------------------------------------------------------------------
SE_Int32 SEObjMetaMesh::GetVertexNormalCount() const
{
    return mVertexNormalCount;
}
//----------------------------------------------------------------------------
SE_Int32 SEObjMetaMesh::GetFaceCount() const
{
    return mFaceCount;
}
//----------------------------------------------------------------------------
SE_UInt32 SEObjMetaMesh::GetFaceFlag() const
{
	return mFaceFlag;
}
//----------------------------------------------------------------------------
void SEObjMetaMesh::SetFaceFlag(SE_UInt32 faceFlag)
{
	mFaceFlag = faceFlag;
}
//----------------------------------------------------------------------------
bool SEObjMetaMesh::IsTriangleMesh() const
{
	return mFaceFlag == (SE_UInt32)MetaMeshFaceType::MMFT_TriangleIndex;
}
//----------------------------------------------------------------------------
bool SEObjMetaMesh::IsUnified() const
{
	return mUnified;
}
//----------------------------------------------------------------------------