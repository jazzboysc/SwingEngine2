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
SERayTracingDeviceVendor SERayTracingDevice::GetDeviceVendor()
{
    return mDeviceVendor;
}
//----------------------------------------------------------------------------
void SERayTracingDevice::RenderStartCallback(void* rtDevice)
{
    SERayTracingDevice*& me = reinterpret_cast<SERayTracingDevice*&>(rtDevice);
    me->RenderStartDelegate(*me);
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