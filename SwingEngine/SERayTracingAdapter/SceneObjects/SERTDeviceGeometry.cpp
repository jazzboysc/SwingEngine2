// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERayTracingAdapterPCH.h"
#include "SERTDeviceGeometry.h"

using namespace Swing;

//----------------------------------------------------------------------------
SERTDeviceGeometry::SERTDeviceGeometry(SERTDeviceGeometryType geometryType)
    :
    mGeometryType(geometryType)
{
}
//----------------------------------------------------------------------------
SERTDeviceGeometry::~SERTDeviceGeometry()
{
}
//----------------------------------------------------------------------------
SERTDeviceGeometryType SERTDeviceGeometry::GetGeometryType() const
{
    return mGeometryType;
}
//----------------------------------------------------------------------------