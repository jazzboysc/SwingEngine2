// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERenderingEnginePCH.h"
#include "SETriangleMesh.h"
#include "SEGeometryAttributes.h"
#include "SEGPUDevice.h"
#include "SEVoxelizer.h"

#include <fstream>
#include <sstream>

using namespace Swing;
using namespace Swing;

//----------------------------------------------------------------------------
SETriangleMesh::SETriangleMesh(SEMaterial* material, SERTGICamera* camera)
	:
	SERenderObject(material),
	mVertexCount(0),
	mFaceCount(0),
	mVertexComponentCount(0),
	mHasTCoord(false),
	mHasNormal(false)
{
    mTriangleMaxEdgeLength = 0.0f;
    IsQuad = false;
    InstanceCount = 1;
    mIsIndirect = false;
    mCommandOffset = 0;
    SetCamera(camera);

    // Create delegates.
    mSpatialInfo = SE_NEW SESpatialInfo();
    mRenderCache = SE_NEW SERenderCache();
    SERenderCache::SECacheFlag flag = SERenderCache::SECacheFlag(
        SERenderCache::SECacheFlag::CF_WorldTransform);
    mRenderCache->SetCacheFlag(flag);

    mPrimitive->Type = PT_Triangles;
}
//----------------------------------------------------------------------------
SETriangleMesh::~SETriangleMesh()
{
    mIndirectCommandBuffer = 0;
    mIndirectCommandBufferView = 0;
}
//----------------------------------------------------------------------------
int SETriangleMesh::GetVoxelizerRasterDimension(SEVoxelizer* voxelizer)
{
    float triangleDim = GetModelSpaceTriangleMaxEdgeLength();
    SEVector3f scale = mSpatialInfo->GetWorldScale();
    float maxScale = SE_MAX(SE_MAX(scale.X, scale.Y), scale.Z);
    triangleDim *= maxScale;
    float ratio = triangleDim / voxelizer->GetSceneBBMaxLength();
    int rasterizerDim = (int)ceilf(ratio * (float)voxelizer->VoxelGridDim) +
        voxelizer->RasterizerDimBias;

    return rasterizerDim;
}
//----------------------------------------------------------------------------
void SETriangleMesh::Render(int technique, int pass, SESubRenderer*)
{
	// Apply current rendering pass.
	mMaterial->Apply(technique, pass);
}
//----------------------------------------------------------------------------
void SETriangleMesh::OnEnableBuffers()
{
}
//----------------------------------------------------------------------------
void SETriangleMesh::OnDisableBuffers()
{
}
//----------------------------------------------------------------------------
void SETriangleMesh::OnRender(SERenderPass* pass, SERenderPassInfo* renderPassInfo)
{
    bool enableTessellation = pass->IsTessellationEnabled();
    // TODO:
    // Hack device for now.
    SEGPUDevice* device = (SEGPUDevice*)renderPassInfo->GetPassInfoHandle()->DeviceBase;
    SE_ASSERT(device);

    if( enableTessellation )
    {
        device->SetPatchVertices(3);
        device->DrawIndexedPrimitive(PT_Patches, (int)mIndexData.size());
    }
    else
    {
        if( !IsQuad )
        {
            if( !mIsIndirect )
            {
                if( InstanceCount == 1 )
                {
                    device->DrawIndexedPrimitive(PT_Triangles, (int)mIndexData.size());
                }
                else
                {
                    // TODO:
                }
            }
            else
            {
                SE_ASSERT(mIndirectCommandBuffer != 0);
                mIndirectCommandBuffer->BindTo(mIndirectCommandBufferView);
                device->DrawIndexedPrimitiveIndirect(PT_Triangles, mCommandOffset);
            }
        }
        else
        {
            // TODO:
            // Modify SETriangleMesh to support quad mesh and change the name to Mesh.
            device->DrawPrimitiveInstanced(PT_Quads, 4, 128);
        }
    }
}
//----------------------------------------------------------------------------
void SETriangleMesh::OnUpdateShaderConstants(int technique, int pass)
{
	if( technique != 0 || pass != 0 )
	{
		SE_ASSERT(false);
	}
    
    SEMatrix4f worldTrans = mSpatialInfo->GetWorldTransform();
    mWorldLoc.SetValue(worldTrans);
	if( mCamera )
	{
		SEMatrix4f viewTrans = mCamera->GetViewTransform();
		mViewLoc.SetValue(viewTrans);

		SEMatrix4f projTrans = mCamera->GetProjectionTransform();
		mProjLoc.SetValue(projTrans);
	}
}
//----------------------------------------------------------------------------
bool SETriangleMesh::LoadFromPLYFile(const std::string& fileName)
{
	std::ifstream fileData("PLY/" + fileName);
    if( !fileData )
    {
        return false;
    }

	std::string curLine;
	std::string curValue;
	std::string::size_type uiBegin, uiEnd;

	while( !fileData.eof() )
	{
		getline(fileData, curLine);
		if( curLine.size() > 14 )
		{
			curValue = curLine.substr(0, 14);
			if( curValue == "element vertex" )
			{
				break;
			}
		}
	}

	// Get vertex count.
	uiBegin = 14;
	uiBegin = curLine.find_first_not_of(" ", uiBegin);
	uiEnd = curLine.find("\n", uiBegin);
	curValue = curLine.substr(uiBegin, uiEnd - uiBegin);
	mVertexCount = atoi(curValue.c_str());
	mVertexData.reserve(mVertexCount);

	while( !fileData.eof() )
	{
		getline(fileData, curLine);
		if( curLine.size() > 12 )
		{
			curValue = curLine.substr(0, 12);
			if( curValue == "element face" )
			{
				break;
			}
		}
	}

	// Get face count.
	uiBegin = 12;
	uiBegin = curLine.find_first_not_of(" ", uiBegin);
	uiEnd = curLine.find("\n", uiBegin);
	curValue = curLine.substr(uiBegin, uiEnd - uiBegin);
	mFaceCount = atoi(curValue.c_str());
	mIndexData.reserve(mFaceCount*3);

	while( !fileData.eof() )
	{
		getline(fileData, curLine);
		if( curLine.size() >= 10 )
		{
			curValue = curLine.substr(0, 10);
			if( curValue == "end_header" )
			{
				break;
			}
		}
	}

	// Get vertex data.
	for( int i = 0; i < mVertexCount; ++i )
	{
		SEVector3f vertex;

		getline(fileData, curLine);
		while( curLine.size() == 0 || curLine[0] == '\n' )
		{
			getline(fileData, curLine);
		}
		uiBegin = 0;
		uiBegin = curLine.find_first_not_of(" ", uiBegin);
		uiEnd = curLine.find(" ", uiBegin);
		curValue = curLine.substr(uiBegin, uiEnd - uiBegin);
		vertex.X = (float)atof(curValue.c_str());

		uiBegin = curLine.find_first_not_of(" ", uiEnd);
		uiEnd = curLine.find(" ", uiBegin);
		curValue = curLine.substr(uiBegin, uiEnd - uiBegin);
		vertex.Y = (float)atof(curValue.c_str());

		uiBegin = curLine.find_first_not_of(" ", uiEnd);
		uiEnd = curLine.find("\n", uiBegin);
		curValue = curLine.substr(uiBegin, uiEnd - uiBegin);
        // Flip the z coordinate to transform the vertex from 
        // ply's standard right-hand system to our left-hand coordinate system.
		vertex.Z = -(float)atof(curValue.c_str());

		mVertexData.push_back(vertex);

		// Update model space bounding box.
		mModelSpaceBB.Min[0] = SE_MIN(mModelSpaceBB.Min[0], vertex[0]);
		mModelSpaceBB.Min[1] = SE_MIN(mModelSpaceBB.Min[1], vertex[1]);
		mModelSpaceBB.Min[2] = SE_MIN(mModelSpaceBB.Min[2], vertex[2]);
		mModelSpaceBB.Max[0] = SE_MAX(mModelSpaceBB.Max[0], vertex[0]);
		mModelSpaceBB.Max[1] = SE_MAX(mModelSpaceBB.Max[1], vertex[1]);
		mModelSpaceBB.Max[2] = SE_MAX(mModelSpaceBB.Max[2], vertex[2]);
	}

	// Get index data.
	for( int i = 0; i < mFaceCount; ++i )
	{
		unsigned int indexCount, i0, i1, i2;

		getline(fileData, curLine);
		while( curLine.size() == 0 || curLine[0] == '\n' )
		{
			getline(fileData, curLine);
		}
		uiBegin = 0;
		uiBegin = curLine.find_first_not_of(" ", uiBegin);
		uiEnd = curLine.find(" ", uiBegin);
		curValue = curLine.substr(uiBegin, uiEnd - uiBegin);
        indexCount = (unsigned int)atoi(curValue.c_str());
		// TODO:
		// Only support trangle for now.
        SE_ASSERT(indexCount == 3);

		uiBegin = curLine.find_first_not_of(" ", uiEnd);
		uiEnd = curLine.find(" ", uiBegin);
		curValue = curLine.substr(uiBegin, uiEnd - uiBegin);
		i0 = (unsigned int)atof(curValue.c_str());

		uiBegin = curLine.find_first_not_of(" ", uiEnd);
		uiEnd = curLine.find(" ", uiBegin);
		curValue = curLine.substr(uiBegin, uiEnd - uiBegin);
		i1 = (unsigned int)atof(curValue.c_str());

		uiBegin = curLine.find_first_not_of(" ", uiEnd);
		uiEnd = curLine.find("\n", uiBegin);
		curValue = curLine.substr(uiBegin, uiEnd - uiBegin);
		i2 = (unsigned int)atof(curValue.c_str());

        // ply uses CCW indices. Flip to CW indices.
		mIndexData.push_back(i0);
        mIndexData.push_back(i2);
        mIndexData.push_back(i1);
	}

    fileData.close();

    Centering();
	OnLoadFromFile();
    
#ifdef RTGI_OUTPUT_MESH_RESOURCE_LOADING
    SETerminal::Output(SETerminal::OC_Success, "Loading mesh %s finished\n", 
        fileName.c_str());
#endif

	return true;
}
//----------------------------------------------------------------------------
bool SETriangleMesh::LoadFromOBJFile(const std::string& fileName, 
    bool centering)
{
    std::ifstream fileData("OBJ/" + fileName);
    if( !fileData )
    {
        return false;
    }

    std::string curLine;
    std::stringstream lineStream;
    std::string token;
    std::vector<std::string> tokens;

    while( getline(fileData, curLine) )
    {
        lineStream.clear();
        lineStream.str(curLine);
        tokens.clear();

        while( lineStream >> token )
        {
            tokens.push_back(token);
        }

        if( !tokens.empty() )
        {
            // Process tokens.

            if( !tokens[1].compare(".obj") )
            {
                if( !tokens[4].compare("vertices,") )
                {
                    mVertexCount = atoi(tokens[3].c_str());
                    mVertexData.reserve(mVertexCount);
                }

                if( !tokens[6].compare("triangles.") )
                {
                    mFaceCount = atoi(tokens[5].c_str());
                    mIndexData.reserve(mFaceCount*3);
                }
            }

            if( !tokens[0].compare("v") )
            {
                // Process vertex.
                SE_ASSERT(tokens.size() >= 4);

                float x = (float)atof(tokens[1].c_str());
                float y = (float)atof(tokens[2].c_str());
                float z = (float)atof(tokens[3].c_str());
                mVertexData.push_back(SEVector3f(x, y, z));

                // Update model space bounding box.
                mModelSpaceBB.Min[0] = SE_MIN(mModelSpaceBB.Min[0], x);
                mModelSpaceBB.Min[1] = SE_MIN(mModelSpaceBB.Min[1], y);
                mModelSpaceBB.Min[2] = SE_MIN(mModelSpaceBB.Min[2], z);
                mModelSpaceBB.Max[0] = SE_MAX(mModelSpaceBB.Max[0], x);
                mModelSpaceBB.Max[1] = SE_MAX(mModelSpaceBB.Max[1], y);
                mModelSpaceBB.Max[2] = SE_MAX(mModelSpaceBB.Max[2], z);
            }
            else if( !tokens.at(0).compare("f") )
            {
                SE_ASSERT(tokens.size() > 3);

                // Possible layouts:
                // f v       v       v
                // f v/vt    v/vt    v/vt
                // f v//vn   v//vn   v//vn
                // f v/vt/vn v/vt/vn v/vt/vn

                // TODO:
                // Only support triangle mesh for now.

                for( int i = 1; i <= 3; ++i )
                {
                    std::string indexToken;
                    std::stringstream curVertexIndex;
                    curVertexIndex.str(tokens[i]);

                    // TODO:
                    // Only interested in vertex index for now.
                    getline(curVertexIndex, indexToken, '/');

                    // obj file's index starts from 1.
                    unsigned int index = 
                        (unsigned int)atoi(indexToken.c_str()) - 1;
                    mIndexData.push_back(index);
                }
            }
        }
    }

    fileData.close();

	if( centering )
	{
		Centering();
	}

    OnLoadFromFile();

#ifdef RTGI_OUTPUT_MESH_RESOURCE_LOADING
    SETerminal::Output(SETerminal::OC_Success, "Loading mesh %s finished\n",
        fileName.c_str());
#endif

    return true;
}
//----------------------------------------------------------------------------
bool SETriangleMesh::LoadFromMemory(std::vector<SEVector3f>& _vData, 
    std::vector<unsigned int>& _iData, std::vector<SEVector3f>& _nData)
{
	mVertexData = _vData;
	mIndexData = _iData;
	mVertexNormalData.resize(_nData.size());
	for (int i = 0, e = _nData.size(); i < e; ++i)
	{
		mVertexNormalData[i] = -_nData[i];
	}
	this->mFaceCount = mIndexData.size() / 3;
	this->mVertexCount = mVertexData.size();
	mHasNormal = true;
	// Adjust bounding box.
	for (auto vertex : mVertexData)
	{
		// Update model space bounding box.
		mModelSpaceBB.Min[0] = SE_MIN(mModelSpaceBB.Min[0], vertex.X);
		mModelSpaceBB.Min[1] = SE_MIN(mModelSpaceBB.Min[1], vertex.Y);
		mModelSpaceBB.Min[2] = SE_MIN(mModelSpaceBB.Min[2], vertex.Z);
		mModelSpaceBB.Max[0] = SE_MAX(mModelSpaceBB.Max[0], vertex.X);
		mModelSpaceBB.Max[1] = SE_MAX(mModelSpaceBB.Max[1], vertex.Y);
		mModelSpaceBB.Max[2] = SE_MAX(mModelSpaceBB.Max[2], vertex.Z);
	}

	return true;
}
//----------------------------------------------------------------------------
bool SETriangleMesh::LoadFromSEMetaMesh(Swing::SEObjMetaMesh* metaMesh)
{
	if( !metaMesh )
	{
		return false;
	}

	Name = metaMesh->MeshName;

	metaMesh->ConvertToTriangleMesh();
	metaMesh->ConvertToUnified();

	std::vector<SEVector3f>& vertexData = metaMesh->GetVertexData();
	std::vector<MetaMeshFaceIndex>& indexData = metaMesh->GetIndexData();
	mVertexData.reserve(vertexData.size());
	mIndexData.reserve(indexData.size()*3);

    // Get vertex data.
	for( int i = 0; i < (int)vertexData.size(); ++i )
	{
        // Flip the z coordinate to transform the vertex from 
        // obj's standard right-hand system to our left-hand coordinate system.
		SEVector3f v;
		v.X = vertexData[i].X;
		v.Y = vertexData[i].Y;
		v.Z = -vertexData[i].Z;
		mVertexData.push_back(v);

		// Update model space bounding box.
		mModelSpaceBB.Min[0] = SE_MIN(mModelSpaceBB.Min[0], v.X);
		mModelSpaceBB.Min[1] = SE_MIN(mModelSpaceBB.Min[1], v.Y);
		mModelSpaceBB.Min[2] = SE_MIN(mModelSpaceBB.Min[2], v.Z);
		mModelSpaceBB.Max[0] = SE_MAX(mModelSpaceBB.Max[0], v.X);
		mModelSpaceBB.Max[1] = SE_MAX(mModelSpaceBB.Max[1], v.Y);
		mModelSpaceBB.Max[2] = SE_MAX(mModelSpaceBB.Max[2], v.Z);
	}
	mVertexCount = (int)mVertexData.size();

    // Get index data.
	for( int i = 0; i < (int)indexData.size(); ++i )
	{
        // obj uses CCW indices. Flip to CW indices.
		mIndexData.push_back((unsigned int)indexData[i].VertexIndices[0]);
		mIndexData.push_back((unsigned int)indexData[i].VertexIndices[2]);
		mIndexData.push_back((unsigned int)indexData[i].VertexIndices[1]);
	}
	mFaceCount = mIndexData.size() / 3;

    // Get texture coordinates.
    if( metaMesh->HasTCoord() )
    {
        std::vector<SEVector3f>& tCoordData = metaMesh->GetTCoordData();
        mTCoordData.reserve(tCoordData.size());

        for( int i = 0; i < (int)tCoordData.size(); ++i )
        {
            SEVector2f vt;
            vt.X = tCoordData[i].X;
            vt.Y = tCoordData[i].Y;
            mTCoordData.push_back(vt);
        }

        mHasTCoord = true;
    }

    // Get vertex normals.
    if( metaMesh->HasNormal() )
    {
        std::vector<SEVector3f>& vertexNormalData = metaMesh->GetVertexNormalData();
        mVertexNormalData.reserve(vertexNormalData.size());

        for( int i = 0; i < (int)vertexNormalData.size(); ++i )
        {
            SEVector3f vn;
            vn.X = vertexNormalData[i].X;
            vn.Y = vertexNormalData[i].Y;
            vn.Z = -vertexNormalData[i].Z;
            mVertexNormalData.push_back(vn);
        }

        mHasNormal = true;
    }

    // Get material parameters.
    if( metaMesh->MeshMaterial )
    {
        mMaterial->SetDiffuseMapName(metaMesh->MeshMaterial->map_Kd);
    }

	return true;
}
//----------------------------------------------------------------------------
void SETriangleMesh::OnLoadFromFile()
{
}
//----------------------------------------------------------------------------
void SETriangleMesh::CreateDeviceResource(SEGPUDevice* device)
{
	// Create VBO and IBO.
	CreateIndexBufferDeviceResource(device);
	CreateVertexBufferDeviceResource(device);

	// Create shader programs.
	SEGeometryAttributes attributes;
    attributes.Prim = mPrimitive;
	attributes.HasNormal = mHasNormal;
	attributes.HasTCoord = mHasTCoord;
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
void SETriangleMesh::OnGetShaderConstants()
{
	SEShaderProgram* program = mMaterial->GetProgram(0, 0);
    program->GetUniformLocation(&mWorldLoc, "World");
    program->GetUniformLocation(&mViewLoc, "View");
    program->GetUniformLocation(&mProjLoc, "Proj");
}
//----------------------------------------------------------------------------
void SETriangleMesh::GenerateNormals()
{
	mVertexNormalData.clear();
	mVertexNormalData.resize(mVertexCount);

	mFaceNormalData.clear();
	mFaceNormalData.reserve(mFaceCount);
	for( int i = 0; i < mFaceCount; ++i )
	{
		unsigned int v1ID, v2ID, v3ID;

		// Get vertex indices.
		v1ID = mIndexData[i*3    ];
		v2ID = mIndexData[i*3 + 1];
		v3ID = mIndexData[i*3 + 2];

		// Get 3 vertices.
		SEVector3f& v1 = mVertexData[v1ID];
		SEVector3f& v2 = mVertexData[v2ID];
		SEVector3f& v3 = mVertexData[v3ID];

		SEVector3f e1 = v2 - v1;
		SEVector3f e2 = v3 - v1;
        SEVector3f e3 = v3 - v2;
		SEVector3f n = e1.Cross(e2);

        // Get max edge length.
        float lenE1 = e1.GetLength();
        float lenE2 = e2.GetLength();
        float lenE3 = e3.GetLength();
        float maxLen = SE_MAX(lenE1, SE_MAX(lenE2, lenE3));
        mTriangleMaxEdgeLength = SE_MAX(maxLen, mTriangleMaxEdgeLength);

		// Update vertex normals.
		mVertexNormalData[v1ID] += n;
		mVertexNormalData[v2ID] += n;
		mVertexNormalData[v3ID] += n;

		// Get face normal.
        n.Normalize();
		mFaceNormalData.push_back(n);
	}

	// Normalize vertex normals.
	for( int i = 0; i < mVertexCount; ++i )
	{
        mVertexNormalData[i].Normalize();
	}

	mHasNormal = true;

	OnGenerateNormals();
}
//----------------------------------------------------------------------------
void SETriangleMesh::OnGenerateNormals()
{
}
//----------------------------------------------------------------------------
void SETriangleMesh::SetTCoord(int i, const SEVector2f& texCoord)
{
	if( !mHasTCoord )
	{
		mTCoordData.resize(mVertexCount);
		mHasTCoord = true;
	}

	SE_ASSERT( mVertexCount > i );
	mTCoordData[i] = texCoord;
}
//----------------------------------------------------------------------------
SEVector2f SETriangleMesh::GetTCoord(int i) const
{
	SE_ASSERT( (int)mTCoordData.size() > i );
	return mTCoordData[i];
}
//----------------------------------------------------------------------------
int SETriangleMesh::GetVertexCount() const
{
	return mVertexCount;
}
//----------------------------------------------------------------------------
SEVector3f SETriangleMesh::GetWorldVertex(int i) const
{
	SE_ASSERT( (int)mVertexData.size() > i );
    SEVector4f temp = SEVector4f(mVertexData[i].X, mVertexData[i].Y, 
        mVertexData[i].Z, 1.0f);
    SEMatrix4f worldTrans = mSpatialInfo->GetWorldTransform();
	temp = worldTrans * temp;
	SEVector3f res;
	res.X = temp.X;
	res.Y = temp.Y;
	res.Z = temp.Z;
	return res;
}
//----------------------------------------------------------------------------
int SETriangleMesh::GetFaceCount() const
{
    return mFaceCount;
}
//----------------------------------------------------------------------------
SEAxisAlignedBox3f SETriangleMesh::GetModelSpaceBB() const
{
	return mModelSpaceBB;
}
//----------------------------------------------------------------------------
SEAxisAlignedBox3f SETriangleMesh::GetWorldSpaceBB() const
{
	SEAxisAlignedBox3f res;
	SEVector4f tempV;
    SEMatrix4f worldTrans = mSpatialInfo->GetWorldTransform();
	for( int i = 0; i < mVertexCount; ++i )
	{
        tempV = SEVector4f(mVertexData[i].X, mVertexData[i].Y, 
            mVertexData[i].Z, 1.0f);
        tempV = tempV * worldTrans;
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
float SETriangleMesh::GetModelSpaceTriangleMaxEdgeLength() const
{
    return mTriangleMaxEdgeLength;
}
//----------------------------------------------------------------------------
void SETriangleMesh::UpdateModelSpaceVertices(const SEMatrix4f& trans)
{
	SEVector4f tempV;
	for( int i = 0; i < mVertexCount; ++i )
	{
        tempV = SEVector4f(mVertexData[i].X, mVertexData[i].Y, 
            mVertexData[i].Z, 1.0f);
        tempV = tempV * trans;
		mVertexData[i].X = tempV.X;
		mVertexData[i].Y = tempV.Y;
		mVertexData[i].Z = tempV.Z;
	}
	// Adjust bounding box.
    tempV = SEVector4f(mModelSpaceBB.Min[0], mModelSpaceBB.Min[1], 
        mModelSpaceBB.Min[2], 1.0f);
    tempV = tempV * trans;
	mModelSpaceBB.Min[0] = tempV.X;
	mModelSpaceBB.Min[1] = tempV.Y;
	mModelSpaceBB.Min[2] = tempV.Z;
    tempV = SEVector4f(mModelSpaceBB.Max[0], mModelSpaceBB.Max[1], 
        mModelSpaceBB.Max[2], 1.0f);
    tempV = tempV * trans;
	mModelSpaceBB.Max[0] = tempV.X;
	mModelSpaceBB.Max[1] = tempV.Y;
	mModelSpaceBB.Max[2] = tempV.Z;

	GenerateNormals();
}
//----------------------------------------------------------------------------
void SETriangleMesh::CreateVertexBufferDeviceResource(SEGPUDevice* device)
{
	mVertexComponentCount = 3;
	if( mHasTCoord )
	{
		mVertexComponentCount += 2;
	}
	if( mHasNormal )
	{
		mVertexComponentCount += 3;
	}
	std::vector<float> tempVB;
	if( mVertexData.size() > 0 )
	{
		for( int i = 0; i < mVertexCount; ++i )
		{
			tempVB.push_back(mVertexData[i].X);
			tempVB.push_back(mVertexData[i].Y);
			tempVB.push_back(mVertexData[i].Z);

			if( mHasTCoord )
			{
				tempVB.push_back(mTCoordData[i].X);
				tempVB.push_back(mTCoordData[i].Y);
			}

			if( mHasNormal )
			{
				tempVB.push_back(mVertexNormalData[i].X);
				tempVB.push_back(mVertexNormalData[i].Y);
				tempVB.push_back(mVertexNormalData[i].Z);
			}
		}

        size_t bufferSize = sizeof(float)*mVertexCount*mVertexComponentCount;
        mPrimitive->VB = SE_NEW SEVertexBuffer();
        mPrimitive->VB->LoadFromSystemMemory(device, bufferSize,
            (void*)&tempVB[0], BU_Static_Draw);
	}
}
//----------------------------------------------------------------------------
void SETriangleMesh::CreateIndexBufferDeviceResource(SEGPUDevice* device)
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
void SETriangleMesh::Centering()
{
    // Adjust vertices based on the center of the model.
    SEVector3f bc = mModelSpaceBB.GetBoxCenter();
    SEMatrix4f modelTrans;
    modelTrans.Identity();
    modelTrans[3][0] = -bc.X;
    modelTrans[3][1] = -bc.Y;
    modelTrans[3][2] = -bc.Z;

    SEVector4f tempV;
    for( int i = 0; i < mVertexCount; ++i )
    {
        tempV = SEVector4f(mVertexData[i].X, mVertexData[i].Y, 
            mVertexData[i].Z, 1.0f);
        tempV = tempV * modelTrans;
        mVertexData[i].X = tempV.X;
        mVertexData[i].Y = tempV.Y;
        mVertexData[i].Z = tempV.Z;
    }
    // Adjust bounding box.
    tempV = SEVector4f(mModelSpaceBB.Min[0], mModelSpaceBB.Min[1], 
        mModelSpaceBB.Min[2], 1.0f);
    tempV = tempV * modelTrans;
    mModelSpaceBB.Min[0] = tempV.X;
    mModelSpaceBB.Min[1] = tempV.Y;
    mModelSpaceBB.Min[2] = tempV.Z;
    tempV = SEVector4f(mModelSpaceBB.Max[0], mModelSpaceBB.Max[1], 
        mModelSpaceBB.Max[2], 1.0f);
    tempV = tempV * modelTrans;
    mModelSpaceBB.Max[0] = tempV.X;
    mModelSpaceBB.Max[1] = tempV.Y;
    mModelSpaceBB.Max[2] = tempV.Z;
}
//----------------------------------------------------------------------------
void SETriangleMesh::SetIndirectCommandBuffer(
    SEStructuredBuffer* indirectCommandBuffer, unsigned int commandOffset)
{
    mIsIndirect = true;
    mIndirectCommandBuffer = indirectCommandBuffer;
    mCommandOffset = commandOffset;
}
//----------------------------------------------------------------------------
bool SETriangleMesh::IsIndirect() const
{
    return mIsIndirect;
}
//----------------------------------------------------------------------------