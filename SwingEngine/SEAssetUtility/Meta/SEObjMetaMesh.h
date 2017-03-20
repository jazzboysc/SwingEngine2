// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_ObjMetaMesh_H
#define Swing_ObjMetaMesh_H

#include "SEAssetUtilityLIB.h"
#include "SEReferencable.h"
#include "SEVector3.h"
#include "SEObjMetaMaterial.h"

namespace Swing
{

enum MetaMeshFaceType
{
	MMFT_Unknown       = 0,
	MMFT_TriangleIndex = 1,
	MMFT_QuadIndex     = 2
};

struct MetaMeshFaceIndex
{
	std::vector<SE_UInt32> VertexIndices;
	std::vector<SE_UInt32> TCoordIndices;
    std::vector<SE_UInt32> VertexNormalIndices;
};

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 07/01/2015
//----------------------------------------------------------------------------
class SE_ASSET_UTILITY_API SEObjMetaMesh : public SEReferencable
{
public:
    SEObjMetaMesh();
    ~SEObjMetaMesh();

    inline SE_Int32 GetVertexCount() const;
	inline SE_Int32 GetTCoordCount() const;
    inline SE_Int32 GetVertexNormalCount() const;
    inline SE_Int32 GetFaceCount() const;
    inline bool HasTCoord() const;
    inline bool HasNormal() const;

	// Indicate what kind of polygon this mesh has.
	// Only support triangle and quad for now.
	inline SE_UInt32 GetFaceFlag() const;
	inline void SetFaceFlag(SE_UInt32 faceFlag);

    inline std::vector<SEVector3f>& GetVertexData();
    inline std::vector<SEVector3f>& GetTCoordData();
    inline std::vector<SEVector3f>& GetVertexNormalData();
    inline std::vector<SEVector3f>& GetFaceNormalData();
	inline std::vector<MetaMeshFaceIndex>& GetIndexData();

    void AppendVertex(SEVector3f& vertex);
	void AppendTCoord(SEVector3f& tCoord);
    void AppendVertexNormal(SEVector3f& vertexNormal);
	void AppendFaceIndices(MetaMeshFaceIndex& indices);

	inline bool IsTriangleMesh() const;
	void ConvertToTriangleMesh();

	inline bool IsUnified() const;
	void ConvertToUnified();

	std::string MeshName;
	std::string MaterialName;
	SEObjMetaMaterialPtr MeshMaterial;

protected:
    SE_Int32 mVertexCount;
	SE_Int32 mTCoordCount;
    SE_Int32 mVertexNormalCount;
    SE_Int32 mFaceCount;
	SE_UInt32 mFaceFlag;

	bool mUnified;
    std::vector<SEVector3f> mVertexData;
    std::vector<SEVector3f> mTCoordData;
    std::vector<SEVector3f> mVertexNormalData;
    std::vector<SEVector3f> mFaceNormalData;
	std::vector<MetaMeshFaceIndex> mIndexData;
};

typedef SESmartPointer<SEObjMetaMesh> SEObjMetaMeshPtr;

#include "SEObjMetaMesh.inl"

}

#endif