// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_RayTracingDevice_H
#define Swing_RayTracingDevice_H

#include "SERayTracingAdapterLIB.h"
#include "SEReferencable.h"
#include "SERayTracingDeviceResource.h"
#include "SERayTracingDeviceDelegate.h"
#include "SEICamera.h"
#include "SEILight.h"
#include "SEIMetaMesh.h"
#include "SEISpatialInfo.h"

namespace Swing
{

//----------------------------------------------------------------------------
#define SE_INSERT_RAY_TRACING_DEVICE_FUNC(function, device) \
    _##function = (RayTracingDevice##function)&##device::__##function
//----------------------------------------------------------------------------

enum SERayTracingDeviceVendor
{
    RTDV_Unknown = 0,
    RTDV_VRay,
    RTDV_IRay,
    RTDV_Redshift
};

enum SERayTracingDeviceRenderMode
{
    RTDRM_Unknown = 0,
    RTDRM_RT_CPU,
    RTDRM_RT_GPU_CUDA,
    RTDRM_RT_GPU_OPENCL,
    RTDRM_PRODUCTION_CPU,
    RTDRM_PRODUCTION_GPU_CUDA,
    RTDRM_PRODUCTION_GPU_OPENCL,
    RTDRM_MAX
};

struct SE_RAY_TRACING_ADAPTER_API SERayTracingDeviceDescription
{
    unsigned int ImageWidth;
    unsigned int ImageHeight;
    SERayTracingDeviceRenderMode RenderMode;

    SERayTracingDeviceDescription() { memset(this, 0, sizeof(SERayTracingDeviceDescription)); }
};

class SERayTracingDevice;
class SERayTracingDeviceImage;
class SERayTracingDeviceBitmap;
class SERTDeviceCamera;
class SERTDeviceLightRectangle;
class SERTDeviceSkyLight;
class SERTDeviceGeometry;
class SERTDeviceStaticMesh;
class SERTDeviceSceneNode;

typedef void (SERayTracingDevice::*RayTracingDeviceInitialize)(SERayTracingDeviceDescription* deviceDesc);
typedef void (SERayTracingDevice::*RayTracingDeviceTerminate)();
typedef bool (SERayTracingDevice::*RayTracingDeviceLoadNativeScene)(const char* fileName);
typedef SERTImageHandle* (SERayTracingDevice::*RayTracingDeviceCreateRTImage)(SERayTracingDeviceImage* img);
typedef void (SERayTracingDevice::*RayTracingDeviceDeleteRTImage)(SERayTracingDeviceImage* img);
typedef SERTBitmapHandle* (SERayTracingDevice::*RayTracingDeviceCreateRTBitmap)(SERayTracingDeviceBitmap* bmp, SERayTracingDeviceImage* img, int width, int height);
typedef void (SERayTracingDevice::*RayTracingDeviceDeleteRTBitmap)(SERayTracingDeviceBitmap* bmp);
typedef bool (SERayTracingDevice::*RayTracingDeviceGetImageSize)(int& width, int& height);
typedef bool (SERayTracingDevice::*RayTracingDeviceSetImageSize)(int width, int height);
typedef void (SERayTracingDevice::*RayTracingDeviceRender)();
typedef SERayTracingDeviceImage* (SERayTracingDevice::*RayTracingDeviceGetImage)();
typedef void* (SERayTracingDevice::*RayTracingDeviceRTBitmapGetPixels)(SERayTracingDeviceBitmap* bmp);
typedef void* (SERayTracingDevice::*RayTracingDeviceRTBitmapGetInfoHeader)(SERayTracingDeviceBitmap* bmp);
typedef bool (SERayTracingDevice::*RayTracingDeviceRTImageSaveToBmpFile)(SERayTracingDeviceImage* img, const std::string& fileName, bool preserveAlpha, bool invertChannels);
typedef SERTDeviceCameraHandle* (SERayTracingDevice::*RayTracingDeviceCreateRTDeviceCamera)(SERTDeviceCamera* camera);
typedef void (SERayTracingDevice::*RayTracingDeviceDeleteRTDeviceCamera)(SERTDeviceCamera* camera);
typedef void (SERayTracingDevice::*RayTracingDeviceSetTransformFromCamera)(SEICamera* srcCamera, SERTDeviceCamera* dstCamera);
typedef SERTDeviceLightRectangleHandle* (SERayTracingDevice::*RayTracingDeviceCreateRTDeviceLightRectangle)(SERTDeviceLightRectangle* lightRectangle, SEILight* srcLight);
typedef void (SERayTracingDevice::*RayTracingDeviceDeleteRTDeviceLightRectangle)(SERTDeviceLightRectangle* lightRectangle);
typedef SERTDeviceSkyLightHandle* (SERayTracingDevice::*RayTracingDeviceCreateRTDeviceSkyLight)(SERTDeviceSkyLight* skyLight, SEILight* srcLight);
typedef void (SERayTracingDevice::*RayTracingDeviceDeleteRTDeviceSkyLight)(SERTDeviceSkyLight* skyLight);
typedef SERTDeviceStaticMeshHandle* (SERayTracingDevice::*RayTracingDeviceCreateRTDeviceStaticMesh)(SERTDeviceStaticMesh* staticMesh, SEIMetaMesh* srcMesh);
typedef void (SERayTracingDevice::*RayTracingDeviceDeleteRTDeviceStaticMesh)(SERTDeviceStaticMesh* staticMesh);
typedef SERTDeviceSceneNodeHandle* (SERayTracingDevice::*RayTracingDeviceCreateSceneNode)(SERTDeviceSceneNode* sceneNode, SEISpatialInfo* spatialInfo);
typedef void (SERayTracingDevice::*RayTracingDeviceDeleteSceneNode)(SERTDeviceSceneNode* sceneNode);
typedef void (SERayTracingDevice::*RayTracingDeviceSceneNodeSetTransform)(SERTDeviceSceneNode* sceneNode, SEMatrix3f* srcRotation, SEVector3f* srcLocation);
typedef void (SERayTracingDevice::*RayTracingDeviceSceneNodeSetGeometry)(SERTDeviceSceneNode* sceneNode, SERTDeviceGeometry* geometry);

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 03/20/2017
//----------------------------------------------------------------------------
class SE_RAY_TRACING_ADAPTER_API SERayTracingDevice : public SEReferencable
{
public:
    SERayTracingDevice();
    ~SERayTracingDevice();

