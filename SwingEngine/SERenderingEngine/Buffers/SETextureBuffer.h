// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_TextureBuffer_H
#define Swing_TextureBuffer_H

#ifndef __APPLE__

#include "SERenderingEngineLIB.h"
#include "SEBuffer.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 07/21/2013
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SETextureBuffer : public SEBuffer
{
public:
	SETextureBuffer();
	~SETextureBuffer();
};

typedef SESmartPointer<SETextureBuffer> SETextureBufferPtr;

}

#endif

#endif