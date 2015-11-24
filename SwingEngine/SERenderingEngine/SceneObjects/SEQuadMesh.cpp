// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SEQuadMesh.h"
#include "SEGeometryAttributes.h"
#include "SEGPUDevice.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEQuadMesh::SEQuadMesh(SEMaterial* material, SERTGICamera* camera)
	:
	SERenderObject(material),
	mVertexCount(0),
	mFaceCount(0),
	mVertexComponentCount(0)
{
    mIsIndirect = false;
    mCommandOffset = 0;
    SetCamera(camera);
    mSpatialInfo = SE_NEW SESpatialInfo();

    mPrimitive->Type = PT_Quads;
}
//----------------------------------------------------------------------------
SEQuadMesh::~SEQuadMesh()
{
    mIndirectCommandBuffer = 0;
    mIndirectCommandBufferView = 0;
}
//----------------------------------------------------------------------------
void SEQuadMesh::Render(int technique, int pass, SESubRenderer*)
{
	// Apply current rendering pass.
	mMaterial->Apply(technique, pass);
}
//----------------------------------------------------------------------------
void SEQuadMesh::OnEnableBuffers()
{
}
//----------------------------------------------------------------------------
void SEQuadMesh::OnDisableBuffers()
{
}
//----------------------------------------------------------------------------
void SEQuadMesh::OnRender(SERenderPass* pass, SERenderPassInfo* renderPassInfo)
{
    bool enableTessellation = pass->IsTessellationEnabled();
    // TODO:
    // Hack device for now.
    SEGPUDevice* device = (SEGPUDevice*)renderPassInfo->GetPassInfoHandle()->DeviceBase;
    SE_ASSERT(device);

    if( enableTessellation )
    {
        // TODO:
        // Check this later.
        SE_ASSERT(false);
        device->SetPatchVertices(4);
        device->DrawIndexedPrimitive(PT_Patches, (int)mIndexData.size());
    }
    else
    {
        if( !mIsIndirect )
        {
            device->DrawIndexedPrimitive(PT_Quads, (int)mIndexData.size());
        }
        else
        {
            SE_ASSERT(mIndirectCommandBuffer != 0);
            mIndirectCommandBuffer->BindTo(mIndirectCommandBufferView);
            device->DrawIndexedPrimitiveIndirect(PT_Quads, mCommandOffset);
        }
    }
}
//----------------------------------------------------------------------------
void SEQuadMesh::OnUpdateShaderConstants(int technique, int pass)
{
	if( technique != 0 || pass != 0 )
	{
		SE_ASSERT(false);
	}
    
    mWorldLoc.SetValue(mSpatialInfo->GetWorldTransform());
	if( mCamera )
	{
		SEMatrix4f viewTrans = mCamera->GetViewTransform();
        mViewLoc.SetValue(viewTrans);

		SEMatrix4f projTrans = mCamera->GetProjectionTransform();
        mProjLoc.SetValue(projTrans);
	}
}
//----------------------------------------------------------------------------
void SEQuadMesh::LoadFromSystemMemory(std::vector<SEVector3f>& vertexData,
    std::vector<unsigned int>& indexData)
{
    mVertexCount = (int)vertexData.size();
    mFaceCount = (int)(indexData.size() >> 2);
    mVertexComponentCount = 3;
    mVertexData = vertexData;
    mIndexData = indexData;
}
//----------------------------------------------------------------------------
void SEQuadMesh::CreateDeviceResource(SEGPUDevice* device)
{
	// Create VBO and IBO.
	CreateIndexBufferDeviceResource(device);
	CreateVertexBufferDeviceResource(device);

	// Create shader programs.
	SEGeometryAttributes attributes;
    attributes.Prim = mPrimitive;
	attributes.HasNormal = false;
	attributes.HasTCoord = false;
	attributes.VertexComponentCount = mVertexComponentCount;
	mMaterial->CreateDeviceResource(device, &attributes);

	// Get shader constants here.
	OnGetShaderConstants();

    if( mIsIndirect )
    {
        SE_ASSERT(mIndirectCommandBuffer);

        SEBufferViewDesc viewDesc;
        viewDesc.Type = BT_DrawIndirect;
        mIndirectCommandBufferView = SE_NEW SEBufferView(viewDesc);
        mIndirectCommandBufferView->CreateDeviceResource(device, 
            mIndirectCommandBuffer);

        mIndirectCommandBuffer->Bind();
        char* bufferData = (char*)mIndirectCommandBuffer->Map(BA_Write_Only);
        SE_ASSERT( bufferData );
        SEDrawElementsIndirectCommand* commandBufferData = 
            (SEDrawElementsIndirectCommand*)(bufferData + mCommandOffset);
        commandBufferData->Count = (unsigned int)mIndexData.size();
        commandBufferData->PrimCount = 0;
        commandBufferData->FirstIndex = 0;
        commandBufferData->BaseVertex = 0;
        commandBufferData->BaseInstance = 0;
        mIndirectCommandBuffer->Unmap();
    }
}
//----------------------------------------------------------------------------
void SEQuadMesh::OnGetShaderConstants()
{
	SEShaderProgram* program = mMaterial->GetProgram(0, 0);
    program->GetUniformLocation(&mWorldLoc, "World");
    program->GetUniformLocation(&mViewLoc, "View");
    program->GetUniformLocation(&mProjLoc, "Proj");
}
//----------------------------------------------------------------------------
int SEQuadMesh::GetVertexCount() const
{
	return mVertexCount;
}
//----------------------------------------------------------------------------
SEVector3f SEQuadMesh::GetWorldVertex(int i) const
{
	SE_ASSERT( (int)mVertexData.size() > i );
    SEVector4f temp = SEVector4f(mVertexData[i].X, mVertexData[i].Y, mVertexData[i].Z, 1.0f);
    SEMatrix4f worldTrans = mSpatialInfo->GetWorldTransform();
	temp = worldTrans * temp;
	SEVector3f res;
	res.X = temp.X;
	res.Y = temp.Y;
	res.Z = temp.Z;
	return res;
}
//----------------------------------------------------------------------------
SEAxisAlignedBox3f SEQuadMesh::GetModelSpaceBB() const
{
	return mModelSpaceBB;
}
//----------------------------------------------------------------------------
SEAxisAlignedBox3f SEQuadMesh::GetWorldSpaceBB() const
{
	SEAxisAlignedBox3f res;
	SEVector4f tempV;
    SEMatrix4f worldTrans = mSpatialInfo->GetWorldTransform();
	for( int i = 0; i < mVertexCount; ++i )
	{
        tempV = SEVector4f(mVertexData[i].X, mVertexData[i].Y, mVertexData[i].Z, 1.0f);
		tempV = worldTrans * tempV;
		res.Min[0] = SE_MIN(res.Min[0], tempV.X);
		res.Min[1] = SE_MIN(res.Min[1], tempV.Y);
		res.Min[2] = SE_MIN(res.Min[2], tempV.Z);
		res.Max[0] = SE_MAX(res.Max[0], tempV.X);
		res.Max[1] = SE_MAX(res.Max[1], tempV.Y);
		res.Max[2] = SE_MAX(res.Max[2], tempV.Z);
	}
	return res;
}
//----------------------------------------------------------------------------
void SEQuadMesh::UpdateModelSpaceVertices(const SEMatrix4f& trans)
{
	SEVector4f tempV;
	for( int i = 0; i < mVertexCount; ++i )
	{
        tempV = SEVector4f(mVertexData[i].X, mVertexData[i].Y, mVertexData[i].Z, 1.0f);
		tempV = trans * tempV;
		mVertexData[i].X = tempV.X;
		mVertexData[i].Y = tempV.Y;
		mVertexData[i].Z = tempV.Z;
	}
	// Adjust bounding box.
    tempV = SEVector4f(mModelSpaceBB.Min[0], mModelSpaceBB.Min[1], mModelSpaceBB.Min[2], 1.0f);
	tempV = trans * tempV;
	mModelSpaceBB.Min[0] = tempV.X;
	mModelSpaceBB.Min[1] = tempV.Y;
	mModelSpaceBB.Min[2] = tempV.Z;
    tempV = SEVector4f(mModelSpaceBB.Max[0], mModelSpaceBB.Max[1], mModelSpaceBB.Max[2], 1.0f);
	tempV = trans * tempV;
	mModelSpaceBB.Max[0] = tempV.X;
	mModelSpaceBB.Max[1] = tempV.Y;
	mModelSpaceBB.Max[2] = tempV.Z;
}
//----------------------------------------------------------------------------
void SEQuadMesh::CreateVertexBufferDeviceResource(SEGPUDevice* device)
{
    size_t bufferSize = sizeof(float) * mVertexCount * mVertexComponentCount;
    mPrimitive->VB = SE_NEW SEVertexBuffer();
    if( mVertexData.size() > 0 )
    {
        mPrimitive->VB->LoadFromSystemMemory(device, bufferSize,
            (void*)&mVertexData[0], BU_Static_Draw);
    }
    else
    {
        mPrimitive->VB->ReserveImmutableDeviceResource(device, bufferSize);
    }
}
//----------------------------------------------------------------------------
void SEQuadMesh::CreateIndexBufferDeviceResource(SEGPUDevice* device)
{
	SE_ASSERT( mIndexData.size() > 0 );
	if( mIndexData.size() > 0 )
	{
        size_t bufferSize = sizeof(unsigned int)*mIndexData.size();
        mPrimitive->IB = SE_NEW SEIndexBuffer();
        mPrimitive->IB->LoadFromSystemMemory(device, bufferSize,
            (void*)&mIndexData[0], BU_Static_Draw);
	}
}
//----------------------------------------------------------------------------
void SEQuadMesh::SetIndirectCommandBuffer(
    SEStructuredBuffer* indirectCommandBuffer, unsigned int commandOffset)
{
    mIsIndirect = true;
    mIndirectCommandBuffer = indirectCommandBuffer;
    mCommandOffset = commandOffset;
}
//----------------------------------------------------------------------------
bool SEQuadMesh::IsIndirect() const
{
    return mIsIndirect;
}
//----------------------------------------------------------------------------