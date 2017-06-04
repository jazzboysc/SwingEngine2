// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_ObjMetaMesh_H
#define Swing_ObjMetaMesh_H

#include "SEAssetUtilityLIB.h"
#include "SEReferencable.h"
#include "SEVector3.h"
#include "SEObjMetaMaterial.h"
#include "SEIMetaMesh.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 07/01/2015
//----------------------------------------------------------------------------
class SE_ASSET_UTILITY_API SEObjMetaMesh : public SEReferencable, public SEIMetaMesh
{
public:
    SEObjMetaMesh();
    ~SEObjMetaMesh();

    inline SE_Int32 GetVertexCount() const;
	inline SE_Int32 GetTCoordCount() const;
    inline SE_Int32 GetVertexNormalCount() const;
    inline SE_Int32 GetFaceCount() const;

	// Indicate what kind of polygon this mesh has.
	// Only support triangle and quad for now.
	inline SE_UInt32 GetFaceFlag() const;
	inline void SetFaceFlag(SE_UInt32 faceFlag);

    // Implement mesh data access interface.
    // Note that obj format only allows one uvw channel.
    virtual std::vector<SEVector3f>& GetVertexData() override;
    virtual std::vector<SEVector3f>& GetTCoordData(unsigned int channel) override;
    virtual std::vector<SEVector3f>& GetVertexNormalData() override;
    virtual std::vector<SEVector3f>& GetFaceNormalData() override;
	virtual std::vector<MetaMeshFaceIndex>& GetIndexData() override;
    virtual bool HasTCoord(unsigned int channel) const override;
    virtual bool HasNormal() const override;
    virtual unsigned int GetTCoordChannelCount() const override;

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