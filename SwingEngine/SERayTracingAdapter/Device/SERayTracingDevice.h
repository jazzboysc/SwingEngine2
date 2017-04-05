// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_RayTracingDevice_H
#define Swing_RayTracingDevice_H

#include "SERayTracingAdapterLIB.h"
#include "SEReferencable.h"

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
    RTDRM_RT_MAX
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

typedef void (SERayTracingDevice::*RayTracingDeviceInitialize)(SERayTracingDeviceDescription* deviceDesc);
typedef void (SERayTracingDevice::*RayTracingDeviceTerminate)();
typedef bool (SERayTracingDevice::*RayTracingDeviceLoadNativeScene)(const char* fileName);

//----------------------------------------------------------------------------
class SERayTracingDeviceDelegate1
{
    typedef void (*RayTracingDeviceDelegate1Stub)(void* pObject, SERayTracingDevice& rtDevice, void* userObj);
    typedef void (*RayTracingDeviceDelegate1Func)(SERayTracingDevice& rtDevice, void* userObj);

public:
    SERayTracingDeviceDelegate1() { pObject = nullptr; pMethod.pfunc = nullptr; pUserObj = nullptr; }

    template <class T, void (T::*TMethod)(SERayTracingDevice&, void*)>
    static SERayTracingDeviceDelegate1 FromMethod(T* pObject, void *pUserObj)
    {
        return SERayTracingDeviceDelegate1(pObject, &MethodStub<T, TMethod>, pUserObj);
    }

    static SERayTracingDeviceDelegate1 FromFunction(RayTracingDeviceDelegate1Func pfunc, void *pUserObj)
    {
        return SERayTracingDeviceDelegate1(pfunc, pUserObj);
    }

    void operator()(SERayTracingDevice& rtDevice) const
    {
        return pObject ? (*pMethod.pstub)(pObject, rtDevice, pUserObj) : (*pMethod.pfunc)(rtDevice, pUserObj);
    }

    inline bool IsValid() const
    {
        return pMethod.pstub ? true : false;
    }

private:
    void* pObject;
    union tmethod
    {
        RayTracingDeviceDelegate1Stub pstub;
        RayTracingDeviceDelegate1Func pfunc;
        tmethod() : pstub() {}
        tmethod(RayTracingDeviceDelegate1Stub pstub) : pstub(pstub) {}
        tmethod(RayTracingDeviceDelegate1Func pfunc) : pfunc(pfunc) {}
    } pMethod;
    void* pUserObj;

    template <class T, void (T::*TMethod)(SERayTracingDevice&, void*)>
    static void MethodStub(void* pObject, SERayTracingDevice& rtDevice, void *pUserObj)
    {
        T* p = static_cast<T*>(pObject);
        return (p->*TMethod)(rtDevice, pUserObj);
    }

    SERayTracingDeviceDelegate1(RayTracingDeviceDelegate1Func pfunc, void *pUserObj) : pObject(nullptr), pMethod(pfunc), pUserObj(pUserObj) {}
    SERayTracingDeviceDelegate1(void *pObject, RayTracingDeviceDelegate1Stub pstub, void *pUserObj) : pObject(pObject), pMethod(pstub), pUserObj(pUserObj) {}
};
//----------------------------------------------------------------------------
class SERayTracingDeviceDelegate2
{
    typedef void (*RayTracingDeviceDelegate2Stub)(void* pObject, SERayTracingDevice& rtDevice, SERayTracingDeviceImage* img, void* userObj);
    typedef void (*RayTracingDeviceDelegate2Func)(SERayTracingDevice& rtDevice, SERayTracingDeviceImage* img, void* userObj);

public:
    SERayTracingDeviceDelegate2() {}

    template <class T, void (T::*TMethod)(SERayTracingDevice&, SERayTracingDeviceImage*, void*)>
    static SERayTracingDeviceDelegate2 FromMethod(T* pObject, void *pUserObj)
    {
        return SERayTracingDeviceDelegate2(pObject, &MethodStub<T, TMethod>, pUserObj);
    }

    static SERayTracingDeviceDelegate2 FromFunction(RayTracingDeviceDelegate2Func pfunc, void *pUserObj)
    {
        return SERayTracingDeviceDelegate2(pfunc, pUserObj);
    }

    void operator()(SERayTracingDevice& rtDevice, SERayTracingDeviceImage* img) const
    {
        return pObject ? (*pMethod.pstub)(pObject, rtDevice, img, pUserObj) : (*pMethod.pfunc)(rtDevice, img, pUserObj);
    }

    inline bool IsValid() const
    {
        return pMethod.pstub ? true : false;
    }

private:
    void* pObject;
    union tmethod
    {
        RayTracingDeviceDelegate2Stub pstub;
        RayTracingDeviceDelegate2Func pfunc;
        tmethod() : pstub() {}
        tmethod(RayTracingDeviceDelegate2Stub pstub) : pstub(pstub) {}
        tmethod(RayTracingDeviceDelegate2Func pfunc) : pfunc(pfunc) {}
    } pMethod;
    void* pUserObj;

