// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

//----------------------------------------------------------------------------
void SERayTracingDevice::Initialize(SERayTracingDeviceDescription* deviceDesc)
{
    for( int i = 0; i < RTDRET_Max; ++i )
    {
        mRenderElements[i] = nullptr;
    }

    mDeviceDesc = *deviceDesc;
    (this->*_Initialize)(deviceDesc);
}
//----------------------------------------------------------------------------
void SERayTracingDevice::Terminate()
{
    (this->*_Terminate)();
}
//----------------------------------------------------------------------------
bool SERayTracingDevice::LoadNativeScene(const char* fileName)
{
    return (this->*_LoadNativeScene)(fileName);
}
//----------------------------------------------------------------------------
SERTImageHandle* SERayTracingDevice::CreateRTImage(SERayTracingDeviceImage* img)
{
    return (this->*_CreateRTImage)(img);
}
//----------------------------------------------------------------------------
void SERayTracingDevice::DeleteRTImage(SERayTracingDeviceImage* img)
{
    (this->*_DeleteRTImage)(img);
}
//----------------------------------------------------------------------------
SERTBitmapHandle* SERayTracingDevice::CreateRTBitmap(SERayTracingDeviceBitmap* bmp, SERayTracingDeviceImage* img, int width, int height)
{
    return (this->*_CreateRTBitmap)(bmp, img, width, height);
}
//----------------------------------------------------------------------------
void SERayTracingDevice::DeleteRTBitmap(SERayTracingDeviceBitmap* bmp)
{
    (this->*_DeleteRTBitmap)(bmp);
}
//----------------------------------------------------------------------------
bool SERayTracingDevice::GetImageSize(int& width, int& height)
{
    return (this->*_GetImageSize)(width, height);
}
//----------------------------------------------------------------------------
bool SERayTracingDevice::SetImageSize(int width, int height)
{
    return (this->*_SetImageSize)(width, height);
}
//----------------------------------------------------------------------------
void SERayTracingDevice::Render()
{
    (this->*_Render)();
}
//----------------------------------------------------------------------------
SERayTracingDeviceImage* SERayTracingDevice::GetImage()
{
    return (this->*_GetImage)();
}
//----------------------------------------------------------------------------
void* SERayTracingDevice::RTBitmapGetPixels(SERayTracingDeviceBitmap* bmp)
{
    return (this->*_RTBitmapGetPixels)(bmp);
}
//----------------------------------------------------------------------------
void* SERayTracingDevice::RTBitmapGetInfoHeader(SERayTracingDeviceBitmap* bmp)
{
    return (this->*_RTBitmapGetInfoHeader)(bmp);
}
//----------------------------------------------------------------------------
bool SERayTracingDevice::RTImageSaveToBmpFile(SERayTracingDeviceImage* img, const std::string& fileName, bool preserveAlpha, bool invertChannels)
{
    return (this->*_RTImageSaveToBmpFile)(img, fileName, preserveAlpha, invertChannels);
}
//----------------------------------------------------------------------------
SERTDeviceCameraHandle* SERayTracingDevice::CreateRTDeviceCamera(SERTDeviceCamera* camera)
{
    return (this->*_CreateRTDeviceCamera)(camera);
}
//----------------------------------------------------------------------------
void SERayTracingDevice::DeleteRTDeviceCamera(SERTDeviceCamera* camera)
{
    (this->*_DeleteRTDeviceCamera)(camera);
}
//----------------------------------------------------------------------------
void SERayTracingDevice::SetTransformFromCamera(SEICamera* srcCamera, SERTDeviceCamera* dstCamera)
{
    (this->*_SetTransformFromCamera)(srcCamera, dstCamera);
}
//----------------------------------------------------------------------------
SERTDeviceLightRectangleHandle* SERayTracingDevice::CreateRTDeviceLightRectangle(SERTDeviceLightRectangle* lightRectangle, SEILight* srcLight)
{
    return (this->*_CreateRTDeviceLightRectangle)(lightRectangle, srcLight);
}
//----------------------------------------------------------------------------
void SERayTracingDevice::DeleteRTDeviceLightRectangle(SERTDeviceLightRectangle* lightRectangle)
{
    (this->*_DeleteRTDeviceLightRectangle)(lightRectangle);
}
//----------------------------------------------------------------------------
SERTDeviceSkyLightHandle* SERayTracingDevice::CreateRTDeviceSkyLight(SERTDeviceSkyLight* skyLight, SEILight* srcLight)
{
    return (this->*_CreateRTDeviceSkyLight)(skyLight, srcLight);
}
//----------------------------------------------------------------------------
void SERayTracingDevice::DeleteRTDeviceSkyLight(SERTDeviceSkyLight* skyLight)
{
    (this->*_DeleteRTDeviceSkyLight)(skyLight);
}
//----------------------------------------------------------------------------
SERTDeviceStaticMeshHandle* SERayTracingDevice::CreateRTDeviceStaticMesh(SERTDeviceStaticMesh* staticMesh, SEIMetaMesh* srcMesh)
{
    return (this->*_CreateRTDeviceStaticMesh)(staticMesh, srcMesh);
}
//----------------------------------------------------------------------------
void SERayTracingDevice::DeleteRTDeviceStaticMesh(SERTDeviceStaticMesh* staticMesh)
{
    (this->*_DeleteRTDeviceStaticMesh)(staticMesh);
}
//----------------------------------------------------------------------------
SERTDeviceSceneNodeHandle* SERayTracingDevice::CreateSceneNode(SERTDeviceSceneNode* sceneNode, SEISpatialInfo* spatialInfo)
{
    return (this->*_CreateSceneNode)(sceneNode, spatialInfo);
}
//----------------------------------------------------------------------------
void SERayTracingDevice::DeleteSceneNode(SERTDeviceSceneNode* sceneNode)
{
    (this->*_DeleteSceneNode)(sceneNode);
}
//----------------------------------------------------------------------------
void SERayTracingDevice::SceneNodeSetTransform(SERTDeviceSceneNode* sceneNode, SEMatrix3f* srcMatrix, SEVector3f* srcOffset)
{
    (this->*_SceneNodeSetTransform)(sceneNode, srcMatrix, srcOffset);
}
//----------------------------------------------------------------------------
void SERayTracingDevice::SceneNodeSetGeometry(SERTDeviceSceneNode* sceneNode, SERTDeviceGeometry* geometry)
{
    (this->*_SceneNodeSetGeometry)(sceneNode, geometry);
}
//----------------------------------------------------------------------------
void SERayTracingDevice::SceneNodeSetMaterial(SERTDeviceSceneNode* sceneNode, SERTDeviceMaterial* material)
{
    (this->*_SceneNodeSetMaterial)(sceneNode, material);
}
//----------------------------------------------------------------------------
SERTDeviceMaterialHandle* SERayTracingDevice::CreateMaterial(SERTDeviceMaterial* material)
{
    return (this->*_CreateMaterial)(material);
}
//----------------------------------------------------------------------------
void SERayTracingDevice::DeleteMaterial(SERTDeviceMaterial* material)
{
    (this->*_DeleteMaterial)(material);
}
//----------------------------------------------------------------------------
SERTDeviceBakeViewHandle* SERayTracingDevice::CreateBakeView(SERTDeviceBakeView* bakeView, SERTDeviceBakeViewDescription* bakeViewDesc)
{
    return (this->*_CreateBakeView)(bakeView, bakeViewDesc);
}
//----------------------------------------------------------------------------
void SERayTracingDevice::DeleteBakeView(SERTDeviceBakeView* bakeView)
{
    (this->*_DeleteBakeView)(bakeView);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void SERayTracingDevice::SetOnRenderStart(void (*CallbackFunc)(SERayTracingDevice&, void*), const void* userData)
{
    RenderStartDelegate = SERayTracingDeviceDelegate1::FromFunction(CallbackFunc, const_cast<void*>(userData));
}
//----------------------------------------------------------------------------
template<class T, void (T::*TMethod)(SERayTracingDevice&, void*)>
void SERayTracingDevice::SetOnRenderStart(T& object, const void* userData)
{
    RenderStartDelegate = SERayTracingDeviceDelegate1::FromMethod<T, TMethod>(&object, const_cast<void*>(userData));
}
//----------------------------------------------------------------------------
void SERayTracingDevice::SetOnImageReady(void (*CallbackFunc)(SERayTracingDevice&, void*), const void* userData)
{
    ImageReadyDelegate = SERayTracingDeviceDelegate1::FromFunction(CallbackFunc, const_cast<void*>(userData));
}
//----------------------------------------------------------------------------
template<class T, void (T::*TMethod)(SERayTracingDevice&, void*)>
void SERayTracingDevice::SetOnImageReady(T& object, const void* userData)
{
    ImageReadyDelegate = SERayTracingDeviceDelegate1::FromMethod<T, TMethod>(&object, const_cast<void*>(userData));
}
//----------------------------------------------------------------------------
void SERayTracingDevice::SetOnDeviceClose(void (*CallbackFunc)(SERayTracingDevice&, void*), const void* userData)
{
    DeviceCloseDelegate = SERayTracingDeviceDelegate1::FromFunction(CallbackFunc, const_cast<void*>(userData));
}
//----------------------------------------------------------------------------
template<class T, void (T::*TMethod)(SERayTracingDevice&, void*)>
void SERayTracingDevice::SetOnDeviceClose(T& object, const void* userData)
{
    DeviceCloseDelegate = SERayTracingDeviceDelegate1::FromMethod<T, TMethod>(&object, const_cast<void*>(userData));
}
//----------------------------------------------------------------------------
void SERayTracingDevice::SetOnRTImageUpdated(void(*CallbackFunc)(SERayTracingDevice&, SERayTracingDeviceImage* img, void*), const void* userData)
{
    RTImageUpdatedDelegate = SERayTracingDeviceDelegate2::FromFunction(CallbackFunc, const_cast<void*>(userData));
}
//----------------------------------------------------------------------------
template<class T, void (T::*TMethod)(SERayTracingDevice&, SERayTracingDeviceImage* img, void*)>
void SERayTracingDevice::SetOnRTImageUpdated(T& object, const void* userData)
{
    RTImageUpdatedDelegate = SERayTracingDeviceDelegate2::FromMethod<T, TMethod>(&object, const_cast<void*>(userData));
}
//----------------------------------------------------------------------------
void SERayTracingDevice::SetOnDumpMessage(void (*CallbackFunc)(SERayTracingDevice&, const char* msg, int level, void*), const void* userData)
{
    DumpMessageDelegate = SERayTracingDeviceDelegate3::FromFunction(CallbackFunc, const_cast<void*>(userData));
}
//----------------------------------------------------------------------------
template<class T, void (T::*TMethod)(SERayTracingDevice&, const char* msg, int level, void*)>
void SERayTracingDevice::SetOnDumpMessage(T& object, const void* userData)
{
    DumpMessageDelegate = SERayTracingDeviceDelegate3::FromMethod<T, TMethod>(&object, const_cast<void*>(userData));
}
//----------------------------------------------------------------------------
SERayTracingDeviceVendor SERayTracingDevice::GetDeviceVendor()
{
    return mDeviceVendor;
}
//----------------------------------------------------------------------------
void SERayTracingDevice::RenderStartCallback(void* rtDevice)
{
    SERayTracingDevice*& me = reinterpret_cast<SERayTracingDevice*&>(rtDevice);
    if( me->RenderStartDelegate.IsValid() )
    {
        me->RenderStartDelegate(*me);
    }
}
//----------------------------------------------------------------------------
void SERayTracingDevice::ImageReadyCallback(void* rtDevice)
{
    SERayTracingDevice*& me = reinterpret_cast<SERayTracingDevice*&>(rtDevice);
    if( me->ImageReadyDelegate.IsValid() )
    {
        me->ImageReadyDelegate(*me);
    }
}
//----------------------------------------------------------------------------
void SERayTracingDevice::DeviceCloseCallback(void* rtDevice)
{
    SERayTracingDevice*& me = reinterpret_cast<SERayTracingDevice*&>(rtDevice);
    if( me->DeviceCloseDelegate.IsValid() )
    {
        me->DeviceCloseDelegate(*me);
    }
}
//----------------------------------------------------------------------------
void SERayTracingDevice::RTImageUpdatedCallback(void* rtDevice, SERayTracingDeviceImage* img)
{
	SERayTracingDevice*& me = reinterpret_cast<SERayTracingDevice*&>(rtDevice);
	if (me->RTImageUpdatedDelegate.IsValid())
	{
		me->RTImageUpdatedDelegate(*me, img);
	}
}
//----------------------------------------------------------------------------
void SERayTracingDevice::DumpMessageCallback(void* rtDevice, const char* msg, int level)
{
    SERayTracingDevice*& me = reinterpret_cast<SERayTracingDevice*&>(rtDevice);
    if( me->DumpMessageDelegate.IsValid() )
    {
        me->DumpMessageDelegate(*me, msg, level);
    }
}
//----------------------------------------------------------------------------
void SERayTracingDevice::AddRenderElement(SERTDeviceRenderElementType renderElementType)
{
    if( !mRenderElements[(int)renderElementType - 1] )
    {
        (this->*_AddRenderElement)(renderElementType);
    }
}
//----------------------------------------------------------------------------
void SERayTracingDevice::DeleteRenderElement(SERTDeviceRenderElement* renderElement)
{
    (this->*_DeleteRenderElement)(renderElement);
}
//----------------------------------------------------------------------------
SERTDeviceRenderElement* SERayTracingDevice::GetRenderElement(SERTDeviceRenderElementType renderElementType)
{
    return mRenderElements[(int)renderElementType - 1];
}
//----------------------------------------------------------------------------
void SERayTracingDevice::SaveRenderElementToFile(SERTDeviceRenderElement* renderElement, const std::string& fileName, SERTDeviceImageFileType fileType)
{
    (this->*_SaveRenderElementToFile)(renderElement, fileName, fileType);
}
//----------------------------------------------------------------------------
void SERayTracingDevice::GenerateLightMapFromRenderElements(SERTDeviceRenderElement* diffuseRE, SERTDeviceRenderElement* lightingRE,
    SERTDeviceRenderElement* giRE, const std::string& dstFileName, SERTDeviceImageFileType dstFileType)
{
    (this->*_GenerateLightMapFromRenderElements)(diffuseRE, lightingRE, giRE, dstFileName, dstFileType);
}
//----------------------------------------------------------------------------