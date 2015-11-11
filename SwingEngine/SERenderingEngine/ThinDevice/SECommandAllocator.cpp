// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015
//
// This part of Swing Engine belongs to an abstraction layer of the next 
// generation GPU graphics and compute API. Currently being constructed to 
// support Microsoft's DirectX 12 and Apple's Metal.

#include "SERenderingEnginePCH.h"
#include "SECommandAllocator.h"

using namespace Swing;

//----------------------------------------------------------------------------
SECommandAllocator::SECommandAllocator(SECommandAllocatorType type)
    :
    mType(type),
    mCommandAllocatorHandle(0)
{
}
//----------------------------------------------------------------------------
SECommandAllocator::~SECommandAllocator()
{
}
//----------------------------------------------------------------------------
void SECommandAllocator::CreateDeviceChild(SEThinGPUDevice* device)
{
    if( mCommandAllocatorHandle || !device )
    {
        SE_ASSERT(false);
        return;
    }

    mCommandAllocatorHandle = device->CreateCommandAllocator(this);
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