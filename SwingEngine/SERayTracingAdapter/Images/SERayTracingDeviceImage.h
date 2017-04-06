// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_RayTracingDeviceImage_H
#define Swing_RayTracingDeviceImage_H

#include "SERayTracingAdapterLIB.h"
#include "SEReferencable.h"
#include "SERayTracingDeviceResource.h"

namespace Swing
{

class SERayTracingDevice;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 04/05/2017
//----------------------------------------------------------------------------
class SE_RAY_TRACING_ADAPTER_API SERayTracingDeviceImage : public SEReferencable
{
public:
	SERayTracingDeviceImage();
	virtual ~SERayTracingDeviceImage();

    SERTImageHandle* GetImageHandle();
    void SetImageHandle(SERTImageHandle* imageHandle);

protected:
	SERTImageHandle* mImageHandle;
};

typedef SESmartPointer<SERayTracingDeviceImage> SERayTracingDeviceImagePtr;

}

#endif