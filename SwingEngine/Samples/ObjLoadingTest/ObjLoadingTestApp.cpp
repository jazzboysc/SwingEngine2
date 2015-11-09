#include "ObjLoadingTestApp.h"
#include <GLFW\glfw3.h>

using namespace Swing;

//----------------------------------------------------------------------------
ObjLoadingTestApp::ObjLoadingTestApp(int width, int height)
{
    Width = width;
    Height = height;
    Title = "Obj Loading Test";
	mIsWireframe = false;
	ShowMode = 0;
}
//----------------------------------------------------------------------------
ObjLoadingTestApp::~ObjLoadingTestApp()
{
}
//----------------------------------------------------------------------------
void ObjLoadingTestApp::Initialize(SEGPUDeviceBase* device)
{
    SEGPUDevice* gpuDevice = (SEGPUDevice*)device;

    // Create scene camera.
    mMainCamera->SetPerspectiveFrustum(45.0f, (float)Width / (float)Height, 0.5f, 4000.0f);
    mMainCamera->SetLookAt(SEVector3f(-850.0f, 350.0f, -10.0f), 
        SEVector3f(200.0f, 200.0f, -10.0f), SEVector3f(0.0f, 1.0f, 0.0f));
	mMainCameraSpeed = 500.0f;
    //mMainCamera->SetPerspectiveFrustum(45.0f, (float)Width / (float)Height, 0.01f, 150.0f);
    //mMainCamera->SetLookAt(SEVector3f(0.0f, 10.0f, -35.0f), SEVector3f(0.0f, 10.0f, 0.0f),
    //    SEVector3f(0.0f, 1.0f, 0.0f));

    // Create material templates.
    SEMaterial* material = 0;

    SEShaderProgramInfo objMeshProgramInfo;
    objMeshProgramInfo.VShaderFileName = "ObjLoadingTest/vObjMesh.glsl";
    objMeshProgramInfo.FShaderFileName = "ObjLoadingTest/fObjMesh.glsl";
    objMeshProgramInfo.ShaderStageFlag = SEShaderStage::SS_Vertex |
                                         SEShaderStage::SS_Fragment;
    SEPass* passObjMesh = new SEPass(objMeshProgramInfo);

    SETechnique* techObjMesh = new SETechnique();
    techObjMesh->AddPass(passObjMesh);
    SEMaterialTemplate* mtObjMesh = new SEMaterialTemplate();
    mtObjMesh->AddTechnique(techObjMesh);

    //material = new SEMaterial(mtObjMesh);
    //ObjMesh* mesh = new ObjMesh(material, mMainCamera);
    //mesh->LoadFromPLYFile("lucy.ply");
    //mesh->GenerateNormals();
    //SEMatrix4f rotM;
    //rotM.FromEulerAnglesXYZ(SEMathf::HALF_PI, SEMathf::PI, 0.0f);
    //mesh->UpdateModelSpaceVertices(rotM);
    //mesh->SetWorldTranslation(SEVector3f(0.0f, 0.0f, 250.0f));
    //mesh->SetWorldScale(SEVector3f(0.1f, 0.1f, 0.1f));
    //mesh->CreateDeviceResource(gpuDevice);
    //mesh->App = this;
    //mMeshes.push_back(mesh);


    // Create scene.
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
        bool isExcluded = false;
        for( int j = 0; j < (int)ignored.size(); ++j )
        {
            if( objMeshes[i]->MeshName == ignored[j] )
            {
                isExcluded = true;
                break;
            }
        }

        if( !isExcluded )
        {
            material = new SEMaterial(mtObjMesh);
            ObjMesh* mesh = new ObjMesh(material, mMainCamera);
            mesh->LoadFromSEMetaMesh(objMeshes[i]);
            mesh->GenerateNormals();
            mesh->CreateDeviceResource(gpuDevice);
			mesh->App = this;
            mMeshes.push_back(mesh);
        }
	}

    //SEMatrix4f rotM;
    //material = new SEMaterial(mtObjMesh);
    //ObjMesh* mesh = new ObjMesh(material, mMainCamera);
    //mesh->LoadFromPLYFile("dragon_s.ply");
    //mesh->GenerateNormals();
    //mesh->CreateDeviceResource(gpuDevice);
    //rotM.FromAxisAngle(SEVector3f(0.0f, 1.0f, 0.0f), -30.0f*SEMathf::DEG_TO_RAD);
    //mesh->SetWorldTransform(rotM);
    //mesh->SetWorldTranslation(SEVector3f(-5.4f, 3.2f, -1.6f));
    //mesh->SetWorldScale(SEVector3f(45.0f, 45.0f, 45.0f));
    //mMeshes.push_back(mesh);
    //mModel = mesh;

    //mModel2Sequence = new SERenderSequence(0);
    //for( int i = 1; i <= 12; ++i )
    //{
    //    material = new SEMaterial(mtObjMesh);
    //    ObjMesh* model2 = new ObjMesh(material, mMainCamera);
    //    char objFileName[64];
    //    sprintf(objFileName, "elephant-gallop/elephant-gallop-%d.obj", i);
    //    model2->LoadFromOBJFile(objFileName);
    //    model2->GenerateNormals();
    //    model2->CreateDeviceResource(gpuDevice);
    //    rotM.FromAxisAngle(SEVector3f(0.0f, 1.0f, 0.0f), 60.0f*SEMathf::DEG_TO_RAD);
    //    model2->SetWorldTransform(rotM);
    //    model2->SetWorldTranslation(SEVector3f(3.2f, 3.2f, -2.4f));
    //    model2->SetWorldScale(SEVector3f(8.0f, 8.0f, 8.0f));
    //    mModel2Sequence->AddRenderObject(model2);
    //}
    //mModel2Sequence->SetActiveRenderObject(0);
    //mModel2Sequence->SetFrequence(0.4f);

    //// Ground.
    //material = new SEMaterial(mtObjMesh);
    //mesh = new ObjMesh(material, mMainCamera);
    //mesh->LoadFromPLYFile("square.ply");
    //mesh->GenerateNormals();
    //mesh->CreateDeviceResource(gpuDevice);
    //mMeshes.push_back(mesh);

    //// Ceiling.
    //material = new SEMaterial(mtObjMesh);
    //mesh = new ObjMesh(material, mMainCamera);
    //mesh->LoadFromPLYFile("square.ply");
    //mesh->GenerateNormals();
    //mesh->CreateDeviceResource(gpuDevice);
    //rotM.FromAxisAngle(SEVector3f(1.0f, 0.0f, 0.0f), 180.0f*SEMathf::DEG_TO_RAD);
    //mesh->SetWorldTransform(rotM);
    //mesh->SetWorldTranslation(SEVector3f(0.0f, 20.0f, 0.0f));
    //mMeshes.push_back(mesh);

    //// Back wall.
    //material = new SEMaterial(mtObjMesh);
    //mesh = new ObjMesh(material, mMainCamera);
    //mesh->LoadFromPLYFile("square.ply");
    //mesh->GenerateNormals();
    //mesh->CreateDeviceResource(gpuDevice);
    //rotM.FromAxisAngle(SEVector3f(1.0f, 0.0f, 0.0f), -90.0f*SEMathf::DEG_TO_RAD);
    //mesh->SetWorldTransform(rotM);
    //mesh->SetWorldTranslation(SEVector3f(0.0f, 10.0f, 10.0f));
    //mMeshes.push_back(mesh);

    //// Left wall.
    //material = new SEMaterial(mtObjMesh);
    //mesh = new ObjMesh(material, mMainCamera);
    //mesh->LoadFromPLYFile("square.ply");
    //mesh->GenerateNormals();
    //mesh->CreateDeviceResource(gpuDevice);
    //rotM.FromAxisAngle(SEVector3f(0.0f, 0.0f, 1.0f), -90.0f*SEMathf::DEG_TO_RAD);
    //mesh->SetWorldTransform(rotM);
    //mesh->SetWorldTranslation(SEVector3f(-10.0f, 10.0f, 0.0f));
    //mMeshes.push_back(mesh);

    //material = new SEMaterial(mtObjMesh);
    //mesh = new ObjMesh(material, mMainCamera);
    //mesh->LoadFromPLYFile("square.ply");
    //mesh->GenerateNormals();
    //mesh->CreateDeviceResource(gpuDevice);
    //rotM.FromAxisAngle(SEVector3f(0.0f, 0.0f, 1.0f), 90.0f*SEMathf::DEG_TO_RAD);
    //mesh->SetWorldTransform(rotM);
    //mesh->SetWorldTranslation(SEVector3f(10.0f, 10.0f, 0.0f));
    //mMeshes.push_back(mesh);

    // Create PSBs.

    mWireframePSB = new SEPipelineStateBlock();
    mWireframePSB->PipelineStageFlag |= PB_OutputMerger;
    mWireframePSB->PipelineStageFlag |= PB_Rasterizer;
    mWireframePSB->Rasterizer.RasterizerOpFlag |= RB_FillMode;
    mWireframePSB->Rasterizer.FillMode = PFM_Wireframe;
    mWireframePSB->OutputMerger.OutputMergerOpFlag |= OMB_Clear | OMB_ClearColor;
    mWireframePSB->OutputMerger.ClearColor = SEVector4f(0.5f, 0.5f, 0.5f, 1.0f);
    mWireframePSB->OutputMerger.ClearMask = OMCB_Color_Buffer | OMCB_Depth_Buffer;
    mWireframePSB->CreateDeviceResource(gpuDevice);

    mSolidPSB = new SEPipelineStateBlock();
    mSolidPSB->PipelineStageFlag |= PB_OutputMerger;
    mSolidPSB->PipelineStageFlag |= PB_Rasterizer;
    mSolidPSB->Rasterizer.RasterizerOpFlag |= RB_FillMode;
    mSolidPSB->Rasterizer.FillMode = PFM_Solid;
    mSolidPSB->OutputMerger.OutputMergerOpFlag |= OMB_Clear | OMB_ClearColor;
    mSolidPSB->OutputMerger.ClearColor = SEVector4f(0.5f, 0.5f, 0.5f, 1.0f);
    mSolidPSB->OutputMerger.ClearMask = OMCB_Color_Buffer | OMCB_Depth_Buffer;
    mSolidPSB->CreateDeviceResource(gpuDevice);
}
//----------------------------------------------------------------------------
void ObjLoadingTestApp::FrameFunc()
{
    //static float angle = 60.0f;
    //static float angle2 = 60.0f;
    //if( true )
    //{
    //    angle += 0.5f;
    //    SEMatrix4f rot;
    //    rot.FromAxisAngle(SEVector3f(0.0f, 1.0f, 0.0f), angle*SEMathf::DEG_TO_RAD);
    //    SEVector3f trans;
    //    trans = mModel->GetWorldTranslation();
    //    mModel->SetWorldTransform(rot);
    //    mModel->SetWorldTranslation(trans);

    //    angle2 -= 1.0f;
    //    rot.FromAxisAngle(SEVector3f(0.0f, 1.0f, 0.0f), angle2*SEMathf::DEG_TO_RAD);
    //    trans = mModel2Sequence->GetWorldTranslation();
    //    mModel2Sequence->SetWorldTransform(rot);
    //    mModel2Sequence->SetWorldTranslation(trans);
    //}
    //mModel2Sequence->Update(FrameCounter);

	if( mIsWireframe )
	{
        ((SEGPUDevice*)(SEGPUDeviceBase*)mDevice)->ApplyPipelineStateBlock(mWireframePSB);
	}
	else
	{
        ((SEGPUDevice*)(SEGPUDeviceBase*)mDevice)->ApplyPipelineStateBlock(mSolidPSB);
	}

	for( int i = 0; i < (int)mMeshes.size(); ++i )
	{
		mMeshes[i]->Render(0, 0);
	}
    //mModel2Sequence->Render(0, 0);
}
//----------------------------------------------------------------------------
void ObjLoadingTestApp::Terminate()
{
	for( int i = 0; i < (int)mMeshes.size(); ++i )
	{
		mMeshes[i] = 0;
	}
    mModel = 0;
    mModel2Sequence = 0;
    mWireframePSB = 0;
    mSolidPSB = 0;
}
//----------------------------------------------------------------------------
void ObjLoadingTestApp::ProcessInput()
{
	if( glfwGetKey(Window, GLFW_KEY_T) == GLFW_PRESS )
	{
		mIsWireframe = true;
	}
	if( glfwGetKey(Window, GLFW_KEY_Y) == GLFW_PRESS )
	{
		mIsWireframe = false;
	}
	if( glfwGetKey(Window, GLFW_KEY_U) == GLFW_PRESS )
	{
		ShowMode = 0;
	}
	if( glfwGetKey(Window, GLFW_KEY_I) == GLFW_PRESS )
	{
		ShowMode = 1;
	}
	if( glfwGetKey(Window, GLFW_KEY_O) == GLFW_PRESS )
	{
		ShowMode = 2;
	}
}
//----------------------------------------------------------------------------