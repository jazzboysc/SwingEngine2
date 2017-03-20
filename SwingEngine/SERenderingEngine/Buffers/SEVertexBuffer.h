// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_VertexBuffer_H
#define Swing_VertexBuffer_H

#include "SERenderingEngineLIB.h"
#include "SEBuffer.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 03/09/2015
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEVertexBuffer : public SEBuffer
{
public:
    SEVertexBuffer();
    ~SEVertexBuffer();
};

typedef SESmartPointer<SEVertexBuffer> SEVertexBufferPtr;

}

#endif