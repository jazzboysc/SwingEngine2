// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017
//
// This part of Swing Engine belongs to an abstraction layer of the next 
// generation GPU graphics and compute API. Currently being constructed to 
// support Microsoft's DirectX 12 and Apple's Metal.

#ifndef Swing_ComputeCommandQueue_H
#define Swing_ComputeCommandQueue_H

#include "SERenderingEngineLIB.h"
#include "SECommandQueue.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/11/2015
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEComputeCommandQueue : public SECommandQueue
{
public:
    SEComputeCommandQueue();
    virtual ~SEComputeCommandQueue();
};

typedef SESmartPointer<SEComputeCommandQueue> SEComputeCommandQueuePtr;

}

#endif