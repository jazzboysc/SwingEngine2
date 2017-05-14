// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_RTDeviceSkyLight_H
#define Swing_RTDeviceSkyLight_H

#include "SERayTracingAdapterLIB.h"
#include "SEReferencable.h"
#include "SERayTracingDeviceResource.h"
#include "SEILight.h"

namespace Swing
{

class SERayTracingDevice;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 05/14/2017
//----------------------------------------------------------------------------
class SE_RAY_TRACING_ADAPTER_API SERTDeviceSkyLight : public SEReferencable
{
public:
    SERTDeviceSkyLight();
    virtual ~SERTDeviceSkyLight();

    void CreateDeviceResource(SERayTracingDevice& device, SEILight* srcLight = nullptr);
    SERTDeviceSkyLightHandle* GetSkyLightHandle();

protected:
    SERTDeviceSkyLightHandle* mSkyLightHandle;
};

typedef SESmartPointer<SERTDeviceSkyLight> SERTDeviceSkyLightPtr;

}

#endif