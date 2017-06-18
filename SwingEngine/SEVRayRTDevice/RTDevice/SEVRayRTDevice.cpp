// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SEVRayRTDevicePCH.h"
#include "SEVRayRTDevice.h"
#include "SERayTracingDeviceImage.h"
#include "SERayTracingDeviceBitmap.h"
#include "SERTDeviceCamera.h"
#include "SERTDeviceLightRectangle.h"
#include "SERTDeviceSkyLight.h"
#include "SERTDeviceStaticMesh.h"
#include "SECoordinateSystemAdapter.h"
#include "SERTDeviceSceneNode.h"
#include "SERTDeviceBakeView.h"
#include "SERTDeviceRenderElement.h"

#ifdef _WIN32
#pragma warning(disable:4189)
#endif

#define VRAY_RUNTIME_LOAD_PRIMARY
#include "vraysdk.hpp"
#include "vrayplugins.hpp"

using namespace Swing;
using namespace VRay;
using namespace VRay::Plugins;

const SE_Int32 SEVRayRTDevice::gsRenderMode[RTDRM_MAX - 1] =
{
    RendererOptions::RENDER_MODE_RT_CPU,
    RendererOptions::RENDER_MODE_RT_GPU_CUDA,
    RendererOptions::RENDER_MODE_RT_GPU_OPENCL,
    RendererOptions::RENDER_MODE_PRODUCTION,
    RendererOptions::RENDER_MODE_PRODUCTION_CUDA,
    RendererOptions::RENDER_MODE_PRODUCTION_OPENCL
};

const SE_Int32 SEVRayRTDevice::gsRenderElementType[RTDRET_Max - 1] =
{
    RenderElement::Type::DIFFUSE,
    RenderElement::Type::LIGHTING,
    RenderElement::Type::GI,
    RenderElement::Type::TOTALLIGHT,
    RenderElement::Type::RAWLIGHT,
    RenderElement::Type::RAWGI,
    RenderElement::Type::RAWTOTALLIGHT
};

