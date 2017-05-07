// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERayTracingAdapterPCH.h"
#include "SERTDeviceCamera.h"

using namespace Swing;

//----------------------------------------------------------------------------
SERTDeviceCamera::SERTDeviceCamera()
    :
    mCameraHandle(nullptr)
{
}
//----------------------------------------------------------------------------
SERTDeviceCamera::~SERTDeviceCamera()
{
    if( mCameraHandle )
    {
        mCameraHandle->RTDevice->DeleteRTDeviceCamera(this);
        SE_DELETE mCameraHandle;
        mCameraHandle = nullptr;
    }
}
//----------------------------------------------------------------------------
void SERTDeviceCamera::CreateDeviceResource(SERayTracingDevice& device)
{
    if( !mCameraHandle )
    {
        mCameraHandle = device.CreateRTDeviceCamera(this);
    }
}
//----------------------------------------------------------------------------
void SERTDeviceCamera::SetTransformFromCamera(SEICamera* camera)
{
    if( camera && mCameraHandle )
    {
        mCameraHandle->RTDevice->SetTransformFromCamera(camera, this);
    }
}
//----------------------------------------------------------------------------
SERTDeviceCameraHandle* SERTDeviceCamera::GetCameraHandle()
{
    return mCameraHandle;
}
//----------------------------------------------------------------------------