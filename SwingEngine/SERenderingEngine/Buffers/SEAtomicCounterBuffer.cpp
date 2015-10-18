// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SEAtomicCounterBuffer.h"

#ifndef __APPLE__

using namespace Swing;

//----------------------------------------------------------------------------
SEAtomicCounterBuffer::SEAtomicCounterBuffer()
	:
    SEIndexableBuffer(BT_AtomicCounter)
{
}
//----------------------------------------------------------------------------
SEAtomicCounterBuffer::~SEAtomicCounterBuffer()
{
}
//----------------------------------------------------------------------------

#endif