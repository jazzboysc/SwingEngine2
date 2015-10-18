#include "VPLviaSVOGI.pch.h"
#include "SceneMesh.h"
#include "VPLviaSVOGIApp.h"
#include "SEGPUDevice.h"

using namespace Swing;

//----------------------------------------------------------------------------
SceneMesh::SceneMesh(SEMaterial* material, SERTGICamera* camera)
	:
	SETriangleMesh(material, camera)
{
    MaterialColor = SEVector3f(0.75f, 0.75f, 0.75f);
    TessLevel = 14.0f;
}
//----------------------------------------------------------------------------
SceneMesh::~SceneMesh()
{
}
//----------------------------------------------------------------------------
void SceneMesh::OnGetShaderConstants()
{
    SEShaderProgram* program = 0;

    // Get scene voxelization pass uniform locations.
    program = mMaterial->GetProgram(0, VPLviaSVOGI::SMP_Voxelization);
    program->GetUniformLocation(&mWorldLocSV, "World");
    program->GetUniformLocation(&mMaterialColorLocSV, "MaterialColor");
    // No need to do these for SVO voxelizer, but it's OK.
    program->GetUniformLocation(&mSceneBBCenterLocSV, "SceneBBCenter");
    program->GetUniformLocation(&mSceneBBExtensionLocSV, "SceneBBExtension");
    program->GetUniformLocation(&mDimLocSV, "dim");
    program->GetUniformLocation(&mInv2SceneBBExtensionLocSV, "Inv2SceneBBExtension");

	// Get point light shadow map pass uniform locations.
    program = mMaterial->GetProgram(0, VPLviaSVOGI::SMP_PointLightShadowMap);
    program->GetUniformLocation(&mWorldLocPointLightShadowMap, "World");
    program->GetUniformLocation(&mTessLevelLoc, "TessLevel");

    // Get spot light shadow map pass uniform locations.
    program = mMaterial->GetProgram(0, VPLviaSVOGI::SMP_SpotLightShadowMap);
    program->GetUniformLocation(&mWorldLocSpotLightShadowMap, "World");

    // Get G-buffer pass uniform locations.
    program = mMaterial->GetProgram(0, VPLviaSVOGI::SMP_GBuffer);
    program->GetUniformLocation(&mWorldLocGBuffer, "World");
    program->GetUniformLocation(&mViewLocGBuffer, "View");
    program->GetUniformLocation(&mProjLocGBuffer, "Proj");
    program->GetUniformLocation(&mWorldCacheLocGBuffer, "WorldCache");
    program->GetUniformLocation(&mViewCacheLocGBuffer, "ViewCache");
    program->GetUniformLocation(&mProjCacheLocGBuffer, "ProjCache");
    program->GetUniformLocation(&mMaterialColorLocGBuffer, "MaterialColor");
    program->GetUniformLocation(&mDiffuseMapLocGBuffer, "DiffuseMapSampler");
    program->GetUniformLocation(&mUseDiffuseMapLocGBuffer, "UseDiffuseMap");

    // Get point light RSM pass uniform locations.
    program = mMaterial->GetProgram(0, VPLviaSVOGI::SMP_PointLightRSM);
    program->GetUniformLocation(&mWorldLocPointLightRSM, "World");
    program->GetUniformLocation(&mMaterialColorLocPointLightRSM, "MaterialColor");
    program->GetUniformLocation(&mDiffuseMapLocPointLightRSM, "DiffuseMapSampler");
    program->GetUniformLocation(&mUseDiffuseMapLocPointLightRSM, "UseDiffuseMap");

    // Get spot light RSM pass uniform locations.
    program = mMaterial->GetProgram(0, VPLviaSVOGI::SMP_SpotLightRSM);
    program->GetUniformLocation(&mWorldLocSpotLightRSM, "World");
    program->GetUniformLocation(&mMaterialColorLocSpotLightRSM, "MaterialColor");
    program->GetUniformLocation(&mDiffuseMapLocSpotLightRSM, "DiffuseMapSampler");
    program->GetUniformLocation(&mUseDiffuseMapLocSpotLightRSM, "UseDiffuseMap");
}
//----------------------------------------------------------------------------
void SceneMesh::OnUpdateShaderConstants(int, int pass)
{
    SEMatrix4f worldTrans = mSpatialInfo->GetWorldTransform();

    SETexture2D* diffuseMap = mMaterial->GetDiffuseMap();
    SESamplerDesc sd;
    sd.MinFilter = FT_Linear_Linear;
    sd.MagFilter = FT_Linear;
    sd.MipMap = FT_Linear_Linear;
    sd.WrapS = WT_Repeat;
    sd.WrapT = WT_Repeat;

    // Update scene voxelization pass uniform data.
    if( pass == VPLviaSVOGI::SMP_Voxelization )
    {
        mWorldLocSV.SetValue(worldTrans);
        mMaterialColorLocSV.SetValue(MaterialColor);

        // No need to do these for SVO voxelizer, but it's OK.

        SEVector3f center = SceneBB->GetBoxCenter();
        SEVector3f extension = SceneBB->GetExtension();
        SEVector3f inv2extension = SEVector3f(1.0f / (2.0f*extension.X), 1.0f / (2.0f*extension.Y), 1.0f / (2.0f*extension.Z));

        mSceneBBCenterLocSV.SetValue(center);
        mSceneBBExtensionLocSV.SetValue(extension);
        mDimLocSV.SetValue(VPLviaSVOGI::VOXEL_DIMENSION);
        mInv2SceneBBExtensionLocSV.SetValue(inv2extension);
    }

    // Update point light shadow map pass uniform data.
    if( pass == VPLviaSVOGI::SMP_PointLightShadowMap )
    {
        mWorldLocPointLightShadowMap.SetValue(worldTrans);
        mTessLevelLoc.SetValue(TessLevel);
	}

    // Update spot light shadow map pass uniform data.
    if( pass == VPLviaSVOGI::SMP_SpotLightShadowMap )
    {
        mWorldLocSpotLightShadowMap.SetValue(worldTrans);
    }

    // Update G-buffer pass uniform data.
    if( pass == VPLviaSVOGI::SMP_GBuffer )
    {
        mWorldLocGBuffer.SetValue(worldTrans);

        if( diffuseMap )
        {
            diffuseMap->BindToSampler(0, &sd);
            mDiffuseMapLocGBuffer.SetValue(0);
            mUseDiffuseMapLocGBuffer.SetValue(true);
        }
        else
        {
            mMaterialColorLocGBuffer.SetValue(MaterialColor);
        }

        SEMatrix4f worldCacheTrans = *mRenderCache->GetWorldCache();
        mWorldCacheLocGBuffer.SetValue(worldCacheTrans);

        if( mCamera )
        {
            SEMatrix4f viewTrans = mCamera->GetViewTransform();
            mViewLocGBuffer.SetValue(viewTrans);

            SEMatrix4f projTrans = mCamera->GetProjectionTransform();
            mProjLocGBuffer.SetValue(projTrans);

            SEMatrix4f viewCacheTrans = mCamera->GetViewCacheTransform();
            mViewCacheLocGBuffer.SetValue(viewCacheTrans);

            SEMatrix4f projCacheTrans = mCamera->GetProjectionCacheTransform();
            mProjCacheLocGBuffer.SetValue(projCacheTrans);
        }
    }

    // Update point light RSM pass uniform data.
    if( pass == VPLviaSVOGI::SMP_PointLightRSM )
    {
        mWorldLocPointLightRSM.SetValue(worldTrans);

        if( diffuseMap )
        {
            diffuseMap->BindToSampler(0, &sd);
            mDiffuseMapLocPointLightRSM.SetValue(0);
            mUseDiffuseMapLocPointLightRSM.SetValue(true);
        }
        else
        {
            mMaterialColorLocPointLightRSM.SetValue(MaterialColor);
        }
    }

    // Update spot light RSM pass uniform data.
    if( pass == VPLviaSVOGI::SMP_SpotLightRSM )
    {
        mWorldLocSpotLightRSM.SetValue(worldTrans);
        
        if( diffuseMap )
        {
            diffuseMap->BindToSampler(0, &sd);
            mDiffuseMapLocSpotLightRSM.SetValue(0);
            mUseDiffuseMapLocSpotLightRSM.SetValue(true);
        }
        else
        {
            mMaterialColorLocSpotLightRSM.SetValue(MaterialColor);
        }
    }
}
//----------------------------------------------------------------------------