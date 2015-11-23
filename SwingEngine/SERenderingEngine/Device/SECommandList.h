// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015
//
// This part of Swing Engine belongs to an abstraction layer of the next 
// generation GPU graphics and compute API. Currently being constructed to 
// support Microsoft's DirectX 12 and Apple's Metal.

#ifndef Swing_CommandList_H
#define Swing_CommandList_H

#include "SERenderingEngineLIB.h"
#include "SEGPUDeviceResident.h"
#include "SEGPUDeviceBaseChild.h"

namespace Swing
{

enum SECommandListType
{
    CLT_Unknown = -1,
    CLT_Render,
    CLT_Compute,
    CommandListType_Max
};

class SECommandAllocator;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/07/2015
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SECommandList : public SEGPUDeviceResident
{
public:
    virtual ~SECommandList();

    void CreateDeviceChild(SEGPUDeviceBase* device, SECommandAllocator* commandAllocator);

    SECommandListType GetType() const;
    SECommandListHandle* GetCommandListHandle() const;

protected:
    SECommandList(SECommandListType type);

    SECommandListType mType;
    SECommandListHandle* mCommandListHandle;
    SECommandAllocator* mCommandAllocator;
};

typedef SESmartPointer<SECommandList> SECommandListPtr;

}

#endif