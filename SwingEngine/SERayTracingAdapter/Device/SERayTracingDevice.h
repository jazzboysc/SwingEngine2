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
    typedef void (*tstub)(void* pobject, SERayTracingDevice& rtDevice, void* userObj);
    typedef void (*tfunc)(SERayTracingDevice& rtDevice, void* userObj);

public:
    SERayTracingDeviceDelegate1() {}

    template <class T, void (T::*TMethod)(SERayTracingDevice&, void*)>
    static SERayTracingDeviceDelegate1 FromMethod(T* pobject, void *puserObj)
    {
        return SERayTracingDeviceDelegate1(pobject, &MethodStub<T, TMethod>, puserObj);
    }

    static SERayTracingDeviceDelegate1 FromFunction(tfunc pfunc, void *puserObj)
    {
        return SERayTracingDeviceDelegate1(pfunc, puserObj);
    }

    void operator()(SERayTracingDevice& rtDevice) const
    {
        return pobject ? (*pmethod.pstub)(pobject, rtDevice, puserObj) : (*pmethod.pfunc)(rtDevice, puserObj);
    }

private:
    void* pobject;
    union tmethod
    {
        tstub pstub;
        tfunc pfunc;
        tmethod() : pstub() {}
        tmethod(tstub pstub) : pstub(pstub) {}
        tmethod(tfunc pfunc) : pfunc(pfunc) {}
    } pmethod;
    void* puserObj;

    template <class T, void (T::*TMethod)(SERayTracingDevice&, void*)>
    static void MethodStub(void* pobject, SERayTracingDevice& rtDevice, void *puserObj)
    {
        T* p = static_cast<T*>(pobject);
        return (p->*TMethod)(rtDevice, puserObj);
    }

    SERayTracingDeviceDelegate1(tfunc pfunc, void *puserObj) : pobject(nullptr), pmethod(pfunc), puserObj(puserObj) {}
    SERayTracingDeviceDelegate1(void *pobject, tstub pstub, void *puserObj) : pobject(pobject), pmethod(pstub), puserObj(puserObj) {}
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

protected:
    RayTracingDeviceInitialize               _Initialize;
    RayTracingDeviceTerminate                _Terminate;

    SERayTracingDeviceDelegate1  mRenderStartDelegate;
    SERayTracingDeviceDelegate1  mImageReadyDelegate;
    SERayTracingDeviceDelegate1  mDeviceCloseDelegate;

    SERayTracingDeviceDescription mDeviceDesc;
    SERayTracingDeviceVendor mDeviceVendor;
};

typedef SESmartPointer<SERayTracingDevice> SERayTracingDevicePtr;

#include "SERayTracingDevice.inl"

}

#endif