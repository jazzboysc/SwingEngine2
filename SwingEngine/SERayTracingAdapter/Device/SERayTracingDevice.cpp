// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERayTracingAdapterPCH.h"
#include "SERayTracingDevice.h"

using namespace Swing;

//----------------------------------------------------------------------------
SERayTracingDevice::SERayTracingDevice()
    :
    mDeviceVendor(RTDV_Unknown)
{
    for( int i = 0; i < RTDRET_Max; ++i )
    {
        mRenderElements[i] = nullptr;
    }
}
//----------------------------------------------------------------------------
SERayTracingDevice::~SERayTracingDevice()
{
}
//----------------------------------------------------------------------------