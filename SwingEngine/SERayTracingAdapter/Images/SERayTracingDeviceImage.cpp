// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERayTracingAdapterPCH.h"
#include "SERayTracingDeviceImage.h"

using namespace Swing;

//----------------------------------------------------------------------------
SERayTracingDeviceImage::SERayTracingDeviceImage()
	:
    Width(0),
    Height(0),
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
SERayTracingDeviceBitmap* SERayTracingDeviceImage::CreateRTBitmap(int width, int height)
{
    SERayTracingDeviceBitmap* rtBitmap = nullptr;
    if( mImageHandle )
    {
        rtBitmap = SE_NEW SERayTracingDeviceBitmap();
        SERTBitmapHandle* bitmapHandle = mImageHandle->RTDevice->CreateRTBitmap(rtBitmap, this, width, height);
        rtBitmap->SetBitmapHandle(bitmapHandle);
    }

    return rtBitmap;
}
//----------------------------------------------------------------------------
bool SERayTracingDeviceImage::SaveToBmpFile(const std::string& fileName, bool preserveAlpha, bool invertChannels)
{
    bool res = false;
    if( mImageHandle )
    {
        res = mImageHandle->RTDevice->RTImageSaveToBmpFile(this, fileName, preserveAlpha, invertChannels);
    }

    return res;
}
//----------------------------------------------------------------------------