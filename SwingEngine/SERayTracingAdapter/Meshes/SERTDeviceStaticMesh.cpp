// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERayTracingAdapterPCH.h"
#include "SERTDeviceStaticMesh.h"

using namespace Swing;

//----------------------------------------------------------------------------
SERTDeviceStaticMesh::SERTDeviceStaticMesh()
    :
    SERTDeviceGeometry(RTDGT_StaticMesh),
    mStaticMeshHandle(nullptr)
{
}
//----------------------------------------------------------------------------
SERTDeviceStaticMesh::~SERTDeviceStaticMesh()
{
    if( mStaticMeshHandle )
    {
        mStaticMeshHandle->RTDevice->DeleteRTDeviceStaticMesh(this);
        SE_DELETE mStaticMeshHandle;
        mStaticMeshHandle = nullptr;
    }
}
//----------------------------------------------------------------------------
void SERTDeviceStaticMesh::CreateDeviceResource(SERayTracingDevice& device, SEIMetaMesh* srcMesh)
{
    if( !mStaticMeshHandle )
    {
        mStaticMeshHandle = device.CreateRTDeviceStaticMesh(this, srcMesh);
    }
}
//----------------------------------------------------------------------------
SERTDeviceStaticMeshHandle* SERTDeviceStaticMesh::GetStaticMeshHandle()
{
    return mStaticMeshHandle;
}
//----------------------------------------------------------------------------