    inline 	void Initialize(SERayTracingDeviceDescription* deviceDesc);
    inline 	void Terminate();

    // Load a renderer native scene, such as VRay's scene file format '.vrscene'.
    inline  bool LoadNativeScene(const char* fileName);

    inline  SERTImageHandle* CreateRTImage(SERayTracingDeviceImage* img);
    inline  void DeleteRTImage(SERayTracingDeviceImage* img);

    inline  SERTBitmapHandle* CreateRTBitmap(SERayTracingDeviceBitmap* bmp, SERayTracingDeviceImage* img, int width, int height);
    inline  void DeleteRTBitmap(SERayTracingDeviceBitmap* bmp);

    inline  bool GetImageSize(int& width, int& height);
    inline  bool SetImageSize(int width, int height);

    inline  void Render();

    inline  SERayTracingDeviceImage* GetImage();

    inline  void* RTBitmapGetPixels(SERayTracingDeviceBitmap* bmp);
    inline  void* RTBitmapGetInfoHeader(SERayTracingDeviceBitmap* bmp);

    inline  bool RTImageSaveToBmpFile(SERayTracingDeviceImage* img, const std::string& fileName, bool preserveAlpha, bool invertChannels);

    inline  SERTDeviceCameraHandle* CreateRTDeviceCamera(SERTDeviceCamera* camera);
    inline  void DeleteRTDeviceCamera(SERTDeviceCamera* camera);
    inline  void SetTransformFromCamera(SEICamera* srcCamera, SERTDeviceCamera* dstCamera);

    inline  SERTDeviceLightRectangleHandle* CreateRTDeviceLightRectangle(SERTDeviceLightRectangle* lightRectangle, SEILight* srcLight);
    inline  void DeleteRTDeviceLightRectangle(SERTDeviceLightRectangle* lightRectangle);

    inline  SERTDeviceSkyLightHandle* CreateRTDeviceSkyLight(SERTDeviceSkyLight* skyLight, SEILight* srcLight);
    inline  void DeleteRTDeviceSkyLight(SERTDeviceSkyLight* skyLight);

    inline  SERTDeviceStaticMeshHandle* CreateRTDeviceStaticMesh(SERTDeviceStaticMesh* staticMesh, SEIMetaMesh* srcMesh);
    inline  void DeleteRTDeviceStaticMesh(SERTDeviceStaticMesh* staticMesh);

    inline  SERTDeviceSceneNodeHandle* CreateSceneNode(SERTDeviceSceneNode* sceneNode, SEISpatialInfo* spatialInfo);
    inline  void DeleteSceneNode(SERTDeviceSceneNode* sceneNode);
    inline  void SceneNodeSetTransform(SERTDeviceSceneNode* sceneNode, SEMatrix3f* srcRotation, SEVector3f* srcLocation);
    inline  void SceneNodeSetGeometry(SERTDeviceSceneNode* sceneNode, SERTDeviceGeometry* geometry);

    inline SERayTracingDeviceVendor GetDeviceVendor();

    // ------------------- Delegate Interface ------------------- //

    inline void SetOnRenderStart(void (*CallbackFunc)(SERayTracingDevice&, void*), const void* userData = nullptr);
    template<class T, void (T::*TMethod)(SERayTracingDevice&, void*)>
    inline void SetOnRenderStart(T& object, const void* userData = nullptr);

