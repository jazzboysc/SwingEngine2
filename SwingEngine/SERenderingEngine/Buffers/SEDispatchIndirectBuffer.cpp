// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERenderingEnginePCH.h"
#include "SEDispatchIndirectBuffer.h"

#ifndef __APPLE__

using namespace Swing;

//----------------------------------------------------------------------------
SEDispatchIndirectBuffer::SEDispatchIndirectBuffer()
    :
    SEBuffer(BT_DispatchIndirect)
{
}
//----------------------------------------------------------------------------
SEDispatchIndirectBuffer::~SEDispatchIndirectBuffer()
{
}
//----------------------------------------------------------------------------

#endif