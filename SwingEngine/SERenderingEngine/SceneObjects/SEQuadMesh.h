// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_QuadMesh_H
#define Swing_QuadMesh_H

#include "SERenderingEngineLIB.h"
#include "SERenderObject.h"
#include "SERTGICamera.h"
#include "SEStructuredBuffer.h"
#include "SEGPUResource.h"
#include "SEShaderUniform.h"
#include "SEAxisAlignedBox3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 02/03/2015
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEQuadMesh : public SERenderObject
{
public:
    SEQuadMesh(SEMaterial* material, SERTGICamera* camera);
    virtual ~SEQuadMesh();

	// Implement base class interface.
	virtual void Render(int technique, int pass, SESubRenderer* subRenderer = 0);
    virtual void OnRender(SERenderPass* pass, SEPassInfo* passInfo);
	virtual void OnUpdateShaderConstants(int technique, int pass);
	virtual void OnEnableBuffers();
	virtual void OnDisableBuffers();

    // Load data from user specified memory location.
    void LoadFromSystemMemory(std::vector<SEVector3f>& vertexData, 
        std::vector<unsigned int>& indexData);

	// Should be called after calling LoadFromSystemMemory.
    void CreateDeviceResource(SEGPUDevice* device);
	virtual void OnGetShaderConstants();

	int GetVertexCount() const;
	SEVector3f GetWorldVertex(int i) const;
	
	SEAxisAlignedBox3f GetModelSpaceBB() const;
	SEAxisAlignedBox3f GetWorldSpaceBB() const;

	void UpdateModelSpaceVertices(const SEMatrix4f& trans);

	SEMatrix4f Offset;

    void SetIndirectCommandBuffer(SEStructuredBuffer* indirectCommandBuffer, 
        unsigned int commandOffset);
    bool IsIndirect() const;

protected:
    void CreateVertexBufferDeviceResource(SEGPUDevice* device);
    void CreateIndexBufferDeviceResource(SEGPUDevice* device);

    bool mIsIndirect;
    SEStructuredBufferPtr mIndirectCommandBuffer;
    SEBufferViewPtr mIndirectCommandBufferView;
    unsigned int mCommandOffset;

	SEAxisAlignedBox3f mModelSpaceBB;

	int mVertexCount;
	int mFaceCount;
	int mVertexComponentCount;
	std::vector<SEVector3f> mVertexData;
	std::vector<unsigned int> mIndexData;

    SEShaderUniform mWorldLoc;
    SEShaderUniform mViewLoc;
    SEShaderUniform mProjLoc;
};

typedef SESmartPointer<SEQuadMesh> SEQuadMeshPtr;

}

#endif