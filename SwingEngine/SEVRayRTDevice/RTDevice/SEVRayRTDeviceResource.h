// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_VRayRTDeviceResource_H
#define Swing_VRayRTDeviceResource_H

#include "SEVRayRTDeviceLIB.h"
#include "SERayTracingDeviceResource.h"

#include "SEVRayTypes.h"

namespace Swing
{

//----------------------------------------------------------------------------
struct SE_VRAY_RT_DEVICE_API SEVRayRTImageHandle : public SERTImageHandle
{
	VRay::VRayImage* mImage;

	SEVRayRTImageHandle();
	~SEVRayRTImageHandle();
};
//----------------------------------------------------------------------------

}

#endif