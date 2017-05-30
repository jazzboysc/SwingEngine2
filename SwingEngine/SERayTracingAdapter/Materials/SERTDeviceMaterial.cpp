// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERayTracingAdapterPCH.h"
#include "SERTDeviceMaterial.h"

using namespace Swing;

//----------------------------------------------------------------------------
SERTDeviceMaterial::SERTDeviceMaterial(SERTDeviceMaterialType materialType)
    :
    mMaterialType(materialType),
    mMaterialHandle(nullptr)
{
}
//----------------------------------------------------------------------------
SERTDeviceMaterial::~SERTDeviceMaterial()
{
    if( mMaterialHandle )
    {
        mMaterialHandle->RTDevice->DeleteMaterial(this);
        SE_DELETE mMaterialHandle;
        mMaterialHandle = nullptr;
    }
}
//----------------------------------------------------------------------------
void SERTDeviceMaterial::CreateDeviceResource(SERayTracingDevice& device)
{
    if( !mMaterialHandle )
    {
        mMaterialHandle = device.CreateMaterial(this);
    }
}
//----------------------------------------------------------------------------
SERTDeviceMaterialType SERTDeviceMaterial::GetMaterialType() const
{
    return mMaterialType;
}
//----------------------------------------------------------------------------
SERTDeviceMaterialHandle* SERTDeviceMaterial::GetMaterialHandle()
{
    return mMaterialHandle;
}
//----------------------------------------------------------------------------