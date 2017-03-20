// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017
//
// This part of Swing Engine belongs to an abstraction layer of the next 
// generation GPU graphics and compute API. Currently being constructed to 
// support Microsoft's DirectX 12 and Apple's Metal.

#include "SERenderingEnginePCH.h"
#include "SECommandAllocator.h"
#include "SEGPUDeviceBase.h"

using namespace Swing;

//----------------------------------------------------------------------------
SECommandAllocator::SECommandAllocator(SECommandAllocatorType type)
    :
    mType(type),
    mCommandAllocatorHandle(nullptr),
    mCommandList(nullptr)
{
}
//----------------------------------------------------------------------------
SECommandAllocator::~SECommandAllocator()
{
    if( mCommandAllocatorHandle )
    {
        mCommandAllocatorHandle->DeviceBase->DeleteCommandAllocator(this, mCommandList);
        SE_DELETE mCommandAllocatorHandle;
        mCommandAllocatorHandle = nullptr;
    }
}
//----------------------------------------------------------------------------
void SECommandAllocator::CreateDeviceChild(SEGPUDeviceBase* device,
    SECommandList* commandList)
{
    if( mCommandAllocatorHandle || !device )
    {
        SE_ASSERT(false);
        return;
    }

    mCommandAllocatorHandle = device->CreateCommandAllocator(this, commandList);
    mCommandList = commandList;
}
//----------------------------------------------------------------------------
SECommandAllocatorType SECommandAllocator::GetType() const
{
    return mType;
}
//----------------------------------------------------------------------------
SECommandAllocatorHandle* SECommandAllocator::GetCommandAllocatorHandle() const
{
    return mCommandAllocatorHandle;
}
//----------------------------------------------------------------------------
SECommandList* SECommandAllocator::GetCommandList() const
{
    return mCommandList;
}
//----------------------------------------------------------------------------