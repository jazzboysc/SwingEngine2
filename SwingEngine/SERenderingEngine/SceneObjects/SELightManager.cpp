// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERenderingEnginePCH.h"
#include "SELightManager.h"
#include "SELightMesh.h"

using namespace Swing;

//----------------------------------------------------------------------------
SELightManager::SELightManager(SEGPUDevice* device)
    :
    mDevice(device),
    mLightBufferBindingPoint(0)
{
    size_t bufferSize = sizeof(SELightBufferHead) + 
        sizeof(SESceneLight)*MAX_LIGHT_COUNT;
    mLightBufferCache = SE_NEW unsigned char[bufferSize];
    memset(mLightBufferCache, 0, bufferSize);

    // TODO:
    //SEShaderProgramInfo lightMeshProgramInfo;
    //lightMeshProgramInfo.VShaderFileName = "VPLviaSVOGI/vLightMesh.glsl";
    //lightMeshProgramInfo.FShaderFileName = "VPLviaSVOGI/fLightMesh.glsl";
    //lightMeshProgramInfo.ShaderStageFlag = SEShaderStage::SS_Vertex |
    //                                       SEShaderStage::SS_Fragment;
    //SERenderPass* passLightMesh = SE_NEW SERenderPass(lightMeshProgramInfo);
    //SETechnique* techLightMesh = SE_NEW SETechnique();
    //techLightMesh->AddPass(passLightMesh);
    //mMtLightMesh = SE_NEW SEMaterialTemplate();
    //mMtLightMesh->AddTechnique(techLightMesh);
}
//----------------------------------------------------------------------------
SELightManager::~SELightManager()
{
    for( int i = 0; i < (int)mPointLights.size(); ++i )
    {
        mPointLights[i] = 0;
    }

    for( int i = 0; i < (int)mSpotLights.size(); ++i )
    {
        mSpotLights[i] = 0;
    }

    SE_DELETE[] mLightBufferCache;
    mLightBuffer = 0;

    mMtLightMesh = 0;
}
//----------------------------------------------------------------------------
SERTGILight* SELightManager::CreatePointLight(SELightProjectorDesc* projectorDesc,
    SERTGICamera* lightMeshCamera, SEPointLightDesc* pointLightDesc)
{
    SE_ASSERT(projectorDesc && pointLightDesc);

    SEMaterial* lightMeshMaterial = SE_NEW SEMaterial(mMtLightMesh);
    SELightMesh* lightMesh = SE_NEW SELightMesh(lightMeshMaterial, lightMeshCamera);
    lightMesh->LoadFromPLYFile("square.ply");
    SEMatrix4f lightMeshScale;
    lightMeshScale.Identity();
    lightMeshScale[0][0] = 0.05f;
    lightMeshScale[1][1] = 0.05f;
    lightMeshScale[2][2] = 0.05f;
    lightMesh->UpdateModelSpaceVertices(lightMeshScale);
    // TODO:
    //SEMatrix4f lightRotM = rotate(mat4(), radians(90.0f), vec3(1, 0, 0));
    SEMatrix4f lightRotM;
    lightRotM.Identity();
    lightMesh->SetWorldTransform(lightRotM);
    lightMesh->SetTCoord(0, SEVector2f(0.0f, 0.0f));
    lightMesh->SetTCoord(1, SEVector2f(1.0f, 0.0f));
    lightMesh->SetTCoord(2, SEVector2f(1.0f, 1.0f));
    lightMesh->SetTCoord(3, SEVector2f(0.0f, 1.0f));
    lightMesh->CreateDeviceResource(mDevice);

    lightMesh->LightMeshTexture = SE_NEW SETexture2D();
    lightMesh->LightMeshTexture->LoadPNGFromFile(mDevice, 
        "Textures/pointLight.png");

    SERTGICamera* lightProjector = SE_NEW SERTGICamera();
    lightProjector->SetPerspectiveFrustum(projectorDesc->UpFovDegrees,
        projectorDesc->AspectRatio, projectorDesc->NearPlane, 
        projectorDesc->FarPlane);
    lightProjector->SetLookAt(projectorDesc->Location, projectorDesc->LookAt, 
        projectorDesc->Up);

    SERTGILight* light = SE_NEW SERTGILight(LT_Point);
    light->Intensity = pointLightDesc->Intensity;
    light->SetProjector(lightProjector);
    light->SetLightMesh(lightMesh);
    lightMesh->SetWorldTranslation(lightProjector->GetLocation());

    AddPointLight(light);
    return light;
}
//----------------------------------------------------------------------------
SERTGILight* SELightManager::CreateSpotLight(SELightProjectorDesc* projectorDesc,
    SERTGICamera*, SESpotLightDesc* spotLightDesc)
{
    SE_ASSERT(projectorDesc && spotLightDesc);

    SERTGICamera* lightProjector = SE_NEW SERTGICamera();
    lightProjector->SetPerspectiveFrustum(projectorDesc->UpFovDegrees,
        projectorDesc->AspectRatio, projectorDesc->NearPlane,
        projectorDesc->FarPlane);
    lightProjector->SetLookAt(projectorDesc->Location, projectorDesc->LookAt,
        projectorDesc->Up);

    SERTGILight* light = SE_NEW SERTGILight(LT_Spot);
    light->Intensity = spotLightDesc->Intensity;
    light->SetProjector(lightProjector);

    SEVector3f dir = lightProjector->GetDirection();
    light->mParams1.X = dir.X;
    light->mParams1.Y = dir.Y;
    light->mParams1.Z = dir.Z;
    light->mParams1.W = spotLightDesc->CosCutoff;
    light->mParams2.X = spotLightDesc->SpotExponent;
    light->mParams2.Y = spotLightDesc->ConstantAttenuation;
    light->mParams2.Z = spotLightDesc->QuadraticAttenuation;
    light->mParams2.W = spotLightDesc->InnerCosCutoff;

    AddSpotLight(light);
    return light;
}
//----------------------------------------------------------------------------
void SELightManager::AddPointLight(SERTGILight* light)
{
    SE_ASSERT((mPointLights.size() + mSpotLights.size()) < MAX_LIGHT_COUNT);
    mPointLights.push_back(light);
}
//----------------------------------------------------------------------------
SERTGILight* SELightManager::GetPointLight(int i) const
{
    SE_ASSERT(i >= 0 && i < (int)mPointLights.size());
    return mPointLights[i];
}
//----------------------------------------------------------------------------
int SELightManager::GetPointLightCount() const
{
    return mPointLights.size();
}
//----------------------------------------------------------------------------
void SELightManager::AddSpotLight(SERTGILight* light)
{
    SE_ASSERT((mPointLights.size() + mSpotLights.size()) < MAX_LIGHT_COUNT);
    mSpotLights.push_back(light);
}
//----------------------------------------------------------------------------
SERTGILight* SELightManager::GetSpotLight(int i) const
{
    SE_ASSERT(i >= 0 && i < (int)mSpotLights.size());
    return mSpotLights[i];
}
//----------------------------------------------------------------------------
int SELightManager::GetSpotLightCount() const
{
    return mSpotLights.size();
}
//----------------------------------------------------------------------------
void SELightManager::CreateLightBuffer(SEGPUDevice* device)
{
    if( mPointLights.size() > 0 || mSpotLights.size() > 0 )
    {
        mLightBuffer = SE_NEW SEUniformBuffer();
        size_t bufferSize = sizeof(SELightBufferHead) + 
            sizeof(SESceneLight)*MAX_LIGHT_COUNT;
        mLightBuffer->ReserveMutableDeviceResource(device, bufferSize,
            BU_Dynamic_Copy);

        UpdateLightBuffer();
    }
}
//----------------------------------------------------------------------------
void SELightManager::UpdateLightBuffer()
{
    SELightBufferHead* info = (SELightBufferHead*)mLightBufferCache;
    info->PointLightCount = (unsigned int)mPointLights.size();
    info->SpotLightCount = (unsigned int)mSpotLights.size();

	if( (info->PointLightCount + info->SpotLightCount) == 0 )
	{
		return;
	}

    SESceneLight* curLight = (SESceneLight*)(mLightBufferCache + 
        sizeof(SELightBufferHead));
    for( int i = 0; i < (int)mPointLights.size(); ++i )
    {
        mPointLights[i]->OnUpdateLightBufferCache(curLight);
        curLight++;
    }
    for( int i = 0; i < (int)mSpotLights.size(); ++i )
    {
        mSpotLights[i]->OnUpdateLightBufferCache(curLight);
        curLight++;
    }

    size_t bufferSize = sizeof(SELightBufferHead) + 
        sizeof(SESceneLight)*(info->PointLightCount + info->SpotLightCount);
    mLightBuffer->UpdateSubData(mLightBufferBindingPoint, 0, bufferSize,
        (void*)mLightBufferCache);
}
//----------------------------------------------------------------------------
void SELightManager::SetLightBufferBindingPoint(unsigned int bindingPoint)
{
    mLightBufferBindingPoint = bindingPoint;
}
//----------------------------------------------------------------------------
unsigned int SELightManager::GetLightBufferBindingPoint() const
{
    return mLightBufferBindingPoint;
}
//----------------------------------------------------------------------------
void SELightManager::RenderLightMesh(int technique, int pass, 
    SESubRenderer* subRenderer)
{
    for( int i = 0; i < (int)mPointLights.size(); ++i )
    {
        mPointLights[i]->RenderLightMesh(technique, pass, subRenderer);
    }
    for( int i = 0; i < (int)mSpotLights.size(); ++i )
    {
        mSpotLights[i]->RenderLightMesh(technique, pass, subRenderer);
    }
}
//----------------------------------------------------------------------------