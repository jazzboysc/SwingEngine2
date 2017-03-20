// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017
//
// This part of Swing Engine belongs to an abstraction layer of the next 
// generation GPU graphics and compute API. Currently being constructed to 
// support Microsoft's DirectX 12 and Apple's Metal.

#ifndef Swing_CommandAllocator_H
#define Swing_CommandAllocator_H

#include "SERenderingEngineLIB.h"
#include "SEGPUDeviceResident.h"
#include "SEGPUDeviceBaseChild.h"

namespace Swing
{

enum SECommandAllocatorType
{
    CAT_Unknown = -1,
    CAT_Render,
    CAT_Compute,
    CommandAllocatorType_Max
};

class SECommandList;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/07/2015
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SECommandAllocator : public SEGPUDeviceResident
{
public:
    virtual ~SECommandAllocator();

    void CreateDeviceChild(SEGPUDeviceBase* device, SECommandList* commandList);

    SECommandAllocatorType GetType() const;
    SECommandAllocatorHandle* GetCommandAllocatorHandle() const;
    SECommandList* GetCommandList() const;

protected:
    SECommandAllocator(SECommandAllocatorType type);

    SECommandAllocatorType mType;
    SECommandAllocatorHandle* mCommandAllocatorHandle;
    SECommandList* mCommandList;
};

typedef SESmartPointer<SECommandAllocator> SECommandAllocatorPtr;

}

#endif