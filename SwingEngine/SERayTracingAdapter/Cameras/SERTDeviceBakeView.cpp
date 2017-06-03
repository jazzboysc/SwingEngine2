// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERayTracingAdapterPCH.h"
#include "SERTDeviceBakeView.h"

using namespace Swing;

//----------------------------------------------------------------------------
SERTDeviceBakeView::SERTDeviceBakeView()
    :
    mBakeViewHandle(nullptr)
{
}
//----------------------------------------------------------------------------
SERTDeviceBakeView::~SERTDeviceBakeView()
{
    if( mBakeViewHandle )
    {
        mBakeViewHandle->RTDevice->DeleteBakeView(this);
        SE_DELETE mBakeViewHandle;
        mBakeViewHandle = nullptr;
    }
}
//----------------------------------------------------------------------------
void SERTDeviceBakeView::CreateDeviceResource(SERayTracingDevice& device, SERTDeviceBakeViewDescription* bakeViewDesc)
{
    if( !mBakeViewHandle )
    {
        mBakeViewHandle = device.CreateBakeView(this, bakeViewDesc);
    }
}
//----------------------------------------------------------------------------
SERTDeviceBakeViewHandle* SERTDeviceBakeView::GetBakeViewHandle()
{
    return mBakeViewHandle;
}
//----------------------------------------------------------------------------