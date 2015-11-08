// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015
//
// This part of Swing Engine belongs to an abstraction layer of the next 
// generation GPU graphics and compute API. Currently being constructed to 
// support Microsoft's DirectX 12 and Apple's Metal.

#ifndef Swing_DescriptorHeap_H
#define Swing_DescriptorHeap_H

#include "SERenderingEngineLIB.h"
#include "SEReferencable.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/07/2015
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEDescriptorHeap : public SEReferencable
{
public:
    SEDescriptorHeap();
    virtual ~SEDescriptorHeap();
};

typedef SESmartPointer<SEDescriptorHeap> SEDescriptorHeapPtr;

}

#endif