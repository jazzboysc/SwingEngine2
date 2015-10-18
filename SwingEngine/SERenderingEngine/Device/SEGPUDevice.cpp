// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SEGPUDevice.h"
#include "SEGPUDeviceInspector.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEGPUDevice::SEGPUDevice()
{
	memset(&mDeviceDesc, 0, sizeof(SEGPUDeviceDescription));
	memset(&mDeviceInfo, 0, sizeof(SEGPUDeviceInfo));
}
//----------------------------------------------------------------------------
SEGPUDevice::~SEGPUDevice()
{
}
//----------------------------------------------------------------------------
void SEGPUDevice::SetInspector(SEGPUDeviceInspector* inspector)
{
    if( mInspector && mInspector != inspector )
    {
        mInspector->OnDetach();
    }
    mInspector = inspector;
}
//----------------------------------------------------------------------------