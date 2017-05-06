// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_RayTracingDeviceResource_H
#define Swing_RayTracingDeviceResource_H

#include "SERayTracingAdapterLIB.h"
#include "SEReferencable.h"

namespace Swing
{

class SERayTracingDevice;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 04/05/2017
//----------------------------------------------------------------------------
class SE_RAY_TRACING_ADAPTER_API SERayTracingDeviceResource : public SEReferencable
{
public:
	SERayTracingDeviceResource();
	virtual ~SERayTracingDeviceResource();

	SERayTracingDevice* RTDevice;
};

typedef SESmartPointer<SERayTracingDeviceResource> SERayTracingDeviceResourcePtr;

struct SE_RAY_TRACING_ADAPTER_API SERTImageHandle : public SERayTracingDeviceResource
{
};

struct SE_RAY_TRACING_ADAPTER_API SERTBitmapHandle : public SERayTracingDeviceResource
{
};

struct SE_RAY_TRACING_ADAPTER_API SERTDeviceCameraHandle : public SERayTracingDeviceResource
{
};

}

#endif