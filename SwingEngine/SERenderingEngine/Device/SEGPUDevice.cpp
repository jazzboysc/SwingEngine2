// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERenderingEnginePCH.h"
#include "SEGPUDevice.h"
#include "SEGPUDeviceInspector.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEGPUDevice::SEGPUDevice()
{
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