// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERayTracingAdapterPCH.h"
#include "SERTDeviceLightRectangle.h"

using namespace Swing;

//----------------------------------------------------------------------------
SERTDeviceLightRectangle::SERTDeviceLightRectangle()
    :
    mLightRectangleHandle(nullptr)
{
}
//----------------------------------------------------------------------------
SERTDeviceLightRectangle::~SERTDeviceLightRectangle()
{
    if( mLightRectangleHandle )
    {
        mLightRectangleHandle->RTDevice->DeleteRTDeviceLightRectangle(this);
        SE_DELETE mLightRectangleHandle;
        mLightRectangleHandle = nullptr;
    }
}
//----------------------------------------------------------------------------
void SERTDeviceLightRectangle::CreateDeviceResource(SERayTracingDevice& device, SEILight* srcLight)
{
    if( !mLightRectangleHandle )
    {
        mLightRectangleHandle = device.CreateRTDeviceLightRectangle(this, srcLight);
    }
}
//----------------------------------------------------------------------------
SERTDeviceLightRectangleHandle* SERTDeviceLightRectangle::GetLightRectangleHandle()
{
    return mLightRectangleHandle;
}
//----------------------------------------------------------------------------