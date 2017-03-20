// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_GPUDeviceResident_H
#define Swing_GPUDeviceResident_H

#include "SERenderingEngineLIB.h"
#include "SEReferencable.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 07/23/2015
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEGPUDeviceResident : public SEReferencable
{
public:
    SEGPUDeviceResident();
    virtual ~SEGPUDeviceResident();

};

typedef SESmartPointer<SEGPUDeviceResident> SEGPUDeviceResidentPtr;

}

#endif