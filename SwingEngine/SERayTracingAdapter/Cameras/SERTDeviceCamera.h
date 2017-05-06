// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_RTDeviceCamera_H
#define Swing_RTDeviceCamera_H

#include "SERayTracingAdapterLIB.h"
#include "SEReferencable.h"
#include "SERayTracingDeviceResource.h"
#include "SEICamera.h"

namespace Swing
{

class SERayTracingDevice;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 05/06/2017
//----------------------------------------------------------------------------
class SE_RAY_TRACING_ADAPTER_API SERTDeviceCamera : public SEReferencable
{
public:
    SERTDeviceCamera();
    virtual ~SERTDeviceCamera();

    void CreateDeviceResource(SERayTracingDevice& device);
    void SetTransformFromCamera(SEICamera* camera);

protected:
    SERTDeviceCameraHandle* mCameraHandle;
};

typedef SESmartPointer<SERTDeviceCamera> SERTDeviceCameraPtr;

}

#endif