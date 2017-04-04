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

typedef void (SERayTracingDevice::*RayTracingDeviceInitialize)(SERayTracingDeviceDescription* deviceDesc);
typedef void (SERayTracingDevice::*RayTracingDeviceTerminate)();

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
        return pMethod.pfunc ? true : false;
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

    SERayTracingDeviceDelegate1  RenderStartDelegate;
    SERayTracingDeviceDelegate1  ImageReadyDelegate;
    SERayTracingDeviceDelegate1  DeviceCloseDelegate;

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