// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_StructuredBuffer_H
#define Swing_StructuredBuffer_H

#ifndef __APPLE__

#include "SERenderingEngineLIB.h"
#include "SEIndexableBuffer.h"

namespace Swing
{

typedef struct _SEDrawElementsIndirectCommand
{
    unsigned int Count;
    unsigned int PrimCount;
    unsigned int FirstIndex;
    unsigned int BaseVertex;
    unsigned int BaseInstance;
} SEDrawElementsIndirectCommand;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 07/22/2013
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEStructuredBuffer : public SEIndexableBuffer
{
public:
	SEStructuredBuffer();
	~SEStructuredBuffer();
};

typedef SESmartPointer<SEStructuredBuffer> SEStructuredBufferPtr;

}

#endif

#endif