    template <class T, void (T::*TMethod)(SERayTracingDevice&, SERayTracingDeviceImage* img, void*)>
    static void MethodStub(void* pObject, SERayTracingDevice& rtDevice, SERayTracingDeviceImage* img, void* pUserObj)
    {
        T* p = static_cast<T*>(pObject);
        return (p->*TMethod)(rtDevice, img, pUserObj);
    }

    SERayTracingDeviceDelegate2(RayTracingDeviceDelegate2Func pfunc, void* pUserObj) : pObject(), pMethod(pfunc), pUserObj(pUserObj) {}
    SERayTracingDeviceDelegate2(void* pObject, RayTracingDeviceDelegate2Stub pstub, void* pUserObj) : pObject(pObject), pMethod(pstub), pUserObj(pUserObj) {}
};
//----------------------------------------------------------------------------
class SERayTracingDeviceDelegate3
{
    typedef void (*RayTracingDeviceDelegate3Stub)(void* pObject, SERayTracingDevice& rtDevice, const char* msg, int level, void* userObj);
    typedef void (*RayTracingDeviceDelegate3Func)(SERayTracingDevice& rtDevice, const char* msg, int level, void* userObj);

public:
    SERayTracingDeviceDelegate3() {}

    template <class T, void (T::*TMethod)(SERayTracingDevice&, const char*, int, void*)>
    static SERayTracingDeviceDelegate3 FromMethod(T* pObject, void *pUserObj)
    {
        return SERayTracingDeviceDelegate3(pObject, &MethodStub<T, TMethod>, pUserObj);
    }

    static SERayTracingDeviceDelegate3 FromFunction(RayTracingDeviceDelegate3Func pfunc, void* pUserObj)
    {
        return SERayTracingDeviceDelegate3(pfunc, pUserObj);
    }

    void operator()(SERayTracingDevice& rtDevice, const char* msg, int level) const
    {
        return pObject ? (*pMethod.pstub)(pObject, rtDevice, msg, level, pUserObj) : (*pMethod.pfunc)(rtDevice, msg, level, pUserObj);
    }

    inline bool IsValid() const
    {
        return pMethod.pstub ? true : false;
    }

private:
    void* pObject;
    union tmethod
    {
        RayTracingDeviceDelegate3Stub pstub;
        RayTracingDeviceDelegate3Func pfunc;
        tmethod() : pstub() {}
        tmethod(RayTracingDeviceDelegate3Stub pstub) : pstub(pstub) {}
        tmethod(RayTracingDeviceDelegate3Func pfunc) : pfunc(pfunc) {}
    } pMethod;
    void* pUserObj;

    template <class T, void (T::*TMethod)(SERayTracingDevice&, const char* msg, int level, void*)>
    static void MethodStub(void* pObject, SERayTracingDevice& rtDevice, const char* msg, int level, void* pUserObj)
    {
        T* p = static_cast<T*>(pObject);
        return (p->*TMethod)(rtDevice, msg, level, pUserObj);
    }

    SERayTracingDeviceDelegate3(RayTracingDeviceDelegate3Func pfunc, void* pUserObj) : pObject(), pMethod(pfunc), pUserObj(pUserObj) {}
    SERayTracingDeviceDelegate3(void* pObject, RayTracingDeviceDelegate3Stub pstub, void* pUserObj) : pObject(pObject), pMethod(pstub), pUserObj(pUserObj) {}
};
//----------------------------------------------------------------------------

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

    inline SERayTracingDeviceVendor GetDeviceVendor();

    inline void SetOnRenderStart(void (*CallbackFunc)(SERayTracingDevice&, void*), const void* userData = nullptr);

    template<class T, void (T::*TMethod)(SERayTracingDevice&, void*)>
    inline void SetOnRenderStart(T& object, const void* userData = nullptr);

    inline void SetOnImageReady(void (*CallbackFunc)(SERayTracingDevice&, void*), const void* userData = nullptr);

    template<class T, void (T::*TMethod)(SERayTracingDevice&, void*)>
    inline void SetOnImageReady(T& object, const void* userData = nullptr);

    inline void SetOnDeviceClose(void (*CallbackFunc)(SERayTracingDevice&, void*), const void* userData = nullptr);

    template<class T, void (T::*TMethod)(SERayTracingDevice&, void*)>
    inline void SetOnDeviceClose(T& object, const void* userData = nullptr);

protected:
    RayTracingDeviceInitialize               _Initialize;
    RayTracingDeviceTerminate                _Terminate;
    RayTracingDeviceLoadNativeScene          _LoadNativeScene;

    SERayTracingDeviceDelegate1  RenderStartDelegate;
    SERayTracingDeviceDelegate1  ImageReadyDelegate;
    SERayTracingDeviceDelegate1  DeviceCloseDelegate;
    SERayTracingDeviceDelegate2  RTImageUpdatedDelegate;
    SERayTracingDeviceDelegate3  DumpMessageDelegate;

    static inline void RenderStartCallback(void* rtDevice);
    static inline void ImageReadyCallback(void* rtDevice);
    static inline void DeviceCloseCallback(void* rtDevice);

    SERayTracingDeviceDescription mDeviceDesc;
    SERayTracingDeviceVendor mDeviceVendor;
};

typedef SESmartPointer<SERayTracingDevice> SERayTracingDevicePtr;

#include "SERayTracingDevice.inl"

}

#endif