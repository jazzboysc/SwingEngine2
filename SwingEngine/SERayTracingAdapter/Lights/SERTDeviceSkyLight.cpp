// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERayTracingAdapterPCH.h"
#include "SERTDeviceSkyLight.h"

using namespace Swing;

//----------------------------------------------------------------------------
SERTDeviceSkyLight::SERTDeviceSkyLight()
    :
    mSkyLightHandle(nullptr)
{
}
//----------------------------------------------------------------------------
SERTDeviceSkyLight::~SERTDeviceSkyLight()
{
    if( mSkyLightHandle )
    {
        mSkyLightHandle->RTDevice->DeleteRTDeviceSkyLight(this);
        SE_DELETE mSkyLightHandle;
        mSkyLightHandle = nullptr;
    }
}
//----------------------------------------------------------------------------
void SERTDeviceSkyLight::CreateDeviceResource(SERayTracingDevice& device, SEILight* srcLight)
{
    if( !mSkyLightHandle )
    {
        mSkyLightHandle = device.CreateRTDeviceSkyLight(this, srcLight);
    }
}
//----------------------------------------------------------------------------
SERTDeviceSkyLightHandle* SERTDeviceSkyLight::GetSkyLightHandle()
{
    return mSkyLightHandle;
}
//----------------------------------------------------------------------------