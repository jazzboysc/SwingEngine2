// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_Texture_H
#define Swing_Texture_H

#include "SERenderingEngineLIB.h"
#include "SEBufferBase.h"

namespace Swing
{

enum SETextureType
{
    TT_Unknown = -1,
    TT_Texture1D,
    TT_Texture2D,
    TT_Texture3D,
    TT_TextureCube,
    TT_Texture2DArray,
    TT_BufferTexture,
    TextureType_Max
};

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/07/2013
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SETexture : public SEBufferBase
{
public:
	virtual ~SETexture();

    bool IsRenderTarget;
    bool IsHDRTexture;
    bool IsTextureBuffer;
    bool HasMipMap;

    inline SETextureHandle* GetTextureHandle() const { return mTextureHandle; }
    inline SEBufferFormat GetFormat() const { return mFormat; }
    inline SEBufferInternalFormat GetInternalFormat() const { return mInternalFormat; }
    inline SEBufferComponentType GetComponentType() const { return mComponentType; }
    inline SETextureType GetType() const { return mType; }

	void BindToImageUnit(unsigned int unit, SEBufferAccess access, bool layered = false);
    void BindToSampler(unsigned int index, SESamplerDesc* sampler);

    void GenerateMipmap();

protected:
    SETexture();

    SETextureHandle* mTextureHandle;
    SEBufferFormat mFormat;
    SEBufferInternalFormat mInternalFormat;
    SEBufferComponentType mComponentType;
    SETextureType mType;
};

typedef SESmartPointer<SETexture> SETexturePtr;

}

#endif