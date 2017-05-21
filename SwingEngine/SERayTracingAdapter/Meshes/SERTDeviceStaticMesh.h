// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_RTDeviceStaticMesh_H
#define Swing_RTDeviceStaticMesh_H

#include "SERayTracingAdapterLIB.h"
#include "SEReferencable.h"
#include "SERayTracingDeviceResource.h"
#include "SEIMetaMesh.h"

namespace Swing
{

class SERayTracingDevice;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 05/21/2017
//----------------------------------------------------------------------------
class SE_RAY_TRACING_ADAPTER_API SERTDeviceStaticMesh : public SEReferencable
{
public:
    SERTDeviceStaticMesh();
    virtual ~SERTDeviceStaticMesh();

    void CreateDeviceResource(SERayTracingDevice& device, SEIMetaMesh* srcMesh = nullptr);
    SERTDeviceStaticMeshHandle* GetStaticMeshHandle();

protected:
    SERTDeviceStaticMeshHandle* mStaticMeshHandle;
};

typedef SESmartPointer<SERTDeviceStaticMesh> SERTDeviceStaticMeshPtr;

}

#endif