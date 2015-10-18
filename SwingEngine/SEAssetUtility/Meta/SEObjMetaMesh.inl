// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

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
bool SEObjMetaMesh::HasTCoord() const
{
    return mTCoordCount > 0;
}
//----------------------------------------------------------------------------
bool SEObjMetaMesh::HasNormal() const
{
    return mVertexNormalCount > 0;
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
std::vector<SEVector3f>& SEObjMetaMesh::GetVertexData()
{
    return mVertexData;
}
//----------------------------------------------------------------------------
std::vector<SEVector3f>& SEObjMetaMesh::GetTCoordData()
{
    return mTCoordData;
}
//----------------------------------------------------------------------------
std::vector<SEVector3f>& SEObjMetaMesh::GetVertexNormalData()
{
    return mVertexNormalData;
}
//----------------------------------------------------------------------------
std::vector<SEVector3f>& SEObjMetaMesh::GetFaceNormalData()
{
    return mFaceNormalData;
}
//----------------------------------------------------------------------------
std::vector<MetaMeshFaceIndex>& SEObjMetaMesh::GetIndexData()
{
    return mIndexData;
}
//----------------------------------------------------------------------------