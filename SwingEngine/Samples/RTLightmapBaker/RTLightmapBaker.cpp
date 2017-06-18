#include "RTLightmapBaker.h"
#include "SERTGICamera.h"
#include "SEObjLoader.h"

using namespace Swing;

//----------------------------------------------------------------------------
RTLightmapBaker::RTLightmapBaker(int width, int height)
{
    Width = width;
    Height = height;
    Title = "Swing Engine V-Ray Lightmap Baker";

    mImgMutex = nullptr;
    mBmpMutex = nullptr;
    mImg = nullptr;
    mBmp = nullptr;
    mFrameNumber = 0;
    mFinished = false;
    mStop = false;
}
//----------------------------------------------------------------------------
RTLightmapBaker::~RTLightmapBaker()
{
}
//----------------------------------------------------------------------------
void RTLightmapBaker::Initialize(SEApplicationDescription* ApplicationDesc)
{
    mImgMutex = SEMutex::Create();
    mBmpMutex = SEMutex::Create();

    // Setup renderer delegate.
    mRayTracingDevice->SetOnRenderStart<RTLightmapBaker, &RTLightmapBaker::OnRenderStart>(*this, nullptr);
    mRayTracingDevice->SetOnImageReady<RTLightmapBaker, &RTLightmapBaker::OnImageReady>(*this, nullptr);
    mRayTracingDevice->SetOnRTImageUpdated<RTLightmapBaker, &RTLightmapBaker::OnRTImageUpdated>(*this, nullptr);
    mRayTracingDevice->SetOnDeviceClose<RTLightmapBaker, &RTLightmapBaker::OnDeviceClose>(*this, nullptr);
    mRayTracingDevice->SetOnDumpMessage<RTLightmapBaker, &RTLightmapBaker::OnDumpMessage>(*this, nullptr);

    const char* sceneFileName = OpenFileDialog("V-Ray scene files (*.vrscene)\0*.vrscene\0All files (*.*)\0*.*\0", "Choose V-Ray scene file to load");
    if( !sceneFileName )
    {
        puts("Scene file not loaded.");
        return;
    }

    printf("Loading %s...\n", sceneFileName);
    bool sceneLoaded = mRayTracingDevice->LoadNativeScene(sceneFileName);
    if( !sceneLoaded )
    {
        puts("Scene file is corrupted or doesn't exist.");
    }

    mMainCamera->SetLookAt(SEVector3f(2.53571f, 82.6768f, -140.814f), SEVector3f(2.53571f, 582.6768f, 0.0f), SEVector3f(0.0f, 1.0f, 0.0f));
    mRTDeviceCamera = SE_NEW SERTDeviceCamera();
    mRTDeviceCamera->CreateDeviceResource(*mRayTracingDevice);
    mRTDeviceCamera->SetTransformFromCamera((SEICamera*)mMainCamera);

    mLightRect01 = SE_NEW SERTGILight(LT_Rectangle);
    mLightRect01->Color = SEColorRGB(30.0f, 30.0f, 30.0f);
    mLightRect01->SetLocation(SEVector3f(16.0f, 196.0f, 0.0f));
    mLightRect01->Width = 30.0f;
    mLightRect01->Height = 30.0f;

    mRTDeviceLightRect01 = SE_NEW SERTDeviceLightRectangle();
    mRTDeviceLightRect01->CreateDeviceResource(*mRayTracingDevice, (SEILight*)mLightRect01);

    // Create couch mesh.
    SEObjLoader objLoader;
    std::vector<SEObjMetaMeshPtr> objMeshes;
    objLoader.Load("F:\\Work\\SwingEngine2\\SwingEngine\\Bin\\OBJ\\", "couch.obj", objMeshes);

    mRTDeviceMesh01 = SE_NEW SERTDeviceStaticMesh();
    mRTDeviceMesh01->CreateDeviceResource(*mRayTracingDevice, (SEIMetaMesh*)objMeshes[0]);

    // Create couch material.
    mRTDeviceMesh01Material = SE_NEW SERTDeviceSingleBRDFMaterial();
    mRTDeviceMesh01Material->CreateDeviceResource(*mRayTracingDevice);

    // Create couch node.
    mRTDeviceMesh01Node = SE_NEW SERTDeviceSceneNode();
    mRTDeviceMesh01Node->CreateDeviceResource(*mRayTracingDevice);
    mRTDeviceMesh01Node->SetGeometry((SERTDeviceGeometry*)mRTDeviceMesh01);
    mRTDeviceMesh01Node->SetMaterial((SERTDeviceMaterial*)mRTDeviceMesh01Material);

    SEMatrix3f scale(0.5f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f,
        0.0f, 0.0f, 0.5f);
    SEMatrix3f rot;
    rot.FromAxisAngle(SEVector3f(0.0f, 1.0f, 0.0f), -90.0f*SEMathf::DEG_TO_RAD);
    SEMatrix3f mat = scale*rot;

    SEVector3f vec(43.5471f, 20.0f, -10.0f);
    mRTDeviceMesh01Node->SetTransform(&mat, &vec);

    // Create couch node bake view.
    SERTDeviceBakeViewDescription bakeViewDesc;
    bakeViewDesc.BakeNode = mRTDeviceMesh01Node;

    mRTDeviceMesh01BakeView = SE_NEW SERTDeviceBakeView();
    mRTDeviceMesh01BakeView->CreateDeviceResource(*mRayTracingDevice, &bakeViewDesc);

    mRayTracingDevice->AddRenderElement(RTDRET_Diffuse);
    mRayTracingDevice->AddRenderElement(RTDRET_Lighting);
    mRayTracingDevice->AddRenderElement(RTDRET_GI);

    mRayTracingDevice->Render();
}
//----------------------------------------------------------------------------
void RTLightmapBaker::FrameFunc()
{
}
//----------------------------------------------------------------------------
void RTLightmapBaker::Terminate()
{
    if( mImgMutex )
    {
        SEMutex::Destroy(mImgMutex);
        mImgMutex = nullptr;
    }

    if( mBmpMutex )
    {
        SEMutex::Destroy(mBmpMutex);
        mBmpMutex = nullptr;
    }

    mRTDeviceCamera = nullptr;
    mLightRect01 = nullptr;
    mRTDeviceLightRect01 = nullptr;

    mRTDeviceMesh01Node = nullptr;
    mRTDeviceMesh01 = nullptr;
    mRTDeviceMesh01Material = nullptr;

    mRTDeviceMesh01BakeView = nullptr;
}
//----------------------------------------------------------------------------
void RTLightmapBaker::ProcessInput()
{
}
//----------------------------------------------------------------------------
void RTLightmapBaker::OnSizing(int newWidth, int newHeight)
{
    mRayTracingDevice->SetImageSize(newWidth, newHeight);
}
//----------------------------------------------------------------------------
void RTLightmapBaker::OnSize(int left, int top, int right, int bottom)
{
    if( !mImg )
    {
        return;
    }

    {
        SEMutexLock lock(*mBmpMutex);

        SERayTracingDeviceBitmap* newBmp = mImg->CreateRTBitmap(right, bottom);
        SE_DELETE mBmp;
        mBmp = newBmp;
    }
}
//----------------------------------------------------------------------------
void RTLightmapBaker::OnWindowClose()
{
    mStop = true;
}
//----------------------------------------------------------------------------
void RTLightmapBaker::OnPaintGetBitmapData(void*& bitmapPixels, void*& bitmapInfoHeader)
{
    if( mBmp )
    {
        bitmapPixels = mBmp->GetPixels();
        bitmapInfoHeader = mBmp->GetBitmapInfoHeader();
    }
}
//----------------------------------------------------------------------------
void RTLightmapBaker::OnRenderStart(SERayTracingDevice& rtDevice, void*)
{
    mFrameNumber = 0;
    mFinished = false;
    mBmp = nullptr;

    printf("\n Rendering started. \n");
}
//----------------------------------------------------------------------------
void RTLightmapBaker::OnImageReady(SERayTracingDevice& rtDevice, void*)
{
    mFinished = true;
    SERayTracingDeviceImage* img = rtDevice.GetImage();
    UpdateImage(img);

    printf("\n Image is ready: %u \n", ++mFrameNumber);
}
//----------------------------------------------------------------------------
void RTLightmapBaker::OnDeviceClose(SERayTracingDevice& rtDevice, void*)
{
    puts("\n Renderer closed.");
}
//----------------------------------------------------------------------------
void RTLightmapBaker::OnRTImageUpdated(SERayTracingDevice& rtDevice, SERayTracingDeviceImage* img, void*)
{
    printf("Image updated: %u \n", ++mFrameNumber);

    if( mFrameNumber == 10 )
    {
        //bool res = img->SaveToBmpFile("F:\\Work\\SwingEngine2\\SwingEngine\\Bin\\test.bmp", false, false);

        SERTDeviceRenderElement* reDiffuse = mRayTracingDevice->GetRenderElement(RTDRET_Diffuse);
        //if( reDiffuse )
        //{
        //    reDiffuse->SaveRenderElementToFile("F:\\Work\\SwingEngine2\\SwingEngine\\Bin\\reDiffuse.bmp", RTDIFT_BMP);
        //}

        SERTDeviceRenderElement* reLighting = mRayTracingDevice->GetRenderElement(RTDRET_Lighting);
        //if( reLighting )
        //{
        //    reLighting->SaveRenderElementToFile("F:\\Work\\SwingEngine2\\SwingEngine\\Bin\\reLighting.bmp", RTDIFT_BMP);
        //}

        SERTDeviceRenderElement* reGI = mRayTracingDevice->GetRenderElement(RTDRET_GI);
        //if( reGI )
        //{
        //    reGI->SaveRenderElementToFile("F:\\Work\\SwingEngine2\\SwingEngine\\Bin\\reGI.bmp", RTDIFT_BMP);
        //}

        mRayTracingDevice->GenerateLightMapFromRenderElements(reDiffuse, reLighting, reGI, "test.tga", RTDIFT_TGA);
    }
    UpdateImage(img);
}
//----------------------------------------------------------------------------
void RTLightmapBaker::OnDumpMessage(SERayTracingDevice& rtDevice, const char* msg, int level, void*)
{
    printf("%s (%u)\n", msg, level);
}
//----------------------------------------------------------------------------
void RTLightmapBaker::UpdateImage(SERayTracingDeviceImage* image)
{
    if( mStop || !image )
    {
        return;
    }

    SERayTracingDeviceImage* oldImage = this->mImg;
    {
        SEMutexLock lock(*mImgMutex);
        mImg = image;
    }
    SE_DELETE oldImage;

    Refresh();
}
//----------------------------------------------------------------------------