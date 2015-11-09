// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SEGPUDeviceBase.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEGPUDeviceBase::SEGPUDeviceBase()
{
    memset(&mDeviceDesc, 0, sizeof(SEGPUDeviceDescription));
    memset(&mDeviceInfo, 0, sizeof(SEGPUDeviceInfo));
    mInspector = 0;
}
//----------------------------------------------------------------------------
SEGPUDeviceBase::~SEGPUDeviceBase()
{
}
//----------------------------------------------------------------------------