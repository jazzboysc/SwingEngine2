// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

//----------------------------------------------------------------------------
void SEGPUDeviceBase::Terminate()
{
    (this->*_Terminate)();
}
//----------------------------------------------------------------------------
void SEGPUDeviceBase::Initialize(SEGPUDeviceDescription* deviceDesc)
{
    mDeviceDesc = *deviceDesc;
    (this->*_Initialize)(deviceDesc);
}
//----------------------------------------------------------------------------
void SEGPUDeviceBase::SetAnisFilterLevel(int maxAnisFilterLevel)
{
    (this->*_SetAnisFilterLevel)(maxAnisFilterLevel);
}
//----------------------------------------------------------------------------
void SEGPUDeviceBase::GetMaxAnisFilterLevel(int* maxAnisFilterLevel)
{
    (this->*_GetMaxAnisFilterLevel)(maxAnisFilterLevel);
}
//----------------------------------------------------------------------------