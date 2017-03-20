// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_DispatchIndirectBuffer_H
#define Swing_DispatchIndirectBuffer_H

#ifndef __APPLE__

#include "SERenderingEngineLIB.h"
#include "SEBuffer.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 09/23/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEDispatchIndirectBuffer : public SEBuffer
{
public:
    SEDispatchIndirectBuffer();
    ~SEDispatchIndirectBuffer();
};

typedef SESmartPointer<SEDispatchIndirectBuffer> SEDispatchIndirectBufferPtr;

}

#endif

#endif