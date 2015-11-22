// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SEGPUDeviceBase.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEGPUDeviceBase::SEGPUDeviceBase()
    :
    mDefaultRenderCommandQueue(nullptr),
    mDefaultRenderCommandAllocator(nullptr),
    mDefaultRenderCommandList(nullptr),
    mInspector(nullptr)
{
    memset(&mDeviceDesc, 0, sizeof(SEGPUDeviceDescription));
    memset(&mDeviceInfo, 0, sizeof(SEGPUDeviceInfo));
}
//----------------------------------------------------------------------------
SEGPUDeviceBase::~SEGPUDeviceBase()
{
}
//----------------------------------------------------------------------------