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
bool SERayTracingDevice::LoadNativeScene(const char* fileName)
{
    (this->*_LoadNativeScene)(fileName);
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