// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_AtomicCounterBuffer_H
#define Swing_AtomicCounterBuffer_H

#ifndef __APPLE__

#include "SERenderingEngineLIB.h"
#include "SEIndexableBuffer.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 07/21/2013
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEAtomicCounterBuffer : public SEIndexableBuffer
{
public:
	SEAtomicCounterBuffer();
	~SEAtomicCounterBuffer();
};

typedef SESmartPointer<SEAtomicCounterBuffer> SEAtomicCounterBufferPtr;

}

#endif

#endif