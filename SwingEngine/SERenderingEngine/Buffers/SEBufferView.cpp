// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERenderingEnginePCH.h"
#include "SEBufferView.h"
#include "SEBufferBase.h"
#include "SEGPUDevice.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEBufferView::SEBufferView(const SEBufferViewDesc& viewDesc)
    :
    mViewDesc(viewDesc),
    mBufferViewHandle(0),
    mBufferBase(0)
{
}
//----------------------------------------------------------------------------
SEBufferView::~SEBufferView()
{
    if( mBufferViewHandle )
    {
        mBufferViewHandle->Device->DeleteBufferView(this);
        SE_DELETE mBufferViewHandle;
    }
}
//----------------------------------------------------------------------------
void SEBufferView::CreateDeviceResource(SEGPUDevice* device, 
    SEBufferBase* bufferBase)
{
    if( !mBufferViewHandle )
    {
        mBufferBase = bufferBase;
        mBufferViewHandle = device->CreateBufferView(this);
    }
}
//----------------------------------------------------------------------------