// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_RTDeviceMaterial_H
#define Swing_RTDeviceMaterial_H

#include "SERayTracingAdapterLIB.h"
#include "SEReferencable.h"
#include "SERayTracingDeviceResource.h"

namespace Swing
{

class SERayTracingDevice;

enum SERTDeviceMaterialType
{
    RTDMT_SingleBRDF,
    RTDMT_Unknown
};

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 05/30/2017
//----------------------------------------------------------------------------
class SE_RAY_TRACING_ADAPTER_API SERTDeviceMaterial : public SEReferencable
{
public:
    virtual ~SERTDeviceMaterial();

    void CreateDeviceResource(SERayTracingDevice& device);

    SERTDeviceMaterialType GetMaterialType() const;
    SERTDeviceMaterialHandle* GetMaterialHandle();

protected:
    // Abstract base class.
    SERTDeviceMaterial(SERTDeviceMaterialType materialType = RTDMT_Unknown);

    SERTDeviceMaterialType mMaterialType;
    SERTDeviceMaterialHandle* mMaterialHandle;
};

typedef SESmartPointer<SERTDeviceMaterial> SERTDeviceMaterialPtr;

}

#endif