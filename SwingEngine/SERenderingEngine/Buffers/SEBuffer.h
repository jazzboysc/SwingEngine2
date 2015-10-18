// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_Buffer_H
#define Swing_Buffer_H

#include "SERenderingEngineLIB.h"
#include "SEBufferBase.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 07/21/2013
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEBuffer : public SEBufferBase
{
public:
    SEBuffer(SEBufferType type);
    SEBuffer(SEBufferView* defaultView);
	virtual ~SEBuffer();

    void* Map(SEBufferAccess access);
	void Unmap();

	void Bind();
    void BindTo(SEBufferView* view);

	// Load buffer data from system memory. User is responsible for deleting
	// the system memory data.
    bool LoadFromSystemMemory(SEGPUDevice* device, size_t size, void* data, 
        SEBufferUsage usage);

    void ReserveMutableDeviceResource(SEGPUDevice* device, size_t size, 
        SEBufferUsage usage);

    void ReserveImmutableDeviceResource(SEGPUDevice* device, size_t size);

    void Clear(SEBufferInternalFormat internalFormat, SEBufferFormat format, 
        SEBufferComponentType type, void* data);

    inline SEBufferHandle* GetBufferHandle() const { return mBufferHandle; }
    inline size_t GetSize() const { return mSize; }
    inline SEBufferType GetType() const { return mDefaultView->GetBufferType(); }

protected:
    SEBufferHandle* mBufferHandle;
    size_t mSize;
    SEBufferViewPtr mDefaultView;
};

typedef SESmartPointer<SEBuffer> SEBufferPtr;

}

#endif