// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_RTDeviceSceneNode_H
#define Swing_RTDeviceSceneNode_H

#include "SERayTracingAdapterLIB.h"
#include "SEReferencable.h"
#include "SERayTracingDeviceResource.h"
#include "SEISpatialInfo.h"

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
    SERTDeviceSceneNodeHandle* GetSceneNodeHandle();

    void SetTransform(SEMatrix3f* srcRotation = nullptr, SEVector3f* srcLocation = nullptr);

protected:
    SERTDeviceSceneNodeHandle* mSceneNodeHandle;
};

typedef SESmartPointer<SERTDeviceSceneNode> SERTDeviceSceneNodePtr;

}

#endif