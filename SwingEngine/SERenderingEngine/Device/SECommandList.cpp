// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015
//
// This part of Swing Engine belongs to an abstraction layer of the next 
// generation GPU graphics and compute API. Currently being constructed to 
// support Microsoft's DirectX 12 and Apple's Metal.

#include "SERenderingEnginePCH.h"
#include "SECommandList.h"

using namespace Swing;

//----------------------------------------------------------------------------
SECommandList::SECommandList(SECommandListType type)
    :
    mType(type),
    mCommandListHandle(nullptr),
    mCommandAllocator(nullptr)
{
}
//----------------------------------------------------------------------------
SECommandList::~SECommandList()
{
    if( mCommandListHandle )
    {
        mCommandListHandle->DeviceBase->DeleteCommandList(this, mCommandAllocator);
        SE_DELETE mCommandListHandle;
        mCommandListHandle = nullptr;
    }
}
//----------------------------------------------------------------------------
void SECommandList::CreateDeviceChild(SEGPUDeviceBase* device, 
    SECommandAllocator* commandAllocator)
{
    if( mCommandListHandle || !device )
    {
        SE_ASSERT(false);
        return;
    }

    mCommandListHandle = device->CreateCommandList(this, commandAllocator);
    mCommandAllocator = commandAllocator;
}
//----------------------------------------------------------------------------
SECommandListType SECommandList::GetType() const
{
    return mType;
}
//----------------------------------------------------------------------------
SECommandListHandle* SECommandList::GetCommandListHandle() const
{
    return mCommandListHandle;
}
//----------------------------------------------------------------------------