#include "VPLviaSVOGI.pch.h"
#include "VPLviaSVOGIApp.h"
#include "SELightMesh.h"
#include "SEObjLoader.h"

#include <GLFW\glfw3.h>

using namespace Swing;
using namespace Swing;

#include "InformationPanel.h"
using namespace Swing::GUIFramework;

//----------------------------------------------------------------------------
VPLviaSVOGI::VPLviaSVOGI(int width, int height)
{
	Width = width;
	Height = height;
	Title = "VPL via SVO GI";
    mIsRotatingModel = false;
    mIsWireframe = false;
    mUseTC = false;
    mVoxelizerType = SEVoxelizer::VT_SVO;
    mSceneTriangleCount = 0;
}
//----------------------------------------------------------------------------
VPLviaSVOGI::~VPLviaSVOGI()
{
}
//----------------------------------------------------------------------------
void VPLviaSVOGI::CreateSceneLights(SEGPUDevice* device)
{
	// Create light manager.
	mLightManager = new SELightManager(device);

#if 0
#ifdef DEMO_CORNELL_SCENE
	SELightProjectorDesc pointLight1ProjDesc;
	pointLight1ProjDesc.UpFovDegrees = 90.0f;
	pointLight1ProjDesc.AspectRatio = 1.0f;
	pointLight1ProjDesc.NearPlane = 0.01f;
	pointLight1ProjDesc.FarPlane = 50.0f;
	pointLight1ProjDesc.Location = SEVector3f(2.0f, 12.0f, 2.0f);
	pointLight1ProjDesc.LookAt = SEVector3f(0.0f, 0.0f, 0.0f);
	pointLight1ProjDesc.Up = SEVector3f(1.0f, 0.0f, 0.0f);
	SEPointLightDesc light1Desc;
	light1Desc.Intensity = SEVector3f(50.0f, 50.0f, 50.0f);
	mLightManager->CreatePointLight(&pointLight1ProjDesc, mMainCamera, &light1Desc);

	SELightProjectorDesc pointLight2ProjDesc;
	pointLight2ProjDesc.UpFovDegrees = 90.0f;
	pointLight2ProjDesc.AspectRatio = 1.0f;
	pointLight2ProjDesc.NearPlane = 0.01f;
	pointLight2ProjDesc.FarPlane = 50.0f;
	pointLight2ProjDesc.Location = SEVector3f(-2.0f, 12.0f, 2.0f);
	pointLight2ProjDesc.LookAt = SEVector3f(0.0f, 0.0f, 0.0f);
	pointLight2ProjDesc.Up = SEVector3f(1.0f, 0.0f, 0.0f);
	SEPointLightDesc light2Desc;
	light2Desc.Intensity = SEVector3f(50.0f, 20.0f, 10.0f);
	mLightManager->CreatePointLight(&pointLight2ProjDesc, mMainCamera, &light2Desc);
#endif
#ifdef DEMO_SPONZA_SCENE
    SELightProjectorDesc pointLight1ProjDesc;
    pointLight1ProjDesc.UpFovDegrees = 90.0f;
    pointLight1ProjDesc.AspectRatio = 1.0f;
    pointLight1ProjDesc.NearPlane = 0.01f;
    pointLight1ProjDesc.FarPlane = 5000.0f;
    pointLight1ProjDesc.Location = SEVector3f(-250.0f, 350.0f, 2.0f);
    pointLight1ProjDesc.LookAt = SEVector3f(0.0f, 0.0f, 0.0f);
    pointLight1ProjDesc.Up = SEVector3f(1.0f, 0.0f, 0.0f);
    SEPointLightDesc light1Desc;
    light1Desc.Intensity = SEVector3f(80000.0f, 80000.0f, 80000.0f);
    mLightManager->CreatePointLight(&pointLight1ProjDesc, mMainCamera, &light1Desc);

    SELightProjectorDesc pointLight2ProjDesc;
    pointLight2ProjDesc.UpFovDegrees = 90.0f;
    pointLight2ProjDesc.AspectRatio = 1.0f;
    pointLight2ProjDesc.NearPlane = 0.01f;
    pointLight2ProjDesc.FarPlane = 5000.0f;
    pointLight2ProjDesc.Location = SEVector3f(250.0f, 350.0f, 2.0f);
    pointLight2ProjDesc.LookAt = SEVector3f(0.0f, 0.0f, 0.0f);
    pointLight2ProjDesc.Up = SEVector3f(1.0f, 0.0f, 0.0f);
    SEPointLightDesc light2Desc;
    light2Desc.Intensity = SEVector3f(80000.0f, 80000.0f, 80000.0f);
    mLightManager->CreatePointLight(&pointLight2ProjDesc, mMainCamera, &light2Desc);
#endif
#endif

#if 1
#ifdef DEMO_CORNELL_SCENE
	SELightProjectorDesc spotLight1ProjDesc;
	spotLight1ProjDesc.UpFovDegrees = 90.0f;
	spotLight1ProjDesc.AspectRatio = 1.0f;
	spotLight1ProjDesc.NearPlane = 0.01f;
	spotLight1ProjDesc.FarPlane = 50.0f;
	spotLight1ProjDesc.Location = SEVector3f(-0.5f, 10.0f, 0.0f);
    //spotLight1ProjDesc.LookAt = SEVector3f(-10.0f, 1.0f, -2.0f);
    spotLight1ProjDesc.LookAt = SEVector3f(-4.0f, 7.0f, -3.0f);
	spotLight1ProjDesc.Up = SEVector3f(0.0f, 1.0f, 0.0f);
	SESpotLightDesc spotLight1Desc;
	spotLight1Desc.Intensity = SEVector3f(50.0f, 50.0f, 50.0f);
	spotLight1Desc.ConstantAttenuation = 0.0f;
	spotLight1Desc.QuadraticAttenuation = 0.2f;
	spotLight1Desc.CosCutoff = cos(40.0f / 180.0f * SEMathf::PI);
	spotLight1Desc.InnerCosCutoff = cos(40.0f * 0.95f / 180.0f * SEMathf::PI);
	spotLight1Desc.SpotExponent = 0.0f;
	mSceneLight1 = mLightManager->CreateSpotLight(&spotLight1ProjDesc, mMainCamera, &spotLight1Desc);

	SELightProjectorDesc spotLight2ProjDesc;
	spotLight2ProjDesc.UpFovDegrees = 90.0f;
	spotLight2ProjDesc.AspectRatio = 1.0f;
	spotLight2ProjDesc.NearPlane = 0.01f;
	spotLight2ProjDesc.FarPlane = 50.0f;
	spotLight2ProjDesc.Location = SEVector3f(0.5f, 10.0f, 0.0f);
	//spotLight2ProjDesc.LookAt = SEVector3f(10.0f, 15.0f, -5.0f);
    spotLight2ProjDesc.LookAt = SEVector3f(10.0f, 0.0f, 2.0f);
	spotLight2ProjDesc.Up = SEVector3f(0.0f, 1.0f, 0.0f);
	SESpotLightDesc spotLight2Desc;
	spotLight2Desc.Intensity = SEVector3f(50.0f, 50.0f, 50.0f);
	spotLight2Desc.ConstantAttenuation = 0.0f;
	spotLight2Desc.QuadraticAttenuation = 0.2f;
	spotLight2Desc.CosCutoff = cos(40.0f / 180.0f * SEMathf::PI);
	spotLight2Desc.InnerCosCutoff = cos(40.0f * 0.95f / 180.0f * SEMathf::PI);
	spotLight2Desc.SpotExponent = 0.0f;
	mSceneLight2 = mLightManager->CreateSpotLight(&spotLight2ProjDesc, mMainCamera, &spotLight2Desc);
#endif
#ifdef DEMO_SPONZA_SCENE
    SELightProjectorDesc spotLight1ProjDesc;
    spotLight1ProjDesc.UpFovDegrees = 90.0f;
    spotLight1ProjDesc.AspectRatio = 1.0f;
    spotLight1ProjDesc.NearPlane = 0.01f;
    spotLight1ProjDesc.FarPlane = 5000.0f;
    spotLight1ProjDesc.Location = SEVector3f(-150.0f, 800.0f, -2.0f);
    spotLight1ProjDesc.LookAt = SEVector3f(-150.0f, 650.0f, 100.0f);
    spotLight1ProjDesc.Up = SEVector3f(0.0f, 1.0f, 0.0f);
    SESpotLightDesc spotLight1Desc;
    spotLight1Desc.Intensity = SEVector3f(500000.0f, 500000.0f, 500000.0f);
    spotLight1Desc.ConstantAttenuation = 0.0f;
    spotLight1Desc.QuadraticAttenuation = 0.2f;
    spotLight1Desc.CosCutoff = cos(40.0f / 180.0f * SEMathf::PI);
    spotLight1Desc.InnerCosCutoff = cos(40.0f * 0.95f / 180.0f * SEMathf::PI);
    spotLight1Desc.SpotExponent = 0.0f;
    mSceneLight1 = mLightManager->CreateSpotLight(&spotLight1ProjDesc, mMainCamera, &spotLight1Desc);

    //SELightProjectorDesc spotLight2ProjDesc;
    //spotLight2ProjDesc.UpFovDegrees = 90.0f;
    //spotLight2ProjDesc.AspectRatio = 1.0f;
    //spotLight2ProjDesc.NearPlane = 0.01f;
    //spotLight2ProjDesc.FarPlane = 5000.0f;
    //spotLight2ProjDesc.Location = SEVector3f(150.0f, 400.0f, -2.0f);
    //spotLight2ProjDesc.LookAt = SEVector3f(500.0f, 100.0f, -20.0f);
    //spotLight2ProjDesc.Up = SEVector3f(0.0f, 1.0f, 0.0f);
    //SESpotLightDesc spotLight2Desc;
    //spotLight2Desc.Intensity = SEVector3f(500000.0f, 500000.0f, 500000.0f);
    //spotLight2Desc.ConstantAttenuation = 0.0f;
    //spotLight2Desc.QuadraticAttenuation = 0.2f;
    //spotLight2Desc.CosCutoff = cos(40.0f / 180.0f * SEMathf::PI);
    //spotLight2Desc.InnerCosCutoff = cos(40.0f * 0.95f / 180.0f * SEMathf::PI);
    //spotLight2Desc.SpotExponent = 0.0f;
    //mSceneLight2 = mLightManager->CreateSpotLight(&spotLight2ProjDesc, mMainCamera, &spotLight2Desc);
#endif
#endif

	mLightManager->CreateLightBuffer(device);
}
//----------------------------------------------------------------------------
SEMaterialTemplate* VPLviaSVOGI::CreateSceneModelMaterialTemplate()
{
	SEShaderProgramInfo voxelizationProgramInfo;
	voxelizationProgramInfo.VShaderFileName = "VPLviaSVOGI/vVoxelization.glsl";
	voxelizationProgramInfo.GShaderFileName = "VPLviaSVOGI/gVoxelization.glsl";
	if( mVoxelizerType == SEVoxelizer::VT_Grid )
	{
		voxelizationProgramInfo.FShaderFileName = "VPLviaSVOGI/fVoxelization.glsl";
	}
	else if( mVoxelizerType == SEVoxelizer::VT_SVO )
	{
		voxelizationProgramInfo.FShaderFileName = "VPLviaSVOGI/fSVOVoxelization.glsl";
	}
	else
	{
		assert(false);
	}
	voxelizationProgramInfo.ShaderStageFlag = SEShaderStage::SS_Vertex |
		SEShaderStage::SS_Geometry |
		SEShaderStage::SS_Fragment;
    SERenderPassTargetsInfo voxelizationTargetsInfo;
    voxelizationTargetsInfo.ColorTargetCount = 0;
	SERenderPass* passVoxelization = new SERenderPass(voxelizationProgramInfo, 
        voxelizationTargetsInfo);

	SEShaderProgramInfo pointLightShadowProgramInfo;
	pointLightShadowProgramInfo.VShaderFileName = "VPLviaSVOGI/vPointLightShadow.glsl";
	pointLightShadowProgramInfo.FShaderFileName = "VPLviaSVOGI/fPointLightShadow.glsl";
	pointLightShadowProgramInfo.TCShaderFileName = "VPLviaSVOGI/tcPointLightShadow.glsl";
	pointLightShadowProgramInfo.TEShaderFileName = "VPLviaSVOGI/tePointLightShadow.glsl";
	pointLightShadowProgramInfo.GShaderFileName = "VPLviaSVOGI/gPointLightShadow.glsl";
	pointLightShadowProgramInfo.ShaderStageFlag = SEShaderStage::SS_Vertex |
		SEShaderStage::SS_Fragment |
		SEShaderStage::SS_TessellationControl |
		SEShaderStage::SS_TessellationEvaluation |
		SEShaderStage::SS_Geometry;
    SERenderPassTargetsInfo pointLightShadowTargetsInfo;
    pointLightShadowTargetsInfo.ColorTargetCount = 1;
    pointLightShadowTargetsInfo.ColorTargetFormats[0] = BF_R32F;
	SERenderPass* passPointLightShadow = new SERenderPass(pointLightShadowProgramInfo, 
        pointLightShadowTargetsInfo);

	SEShaderProgramInfo spotLightShadowProgramInfo;
	spotLightShadowProgramInfo.VShaderFileName = "VPLviaSVOGI/vSpotLightShadow.glsl";
	spotLightShadowProgramInfo.GShaderFileName = "VPLviaSVOGI/gSpotLightShadow.glsl";
	spotLightShadowProgramInfo.FShaderFileName = "VPLviaSVOGI/fSpotLightShadow.glsl";
	spotLightShadowProgramInfo.ShaderStageFlag = SEShaderStage::SS_Vertex |
		SEShaderStage::SS_Geometry |
		SEShaderStage::SS_Fragment;
    SERenderPassTargetsInfo spotLightShadowTargetsInfo;
    spotLightShadowTargetsInfo.ColorTargetCount = 1;
    spotLightShadowTargetsInfo.ColorTargetFormats[0] = BF_R32F;
	SERenderPass* passSpotLightShadow = new SERenderPass(spotLightShadowProgramInfo, 
        spotLightShadowTargetsInfo);

	SEShaderProgramInfo gbufferProgramInfo;
	if( mUseTC )
	{
		gbufferProgramInfo.VShaderFileName = "VPLviaSVOGI/vGBufferRPC.glsl";
		gbufferProgramInfo.FShaderFileName = "VPLviaSVOGI/fGBufferRPC.glsl";
	}
	else
	{
		gbufferProgramInfo.VShaderFileName = "VPLviaSVOGI/vGBuffer.glsl";
		gbufferProgramInfo.FShaderFileName = "VPLviaSVOGI/fGBuffer.glsl";
	}
	gbufferProgramInfo.ShaderStageFlag = SEShaderStage::SS_Vertex |
		SEShaderStage::SS_Fragment;
    SERenderPassTargetsInfo gBufferTargetsInfo;
    gBufferTargetsInfo.ColorTargetCount = 3;
    gBufferTargetsInfo.ColorTargetFormats[0] = BF_RGBAF;
    gBufferTargetsInfo.ColorTargetFormats[1] = BF_RGBAF;
    gBufferTargetsInfo.ColorTargetFormats[2] = BF_RGBAF;
	SERenderPass* passGBuffer = new SERenderPass(gbufferProgramInfo, gBufferTargetsInfo);

	SEShaderProgramInfo pointLightRSMProgramInfo;
	pointLightRSMProgramInfo.VShaderFileName = "VPLviaSVOGI/vPointLightRSM.glsl";
	pointLightRSMProgramInfo.GShaderFileName = "VPLviaSVOGI/gPointLightRSM.glsl";
	pointLightRSMProgramInfo.FShaderFileName = "VPLviaSVOGI/fPointLightRSM.glsl";
	pointLightRSMProgramInfo.ShaderStageFlag = SEShaderStage::SS_Vertex |
		SEShaderStage::SS_Geometry |
		SEShaderStage::SS_Fragment;
	pointLightRSMProgramInfo.Parameters.push_back(
		SEShaderProgramParameterValue(SPP_Geometry_Vertices_Out,
		RSM_POINT_LIGHT_FACE_COUNT * 3));
    SERenderPassTargetsInfo pointLightRSMTargetsInfo;
    pointLightRSMTargetsInfo.ColorTargetCount = 3;
    pointLightRSMTargetsInfo.ColorTargetFormats[0] = BF_RGBAF;
    pointLightRSMTargetsInfo.ColorTargetFormats[1] = BF_RGBAF;
    pointLightRSMTargetsInfo.ColorTargetFormats[2] = BF_RGBAF;
	SERenderPass* passPointLightRSM = new SERenderPass(pointLightRSMProgramInfo, 
        pointLightRSMTargetsInfo);

	SEShaderProgramInfo spotLightRSMProgramInfo;
	spotLightRSMProgramInfo.VShaderFileName = "VPLviaSVOGI/vSpotLightRSM.glsl";
	spotLightRSMProgramInfo.GShaderFileName = "VPLviaSVOGI/gSpotLightRSM.glsl";
	spotLightRSMProgramInfo.FShaderFileName = "VPLviaSVOGI/fSpotLightRSM.glsl";
	spotLightRSMProgramInfo.ShaderStageFlag = SEShaderStage::SS_Vertex |
		SEShaderStage::SS_Geometry |
		SEShaderStage::SS_Fragment;
    SERenderPassTargetsInfo spotLightRSMTargetsInfo;
    spotLightRSMTargetsInfo.ColorTargetCount = 3;
    spotLightRSMTargetsInfo.ColorTargetFormats[0] = BF_RGBAF;
    spotLightRSMTargetsInfo.ColorTargetFormats[1] = BF_RGBAF;
    spotLightRSMTargetsInfo.ColorTargetFormats[2] = BF_RGBAF;
	SERenderPass* passSpotLightRSM = new SERenderPass(spotLightRSMProgramInfo, 
        spotLightRSMTargetsInfo);

	SETechnique* techSceneModel = new SETechnique();
	techSceneModel->AddPass(passVoxelization);
	techSceneModel->AddPass(passPointLightShadow);
	techSceneModel->AddPass(passSpotLightShadow);
	techSceneModel->AddPass(passGBuffer);
	techSceneModel->AddPass(passPointLightRSM);
	techSceneModel->AddPass(passSpotLightRSM);
	SEMaterialTemplate* mtSceneModel = new SEMaterialTemplate();
	mtSceneModel->AddTechnique(techSceneModel);

	return mtSceneModel;
}
//----------------------------------------------------------------------------
void VPLviaSVOGI::CreateScene(SEMaterialTemplate* sceneMT, SEGPUDevice* device, 
	SEAxisAlignedBox3f* sceneBB, SERenderSet* voxelizerSet, SERenderSet* gbufferSet,
	SERenderSet* shadowMapSet, SERenderSet* rsmSet)
{
	assert(sceneMT && device &&sceneBB);

	SEMaterial* material = 0;

#ifdef DEMO_CORNELL_SCENE
	SEMatrix4f rotM;
	material = new SEMaterial(sceneMT);
	mModel = new SceneMesh(material, mMainCamera);
	mModel->LoadFromPLYFile("dragon_s.ply");
	mModel->GenerateNormals();
	mModel->CreateDeviceResource(device);
    rotM.FromAxisAngle(SEVector3f(0.0f, 1.0f, 0.0f), -30.0f*SEMathf::DEG_TO_RAD);
	mModel->SetWorldTransform(rotM);
	mModel->SetWorldTranslation(SEVector3f(-5.4f, 3.2f, -1.6f));
    mModel->SetWorldScale(SEVector3f(45.0f, 45.0f, 45.0f));
	mModel->MaterialColor = SEVector3f(0.1f, 0.9f, 0.9f)*0.5f;
	mModel->SceneBB = sceneBB;
	sceneBB->Merge(mModel->GetWorldSpaceBB());
    mSceneTriangleCount += mModel->GetFaceCount();

	mModel2Sequence = new SERenderSequence(0);
	for( int i = 1; i <= 12; ++i )
	{
		material = new SEMaterial(sceneMT);
		SceneMesh* model2 = new SceneMesh(material, mMainCamera);
		char objFileName[64];
		sprintf(objFileName, "elephant-gallop/elephant-gallop-%d.obj", i);
		model2->LoadFromOBJFile(objFileName);
		model2->GenerateNormals();
		model2->CreateDeviceResource(device);
        rotM.FromAxisAngle(SEVector3f(0.0f, 1.0f, 0.0f), 60.0f*SEMathf::DEG_TO_RAD);
		model2->SetWorldTransform(rotM);
		model2->SetWorldTranslation(SEVector3f(3.2f, 3.2f, -2.4f));
        model2->SetWorldScale(SEVector3f(8.0f, 8.0f, 8.0f));
		model2->MaterialColor = SEVector3f(0.2f, 0.2f, 0.9f);
		model2->SceneBB = sceneBB;
		model2->TessLevel = 1.0f;
		sceneBB->Merge(model2->GetWorldSpaceBB());
		mModel2Sequence->AddRenderObject(model2);

        if( i == 1 )
        {
            mSceneTriangleCount += model2->GetFaceCount();
        }
	}
	mModel2Sequence->SetActiveRenderObject(0);
	mModel2Sequence->SetFrequence(0.4f);

	material = new SEMaterial(sceneMT);
	mGround = new SceneMesh(material, mMainCamera);
	mGround->LoadFromPLYFile("square.ply");
	mGround->GenerateNormals();
	mGround->CreateDeviceResource(device);
	mGround->MaterialColor = SEVector3f(1.0f, 1.0f, 1.0f);
	mGround->SceneBB = sceneBB;
	sceneBB->Merge(mGround->GetWorldSpaceBB());
    mSceneTriangleCount += mGround->GetFaceCount();

	material = new SEMaterial(sceneMT);
	mCeiling = new SceneMesh(material, mMainCamera);
	mCeiling->LoadFromPLYFile("square.ply");
	mCeiling->GenerateNormals();
	mCeiling->CreateDeviceResource(device);
    rotM.FromAxisAngle(SEVector3f(1.0f, 0.0f, 0.0f), 180.0f*SEMathf::DEG_TO_RAD);
	mCeiling->SetWorldTransform(rotM);
	mCeiling->SetWorldTranslation(SEVector3f(0.0f, 20.0f, 0.0f));
	mCeiling->MaterialColor = SEVector3f(1.0f, 1.0f, 1.0f);
	mCeiling->SceneBB = sceneBB;
	sceneBB->Merge(mCeiling->GetWorldSpaceBB());
    mSceneTriangleCount += mCeiling->GetFaceCount();

	material = new SEMaterial(sceneMT);
	mBackWall = new SceneMesh(material, mMainCamera);
	mBackWall->LoadFromPLYFile("square.ply");
	mBackWall->GenerateNormals();
	mBackWall->CreateDeviceResource(device);
    rotM.FromAxisAngle(SEVector3f(1.0f, 0.0f, 0.0f), -90.0f*SEMathf::DEG_TO_RAD);
	mBackWall->SetWorldTransform(rotM);
	mBackWall->SetWorldTranslation(SEVector3f(0.0f, 10.0f, 10.0f));
	mBackWall->MaterialColor = SEVector3f(1.0f, 1.0f, 1.0f);
	mBackWall->SceneBB = sceneBB;
	sceneBB->Merge(mBackWall->GetWorldSpaceBB());
    mSceneTriangleCount += mBackWall->GetFaceCount();

	material = new SEMaterial(sceneMT);
	mLeftWall = new SceneMesh(material, mMainCamera);
	mLeftWall->LoadFromPLYFile("square.ply");
	mLeftWall->GenerateNormals();
	mLeftWall->CreateDeviceResource(device);
    rotM.FromAxisAngle(SEVector3f(0.0f, 0.0f, 1.0f), -90.0f*SEMathf::DEG_TO_RAD);
	mLeftWall->SetWorldTransform(rotM);
	mLeftWall->SetWorldTranslation(SEVector3f(-10.0f, 10.0f, 0.0f));
	mLeftWall->MaterialColor = SEVector3f(0.95f, 0.2f, 0.2f);
	mLeftWall->SceneBB = sceneBB;
	sceneBB->Merge(mLeftWall->GetWorldSpaceBB());
    mSceneTriangleCount += mLeftWall->GetFaceCount();

	material = new SEMaterial(sceneMT);
	mRightWall = new SceneMesh(material, mMainCamera);
	mRightWall->LoadFromPLYFile("square.ply");
	mRightWall->GenerateNormals();
	mRightWall->CreateDeviceResource(device);
    rotM.FromAxisAngle(SEVector3f(0.0f, 0.0f, 1.0f), 90.0f*SEMathf::DEG_TO_RAD);
	mRightWall->SetWorldTransform(rotM);
	mRightWall->SetWorldTranslation(SEVector3f(10.0f, 10.0f, 0.0f));
	mRightWall->MaterialColor = SEVector3f(0.2f, 0.95f, 0.2f);
	mRightWall->SceneBB = sceneBB;
	sceneBB->Merge(mRightWall->GetWorldSpaceBB());
    mSceneTriangleCount += mRightWall->GetFaceCount();

#endif

#ifdef DEMO_SPONZA_SCENE
	SEObjLoader objLoader;
	std::vector<SEObjMetaMeshPtr> objMeshes;
	objLoader.Load("OBJ/crytek-sponza/", "sponza.obj", objMeshes);

    std::vector<std::string> ignored;

    ignored.push_back("sponza_371");
    ignored.push_back("sponza_370");
    ignored.push_back("sponza_372");
    ignored.push_back("sponza_369");
    ignored.push_back("sponza_280");
    ignored.push_back("sponza_00");
    ignored.push_back("sponza_279");
    ignored.push_back("sponza_281");
    ignored.push_back("sponza_278");
    ignored.push_back("sponza_275");
    ignored.push_back("sponza_01");
    ignored.push_back("sponza_368");
    ignored.push_back("sponza_276");
    ignored.push_back("sponza_277");
    ignored.push_back("sponza_367");
    ignored.push_back("sponza_366");
    ignored.push_back("sponza_344");
    ignored.push_back("sponza_345");
    ignored.push_back("sponza_346");
    ignored.push_back("sponza_347");
    ignored.push_back("sponza_340");
    ignored.push_back("sponza_341");
    ignored.push_back("sponza_342");
    ignored.push_back("sponza_343");
    ignored.push_back("sponza_339");
    ignored.push_back("sponza_356");
    ignored.push_back("sponza_348");
    ignored.push_back("sponza_349");
    ignored.push_back("sponza_350");
    ignored.push_back("sponza_351");
    ignored.push_back("sponza_352");
    ignored.push_back("sponza_353");
    ignored.push_back("sponza_354");
    ignored.push_back("sponza_355");
    ignored.push_back("sponza_361");
    ignored.push_back("sponza_362");
    ignored.push_back("sponza_363");
    ignored.push_back("sponza_364");
    ignored.push_back("sponza_365");
    ignored.push_back("sponza_360");
    ignored.push_back("sponza_357");
    ignored.push_back("sponza_358");
    ignored.push_back("sponza_359");
    ignored.push_back("sponza_338");
    ignored.push_back("sponza_330");
    ignored.push_back("sponza_331");
    ignored.push_back("sponza_332");
    ignored.push_back("sponza_333");
    ignored.push_back("sponza_334");
    ignored.push_back("sponza_335");
    ignored.push_back("sponza_336");
    ignored.push_back("sponza_337");

    ignored.push_back("sponza_320");
    ignored.push_back("sponza_324");

	for( int i = 0; i < (int)objMeshes.size(); ++i )
	{
        bool isIgnored = false;
        for( int j = 0; j < (int)ignored.size(); ++j )
        {
            if( objMeshes[i]->MeshName == ignored[j] )
            {
                isIgnored = true;
                break;
            }
        }

        if( !isIgnored )
        {
            material = new SEMaterial(sceneMT);
            SceneMesh* mesh = new SceneMesh(material, mMainCamera);
            mesh->LoadFromSEMetaMesh(objMeshes[i]);
            mesh->GenerateNormals();
            mesh->CreateDeviceResource(device);
            mesh->MaterialColor = SEVector3f(1.0f, 1.0f, 1.0f);
            mesh->SceneBB = sceneBB;
            mesh->TessLevel = 1.0f;
            sceneBB->Merge(mesh->GetWorldSpaceBB());
            mSponza.push_back(mesh);
            mSceneTriangleCount += mesh->GetFaceCount();
        }
	}
#endif

    sceneBB->ExpandToCube();

	if( gbufferSet )
	{
#ifdef DEMO_CORNELL_SCENE
		gbufferSet->AddRenderObject(mModel);
		gbufferSet->AddRenderObject(mModel2Sequence);
		gbufferSet->AddRenderObject(mGround);
		gbufferSet->AddRenderObject(mCeiling);
		gbufferSet->AddRenderObject(mBackWall);
		gbufferSet->AddRenderObject(mLeftWall);
		gbufferSet->AddRenderObject(mRightWall);
		gbufferSet->UpdateRenderCache();
#endif

#ifdef DEMO_SPONZA_SCENE
		for( int i = 0; i < (int)mSponza.size(); ++i )
		{
			gbufferSet->AddRenderObject(mSponza[i]);
		}
#endif
	}

	if( shadowMapSet )
	{
#ifdef DEMO_CORNELL_SCENE
		shadowMapSet->AddRenderObject(mModel);
		shadowMapSet->AddRenderObject(mModel2Sequence);
		shadowMapSet->AddRenderObject(mGround);
		shadowMapSet->AddRenderObject(mCeiling);
		shadowMapSet->AddRenderObject(mBackWall);
		shadowMapSet->AddRenderObject(mLeftWall);
		shadowMapSet->AddRenderObject(mRightWall);
#endif

#ifdef DEMO_SPONZA_SCENE
		for( int i = 0; i < (int)mSponza.size(); ++i )
		{
			shadowMapSet->AddRenderObject(mSponza[i]);
		}
#endif
	}

	if( rsmSet )
	{
#ifdef DEMO_CORNELL_SCENE
		rsmSet->AddRenderObject(mModel);
		rsmSet->AddRenderObject(mModel2Sequence);
		rsmSet->AddRenderObject(mGround);
		rsmSet->AddRenderObject(mCeiling);
		rsmSet->AddRenderObject(mBackWall);
		rsmSet->AddRenderObject(mLeftWall);
		rsmSet->AddRenderObject(mRightWall);
#endif

#ifdef DEMO_SPONZA_SCENE
		for( int i = 0; i < (int)mSponza.size(); ++i )
		{
			rsmSet->AddRenderObject(mSponza[i]);
		}
#endif
	}

	if( voxelizerSet )
	{
#ifdef DEMO_CORNELL_SCENE
		if( mVoxelizerType == SEVoxelizer::VT_Grid )
		{
			voxelizerSet->AddRenderObject(mModel);
			voxelizerSet->AddRenderObject(mModel2Sequence);
			voxelizerSet->AddRenderObject(mGround);
			voxelizerSet->AddRenderObject(mCeiling);
			voxelizerSet->AddRenderObject(mBackWall);
			voxelizerSet->AddRenderObject(mLeftWall);
			voxelizerSet->AddRenderObject(mRightWall);
		}
		else if( mVoxelizerType == SEVoxelizer::VT_SVO )
		{
			voxelizerSet->AddRenderObject(mModel);
            voxelizerSet->AddRenderObject(mModel2Sequence);
            //voxelizerSet->AddRenderObject(mGround);
            //voxelizerSet->AddRenderObject(mCeiling);
            //voxelizerSet->AddRenderObject(mBackWall);
            //voxelizerSet->AddRenderObject(mLeftWall);
            //voxelizerSet->AddRenderObject(mRightWall);
		}
		else
		{
			assert(false);
		}
#endif

#ifdef DEMO_SPONZA_SCENE
		std::vector<std::string> voxelizerExcluded;
		voxelizerExcluded.push_back("sponza_03");
		voxelizerExcluded.push_back("sponza_45");
		voxelizerExcluded.push_back("sponza_68");
		voxelizerExcluded.push_back("sponza_69");
		voxelizerExcluded.push_back("sponza_70");
		voxelizerExcluded.push_back("sponza_71");
		voxelizerExcluded.push_back("sponza_72");
		voxelizerExcluded.push_back("sponza_73");
		voxelizerExcluded.push_back("sponza_74");
		voxelizerExcluded.push_back("sponza_116");
		voxelizerExcluded.push_back("sponza_117");
		voxelizerExcluded.push_back("sponza_257");
		voxelizerExcluded.push_back("sponza_377");
		voxelizerExcluded.push_back("sponza_378");
		voxelizerExcluded.push_back("sponza_379");
		voxelizerExcluded.push_back("sponza_380");
		voxelizerExcluded.push_back("sponza_381");
		voxelizerExcluded.push_back("sponza_382");

		for( int i = 0; i < (int)mSponza.size(); ++i )
		{
			bool excluded = false;
			for( int j = 0; j < (int)voxelizerExcluded.size(); ++j )
			{
				if( mSponza[i]->Name == voxelizerExcluded[j] )
				{
					excluded = true;
					break;
				}
			}

			if( !excluded )
			{
				voxelizerSet->AddRenderObject(mSponza[i]);
			}
		}
#endif
	}
}
//----------------------------------------------------------------------------
void VPLviaSVOGI::Initialize(SEGPUDeviceBase* device)
{
    SEGPUDevice* gpuDevice = (SEGPUDevice*)device;

    mInitPSB = new SEPipelineStateBlock();
    mInitPSB->PipelineStageFlag = PB_OutputMerger | PB_Rasterizer;
    mInitPSB->OutputMerger.OutputMergerOpFlag = OMB_ClearColor;
    mInitPSB->OutputMerger.ClearColor = SEVector4f(0.5f, 0.5f, 0.5f, 0.0f);
    mInitPSB->Rasterizer.RasterizerOpFlag = RB_CullMode | RB_FillMode;
    mInitPSB->Rasterizer.CullMode = PCM_Cull_Back;
    mInitPSB->Rasterizer.FillMode = PFM_Solid;
    mInitPSB->OutputMerger.DepthStencil.DepthEnable = true;
    gpuDevice->ApplyPipelineStateBlock(mInitPSB);

    mDeviceInspector = new SEGPUDeviceInspector(gpuDevice);
    
    // Create scene camera.
#ifdef DEMO_SPONZA_SCENE
	mMainCamera->SetPerspectiveFrustum(70.0f, (float)Width/(float)Height, 0.5f, 4000.0f);
	mMainCamera->SetLookAt(SEVector3f(-850.0f, 350.0f, 10.0f), SEVector3f(200.0f, 200.0f, 10.0f),
        SEVector3f(0.0f, 1.0f, 0.0f));
	mMainCameraSpeed = 500.0f;
#endif
#ifdef DEMO_CORNELL_SCENE
    mMainCamera->SetPerspectiveFrustum(45.0f, (float)Width / (float)Height, 0.01f, 150.0f);
    mMainCamera->SetLookAt(SEVector3f(0.0f, 10.0f, -35.0f), SEVector3f(0.0f, 10.0f, 0.0f),
        SEVector3f(0.0f, 1.0f, 0.0f));
#endif

	// Create scene lights and light manager. Light manager maintains a lgiht
	// buffer for shaders accessing scene lights info.
	CreateSceneLights(gpuDevice);

	// Create a mult-pass material template used by scene models.
	SEMaterialTemplate* mtSceneModel = CreateSceneModelMaterialTemplate();

    // Create scene and render sets.
	mGBufferObjects = new SERenderSet();
	mShadowCasters = new SERenderSet();
	mRSMObjects = new SERenderSet();
	mVoxelizedObjects = new SERenderSet();
	CreateScene(mtSceneModel, gpuDevice, &mSceneBB, mVoxelizedObjects, mGBufferObjects,
		mShadowCasters, mRSMObjects);

    // Create scene voxelizer.
    if( mVoxelizerType == SEVoxelizer::VT_Grid )
    {
        mVoxelizer = new GridVoxelizer(gpuDevice);
        ((GridVoxelizer*)(SEVoxelizer*)mVoxelizer)->Initialize(gpuDevice, 
            VOXEL_DIMENSION, VOXEL_LOCAL_GROUP_DIM, &mSceneBB);
    }
    else if( mVoxelizerType == SEVoxelizer::VT_SVO )
    {
        mVoxelizer = new SVOVoxelizer(gpuDevice);
        ((SVOVoxelizer*)(SEVoxelizer*)mVoxelizer)->Initialize(gpuDevice, 
            VOXEL_DIMENSION, &mSceneBB);
    }
    else
    {
        assert(false);
    }
    mVoxelizer->RasterizerDimBias = -1;
    mVoxelizer->SetRenderSet(mVoxelizedObjects);

    // Create G-buffer renderer.
    SEGBufferDesc gbufferDesc;
    gbufferDesc.Width = Width;
    gbufferDesc.Height = Height;
    gbufferDesc.PositionFormat = BF_RGBAF;
    gbufferDesc.NormalFormat = BF_RGBAF;
    gbufferDesc.AlbedoFormat = BF_RGBAF;
    gbufferDesc.RPCFormat = BF_RGBAF;
    mGBufferRenderer = new SEGBufferRenderer(gpuDevice);
    mGBufferRenderer->Initialize(&gbufferDesc);
    mGBufferRenderer->SetRenderSet(mGBufferObjects);

	// Create G-buffer splitter.
	mGBufferSplitter = new GBufferSplitter(gpuDevice);
	mGBufferSplitter->Initialize(gpuDevice, mGBufferRenderer, 
		INTERLEAVED_PATTERN_SIZE, INTERLEAVED_PATTERN_SIZE);

    // Create shadow map renderer.
    mShadowMapRenderer = new ShadowMapsGenerator(gpuDevice);
    mShadowMapRenderer->Initialize(SHADOWMAP_DIMENSION, SHADOWMAP_DIMENSION, BF_R32F, mLightManager);
    mShadowMapRenderer->SetRenderSet(mShadowCasters);

    // Create RSM renderer.
    mRSMRenderer = new RSMRenderer(gpuDevice);
    mRSMRenderer->Initialize(RSM_DIMENSION, RSM_DIMENSION, RSM_POINT_LIGHT_FACE_COUNT, BF_RGBAF,
		mLightManager, true);
    mRSMRenderer->SetRenderSet(mRSMObjects);

    // Create VPL generator.
    mVPLGenerator = new VPLGenerator(gpuDevice);
	mVPLGenerator->Initialize(gpuDevice, mRSMRenderer, VPL_SAMPLE_COUNT);

    // Create direct lighting renderer.
    mDirectLightingRenderer = new DirectLightingRenderer(gpuDevice);
    mDirectLightingRenderer->Initialize(gpuDevice, Width, Height, BF_RGBAF, 
        mGBufferRenderer, mShadowMapRenderer);

    // Create indirect lighting renderer.
    mIndirectLightingRenderer = new IndirectLightingRenderer(gpuDevice);
    mIndirectLightingRenderer->Initialize(gpuDevice, Width, Height, BF_RGBAF, 
        VPL_SAMPLE_COUNT, INTERLEAVED_PATTERN_SIZE, &mSceneBB, VOXEL_DIMENSION, 
        mGBufferSplitter, mVPLGenerator, mVoxelizer, mUseTC);

	// Create splitted buffer merger.
	mSplittedBufferMerger = new SplittedBufferMerger(gpuDevice);
	mSplittedBufferMerger->Initialize(gpuDevice, mIndirectLightingRenderer,
        RTGI_IndirectLightingRenderer_IndirectLighting_Name, 
		RTGI_SplittedBufferMerger_MergedBuffer_Name, 
		INTERLEAVED_PATTERN_SIZE, INTERLEAVED_PATTERN_SIZE);

	// Create G-aware filter.
    float thresholdPosition, thresholdNormal;
    thresholdNormal = 0.8f;
#ifdef DEMO_SPONZA_SCENE
    thresholdPosition = 400.0f;
#endif
#ifdef DEMO_CORNELL_SCENE
    thresholdPosition = 0.5f;
#endif
	mGAwareFilter = new GAwareFilter(gpuDevice);
	mGAwareFilter->Initialize(gpuDevice, mGBufferRenderer, mSplittedBufferMerger, 
		RTGI_SplittedBufferMerger_MergedBuffer_Name, 
		RTGI_GAwareFilter_FilteredBuffer_Name, 
		INTERLEAVED_PATTERN_SIZE, INTERLEAVED_PATTERN_SIZE, thresholdPosition, 
        thresholdNormal);

    // Create visualizer.
    float toneMapperMaxRadiance;
#ifdef DEMO_SPONZA_SCENE
    toneMapperMaxRadiance = 150000.0f;
#endif
#ifdef DEMO_CORNELL_SCENE
    toneMapperMaxRadiance = 80.0f;
#endif
    mVisualizer = new Visualizer(gpuDevice);
    mVisualizer->Initialize(gpuDevice, mVoxelizer, mVPLGenerator, 
        mShadowMapRenderer, mGBufferRenderer, mGBufferSplitter, mRSMRenderer, 
        mDirectLightingRenderer, mIndirectLightingRenderer, mSplittedBufferMerger, 
        mGAwareFilter, &mSceneBB, VOXEL_DIMENSION, VOXEL_LOCAL_GROUP_DIM, 
        mMainCamera, INTERLEAVED_PATTERN_SIZE, VPL_SAMPLE_COUNT, 
		toneMapperMaxRadiance, mLightManager);

    // Create GPU timer.
    mTimer = new SEGPUTimer();
    mTimer->CreateDeviceResource(gpuDevice);
    mVoxelizer->SetTimer(mTimer);
    mShadowMapRenderer->SetTimer(mTimer);
    mGBufferRenderer->SetTimer(mTimer);
	mGBufferSplitter->SetTimer(mTimer);
    mRSMRenderer->SetTimer(mTimer);
    mVPLGenerator->SetTimer(mTimer);
    mDirectLightingRenderer->SetTimer(mTimer);
    mIndirectLightingRenderer->SetTimer(mTimer);
    mSplittedBufferMerger->SetTimer(mTimer);
	mGAwareFilter->SetTimer(mTimer);
    mVisualizer->SetTimer(mTimer);

	// Create GUI elements.
	CreateGUI();

    //mDeviceInspector->DumpDeviceMemoryOperation();
}
//----------------------------------------------------------------------------
void VPLviaSVOGI::CreateGUI()
{
	// Create information panel.
	int screenX, screenY;
	glfwGetWindowPos(Window, &screenX, &screenY);
	InformationPanel^ infoPanel = gcnew InformationPanel();
	infoPanel->Show();
	infoPanel->SetDesktopLocation(screenX + Width + 12, screenY - 150);

	InformationPanel::GetInstance()->AddListener(this);

	int infoStartY = 20;
	int infoIncY = 20;
    InformationPanel::GetInstance()->AddTimingLabel("Scene Triangle Count", 16, infoStartY);
    InformationPanel::GetInstance()->SetTimingLabelValue("Scene Triangle Count", (double)mSceneTriangleCount);
    infoStartY += infoIncY;
#ifdef SHOW_TIMING
	InformationPanel::GetInstance()->AddTimingLabel("Scene Voxelization Pass", 16, infoStartY);
	infoStartY += infoIncY;
	InformationPanel::GetInstance()->AddTimingLabel("Scene Shadow Pass", 16, infoStartY);
	infoStartY += infoIncY;
	InformationPanel::GetInstance()->AddTimingLabel("Scene G-buffer Pass", 16, infoStartY);
	infoStartY += infoIncY;
	InformationPanel::GetInstance()->AddTimingLabel("Scene G-buffer Splitter Pass", 16, infoStartY);
	infoStartY += infoIncY;
	InformationPanel::GetInstance()->AddTimingLabel("RSM Pass", 16, infoStartY);
	infoStartY += infoIncY;
	InformationPanel::GetInstance()->AddTimingLabel("VPL Creation Pass", 16, infoStartY);
	infoStartY += infoIncY;
	InformationPanel::GetInstance()->AddTimingLabel("Direct Lighting Pass", 16, infoStartY);
	infoStartY += infoIncY;
	InformationPanel::GetInstance()->AddTimingLabel("Indirect Lighting Pass", 16, infoStartY);
	infoStartY += infoIncY;
    InformationPanel::GetInstance()->AddTimingLabel("Merge Buffer Pass", 16, infoStartY);
    infoStartY += infoIncY;
	InformationPanel::GetInstance()->AddTimingLabel("G-aware Filter Pass", 16, infoStartY);
	infoStartY += infoIncY;
	InformationPanel::GetInstance()->AddTimingLabel("Visualizer Pass", 16, infoStartY);
	infoStartY += infoIncY;
	InformationPanel::GetInstance()->AddTimingLabel("Total", 16, infoStartY);
	infoStartY += infoIncY;
	InformationPanel::GetInstance()->AddTimingLabel("Frame Counter", 16, infoStartY);
#endif

	infoStartY = 20;
	InformationPanel::GetInstance()->AddButton("Prev Texture Map", 160, infoStartY, 110, 24);
	InformationPanel::GetInstance()->AddButton("Next Texture Map", 275, infoStartY, 110, 24);
	InformationPanel::GetInstance()->AddButton("Prev Mipmap Level", 160, infoStartY + 32, 110, 24);
	InformationPanel::GetInstance()->AddButton("Next Mipmap Level", 275, infoStartY + 32, 110, 24);
	InformationPanel::GetInstance()->AddRadioButton("Voxel Buffer", 16, infoStartY, 60, 20, false);
	infoStartY += infoIncY;
	if( mVoxelizerType == SEVoxelizer::VT_Grid )
	{
		InformationPanel::GetInstance()->AddRadioButton("Voxel Grid", 16, infoStartY, 60, 20, false);
	}
	else if( mVoxelizerType == SEVoxelizer::VT_SVO )
	{
		InformationPanel::GetInstance()->AddRadioButton("SVO Grid", 16, infoStartY, 60, 20, false);
	}
	else
	{
		assert(false);
	}
	infoStartY += infoIncY;
	InformationPanel::GetInstance()->AddRadioButton("Scene Shadow Map", 16, infoStartY, 60, 20, false);
	infoStartY += infoIncY + 5;
	InformationPanel::GetInstance()->AddRadioButton("G-Buffer Position", 16, infoStartY, 60, 20, false);
	infoStartY += infoIncY;
	InformationPanel::GetInstance()->AddRadioButton("G-Buffer Normal", 16, infoStartY, 60, 20, false);
	infoStartY += infoIncY;
	InformationPanel::GetInstance()->AddRadioButton("G-Buffer Albedo", 16, infoStartY, 60, 20, false);
	if( mUseTC )
	{
		infoStartY += infoIncY;
		InformationPanel::GetInstance()->AddRadioButton("G-Buffer RPC", 16, infoStartY, 60, 20, false);
	}
	infoStartY += infoIncY;
	InformationPanel::GetInstance()->AddRadioButton("G-Buffer Splitter Position", 16, infoStartY, 60, 20, false);
	infoStartY += infoIncY;
	InformationPanel::GetInstance()->AddRadioButton("G-Buffer Splitter Normal", 16, infoStartY, 60, 20, false);
	infoStartY += infoIncY;
	InformationPanel::GetInstance()->AddRadioButton("RSM Position", 16, infoStartY, 60, 20, false);
	infoStartY += infoIncY;
	InformationPanel::GetInstance()->AddRadioButton("RSM Normal", 16, infoStartY, 60, 20, false);
	infoStartY += infoIncY;
	InformationPanel::GetInstance()->AddRadioButton("RSM Flux", 16, infoStartY, 60, 20, false);
	infoStartY += infoIncY;
	InformationPanel::GetInstance()->AddRadioButton("Direct Lighting", 16, infoStartY, 60, 20, false);
	infoStartY += infoIncY;
	InformationPanel::GetInstance()->AddRadioButton("Indirect Lighting", 16, infoStartY, 60, 20, false);
	infoStartY += infoIncY;
    InformationPanel::GetInstance()->AddRadioButton("Merged Buffer", 16, infoStartY, 60, 20, false);
    infoStartY += infoIncY;
	InformationPanel::GetInstance()->AddRadioButton("Filtered Indirect Lighting", 16, infoStartY, 60, 20, false);
	infoStartY += infoIncY;
	InformationPanel::GetInstance()->AddRadioButton("Final Result", 16, infoStartY, 60, 20, true);
	infoStartY += infoIncY;
	InformationPanel::GetInstance()->AddCheckBox("Enable Rotation", 16, infoStartY, 60, 20, false);
	infoStartY += infoIncY;
	InformationPanel::GetInstance()->AddCheckBox("Show Direct Shadow", 16, infoStartY, 60, 20, true);
	infoStartY += infoIncY;
	InformationPanel::GetInstance()->AddCheckBox("VPL Visibility Test", 16, infoStartY, 60, 20, true);
	infoStartY += infoIncY;
	InformationPanel::GetInstance()->AddCheckBox("Show VPL", 16, infoStartY, 60, 20, false);
	infoStartY += infoIncY;
	InformationPanel::GetInstance()->AddCheckBox("Show VPL Interleaved Sampling Subset", 16, infoStartY, 60, 20, false);
	infoStartY += infoIncY;
	InformationPanel::GetInstance()->AddCheckBox("Show VPL Flux Contrast", 16, infoStartY, 60, 20, false);
	infoStartY += infoIncY;
	InformationPanel::GetInstance()->AddCheckBox("Show RSM Flux Importance", 16, infoStartY, 60, 20, false);
	infoStartY += 24;
	InformationPanel::GetInstance()->AddButton("Prev VPL Subset", 16, infoStartY, 100, 24);
	InformationPanel::GetInstance()->AddButton("Next VPL Subset", 120, infoStartY, 100, 24);
    infoStartY += 28;
    InformationPanel::GetInstance()->AddCheckBox("Use HDR Tone Mapping", 16, infoStartY, 60, 20, true);
    infoStartY += 24;
    InformationPanel::GetInstance()->AddTextBox("Scene Light 1 Intensity", 16, infoStartY, 100, 24);
    InformationPanel::GetInstance()->AddButton("Update Intensity", 240, infoStartY, 100, 24);
    infoStartY += 24;
    InformationPanel::GetInstance()->AddTextBox("Scene Light 2 Intensity", 16, infoStartY, 100, 24);
    infoStartY += 24;
    InformationPanel::GetInstance()->AddTrackBar("HDR Exposure", 16, infoStartY, 200, 40, 50, 0, 50);
}
//----------------------------------------------------------------------------
void VPLviaSVOGI::FrameFunc()
{
#ifdef DEMO_CORNELL_SCENE
    mModel->UpdateRenderCache();

    static float angle = 60.0f;
    static float angle2 = 60.0f;
    if( mIsRotatingModel )
    {
        angle += 0.5f;
        SEMatrix4f rot;
        rot.FromAxisAngle(SEVector3f(0.0f, 1.0f, 0.0f), angle*SEMathf::DEG_TO_RAD);
        SEVector3f trans = mModel->GetWorldTranslation();
        mModel->SetWorldTransform(rot);
        mModel->SetWorldTranslation(trans);

        angle2 -= 1.0f;
        rot.FromAxisAngle(SEVector3f(0.0f, 1.0f, 0.0f), angle2*SEMathf::DEG_TO_RAD);
        trans = mModel2Sequence->GetWorldTranslation();
        mModel2Sequence->SetWorldTransform(rot);
        mModel2Sequence->SetWorldTranslation(trans);
    }
    mModel2Sequence->Update(FrameCounter);
#endif

#ifdef SHOW_TIMING
    InformationPanel^ infoPanel = InformationPanel::GetInstance();
    static double workLoad;
    static double totalWorkLoad;
    totalWorkLoad = 0.0;
#endif

    // Update light uniform buffer.
    mLightManager->SetLightBufferBindingPoint(1);
    mLightManager->UpdateLightBuffer();

    // Scene voxelization pass.
    mVoxelizer->Render(0, SMP_Voxelization);
#ifdef SHOW_TIMING
    workLoad = mVoxelizer->GetTimeElapsed();
    totalWorkLoad += workLoad;
    infoPanel->SetTimingLabelValue("Scene Voxelization Pass", workLoad);
#endif

    // Scene shadow pass.
    mShadowMapRenderer->SetShadowMapInfoBufferBindingPoint(2);
    mShadowMapRenderer->Render(0, SMP_PointLightShadowMap);
#ifdef SHOW_TIMING
    workLoad = mShadowMapRenderer->GetTimeElapsed();
    totalWorkLoad += workLoad;
    infoPanel->SetTimingLabelValue("Scene Shadow Pass", workLoad);
#endif

    // Scene G-buffer pass.
    mGBufferRenderer->Render(0, SMP_GBuffer, mMainCamera);
#ifdef SHOW_TIMING
    workLoad = mGBufferRenderer->GetTimeElapsed();
    totalWorkLoad += workLoad;
    infoPanel->SetTimingLabelValue("Scene G-buffer Pass", workLoad);
#endif

	// Scene G-buffer splitter pass.
	mGBufferSplitter->Render();
#ifdef SHOW_TIMING
	workLoad = mGBufferSplitter->GetTimeElapsed();
	totalWorkLoad += workLoad;
	infoPanel->SetTimingLabelValue("Scene G-buffer Splitter Pass", workLoad);
#endif

    // Scene light RSM pass.
    mRSMRenderer->SetRSMInfoBufferBindingPoint(3);
    mRSMRenderer->Render(0, SMP_PointLightRSM);
#ifdef SHOW_TIMING
    workLoad = mRSMRenderer->GetTimeElapsed();
    totalWorkLoad += workLoad;
    infoPanel->SetTimingLabelValue("RSM Pass", workLoad);
#endif

    // Sample RSM pass (VPL generation).
    mVPLGenerator->Run();
#ifdef SHOW_TIMING
    workLoad = mVPLGenerator->GetTimeElapsed();
    totalWorkLoad += workLoad;
    infoPanel->SetTimingLabelValue("VPL Creation Pass", workLoad);
#endif

    // Deferred direct illumination pass.
    mDirectLightingRenderer->Render();
#ifdef SHOW_TIMING
    workLoad = mDirectLightingRenderer->GetTimeElapsed();
    totalWorkLoad += workLoad;
    infoPanel->SetTimingLabelValue("Direct Lighting Pass", workLoad);
#endif

    // Deferred indirect illumination pass.
    mIndirectLightingRenderer->Render();
#ifdef SHOW_TIMING
    workLoad = mIndirectLightingRenderer->GetTimeElapsed();
    totalWorkLoad += workLoad;
    infoPanel->SetTimingLabelValue("Indirect Lighting Pass", workLoad);
#endif

    // Merge buffer pass.
    mSplittedBufferMerger->Render();
#ifdef SHOW_TIMING
    workLoad = mSplittedBufferMerger->GetTimeElapsed();
    totalWorkLoad += workLoad;
    infoPanel->SetTimingLabelValue("Merge Buffer Pass", workLoad);
#endif

	// Filter merged buffer pass.
	mGAwareFilter->Render();
#ifdef SHOW_TIMING
	workLoad = mGAwareFilter->GetTimeElapsed();
	totalWorkLoad += workLoad;
	infoPanel->SetTimingLabelValue("G-aware Filter Pass", workLoad);
#endif

    // Show rendering result.
    mVisualizer->Render(0, 0);
#ifdef SHOW_TIMING
    workLoad = mVisualizer->GetTimeElapsed();
    totalWorkLoad += workLoad;
    infoPanel->SetTimingLabelValue("Visualizer Pass", workLoad);
    infoPanel->SetTimingLabelValue("Total", totalWorkLoad);
    infoPanel->SetTimingLabelValue("Frame Counter", FrameCounter);
#endif

    // Visualize lights in the scene.
    mLightManager->RenderLightMesh(0, 0);
}
//----------------------------------------------------------------------------
void VPLviaSVOGI::Terminate()
{
	// Release all resources.

    mDeviceInspector = 0;
    mLightManager = 0;

    mVoxelizer = 0;
    mGBufferRenderer = 0;
	mGBufferSplitter = 0;
    mShadowMapRenderer = 0;
    mDirectLightingRenderer = 0;
    mIndirectLightingRenderer = 0;
	mSplittedBufferMerger = 0;
	mGAwareFilter = 0;
    mRSMRenderer = 0;
    mVPLGenerator = 0;
    mVisualizer = 0;

	mGround = 0;
	mCeiling = 0;
	mBackWall = 0;
	mLeftWall = 0;
	mRightWall = 0;
	mModel = 0;
    mModel2Sequence = 0;

    mSceneLight1 = 0;
    mSceneLight2 = 0;

	for( int i = 0; i < (int)mSponza.size(); ++i )
	{
		mSponza[i] = 0;
	}

    mGBufferObjects = 0;
	mRSMObjects = 0;
    mShadowCasters = 0;
    mVoxelizedObjects = 0;

    mTimer = 0;
    mInitPSB = 0;
}
//----------------------------------------------------------------------------
void VPLviaSVOGI::ProcessInput()
{
	if (glfwGetKey(Window, GLFW_KEY_T) == GLFW_PRESS)
	{
		mIsWireframe = !mIsWireframe;
	}
}
//----------------------------------------------------------------------------
void VPLviaSVOGI::OnRadioButtonClick(System::Object^ sender, 
    System::EventArgs^ e)
{
    RadioButton^ radioButton = (RadioButton^)sender;
    if( !mVisualizer )
    {
        return;
    }

    if( radioButton->Name == "Voxel Buffer" )
    {
        mVisualizer->SetShowMode(Visualizer::SM_VoxelBuffer);
    }

    if( radioButton->Name == "Voxel Grid" )
    {
        mVisualizer->SetShowMode(Visualizer::SM_VoxelGrid);
    }

    if( radioButton->Name == "SVO Grid" )
    {
        mVisualizer->SetShowMode(Visualizer::SM_SVOGrid);
    }

    if( radioButton->Name == "Scene Shadow Map" )
    {
        mVisualizer->SetShowMode(Visualizer::SM_Shadow);
    }

    if( radioButton->Name == "G-Buffer Position" )
    {
        mVisualizer->SetShowMode(Visualizer::SM_GBufferPosition);
    }

    if( radioButton->Name == "G-Buffer Normal" )
    {
        mVisualizer->SetShowMode(Visualizer::SM_GBufferNormal);
    }

    if( radioButton->Name == "G-Buffer Albedo" )
    {
        mVisualizer->SetShowMode(Visualizer::SM_GBufferAlbedo);
    }

    if( radioButton->Name == "G-Buffer RPC" )
    {
        mVisualizer->SetShowMode(Visualizer::SM_GBufferRPC);
    }

	if( radioButton->Name == "G-Buffer Splitter Position" )
	{
		mVisualizer->SetShowMode(Visualizer::SM_GBufferSplitterPosition);
	}

	if( radioButton->Name == "G-Buffer Splitter Normal" )
	{
		mVisualizer->SetShowMode(Visualizer::SM_GBufferSplitterNormal);
	}

    if( radioButton->Name == "RSM Position" )
    {
        mVisualizer->SetShowMode(Visualizer::SM_RSMPosition);
    }

    if( radioButton->Name == "RSM Normal" )
    {
        mVisualizer->SetShowMode(Visualizer::SM_RSMNormal);
    }

    if( radioButton->Name == "RSM Flux" )
    {
        mVisualizer->SetShowMode(Visualizer::SM_RSMFlux);
    }

    if( radioButton->Name == "Direct Lighting" )
    {
        mVisualizer->SetShowMode(Visualizer::SM_DirectLighting);
    }

    if( radioButton->Name == "Indirect Lighting" )
    {
        mVisualizer->SetShowMode(Visualizer::SM_IndirectLighting);
    }

    if( radioButton->Name == "Merged Buffer" )
    {
        mVisualizer->SetShowMode(Visualizer::SM_MergedBuffer);
    }

    if( radioButton->Name == "Filtered Indirect Lighting" )
    {
        mVisualizer->SetShowMode(Visualizer::SM_FilteredIndirectLighting);
    }

    if( radioButton->Name == "Final Result" )
    {
        mVisualizer->SetShowMode(Visualizer::SM_Final);
    }
}
//----------------------------------------------------------------------------
void VPLviaSVOGI::OnCheckBoxClick(System::Object^ sender, 
    System::EventArgs^ e)
{
    CheckBox^ checkBox = (CheckBox^)sender;

    if( checkBox->Name == "Show Direct Shadow" )
    {
        if( !mDirectLightingRenderer )
        {
            return;
        }

        mDirectLightingRenderer->ShowShadow(checkBox->Checked);
    }

    if( checkBox->Name == "Enable Rotation" )
    {
        mIsRotatingModel = checkBox->Checked;
    }

    if( checkBox->Name == "VPL Visibility Test" )
    {
        if( !mIndirectLightingRenderer )
        {
            return;
        }

        mIndirectLightingRenderer->VPLVisibilityTest(checkBox->Checked);
    }

    if( checkBox->Name == "Show VPL" )
    {
        if( !mVisualizer )
        {
            return;
        }

        mVisualizer->SetShowVPL(checkBox->Checked);
    }

    if( checkBox->Name == "Show VPL Interleaved Sampling Subset" )
    {
        if( !mVisualizer )
        {
            return;
        }

        mVisualizer->SetShowVPLSubset(checkBox->Checked);
    }

    if( checkBox->Name == "Show VPL Flux Contrast" )
    {
        if( !mVisualizer )
        {
            return;
        }

        mVisualizer->SetShowVPLFluxContrast(checkBox->Checked);
    }

	if( checkBox->Name == "Show RSM Flux Importance" )
	{
		if( !mVisualizer )
		{
			return;
		}

		mVisualizer->SetShowRSMFluxImportance(checkBox->Checked);
	}

    if( checkBox->Name == "Use HDR Tone Mapping" )
    {
        if( !mVisualizer )
        {
            return;
        }

        mVisualizer->SetUseHDRToneMapping(checkBox->Checked);
    }
}
//----------------------------------------------------------------------------
void VPLviaSVOGI::OnButtonClick(System::Object^  sender,
    System::EventArgs^  e)
{
    Button^ button = (Button^)sender;

    if( button->Name == "Prev VPL Subset" )
    {
        int curVPLSubsetIndex = mVisualizer->GetCurVPLSubsetIndex();
        --curVPLSubsetIndex;
        mVisualizer->SetCurVPLSubsetIndex(curVPLSubsetIndex);
    }

    if( button->Name == "Next VPL Subset" )
    {
        int curVPLSubsetIndex = mVisualizer->GetCurVPLSubsetIndex();
        ++curVPLSubsetIndex;
        mVisualizer->SetCurVPLSubsetIndex(curVPLSubsetIndex);
    }

    if( button->Name == "Prev Texture Map" )
    {
        int curShadowMapIndex = mVisualizer->GetCurTextureMapIndex();
        --curShadowMapIndex;
        mVisualizer->SetCurTextureMapIndex(curShadowMapIndex);
    }

    if( button->Name == "Next Texture Map" )
    {
        int curShadowMapIndex = mVisualizer->GetCurTextureMapIndex();
        ++curShadowMapIndex;
        mVisualizer->SetCurTextureMapIndex(curShadowMapIndex);
    }

	if( button->Name == "Prev Mipmap Level" )
	{
		int curMipmapLevel = mVisualizer->GetCurMipmapLevel();
		--curMipmapLevel;
		mVisualizer->SetCurMipmapLevel(curMipmapLevel);
	}

	if( button->Name == "Next Mipmap Level" )
	{
		int curMipmapLevel = mVisualizer->GetCurMipmapLevel();
		++curMipmapLevel;
		mVisualizer->SetCurMipmapLevel(curMipmapLevel);
	}

    if( button->Name == "Update Intensity" )
    {
        if( mSceneLight1 )
        {
            String^ light1Intensity = InformationPanel::GetInstance()->GetTextBoxValue("Scene Light 1 Intensity");
            array<String^>^ rgbString = light1Intensity->Split(',');
            if( rgbString && rgbString->Length == 3 )
            {
                int i = 0;
                for each (String^ var in rgbString)
                {
                    mSceneLight1->Intensity[i++] = (float)System::Convert::ToDouble(var);
                }
            }
        }

        if( mSceneLight2 )
        {
            String^ light2Intensity = InformationPanel::GetInstance()->GetTextBoxValue("Scene Light 2 Intensity");
            array<String^>^ rgbString = light2Intensity->Split(',');
            if( rgbString && rgbString->Length == 3 )
            {
                int i = 0;
                for each (String^ var in rgbString)
                {
                    mSceneLight2->Intensity[i++] = (float)System::Convert::ToDouble(var);
                }
            }
        }
    }
}
//----------------------------------------------------------------------------
void VPLviaSVOGI::OnTrackBarScroll(System::Object^ sender, System::EventArgs^ e)
{
    TrackBar^ trackBar = (TrackBar^)sender;

    if( trackBar->Name == "HDR Exposure" )
    {
        float p = (float)trackBar->Value / (float)trackBar->Maximum;
        mVisualizer->SetHDRExposure(p);
    }
}
//----------------------------------------------------------------------------