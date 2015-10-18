// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_UniformBuffer_H
#define Swing_UniformBuffer_H

#include "SERenderingEngineLIB.h"
#include "SEIndexableBuffer.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 09/23/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEUniformBuffer : public SEIndexableBuffer
{
public:
    SEUniformBuffer();
    ~SEUniformBuffer();
};

typedef SESmartPointer<SEUniformBuffer> SEUniformBufferPtr;

}

#endif