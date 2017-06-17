// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_RTDeviceRenderElement_H
#define Swing_RTDeviceRenderElement_H

#include "SERayTracingAdapterLIB.h"
#include "SEReferencable.h"
#include "SERayTracingDeviceResource.h"
#include "SERayTracingDeviceImage.h"

namespace Swing
{

class SERayTracingDevice;

enum SERTDeviceRenderElementType;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 05/30/2017
//----------------------------------------------------------------------------
class SE_RAY_TRACING_ADAPTER_API SERTDeviceRenderElement : public SEReferencable
{
public:
    SERTDeviceRenderElement(SERTDeviceRenderElementType type);
    virtual ~SERTDeviceRenderElement();

    SERTDeviceRenderElementType GetRenderElementType() const;
    SERTDeviceRenderElementHandle* GetRenderElementHandle();

    void SaveRenderElementToFile(const std::string& fileName, SERTDeviceImageFileType fileType);

protected:
    SERTDeviceRenderElementType mRenderElementType;
    SERTDeviceRenderElementHandle* mRenderElementHandle;
};

typedef SESmartPointer<SERTDeviceRenderElement> SERTDeviceRenderElementPtr;

}

#endif