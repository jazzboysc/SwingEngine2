// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015
//
// This part of Swing Engine belongs to an abstraction layer of the next 
// generation GPU graphics and compute API. Currently being constructed to 
// support Microsoft's DirectX 12 and Apple's Metal.

#include "SERenderingEnginePCH.h"
#include "SERootSignature.h"

using namespace Swing;

//----------------------------------------------------------------------------
SERootSignature::SERootSignature(const SERootSignatureInfo& rootSignatureInfo)
    :
    mRootSignatureHandle(nullptr)
{
    mRootSignatureInfo = rootSignatureInfo;
}
//----------------------------------------------------------------------------
SERootSignature::~SERootSignature()
{
    if( mRootSignatureHandle )
    {
        mRootSignatureHandle->DeviceBase->DeleteRootSignature(this);
        SE_DELETE mRootSignatureHandle;
        mRootSignatureHandle = nullptr;
    }
}
//----------------------------------------------------------------------------
void SERootSignature::CreateDeviceChild(SEGPUDeviceBase* device)
{
    if( mRootSignatureHandle || !device )
    {
        SE_ASSERT(false);
        return;
    }

    mRootSignatureHandle = device->CreateRootSignature(this);
}
//----------------------------------------------------------------------------
SERootSignatureHandle* SERootSignature::GetRootSignatureHandle() const
{
    return mRootSignatureHandle;
}
//----------------------------------------------------------------------------
const SERootSignatureInfo& SERootSignature::GetRootSignatureInfo() const
{
    return mRootSignatureInfo;
}
//----------------------------------------------------------------------------