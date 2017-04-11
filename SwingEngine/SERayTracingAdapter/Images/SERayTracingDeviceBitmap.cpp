// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERayTracingAdapterPCH.h"
#include "SERayTracingDeviceBitmap.h"

using namespace Swing;

//----------------------------------------------------------------------------
SERayTracingDeviceBitmap::SERayTracingDeviceBitmap()
	:
	mBitmapHandle(nullptr)
{
}
//----------------------------------------------------------------------------
SERayTracingDeviceBitmap::~SERayTracingDeviceBitmap()
{
    if( mBitmapHandle )
    {
        mBitmapHandle->RTDevice->DeleteRTBitmap(this);
        SE_DELETE mBitmapHandle;
    }
}
//----------------------------------------------------------------------------
SERTBitmapHandle* SERayTracingDeviceBitmap::GetBitmapHandle()
{
    return mBitmapHandle;
}
//----------------------------------------------------------------------------
void SERayTracingDeviceBitmap::SetBitmapHandle(SERTBitmapHandle* bitmapHandle)
{
    mBitmapHandle = bitmapHandle;
}
//----------------------------------------------------------------------------
void* SERayTracingDeviceBitmap::GetPixels()
{
    void* res = nullptr;
    if( mBitmapHandle )
    {
        res = mBitmapHandle->RTDevice->RTBitmapGetPixels(this);
    }

    return res;
}
//----------------------------------------------------------------------------
void* SERayTracingDeviceBitmap::GetBitmapInfoHeader()
{
    void* res = nullptr;
    if( mBitmapHandle )
    {
        res = mBitmapHandle->RTDevice->RTBitmapGetInfoHeader(this);
    }

    return res;
}
//----------------------------------------------------------------------------