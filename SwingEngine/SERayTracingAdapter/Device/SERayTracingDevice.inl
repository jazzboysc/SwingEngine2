// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

//----------------------------------------------------------------------------
void SERayTracingDevice::Initialize(SERayTracingDeviceDescription* deviceDesc)
{
    mDeviceDesc = *deviceDesc;
    (this->*_Initialize)(deviceDesc);
}
//----------------------------------------------------------------------------
void SERayTracingDevice::Terminate()
{
    (this->*_Terminate)();
}
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
    me->ImageReadyDelegate(*me);
}
//----------------------------------------------------------------------------
void SERayTracingDevice::DeviceCloseCallback(void* rtDevice)
{
    SERayTracingDevice*& me = reinterpret_cast<SERayTracingDevice*&>(rtDevice);
    me->DeviceCloseDelegate(*me);
}
//----------------------------------------------------------------------------