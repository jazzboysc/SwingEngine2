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