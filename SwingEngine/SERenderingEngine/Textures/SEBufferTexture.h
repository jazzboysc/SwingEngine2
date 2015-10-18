// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_BufferTexture_H
#define Swing_BufferTexture_H

#include "SERenderingEngineLIB.h"
#include "SETexture.h"
#include "SETextureBuffer.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 02/27/2015
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEBufferTexture : public SETexture
{
public:
    SEBufferTexture();
    ~SEBufferTexture();

    bool LoadFromTextureBuffer(SEGPUDevice* device, SETextureBuffer* textureBuffer,
        SEBufferInternalFormat internalFormat);
};

typedef SESmartPointer<SEBufferTexture> SEBufferTexturePtr;

}

#endif