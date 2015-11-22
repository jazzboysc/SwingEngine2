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
        mPassInfoHandle->Device->DeletePassInfo(this);
        SE_DELETE mPassInfoHandle;
        mPassInfoHandle = 0;
    }
}
//----------------------------------------------------------------------------
void SEPassInfo::Create(SEGPUDevice* device, SEShaderProgram* program, 
    SEGeometryAttributes* geometryAttr, SEPipelineStateBlock*)
{
    if( mPassInfoHandle )
    {
        return;
    }

    mPassInfoHandle = device->CreatePassInfo(this, program, geometryAttr->Prim,
        geometryAttr->HasNormal, geometryAttr->HasTCoord, 
        geometryAttr->VertexComponentCount);
}
//----------------------------------------------------------------------------
void SEPassInfo::Enable()
{
    mPassInfoHandle->Device->EnablePassInfo(this);
}
//----------------------------------------------------------------------------
void SEPassInfo::Disable()
{
    mPassInfoHandle->Device->DisablePassInfo(this);
}
//----------------------------------------------------------------------------
SEPassInfoHandle* SEPassInfo::GetPassInfoHandle() const
{
    return mPassInfoHandle;
}
//----------------------------------------------------------------------------