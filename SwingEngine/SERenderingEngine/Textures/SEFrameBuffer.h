// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_FrameBuffer_H
#define Swing_FrameBuffer_H

#include "SERenderingEngineLIB.h"
#include "SEGPUDeviceResident.h"
#include "SEGPUResource.h"
#include "SETexture1D.h"
#include "SETexture2D.h"
#include "SETexture3D.h"
#include "SETextureCube.h"
#include "SETexture2DArray.h"

#include <vector>

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 04/22/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEFrameBuffer : public SEGPUDeviceResident
{
public:
    SEFrameBuffer(SEGPUDevice* device);
	~SEFrameBuffer();
	
    SEFBOHandle* GetFBOHandle();

    void SetRenderTargets(unsigned int colorTextureCount,
		SETexture** colorTextures, SETexture* depthTexture = 0, 
		SETexture* stencilTexture = 0);

    void ReserveColorTargets(unsigned int colorTextureCount, int width, 
        int height, int depth);
    void SetColorTarget(unsigned int attachPoint, SETexture* colorTexture);
    void SetDepthTarget(SETexture* depthTexture);
    void SetStencilTarget(SETexture* stencilTexture);

	void Enable();
	void Disable();

    enum { FBO_MAX_COLOR_TARGETS = 16 };

private:
    SEFBOHandle* mFBO;

// Internal.
public:
    int mWidth, mHeight, mDepth;
	unsigned int mColorTextureCount;
	std::vector<SETexture*> mColorTextures;
	unsigned int* mColorBuffers;
	SETexture* mDepthTexture;
	SETexture* mStencilTexture;
};

typedef SESmartPointer<SEFrameBuffer> SEFrameBufferPtr;

}

#endif