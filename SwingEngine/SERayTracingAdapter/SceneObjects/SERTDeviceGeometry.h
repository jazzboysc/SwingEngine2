// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_RTDeviceGeometry_H
#define Swing_RTDeviceGeometry_H

#include "SERayTracingAdapterLIB.h"
#include "SEReferencable.h"

namespace Swing
{

enum SERTDeviceGeometryType
{
    RTDGT_StaticMesh,
    RTDGT_Unknown
};

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 05/28/2017
//----------------------------------------------------------------------------
class SE_RAY_TRACING_ADAPTER_API SERTDeviceGeometry : public SEReferencable
{
public:
    virtual ~SERTDeviceGeometry();

    SERTDeviceGeometryType GetGeometryType() const;

protected:
    // Abstract base class.
    SERTDeviceGeometry(SERTDeviceGeometryType geometryType = RTDGT_Unknown);

    SERTDeviceGeometryType mGeometryType;
};

typedef SESmartPointer<SERTDeviceGeometry> SERTDeviceGeometryPtr;

}

#endif