// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015
//
// This part of Swing Engine belongs to an abstraction layer of the next 
// generation GPU graphics and compute API. Currently being constructed to 
// support Microsoft's DirectX 12 and Apple's Metal.

#include "SERenderingEnginePCH.h"
#include "SEDescriptorHeap.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEDescriptorHeap::SEDescriptorHeap()
    :
    mDescriptorHeapHandle(0)
{
}
//----------------------------------------------------------------------------
SEDescriptorHeap::~SEDescriptorHeap()
{
}
//----------------------------------------------------------------------------