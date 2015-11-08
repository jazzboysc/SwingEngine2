// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_GPUDeviceBase_H
#define Swing_GPUDeviceBase_H

#include "SERenderingEngineLIB.h"
#include "SEReferencable.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/08/2015
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEGPUDeviceBase : public SEReferencable
{
public:
    SEGPUDeviceBase();
    ~SEGPUDeviceBase();
};

typedef SESmartPointer<SEGPUDeviceBase> SEGPUDeviceBasePtr;

#include "SEGPUDeviceBase.inl"

}

#endif