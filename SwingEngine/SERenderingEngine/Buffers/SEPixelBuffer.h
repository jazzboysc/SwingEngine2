// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_PixelBuffer_H
#define Swing_PixelBuffer_H

#include "SERenderingEngineLIB.h"
#include "SEBuffer.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 07/21/2013
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEPixelBuffer : public SEBuffer
{
public:
	SEPixelBuffer();
	~SEPixelBuffer();
};

typedef SESmartPointer<SEPixelBuffer> SEPixelBufferPtr;

}

#endif