// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SEBuffer.h"
#include "SEGPUDevice.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEBuffer::SEBuffer(SEBufferType type)
    :
    mBufferHandle(0),
    mSize(0)
{
    SEBufferViewDesc viewDesc;
    viewDesc.Type = type;
    mDefaultView = SE_NEW SEBufferView(viewDesc);
}
//----------------------------------------------------------------------------
SEBuffer::SEBuffer(SEBufferView* defaultView)
	:
	mBufferHandle(0),
	mSize(0),
    mDefaultView(defaultView)
{
}
//----------------------------------------------------------------------------
SEBuffer::~SEBuffer()
{
    if( mBufferHandle )
    {
        mBufferHandle->Device->DeleteBuffer(this);
        SE_DELETE mBufferHandle;
    }
    mDefaultView = 0;
}
//----------------------------------------------------------------------------
void* SEBuffer::Map(SEBufferAccess access)
{
    SE_ASSERT(mBufferHandle);
    return mBufferHandle->Device->BufferMap(this, access);
}
//----------------------------------------------------------------------------
void SEBuffer::Unmap()
{
    SE_ASSERT(mBufferHandle);
	mBufferHandle->Device->BufferUnmap(this);
}
//----------------------------------------------------------------------------
void SEBuffer::Bind()
{
    SE_ASSERT(mBufferHandle);
	mBufferHandle->Device->BufferBind(this);
}
//----------------------------------------------------------------------------
void SEBuffer::BindTo(SEBufferView* view)
{
    SE_ASSERT(mBufferHandle && view);
    mBufferHandle->Device->BufferBindTo(this, view);
}
//----------------------------------------------------------------------------
bool SEBuffer::LoadFromSystemMemory(SEGPUDevice* device, size_t size, void* data, 
    SEBufferUsage usage)
{
    mDefaultView->CreateDeviceResource(device, this);

	mSize = size;
    mBufferHandle = device->BufferLoadFromSystemMemory(this, 
        size, data, usage);

	return true;
}
//----------------------------------------------------------------------------
void SEBuffer::ReserveMutableDeviceResource(SEGPUDevice* device, size_t size, 
    SEBufferUsage usage)
{
    mDefaultView->CreateDeviceResource(device, this);

	mSize = size;
    mBufferHandle = device->BufferLoadFromSystemMemory(this, 
        size, 0, usage);
}
//----------------------------------------------------------------------------
void SEBuffer::ReserveImmutableDeviceResource(SEGPUDevice* device, size_t size)
{
    mDefaultView->CreateDeviceResource(device, this);

    mSize = size;
    mBufferHandle = device->BufferLoadImmutableFromSystemMemory(this, size, 0);
}
//----------------------------------------------------------------------------
void SEBuffer::Clear(SEBufferInternalFormat internalFormat, SEBufferFormat format,
    SEBufferComponentType type, void* data)
{
    SE_ASSERT(mBufferHandle);
    mBufferHandle->Device->BufferClear(this, internalFormat, 
        format, type, data);
}
//----------------------------------------------------------------------------