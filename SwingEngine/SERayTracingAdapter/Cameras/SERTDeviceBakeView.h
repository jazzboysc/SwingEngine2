// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_RTDeviceBakeView_H
#define Swing_RTDeviceBakeView_H

#include "SERayTracingAdapterLIB.h"
#include "SEReferencable.h"
#include "SERayTracingDeviceResource.h"

namespace Swing
{

class SERayTracingDevice;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 06/03/2017
//----------------------------------------------------------------------------
class SE_RAY_TRACING_ADAPTER_API SERTDeviceBakeView : public SEReferencable
{
public:
    SERTDeviceBakeView();
    virtual ~SERTDeviceBakeView();

    void CreateDeviceResource(SERayTracingDevice& device);
    SERTDeviceBakeViewHandle* GetBakeViewHandle();

protected:
    SERTDeviceBakeViewHandle* mBakeViewHandle;
};

typedef SESmartPointer<SERTDeviceBakeView> SERTDeviceBakeViewPtr;

}

#endif