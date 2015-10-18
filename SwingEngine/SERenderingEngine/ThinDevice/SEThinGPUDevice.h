// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015
//
// This part of Swing Engine belongs to an abstraction layer of the next 
// generation GPU graphics and compute API. Currently being constructed to 
// support Apple's Metal.

#ifndef Swing_ThinGPUDevice_H
#define Swing_ThinGPUDevice_H

#include "SERenderingEngineLIB.h"
#include "SEReferencable.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 10/17/2015
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEThinGPUDevice : public SEReferencable
{
public:
    SEThinGPUDevice();
    ~SEThinGPUDevice();
};

typedef SESmartPointer<SEThinGPUDevice> SEThinGPUDevicePtr;

#include "SEThinGPUDevice.inl"

}

#endif