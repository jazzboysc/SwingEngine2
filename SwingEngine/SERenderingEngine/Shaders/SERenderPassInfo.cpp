// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SERenderPassInfo.h"
#include <cstdio>

using namespace Swing;

//----------------------------------------------------------------------------
SERenderPassInfo::SERenderPassInfo()
	:
    mPassInfoHandle(0)
{
}
//----------------------------------------------------------------------------
SERenderPassInfo::~SERenderPassInfo()
{
    if( mPassInfoHandle )
    {
        mPassInfoHandle->DeviceBase->DeleteRenderPassInfo(this);
        SE_DELETE mPassInfoHandle;
        mPassInfoHandle = 0;
    }
}
//----------------------------------------------------------------------------
void SERenderPassInfo::CreateDeviceChild(SEGPUDeviceBase* device, 
    SEShaderProgram* program, SEGeometryAttributes* geometryAttr, 
    SEPipelineStateBlock* psb, SERootSignature* rootSignature, 
    SERenderPassTargetsInfo* targetsInfo)
{
    if( mPassInfoHandle )
    {
        return;
    }

    mPassInfoHandle = device->CreateRenderPassInfo(this, program, 
        geometryAttr, psb, rootSignature, targetsInfo);
}
//----------------------------------------------------------------------------
void SERenderPassInfo::Enable()
{
    mPassInfoHandle->DeviceBase->EnableRenderPassInfo(this);
}
//----------------------------------------------------------------------------
void SERenderPassInfo::Disable()
{
    mPassInfoHandle->DeviceBase->DisableRenderPassInfo(this);
}
//----------------------------------------------------------------------------
SERenderPassInfoHandle* SERenderPassInfo::GetPassInfoHandle() const
{
    return mPassInfoHandle;
}
//----------------------------------------------------------------------------