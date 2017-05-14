// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_RTDeviceLightRectangle_H
#define Swing_RTDeviceLightRectangle_H

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
class SE_RAY_TRACING_ADAPTER_API SERTDeviceLightRectangle : public SEReferencable
{
public:
    SERTDeviceLightRectangle();
    virtual ~SERTDeviceLightRectangle();

    void CreateDeviceResource(SERayTracingDevice& device, SEILight* srcLight = nullptr);
    SERTDeviceLightRectangleHandle* GetLightRectangleHandle();

protected:
    SERTDeviceLightRectangleHandle* mLightRectangleHandle;
};

typedef SESmartPointer<SERTDeviceLightRectangle> SERTDeviceLightRectanglePtr;

}

#endif