    inline void SetOnImageReady(void (*CallbackFunc)(SERayTracingDevice&, void*), const void* userData = nullptr);
    template<class T, void (T::*TMethod)(SERayTracingDevice&, void*)>
    inline void SetOnImageReady(T& object, const void* userData = nullptr);

    inline void SetOnDeviceClose(void (*CallbackFunc)(SERayTracingDevice&, void*), const void* userData = nullptr);
    template<class T, void (T::*TMethod)(SERayTracingDevice&, void*)>
    inline void SetOnDeviceClose(T& object, const void* userData = nullptr);

    inline void SetOnRTImageUpdated(void (*CallbackFunc)(SERayTracingDevice&, SERayTracingDeviceImage* img, void*), const void* userData = nullptr);
    template<class T, void (T::*TMethod)(SERayTracingDevice&, SERayTracingDeviceImage* img, void*)>
    inline void SetOnRTImageUpdated(T& object, const void* userData = nullptr);

    inline void SetOnDumpMessage(void (*CallbackFunc)(SERayTracingDevice&, const char* msg, int level, void*), const void* userData = nullptr);
    template<class T, void (T::*TMethod)(SERayTracingDevice&, const char* msg, int level, void*)>
    inline void SetOnDumpMessage(T& object, const void* userData = nullptr);

    // ---------------------------------------------------------- //

protected:
    RayTracingDeviceInitialize                          _Initialize;
    RayTracingDeviceTerminate                           _Terminate;
    RayTracingDeviceLoadNativeScene                     _LoadNativeScene;
    RayTracingDeviceCreateRTImage                       _CreateRTImage;
    RayTracingDeviceDeleteRTImage                       _DeleteRTImage;
    RayTracingDeviceCreateRTBitmap                      _CreateRTBitmap;
    RayTracingDeviceDeleteRTBitmap                      _DeleteRTBitmap;
    RayTracingDeviceGetImageSize                        _GetImageSize;
    RayTracingDeviceSetImageSize                        _SetImageSize;
    RayTracingDeviceRender                              _Render;
    RayTracingDeviceGetImage                            _GetImage;
    RayTracingDeviceRTBitmapGetPixels                   _RTBitmapGetPixels;
    RayTracingDeviceRTBitmapGetInfoHeader               _RTBitmapGetInfoHeader;
    RayTracingDeviceRTImageSaveToBmpFile                _RTImageSaveToBmpFile;
    RayTracingDeviceCreateRTDeviceCamera                _CreateRTDeviceCamera;
    RayTracingDeviceDeleteRTDeviceCamera                _DeleteRTDeviceCamera;
    RayTracingDeviceSetTransformFromCamera              _SetTransformFromCamera;
    RayTracingDeviceCreateRTDeviceLightRectangle        _CreateRTDeviceLightRectangle;
    RayTracingDeviceDeleteRTDeviceLightRectangle        _DeleteRTDeviceLightRectangle;
    RayTracingDeviceCreateRTDeviceSkyLight              _CreateRTDeviceSkyLight;
    RayTracingDeviceDeleteRTDeviceSkyLight              _DeleteRTDeviceSkyLight;
    RayTracingDeviceCreateRTDeviceStaticMesh            _CreateRTDeviceStaticMesh;
    RayTracingDeviceDeleteRTDeviceStaticMesh            _DeleteRTDeviceStaticMesh;
    RayTracingDeviceCreateSceneNode                     _CreateSceneNode;
    RayTracingDeviceDeleteSceneNode                     _DeleteSceneNode;
    RayTracingDeviceSceneNodeSetTransform               _SceneNodeSetTransform;
    RayTracingDeviceSceneNodeSetGeometry                _SceneNodeSetGeometry;


    SERayTracingDeviceDelegate1  RenderStartDelegate;
    SERayTracingDeviceDelegate1  ImageReadyDelegate;
    SERayTracingDeviceDelegate1  DeviceCloseDelegate;
    SERayTracingDeviceDelegate2  RTImageUpdatedDelegate;
    SERayTracingDeviceDelegate3  DumpMessageDelegate;

    static inline void RenderStartCallback(void* rtDevice);
    static inline void ImageReadyCallback(void* rtDevice);
    static inline void DeviceCloseCallback(void* rtDevice);
	static inline void RTImageUpdatedCallback(void* rtDevice, SERayTracingDeviceImage* img);
    static inline void DumpMessageCallback(void* rtDevice, const char* msg, int level);

    SERayTracingDeviceDescription mDeviceDesc;
    SERayTracingDeviceVendor mDeviceVendor;
};

typedef SESmartPointer<SERayTracingDevice> SERayTracingDevicePtr;

#include "SERayTracingDevice.inl"

}

#endif