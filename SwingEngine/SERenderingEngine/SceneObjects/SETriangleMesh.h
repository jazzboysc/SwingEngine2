// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_TriangleMesh_H
#define Swing_TriangleMesh_H

#include "SERenderingEngineLIB.h"
#include "SERenderObject.h"
#include "SERTGICamera.h"
#include "SEStructuredBuffer.h"
#include "SEGPUResource.h"
#include "SEShaderUniform.h"
#include "SEAxisAlignedBox3.h"
#include "SEObjMetaMesh.h"
#include "SEVector2.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 09/28/2013
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SETriangleMesh : public SERenderObject
{
public:
	SETriangleMesh(SEMaterial* material, SERTGICamera* camera);
	virtual ~SETriangleMesh();

	// Implement base class interface.
    virtual int GetVoxelizerRasterDimension(SEVoxelizer* voxelizer);
	virtual void Render(int technique, int pass, SESubRenderer* subRenderer = 0);
    virtual void OnRender(SERenderPass* pass, SERenderPassInfo* renderPassInfo);
	virtual void OnUpdateShaderConstants(int technique, int pass);
	virtual void OnEnableBuffers();
	virtual void OnDisableBuffers();

	// Load data from a ".ply" file and create its VBO and IBO.
	bool LoadFromPLYFile(const std::string& fileName);

    // Load data from a ".obj" file and create its VBO and IBO.
    bool LoadFromOBJFile(const std::string& fileName, bool centering = true);

	bool LoadFromMemory(std::vector<SEVector3f>& _vData,
						std::vector<unsigned int>& _iData,
						std::vector<SEVector3f>& _nData);

	bool LoadFromSEMetaMesh(Swing::SEObjMetaMesh* metaMesh);

	virtual void OnLoadFromFile();

	// Should be called after calling LoadFromFile or LoadFromMemory.
    void CreateDeviceResource(SEGPUDevice* device);
	virtual void OnGetShaderConstants();

	void GenerateNormals();
	virtual void OnGenerateNormals();

	void SetTCoord(int i, const SEVector2f& texCoord);
	SEVector2f GetTCoord(int i) const;

	int GetVertexCount() const;
	SEVector3f GetWorldVertex(int i) const;

    int GetFaceCount() const;
	
	SEAxisAlignedBox3f GetModelSpaceBB() const;
	SEAxisAlignedBox3f GetWorldSpaceBB() const;

    float GetModelSpaceTriangleMaxEdgeLength() const;

	void UpdateModelSpaceVertices(const SEMatrix4f& trans);

    // TODO:
    // Modify SETriangleMesh to support quad mesh and change the name to Mesh.
    bool IsQuad;

    unsigned int InstanceCount;

    void SetIndirectCommandBuffer(SEStructuredBuffer* indirectCommandBuffer, 
        unsigned int commandOffset);
    bool IsIndirect() const;

protected:
	void CreateVertexBufferDeviceResource(SEGPUDevice* device);
    void CreateIndexBufferDeviceResource(SEGPUDevice* device);

    // Adjust vertices based on the center of the model.
    void Centering();

    bool mIsIndirect;
    SEStructuredBufferPtr mIndirectCommandBuffer;
    SEBufferViewPtr mIndirectCommandBufferView;
    unsigned int mCommandOffset;

	SEAxisAlignedBox3f mModelSpaceBB;
    float mTriangleMaxEdgeLength;

	int mVertexCount;
	int mFaceCount;
	int mVertexComponentCount;
	std::vector<SEVector3f> mVertexData;
	std::vector<SEVector2f> mTCoordData;
	std::vector<SEVector3f> mVertexNormalData;
	std::vector<SEVector3f> mFaceNormalData;
	std::vector<unsigned int> mIndexData;
	bool mHasTCoord;
	bool mHasNormal;

    SEShaderUniform mWorldLoc;
    SEShaderUniform mViewLoc;
    SEShaderUniform mProjLoc;
};

typedef SESmartPointer<SETriangleMesh> SETriangleMeshPtr; 

}

#endif