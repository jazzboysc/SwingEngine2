// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_RTDeviceBakeView_H
#define Swing_RTDeviceBakeView_H

#include "SERayTracingAdapterLIB.h"
#include "SEReferencable.h"
#include "SERayTracingDeviceResource.h"
#include "SERTDeviceSceneNode.h"

namespace Swing
{

class SERayTracingDevice;

struct SE_RAY_TRACING_ADAPTER_API SERTDeviceBakeViewDescription
{
    SERTDeviceSceneNode* BakeNode;
    unsigned int UVChannel;
    float Dilation;
    float UMin;
    float VMin;
    float UMax;
    float VMax;
    bool FromCamera;

    SERTDeviceBakeViewDescription()
        :
        BakeNode(nullptr),
        UVChannel(0),
        Dilation(2.0f),
        UMin(0.0f),
        VMin(0.0f),
        UMax(1.0f),
        VMax(1.0f),
        FromCamera(false)
    {}
};

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 06/03/2017
//----------------------------------------------------------------------------
class SE_RAY_TRACING_ADAPTER_API SERTDeviceBakeView : public SEReferencable
{
public:
    SERTDeviceBakeView();
    virtual ~SERTDeviceBakeView();

    void CreateDeviceResource(SERayTracingDevice& device, SERTDeviceBakeViewDescription* bakeViewDesc);
    SERTDeviceBakeViewHandle* GetBakeViewHandle();

protected:
    SERTDeviceBakeViewHandle* mBakeViewHandle;
};

typedef SESmartPointer<SERTDeviceBakeView> SERTDeviceBakeViewPtr;

}

#endif