//----------------------------------------------------------------------------
void SEVRayRTDevice::InsertRayTracingDeviceFunctions()
{
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(Initialize, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(Terminate, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(LoadNativeScene, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(CreateRTImage, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(DeleteRTImage, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(CreateRTBitmap, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(DeleteRTBitmap, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(GetImageSize, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(SetImageSize, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(Render, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(GetImage, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(RTBitmapGetPixels, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(RTBitmapGetInfoHeader, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(RTImageSaveToBmpFile, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(CreateRTDeviceCamera, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(DeleteRTDeviceCamera, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(SetTransformFromCamera, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(CreateRTDeviceLightRectangle, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(DeleteRTDeviceLightRectangle, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(CreateRTDeviceSkyLight, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(DeleteRTDeviceSkyLight, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(CreateRTDeviceStaticMesh, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(DeleteRTDeviceStaticMesh, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(CreateSceneNode, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(DeleteSceneNode, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(SceneNodeSetTransform, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(SceneNodeSetGeometry, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(SceneNodeSetMaterial, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(CreateMaterial, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(DeleteMaterial, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(CreateBakeView, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(DeleteBakeView, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(AddRenderElement, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(DeleteRenderElement, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(SaveRenderElementToFile, SEVRayRTDevice);
    SE_INSERT_RAY_TRACING_DEVICE_FUNC(GenerateLightMapFromRenderElements, SEVRayRTDevice);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
SEVRayRTDevice::SEVRayRTDevice()
    :
    mVRayInit(nullptr),
    mVRayRenderer(nullptr),
    mImageOpMutex(nullptr)
{
    InsertRayTracingDeviceFunctions();
}
//----------------------------------------------------------------------------
SEVRayRTDevice::~SEVRayRTDevice()
{
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__Initialize(SERayTracingDeviceDescription* deviceDesc)
{
    if( mVRayInit )
    {
        return;
    }

    mImageOpMutex = SEMutex::Create();

    puts("Initializing VRay...");
    // Initializing without VRay framebuffer support.
    mVRayInit = new VRayInit("VRaySDKLibrary", false);
    puts("Done.");

    if (deviceDesc->RenderMode == RTDRM_Unknown)
    {
        puts("Unknown Render Mode.");
        return;
    }

    RendererOptions options;
    options.imageWidth = deviceDesc->ImageWidth;
    options.imageHeight = deviceDesc->ImageHeight;
    options.rtNoiseThreshold = 0;
    options.renderMode = (RendererOptions::RenderMode)gsRenderMode[(int)deviceDesc->RenderMode - 1];

    // Create VRay renderer.
    puts("Initializing Renderer...\n");
    mVRayRenderer = new VRayRenderer(options);
    mVRayRenderer->setRTImageUpdateDifference(2);
    mVRayRenderer->setRTImageUpdateTimeout(5000);
    mVRayRenderer->setKeepRTframesInCallback(true);

    // Setup renderer settings.
    Plugin settingsRTEngine = mVRayRenderer->getInstanceOrCreate("settingsRTEngine", "SettingsRTEngine");
    settingsRTEngine.setValue("undersampling", false);
    settingsRTEngine.setValue("gi_depth", 1);
    settingsRTEngine.setValue("gpu_bundle_size", 128);
    settingsRTEngine.setValue("gpu_samples_per_pixel", 16);
    
    // Setup callback functions.
    mVRayRenderer->setOnRenderStart<SEVRayRTDevice, &SEVRayRTDevice::__OnRenderStart>(*this, nullptr);
    mVRayRenderer->setOnImageReady<SEVRayRTDevice, &SEVRayRTDevice::__OnImageReady>(*this, nullptr);
    mVRayRenderer->setOnRTImageUpdated<SEVRayRTDevice, &SEVRayRTDevice::__OnRTimageUpdated>(*this, nullptr);
    mVRayRenderer->setOnRendererClose<SEVRayRTDevice, &SEVRayRTDevice::__OnDeviceClose>(*this, nullptr);
    mVRayRenderer->setOnDumpMessage<SEVRayRTDevice, &SEVRayRTDevice::__OnDumpMessage>(*this, nullptr);
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__Terminate()
{
    if( mVRayInit )
    {
        puts("Terminating VRay...");
        delete mVRayInit;
        mVRayInit = nullptr;
        puts("Done.");
    }

    if( mVRayRenderer )
    {
        delete mVRayRenderer;
        mVRayRenderer = nullptr;
    }

    if( mImageOpMutex )
    {
        SEMutex::Destroy(mImageOpMutex);
        mImageOpMutex = nullptr;
    }

    for( int i = 0; i < RTDRET_Max; ++i )
    {
        SE_DELETE mRenderElements[i];
        mRenderElements[i] = nullptr;
    }
}
//----------------------------------------------------------------------------
bool SEVRayRTDevice::__LoadNativeScene(const char* fileName)
{
    if( mVRayRenderer )
    {
        return mVRayRenderer->load(fileName) == 0 ? true : false;
    }

    return false;
}
//----------------------------------------------------------------------------
SERTImageHandle* SEVRayRTDevice::__CreateRTImage(SERayTracingDeviceImage*)
{
    SERTImageHandle* imageHandle = SE_NEW SEVRayRTImageHandle();
    imageHandle->RTDevice = this;
    return imageHandle;
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__DeleteRTImage(SERayTracingDeviceImage* img)
{
    if( img )
    {
        SEVRayRTImageHandle* imageHandle = (SEVRayRTImageHandle*)img->GetImageHandle();
        delete imageHandle->mImage;
        imageHandle->mImage = nullptr;
    }
}
//----------------------------------------------------------------------------
SERTBitmapHandle* SEVRayRTDevice::__CreateRTBitmap(SERayTracingDeviceBitmap*, SERayTracingDeviceImage* img, int width, int height)
{
    SEMutexLock lock(*mImageOpMutex);

    SEVRayRTBitmapHandle* bitmapHandle = SE_NEW SEVRayRTBitmapHandle();
    bitmapHandle->RTDevice = this;

    SEVRayRTImageHandle* imageHandle = (SEVRayRTImageHandle*)img->GetImageHandle();
    if( imageHandle && imageHandle->mImage )
    {
        VRay::VRayImage* image = imageHandle->mImage->downscale(width, height);
        VRay::LocalVRayImage tempImage(image);
        bitmapHandle->mBitmap = tempImage->getBmp();
    }
    return bitmapHandle;
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__DeleteRTBitmap(SERayTracingDeviceBitmap* bmp)
{
    if( bmp )
    {
        SEVRayRTBitmapHandle* bitmapHandle = (SEVRayRTBitmapHandle*)bmp->GetBitmapHandle();
        delete bitmapHandle->mBitmap;
        bitmapHandle->mBitmap = nullptr;
    }
}
//----------------------------------------------------------------------------
bool SEVRayRTDevice::__GetImageSize(int& width, int& height)
{
    if( mVRayRenderer )
    {
        return mVRayRenderer->getImageSize(width, height);
    }

    return false;
}
//----------------------------------------------------------------------------
bool SEVRayRTDevice::__SetImageSize(int width, int height)
{
    if( mVRayRenderer )
    {
        return mVRayRenderer->setImageSize(width, height);
    }

    return false;
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__Render()
{
    if( mVRayRenderer )
    {
        mVRayRenderer->start();
    }
}
//----------------------------------------------------------------------------
SERayTracingDeviceImage* SEVRayRTDevice::__GetImage()
{
    SERayTracingDeviceImage* res = nullptr;
    if( mVRayRenderer )
    {
        VRay::VRayImage* img = mVRayRenderer->getImage();
        if( img )
        {
            res = SE_NEW SERayTracingDeviceImage();
            SEVRayRTImageHandle* imageHandle = (SEVRayRTImageHandle*)this->CreateRTImage(res);
            imageHandle->mImage = img;
            res->SetImageHandle(imageHandle);
            img->getSize(res->Width, res->Height);
        }

    }

    return res;
}
//----------------------------------------------------------------------------
void* SEVRayRTDevice::__RTBitmapGetPixels(SERayTracingDeviceBitmap* bmp)
{
    void* res = nullptr;
    if( bmp )
    {
        SEVRayRTBitmapHandle* bitmapHandle = (SEVRayRTBitmapHandle*)bmp->GetBitmapHandle();
        if( bitmapHandle && bitmapHandle->mBitmap )
        {
            res = bitmapHandle->mBitmap->getPixels();
        }
    }

    return res;
}
//----------------------------------------------------------------------------
void* SEVRayRTDevice::__RTBitmapGetInfoHeader(SERayTracingDeviceBitmap* bmp)
{
    void* res = nullptr;
    if( bmp )
    {
        SEVRayRTBitmapHandle* bitmapHandle = (SEVRayRTBitmapHandle*)bmp->GetBitmapHandle();
        if( bitmapHandle )
        {
            res = reinterpret_cast<void*>(static_cast<VRay::BitmapInfoHeader*>(bitmapHandle->mBitmap));
        }
    }

    return res;
}
//----------------------------------------------------------------------------
bool SEVRayRTDevice::__RTImageSaveToBmpFile(SERayTracingDeviceImage* img, const std::string& fileName, bool preserveAlpha, bool invertChannels)
{
    bool res = false;
    if( img )
    {
        SEVRayRTImageHandle* imageHandle = (SEVRayRTImageHandle*)img->GetImageHandle();
        if( imageHandle )
        {
            res = imageHandle->mImage->saveToBmpFile(fileName, preserveAlpha, invertChannels);
        }
    }

    return res;
}
//----------------------------------------------------------------------------
SERTDeviceCameraHandle* SEVRayRTDevice::__CreateRTDeviceCamera(SERTDeviceCamera* camera)
{
    SEVRayRTDeviceCameraHandle* cameraHandle = nullptr;
    if( camera )
    {
        cameraHandle = SE_NEW SEVRayRTDeviceCameraHandle();
        cameraHandle->RTDevice = this;
        cameraHandle->mRenderView = new VRay::Plugins::RenderView();
        //*(cameraHandle->mRenderView) = mVRayRenderer->newPlugin<RenderView>();
        *(cameraHandle->mRenderView) = mVRayRenderer->getOrCreatePlugin<RenderView>("renderView");
    }

    return cameraHandle;
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__DeleteRTDeviceCamera(SERTDeviceCamera* camera)
{
    if( camera )
    {
        SEVRayRTDeviceCameraHandle* cameraHandle = (SEVRayRTDeviceCameraHandle*)camera->GetCameraHandle();
        if( cameraHandle )
        {
            delete cameraHandle->mRenderView;
            cameraHandle->mRenderView = nullptr;
        }
    }
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__SetTransformFromCamera(SEICamera* srcCamera, SERTDeviceCamera* dstCamera)
{
    if( srcCamera && dstCamera )
    {
        SEVRayRTDeviceCameraHandle* cameraHandle = (SEVRayRTDeviceCameraHandle*)dstCamera->GetCameraHandle();
        if( cameraHandle )
        {
            SEVector3f srcLoc = srcCamera->GetLocation();
            SEMatrix3f srcRot = srcCamera->GetRotation();
            Matrix dstRot;
            Vector dstLoc;

            SECoordinateSystemAdapter::SEToZUpRHColumnMajorOrder<Matrix, Vector>(srcRot, srcLoc, dstRot, dstLoc);
            Transform trans(dstRot, dstLoc);

            RenderView& renderView = *cameraHandle->mRenderView;
            renderView.set_transform(trans);
        }
    }
}
//----------------------------------------------------------------------------
SERTDeviceLightRectangleHandle* SEVRayRTDevice::__CreateRTDeviceLightRectangle(SERTDeviceLightRectangle* lightRectangle, SEILight* srcLight)
{
    SEVRayRTDeviceLightRectangleHandle* lightRectangleHandle = nullptr;
    if( lightRectangle )
    {
        lightRectangleHandle = SE_NEW SEVRayRTDeviceLightRectangleHandle();
        lightRectangleHandle->RTDevice = this;
        lightRectangleHandle->mLightRectangle = new VRay::Plugins::LightRectangle();
        *(lightRectangleHandle->mLightRectangle) = mVRayRenderer->newPlugin<LightRectangle>();

        if( srcLight )
        {
            SetLightCommon<LightRectangle>(srcLight, lightRectangleHandle->mLightRectangle);

            // Set light rectangle parameters.
            lightRectangleHandle->mLightRectangle->set_u_size(srcLight->GetWidth());
            lightRectangleHandle->mLightRectangle->set_v_size(srcLight->GetHeight());
        }
    }

    return lightRectangleHandle;
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__DeleteRTDeviceLightRectangle(SERTDeviceLightRectangle* lightRectangle)
{
    if( lightRectangle )
    {
        SEVRayRTDeviceLightRectangleHandle* lightRectangleHandle = (SEVRayRTDeviceLightRectangleHandle*)lightRectangle->GetLightRectangleHandle();
        if( lightRectangleHandle )
        {
            delete lightRectangleHandle->mLightRectangle;
            lightRectangleHandle->mLightRectangle = nullptr;
        }
    }
}
//----------------------------------------------------------------------------
SERTDeviceSkyLightHandle* SEVRayRTDevice::__CreateRTDeviceSkyLight(SERTDeviceSkyLight* skyLight, SEILight* srcLight)
{
    SEVRayRTDeviceSkyLightHandle* skyLightHandle = nullptr;
    if( skyLight )
    {
        skyLightHandle = SE_NEW SEVRayRTDeviceSkyLightHandle();
        skyLightHandle->RTDevice = this;
        skyLightHandle->mLightDome = new VRay::Plugins::LightDome();
        *(skyLightHandle->mLightDome) = mVRayRenderer->newPlugin<LightDome>();

        if( srcLight )
        {
            SetLightCommon<LightDome>(srcLight, skyLightHandle->mLightDome);
        }
    }

    return skyLightHandle;
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__DeleteRTDeviceSkyLight(SERTDeviceSkyLight* skyLight)
{
    if( skyLight )
    {
        SEVRayRTDeviceSkyLightHandle* skyLightHandle = (SEVRayRTDeviceSkyLightHandle*)skyLight->GetSkyLightHandle();
        if( skyLightHandle )
        {
            delete skyLightHandle->mLightDome;
            skyLightHandle->mLightDome = nullptr;
        }
    }
}
//----------------------------------------------------------------------------
SERTDeviceStaticMeshHandle* SEVRayRTDevice::__CreateRTDeviceStaticMesh(SERTDeviceStaticMesh* staticMesh, SEIMetaMesh* srcMesh)
{
    SEVRayRTDeviceStaticMeshHandle* staticMeshHandle = nullptr;
    if( staticMesh )
    {
        staticMeshHandle = SE_NEW SEVRayRTDeviceStaticMeshHandle();
        staticMeshHandle->RTDevice = this;
        staticMeshHandle->mStaticMesh = new VRay::Plugins::GeomStaticMesh();
        *(staticMeshHandle->mStaticMesh) = mVRayRenderer->newPlugin<GeomStaticMesh>();

        if( srcMesh )
        {
            std::vector<SEVector3f>& srcVertices = srcMesh->GetVertexData();
            std::vector<MetaMeshFaceIndex>& srcIndices = srcMesh->GetIndexData();

            // Get vertex data.
            std::vector<Vector> dstVertices;
            dstVertices.reserve(srcVertices.size());
            for( unsigned int i = 0; i < srcVertices.size(); ++i )
            {
                Vector v;
                v.x = srcVertices[i].X;
                v.y = -srcVertices[i].Z;
                v.z = srcVertices[i].Y;

                dstVertices.push_back(v);
            }
            staticMeshHandle->mStaticMesh->set_vertices(dstVertices.data(), dstVertices.size());

            // Get vertex face index data.
            std::vector<int> dstFaces;
            dstFaces.reserve(srcIndices.size()*3);
            for( unsigned int i = 0; i < srcIndices.size(); ++i )
            {
                dstFaces.push_back(srcIndices[i].VertexIndices[0]);
                dstFaces.push_back(srcIndices[i].VertexIndices[1]);
                dstFaces.push_back(srcIndices[i].VertexIndices[2]);
            }
            staticMeshHandle->mStaticMesh->set_faces(dstFaces.data(), dstFaces.size());

            // Get vertex normal data.
            if( srcMesh->HasNormal() )
            {
                std::vector<SEVector3f>& srcNormals = srcMesh->GetVertexNormalData();

                std::vector<Vector> dstNormals;
                dstNormals.reserve(srcNormals.size());
                for( unsigned int i = 0; i < srcNormals.size(); ++i )
                {
                    Vector n;
                    n.x = srcNormals[i].X;
                    n.y = -srcNormals[i].Z;
                    n.z = srcNormals[i].Y;

                    dstNormals.push_back(n);
                }
                staticMeshHandle->mStaticMesh->set_normals(dstNormals.data(), dstNormals.size());

                // Get vertex normal face index data.
                std::vector<int> dstFaceNormals;
                dstFaceNormals.reserve(srcIndices.size() * 3);
                for( unsigned int i = 0; i < srcIndices.size(); ++i )
                {
                    dstFaceNormals.push_back(srcIndices[i].VertexNormalIndices[0]);
                    dstFaceNormals.push_back(srcIndices[i].VertexNormalIndices[1]);
                    dstFaceNormals.push_back(srcIndices[i].VertexNormalIndices[2]);
                }
                staticMeshHandle->mStaticMesh->set_faceNormals(dstFaceNormals.data(), dstFaceNormals.size());
            }

            // Get vertex texture coord data.
            // TODO:
            // Only support one uvw channel for now.
            unsigned int channelCount = srcMesh->GetTCoordChannelCount();
            if( channelCount > 0 )
            {
                // Get texture coordinates.
                std::vector<SEVector3f>& srcTCoords = srcMesh->GetTCoordData(0);
                std::vector<Vector> dstTCoords;
                dstTCoords.reserve(srcTCoords.size());
                for( unsigned int i = 0; i < srcTCoords.size(); ++i )
                {
                    Vector t;
                    t.x = srcTCoords[i].X;
                    t.y = srcTCoords[i].Y;
                    t.z = srcTCoords[i].Z;

                    dstTCoords.push_back(t);
                }

                // Get uvw face index data.
                std::vector<int> dstFaceTCoords;
                dstFaceTCoords.reserve(srcIndices.size() * 3);
                for( unsigned int i = 0; i < srcIndices.size(); ++i )
                {
                    dstFaceTCoords.push_back(srcIndices[i].TCoordIndices[0]);
                    dstFaceTCoords.push_back(srcIndices[i].TCoordIndices[1]);
                    dstFaceTCoords.push_back(srcIndices[i].TCoordIndices[2]);
                }

                ValueList uvwChannel;
                uvwChannel.push_back(Value(0));
                uvwChannel.push_back(Value(VectorList(dstTCoords.data(), dstTCoords.data() + dstTCoords.size())));
                uvwChannel.push_back(Value(IntList(dstFaceTCoords.data(), dstFaceTCoords.data() + dstFaceTCoords.size())));

                ValueList mapChannels;
                mapChannels.push_back(Value(uvwChannel));
                staticMeshHandle->mStaticMesh->set_map_channels(mapChannels);
            }
        }
    }

    return staticMeshHandle;
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__DeleteRTDeviceStaticMesh(SERTDeviceStaticMesh* staticMesh)
{
    if( staticMesh )
    {
        SEVRayRTDeviceStaticMeshHandle* staticMeshHandle = (SEVRayRTDeviceStaticMeshHandle*)staticMesh->GetStaticMeshHandle();
        if( staticMeshHandle )
        {
            delete staticMeshHandle->mStaticMesh;
            staticMeshHandle->mStaticMesh = nullptr;
        }
    }
}
//----------------------------------------------------------------------------
SERTDeviceSceneNodeHandle* SEVRayRTDevice::__CreateSceneNode(SERTDeviceSceneNode* sceneNode, SEISpatialInfo* spatialInfo)
{
    SEVRayRTDeviceSceneNodeHandle* sceneNodeHandle = nullptr;
    if( sceneNode )
    {
        sceneNodeHandle = SE_NEW SEVRayRTDeviceSceneNodeHandle();
        sceneNodeHandle->RTDevice = this;
        sceneNodeHandle->mNode = new VRay::Plugins::Node();
        *(sceneNodeHandle->mNode) = mVRayRenderer->newPlugin<Node>();

        if( spatialInfo )
        {
            SEVector3f srcScale = spatialInfo->GetScale();
            SEMatrix3f srcRot = spatialInfo->GetRotation();
            srcRot[0][0] *= srcScale[0];
            srcRot[1][1] *= srcScale[1];
            srcRot[2][2] *= srcScale[2];
            SEVector3f srcLoc = spatialInfo->GetLocation();
            SetTransformHelper<VRay::Plugins::Node>(sceneNodeHandle->mNode, &srcRot, &srcLoc);
        }
    }

    return sceneNodeHandle;
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__DeleteSceneNode(SERTDeviceSceneNode* sceneNode)
{
    if( sceneNode )
    {
        SEVRayRTDeviceSceneNodeHandle* sceneNodeHandle = (SEVRayRTDeviceSceneNodeHandle*)sceneNode->GetSceneNodeHandle();
        if( sceneNodeHandle )
        {
            delete sceneNodeHandle->mNode;
            sceneNodeHandle->mNode = nullptr;
        }
    }
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__SceneNodeSetTransform(SERTDeviceSceneNode* sceneNode, SEMatrix3f* srcMatrix, SEVector3f* srcOffset)
{
    if( sceneNode )
    {
        SEVRayRTDeviceSceneNodeHandle* sceneNodeHandle = (SEVRayRTDeviceSceneNodeHandle*)sceneNode->GetSceneNodeHandle();

        if( sceneNodeHandle && sceneNodeHandle->mNode )
        {
            SetTransformHelper<VRay::Plugins::Node>(sceneNodeHandle->mNode, srcMatrix, srcOffset);
        }
    }
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__SceneNodeSetGeometry(SERTDeviceSceneNode* sceneNode, SERTDeviceGeometry* geometry)
{
    if( sceneNode && geometry )
    {
        SEVRayRTDeviceSceneNodeHandle* sceneNodeHandle = (SEVRayRTDeviceSceneNodeHandle*)sceneNode->GetSceneNodeHandle();

        if( sceneNodeHandle && sceneNodeHandle->mNode )
        {
            SERTDeviceGeometryType geometryType = geometry->GetGeometryType();
            switch( geometryType )
            {
                case Swing::RTDGT_StaticMesh:
                {
                    SERTDeviceStaticMesh* staticMesh = static_cast<SERTDeviceStaticMesh*>(geometry);
                    SEVRayRTDeviceStaticMeshHandle* staticMeshHandle = static_cast<SEVRayRTDeviceStaticMeshHandle*>(staticMesh->GetStaticMeshHandle());
                    if( staticMeshHandle )
                    {
                        sceneNodeHandle->mNode->set_geometry(*staticMeshHandle->mStaticMesh);
                    }
                }
                break;

                case Swing::RTDGT_Unknown:
                    break;
                default:
                    break;
            }
        }
    }
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__SceneNodeSetMaterial(SERTDeviceSceneNode* sceneNode, SERTDeviceMaterial* material)
{
    if( sceneNode && material )
    {
        SEVRayRTDeviceSceneNodeHandle* sceneNodeHandle = static_cast<SEVRayRTDeviceSceneNodeHandle*>(sceneNode->GetSceneNodeHandle());

        if( sceneNodeHandle && sceneNodeHandle->mNode )
        {
            SEVRayRTDeviceMaterialHandle* materialHandle = static_cast<SEVRayRTDeviceMaterialHandle*>(material->GetMaterialHandle());
            if( materialHandle )
            {
                sceneNodeHandle->mNode->set_material(*materialHandle->mMaterial);
            }
        }
    }
}
//----------------------------------------------------------------------------
SERTDeviceMaterialHandle* SEVRayRTDevice::__CreateMaterial(SERTDeviceMaterial* material)
{
    SEVRayRTDeviceMaterialHandle* materialHandle = nullptr;
    if( material )
    {
        materialHandle = SE_NEW SEVRayRTDeviceMaterialHandle();
        materialHandle->RTDevice = this;

        materialHandle->mBRDF = new BRDFDiffuse();
        *(materialHandle->mBRDF) = mVRayRenderer->newPlugin<BRDFDiffuse>();
        materialHandle->mBRDF->set_color(Color(0.0f, 0.6f, 0.6f));

        materialHandle->mMaterial = new MtlSingleBRDF();
        *(materialHandle->mMaterial) = mVRayRenderer->newPlugin<MtlSingleBRDF>();
        materialHandle->mMaterial->set_brdf(*materialHandle->mBRDF);
    }

    return materialHandle;
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__DeleteMaterial(SERTDeviceMaterial* material)
{
    if( material )
    {
        SEVRayRTDeviceMaterialHandle* materialHandle = static_cast<SEVRayRTDeviceMaterialHandle*>(material->GetMaterialHandle());
        if( materialHandle )
        {
            delete materialHandle->mMaterial;
            materialHandle->mMaterial = nullptr;

            delete materialHandle->mBRDF;
            materialHandle->mBRDF = nullptr;
        }
    }
}
//----------------------------------------------------------------------------
SERTDeviceBakeViewHandle* SEVRayRTDevice::__CreateBakeView(SERTDeviceBakeView* bakeView, SERTDeviceBakeViewDescription* bakeViewDesc)
{
    SEVRayRTDeviceBakeViewHandle* bakeViewHandle = nullptr;
    if( bakeView )
    {
        bakeViewHandle = SE_NEW SEVRayRTDeviceBakeViewHandle();
        bakeViewHandle->RTDevice = this;

        bakeViewHandle->mUVWGenChannel = new UVWGenChannel();
        *(bakeViewHandle->mUVWGenChannel) = mVRayRenderer->newPlugin<UVWGenChannel>();

        bakeViewHandle->mBakeView = new BakeView();
        *(bakeViewHandle->mBakeView) = mVRayRenderer->newPlugin<BakeView>();
        bakeViewHandle->mBakeView->set_bake_uvwgen(*bakeViewHandle->mUVWGenChannel);

        if( bakeViewDesc )
        {
            if( bakeViewDesc->BakeNode )
            {
                SEVRayRTDeviceSceneNodeHandle* sceneNodeHandle = static_cast<SEVRayRTDeviceSceneNodeHandle*>(bakeViewDesc->BakeNode->GetSceneNodeHandle());
                if( sceneNodeHandle->mNode )
                {
                    bakeViewHandle->mBakeView->set_bake_node(*sceneNodeHandle->mNode);
                }
            }

            bakeViewHandle->mBakeView->set_dilation(bakeViewDesc->Dilation);
            bakeViewHandle->mUVWGenChannel->set_uvw_channel(bakeViewDesc->UVChannel);
            bakeViewHandle->mBakeView->set_from_camera(bakeViewDesc->FromCamera);
            bakeViewHandle->mBakeView->set_u_min(bakeViewDesc->UMin);
            bakeViewHandle->mBakeView->set_u_max(bakeViewDesc->UMax);
            bakeViewHandle->mBakeView->set_v_min(bakeViewDesc->VMin);
            bakeViewHandle->mBakeView->set_v_max(bakeViewDesc->VMax);
        }
    }

    return bakeViewHandle;
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__DeleteBakeView(SERTDeviceBakeView* bakeView)
{
    if( bakeView )
    {
        SEVRayRTDeviceBakeViewHandle* bakeViewHandle = static_cast<SEVRayRTDeviceBakeViewHandle*>(bakeView->GetBakeViewHandle());
        if( bakeViewHandle )
        {
            delete bakeViewHandle->mBakeView;
            bakeViewHandle->mBakeView = nullptr;

            delete bakeViewHandle->mUVWGenChannel;
            bakeViewHandle->mUVWGenChannel = nullptr;
        }
    }
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__AddRenderElement(SERTDeviceRenderElementType renderElementType)
{
    int reIndex = (int)renderElementType - 1;
    RenderElements reManager = mVRayRenderer->getRenderElements();
    RenderElement::Type reType = (RenderElement::Type)gsRenderElementType[reIndex];
    reManager.addNew(reType, nullptr, nullptr);

    SE_ASSERT( mRenderElements[reIndex] == nullptr );
    if( !mRenderElements[reIndex] )
    {
        mRenderElements[reIndex] = SE_NEW SERTDeviceRenderElement(renderElementType);

        SEVRayRTDeviceRenderElementHandle* renderElementHandle = SE_NEW SEVRayRTDeviceRenderElementHandle();
        renderElementHandle->RTDevice = this;

        RenderElement re = reManager.getByType(reType);
        if( re )
        {
            renderElementHandle->mRenderElement = new VRay::RenderElement();
            (*renderElementHandle->mRenderElement) = re;
        }

        mRenderElements[reIndex]->SetRenderElementHandle(renderElementHandle);
    }
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__DeleteRenderElement(SERTDeviceRenderElement* renderElement)
{
    if( renderElement )
    {
        SEVRayRTDeviceRenderElementHandle* renderElementHandle = static_cast<SEVRayRTDeviceRenderElementHandle*>(renderElement->GetRenderElementHandle());
        if( renderElementHandle )
        {
            delete renderElementHandle->mRenderElement;
            renderElementHandle->mRenderElement = nullptr;
        }
    }
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__SaveRenderElementToFile(SERTDeviceRenderElement* renderElement, const std::string& fileName, SERTDeviceImageFileType fileType)
{
    if( renderElement )
    {
        SEVRayRTDeviceRenderElementHandle* renderElementHandle = static_cast<SEVRayRTDeviceRenderElementHandle*>(renderElement->GetRenderElementHandle());
        if( renderElementHandle && renderElementHandle->mRenderElement && (*renderElementHandle->mRenderElement) == true )
        {
            VRayImage* image = renderElementHandle->mRenderElement->getImage();
            if( image )
            {
                switch( fileType )
                {
                case Swing::RTDIFT_BMP:
                    image->saveToBmpFile(fileName.c_str());
                    break;

                case Swing::RTDIFT_JPG:
                    image->saveToJpegFile(fileName.c_str());
                    break;

                case Swing::RTDIFT_PNG:
                    image->saveToPngFile(fileName.c_str());
                    break;

                default:
                    break;
                }

                delete image;
            }
        }
    }
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__GenerateLightMapFromRenderElements(SERTDeviceRenderElement* diffuseRE, SERTDeviceRenderElement* lightingRE,
    SERTDeviceRenderElement* giRE, const std::string& dstFileName, SERTDeviceImageFileType dstFileType)
{

}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void SEVRayRTDevice::__OnRenderStart(VRay::VRayRenderer&, void*)
{
    SERayTracingDevice::RenderStartCallback(this);
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__OnImageReady(VRay::VRayRenderer&, void*)
{
    SERayTracingDevice::ImageReadyCallback(this);
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__OnRTimageUpdated(VRay::VRayRenderer&, VRay::VRayImage* img, void*)
{
	if( img )
	{
		SERayTracingDeviceImage* rtImage = SE_NEW SERayTracingDeviceImage();
        SEVRayRTImageHandle* imageHandle = (SEVRayRTImageHandle*)this->CreateRTImage(rtImage);
        imageHandle->mImage = img;
        rtImage->SetImageHandle(imageHandle);
        img->getSize(rtImage->Width, rtImage->Height);

		SERayTracingDevice::RTImageUpdatedCallback(this, rtImage);
	}
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__OnDeviceClose(VRay::VRayRenderer&, void*)
{
    SERayTracingDevice::DeviceCloseCallback(this);
}
//----------------------------------------------------------------------------
void SEVRayRTDevice::__OnDumpMessage(VRay::VRayRenderer&, const char* msg, int level, void*)
{
    SERayTracingDevice::DumpMessageCallback(this, msg, level);
}
//----------------------------------------------------------------------------