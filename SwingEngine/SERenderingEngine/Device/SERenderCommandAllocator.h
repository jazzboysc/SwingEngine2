// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017
//
// This part of Swing Engine belongs to an abstraction layer of the next 
// generation GPU graphics and compute API. Currently being constructed to 
// support Microsoft's DirectX 12 and Apple's Metal.

#ifndef Swing_RenderCommandAllocator_H
#define Swing_RenderCommandAllocator_H

#include "SERenderingEngineLIB.h"
#include "SECommandAllocator.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/11/2015
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SERenderCommandAllocator : public SECommandAllocator
{
public:
    SERenderCommandAllocator();
    virtual ~SERenderCommandAllocator();
};

typedef SESmartPointer<SERenderCommandAllocator> SERenderCommandAllocatorPtr;

}

#endif