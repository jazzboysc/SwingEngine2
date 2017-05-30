// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_RTDeviceSingleBRDFMaterial_H
#define Swing_RTDeviceSingleBRDFMaterial_H

#include "SERayTracingAdapterLIB.h"
#include "SEReferencable.h"
#include "SERayTracingDeviceResource.h"
#include "SERTDeviceMaterial.h"

namespace Swing
{

class SERayTracingDevice;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 05/30/2017
//----------------------------------------------------------------------------
class SE_RAY_TRACING_ADAPTER_API SERTDeviceSingleBRDFMaterial : public SERTDeviceMaterial
{
public:
    SERTDeviceSingleBRDFMaterial();
    virtual ~SERTDeviceSingleBRDFMaterial();

};

typedef SESmartPointer<SERTDeviceSingleBRDFMaterial> SERTDeviceSingleBRDFMaterialPtr;

}

#endif