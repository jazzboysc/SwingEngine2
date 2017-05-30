// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_RTDeviceSceneNode_H
#define Swing_RTDeviceSceneNode_H

#include "SERayTracingAdapterLIB.h"
#include "SEReferencable.h"
#include "SERayTracingDeviceResource.h"
#include "SEISpatialInfo.h"
#include "SERTDeviceGeometry.h"
#include "SERTDeviceMaterial.h"

namespace Swing
{

class SERayTracingDevice;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 05/26/2017
//----------------------------------------------------------------------------
class SE_RAY_TRACING_ADAPTER_API SERTDeviceSceneNode : public SEReferencable
{
public:
    SERTDeviceSceneNode();
    virtual ~SERTDeviceSceneNode();

    void CreateDeviceResource(SERayTracingDevice& device, SEISpatialInfo* spatialInfo = nullptr);

    void SetTransform(SEMatrix3f* srcMatrix = nullptr, SEVector3f* srcOffset = nullptr);

    void SetGeometry(SERTDeviceGeometry* geometry);

    void SetMaterial(SERTDeviceMaterial* material);

    SERTDeviceSceneNodeHandle* GetSceneNodeHandle();

protected:
    SERTDeviceSceneNodeHandle* mSceneNodeHandle;
};

typedef SESmartPointer<SERTDeviceSceneNode> SERTDeviceSceneNodePtr;

}

#endif