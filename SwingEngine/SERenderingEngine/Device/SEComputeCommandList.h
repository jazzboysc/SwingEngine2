// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017
//
// This part of Swing Engine belongs to an abstraction layer of the next 
// generation GPU graphics and compute API. Currently being constructed to 
// support Microsoft's DirectX 12 and Apple's Metal.

#ifndef Swing_ComputeCommandList_H
#define Swing_ComputeCommandList_H

#include "SERenderingEngineLIB.h"
#include "SECommandList.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/09/2015
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEComputeCommandList : public SECommandList
{
public:
    SEComputeCommandList();
    virtual ~SEComputeCommandList();
};

typedef SESmartPointer<SEComputeCommandList> SEComputeCommandListPtr;

}

#endif