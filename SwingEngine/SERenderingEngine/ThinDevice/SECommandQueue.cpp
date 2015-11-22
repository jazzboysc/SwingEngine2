// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015
//
// This part of Swing Engine belongs to an abstraction layer of the next 
// generation GPU graphics and compute API. Currently being constructed to 
// support Microsoft's DirectX 12 and Apple's Metal.

#include "SERenderingEnginePCH.h"
#include "SECommandQueue.h"

using namespace Swing;

//----------------------------------------------------------------------------
SECommandQueue::SECommandQueue(SECommandQueueType type)
    :
    mType(type),
    mCommandQueueHandle(0)
{
}
//----------------------------------------------------------------------------
SECommandQueue::~SECommandQueue()
{
    if( mCommandQueueHandle )
    {
        mCommandQueueHandle->DeviceBase->DeleteCommandQueue(this);
        SE_DELETE mCommandQueueHandle;
        mCommandQueueHandle = 0;
    }
}
//----------------------------------------------------------------------------
void SECommandQueue::CreateDeviceChild(SEThinGPUDevice* device)
{
    if( mCommandQueueHandle || !device )
    {
        SE_ASSERT(false);
        return;
    }

    mCommandQueueHandle = device->CreateCommandQueue(this);
}
//----------------------------------------------------------------------------
SECommandQueueType SECommandQueue::GetType() const
{
    return mType;
}
//----------------------------------------------------------------------------
SECommandQueueHandle* SECommandQueue::GetCommandQueueHandle() const
{
    return mCommandQueueHandle;
}
//----------------------------------------------------------------------------