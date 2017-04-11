// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_RayTracingDeviceBitmap_H
#define Swing_RayTracingDeviceBitmap_H

#include "SERayTracingAdapterLIB.h"
#include "SEReferencable.h"
#include "SERayTracingDeviceResource.h"

namespace Swing
{

class SERayTracingDevice;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 04/09/2017
//----------------------------------------------------------------------------
class SE_RAY_TRACING_ADAPTER_API SERayTracingDeviceBitmap : public SEReferencable
{
public:
	SERayTracingDeviceBitmap();
	virtual ~SERayTracingDeviceBitmap();

    SERTBitmapHandle* GetBitmapHandle();
    void SetBitmapHandle(SERTBitmapHandle* bitmapHandle);

    void* GetPixels();
    void* GetBitmapInfoHeader();

protected:
	SERTBitmapHandle* mBitmapHandle;
};

typedef SESmartPointer<SERayTracingDeviceBitmap> SERayTracingDeviceBitmapPtr;

}

#endif