// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SEIndexableBuffer.h"
#include "SEGPUDevice.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEIndexableBuffer::SEIndexableBuffer(SEBufferType type)
    :
    SEBuffer(type)
{
}
//----------------------------------------------------------------------------
SEIndexableBuffer::SEIndexableBuffer()
    :
    SEBuffer(BT_Typeless)
{
}
//----------------------------------------------------------------------------
SEIndexableBuffer::~SEIndexableBuffer()
{
}
//----------------------------------------------------------------------------
void SEIndexableBuffer::Bind(unsigned int index)
{
    SE_ASSERT(mBufferHandle);
    mBufferHandle->Device->BufferBindIndex(this, index);
}
//----------------------------------------------------------------------------
void SEIndexableBuffer::BindTo(unsigned int index, SEBufferView* view)
{
    SE_ASSERT(mBufferHandle);
    mBufferHandle->Device->BufferBindIndexTo(this, index, view);
}
//----------------------------------------------------------------------------
void SEIndexableBuffer::UpdateSubData(unsigned int index, int offset, 
    size_t size, void* data)
{
    SE_ASSERT(mBufferHandle);
    Bind(index);
    mBufferHandle->Device->BufferUpdateSubData(this, offset,
        size, data);
}
//----------------------------------------------------------------------------