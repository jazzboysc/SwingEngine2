// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERayTracingAdapterPCH.h"
#include "SERayTracingDeviceImage.h"

using namespace Swing;

//----------------------------------------------------------------------------
SERayTracingDeviceImage::SERayTracingDeviceImage()
	:
	mImageHandle(nullptr)
{
}
//----------------------------------------------------------------------------
SERayTracingDeviceImage::~SERayTracingDeviceImage()
{
    if( mImageHandle )
    {
        mImageHandle->RTDevice->DeleteRTImage(this);
        SE_DELETE mImageHandle;
    }
}
//----------------------------------------------------------------------------
SERTImageHandle* SERayTracingDeviceImage::GetImageHandle()
{
    return mImageHandle;
}
//----------------------------------------------------------------------------
void SERayTracingDeviceImage::SetImageHandle(SERTImageHandle* imageHandle)
{
    mImageHandle = imageHandle;
}
//----------------------------------------------------------------------------