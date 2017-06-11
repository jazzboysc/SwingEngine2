// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_RTDeviceRenderElement_H
#define Swing_RTDeviceRenderElement_H

#include "SERayTracingAdapterLIB.h"
#include "SEReferencable.h"
#include "SERayTracingDeviceResource.h"

namespace Swing
{

class SERayTracingDevice;

enum SERTDeviceRenderElementType
{
    RTDRET_RawTotalLighting,
    RTDRET_Max
};

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 05/30/2017
//----------------------------------------------------------------------------
class SE_RAY_TRACING_ADAPTER_API SERTDeviceRenderElement : public SEReferencable
{
public:
    SERTDeviceRenderElement(SERTDeviceRenderElementType type);
    virtual ~SERTDeviceRenderElement();

    void CreateDeviceResource(SERayTracingDevice& device);

    SERTDeviceRenderElementType GetRenderElementType() const;
    SERTDeviceRenderElementHandle* GetRenderElementHandle();

protected:
    SERTDeviceRenderElementType mRenderElementType;
    SERTDeviceRenderElementHandle* mRenderElementHandle;
};

typedef SESmartPointer<SERTDeviceRenderElement> SERTDeviceRenderElementPtr;

}

#endif