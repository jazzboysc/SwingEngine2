// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SEPassInfo.h"
#include <cstdio>

using namespace Swing;

//----------------------------------------------------------------------------
SEPassInfo::SEPassInfo()
	:
    mPassInfoHandle(0)
{
}
//----------------------------------------------------------------------------
SEPassInfo::~SEPassInfo()
{
    if( mPassInfoHandle )
    {
        mPassInfoHandle->DeviceBase->DeletePassInfo(this);
        SE_DELETE mPassInfoHandle;
        mPassInfoHandle = 0;
    }
}
//----------------------------------------------------------------------------
void SEPassInfo::Create(SEGPUDeviceBase* device, SEShaderProgram* program, 
    SEGeometryAttributes* geometryAttr, SEPipelineStateBlock* psb, 
    SERootSignature* rootSignature)
{
    if( mPassInfoHandle )
    {
        return;
    }

    mPassInfoHandle = device->CreatePassInfo(this, program, geometryAttr, psb,
        rootSignature);
}
//----------------------------------------------------------------------------
void SEPassInfo::Enable()
{
    mPassInfoHandle->DeviceBase->EnablePassInfo(this);
}
//----------------------------------------------------------------------------
void SEPassInfo::Disable()
{
    mPassInfoHandle->DeviceBase->DisablePassInfo(this);
}
//----------------------------------------------------------------------------
SEPassInfoHandle* SEPassInfo::GetPassInfoHandle() const
{
    return mPassInfoHandle;
}
//----------------------------------------------------------------------------