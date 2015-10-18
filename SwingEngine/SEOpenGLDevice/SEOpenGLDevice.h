// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_OpenGLDevice_H
#define Swing_OpenGLDevice_H

#include "SEOpenGLDeviceLIB.h"
#include "SEGPUDevice.h"
#include "SEOpenGLResource.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 12/08/2014
//----------------------------------------------------------------------------
class SE_OPENGL_DEVICE_API SEOpenGLDevice : public SEGPUDevice
{
public:
    SEOpenGLDevice();
    ~SEOpenGLDevice();

private:
    void __Initialize(SEGPUDeviceDescription* deviceDesc);
    void __Terminate();
    void __OnResize(unsigned int width, unsigned int height);

	// Misc.
	int __IsExtensionSupported(const char *extension);
	void __GetMaxAnisFilterLevel(int* maxAnisFilterLevel);
	void __SetAnisFilterLevel(int anisFilterLevel);

    // Shader.
    SEShaderHandle* __CreateShader(SEShader* shader);
    void __DeleteShader(SEShader* shader);

    // Program.
    SEShaderProgramHandle* __CreateProgram(SEShaderProgram* program);
    void __DeleteProgram(SEShaderProgram* program);
    void __EnableProgram(SEShaderProgram* program);
    void __DisableProgram(SEShaderProgram* program);
    void __SetProgramParameterInt(SEShaderProgram* program, 
        SEShaderProgramParameter pname, int value);

    // SEPass info stuff.
    SEPassInfoHandle* __CreatePassInfo(SEPassInfo* passInfo, SEShaderProgram* program, 
        SEPrimitive* primitive, bool hasNormal, bool hasTCoord, 
        int vertexComponentCount);
    void __DeletePassInfo(SEPassInfo* passInfo);
    void __EnablePassInfo(SEPassInfo* passInfo);
    void __DisablePassInfo(SEPassInfo* passInfo);

    // Uniform.
    void __GetUniformLocation(SEShaderProgram* program, SEShaderUniform* uniform, 
        const char* name);
    void __SetUniformValueMat4(SEShaderUniform* uniform, const SEMatrix4f* value);
	void __SetUniformValueVec3(SEShaderUniform* uniform, const SEVector3f* value);
	void __SetUniformValueVec4(SEShaderUniform* uniform, const SEVector4f* value);
    void __SetUniformValueInt(SEShaderUniform* uniform, int value);
    void __SetUniformValueUInt(SEShaderUniform* uniform, unsigned int value);
    void __SetUniformValueFloat(SEShaderUniform* uniform, float value);
    void __SetUniformValueFloat2(SEShaderUniform* uniform, const float* value);

    // SETexture.
    void __DeleteTexture(SETexture* texture);
    SETextureHandle* __Texture1DLoadFromSystemMemory(SETexture* texture,
        SEBufferInternalFormat internalFormat, int width, SEBufferFormat format, 
        SEBufferComponentType type, void* pixels);
    void __Texture1DUpdateFromPixelBuffer(SETexture* texture, SEPixelBuffer* pixelBuffer);
	void __TextureBindToImageUnit(SETexture* texture, unsigned int unit, bool layered, SEBufferAccess access);
    void __TextureBindToSampler(SETexture* texture, unsigned int index, SESamplerDesc* sampler);
    void __TextureGenerateMipmap(SETexture* texture);
    void __Texture1DGetDataFromGPUMemory(SETexture* texture, void* dstData);
    SETextureHandle* __Texture2DLoadFromSystemMemory(SETexture* texture, 
        SEBufferInternalFormat internalFormat, int width, int height, 
        SEBufferFormat format, SEBufferComponentType type, bool mipMap, 
        SESamplerDesc* sampler, void* pixels);
    SETextureHandle* __Texture2DLoadFromTextureBuffer(SETexture* texture, 
        SETextureBuffer* textureBuffer, SEBufferInternalFormat internalFormat);
    void __Texture2DUpdateFromPixelBuffer(SETexture* texture, SEPixelBuffer* pixelBuffer);
    void __Texture2DGetImageData(SETexture* texture, void* dstPixels);
    SETextureHandle* __Tex2DArrayLoadFromSystemMemory(SETexture* texture, 
        SEBufferInternalFormat internalFormat, int width, int height, int depth, 
		SEBufferFormat format, SEBufferComponentType type, bool mipMap, void* pixels);
    SETextureHandle* __Texture3DLoadFromSystemMemory(SETexture* texture, 
        SEBufferInternalFormat internalFormat, int width, int height, int depth, 
        SEBufferFormat format, SEBufferComponentType type, void* pixels);
    void __Texture3DUpdateFromPixelBuffer(SETexture* texture, SEPixelBuffer* pixelBuffer);
    SETextureHandle* __TextureCubeLoadFromSystemMemory(SETexture* texture,
        SEBufferInternalFormat internalFormat, int width, int height, 
        SEBufferFormat format, SEBufferComponentType type, bool mipMap, 
        void* pixelsPX, void* pixelsNX, void* pixelsPY, void* pixelsNY,
        void* pixelsPZ, void* pixelsNZ);
    SETextureHandle* __BufferTextureLoadFromTextureBuffer(SETexture* texture, 
        SETextureBuffer* textureBuffer, SEBufferInternalFormat internalFormat);

    // SEFrameBuffer.
    SEFBOHandle* __CreateFrameBuffer(SEFrameBuffer* frameBuffer);
    void __DeleteFrameBuffer(SEFrameBuffer* frameBuffer);
    void __FrameBufferSetRenderTargets(SEFrameBuffer* frameBuffer, 
        unsigned int colorTextureCount, SETexture** colorTextures, 
        SETexture* depthTexture, SETexture* stencilTexture);
    void __FrameBufferSetColorTarget(SEFrameBuffer* frameBuffer, 
        unsigned int attachPoint, SETexture* colorTexture);
    void __FrameBufferSetDepthTarget(SEFrameBuffer* frameBuffer, 
        SETexture* depthTexture);
    void __FrameBufferSetStencilTarget(SEFrameBuffer* frameBuffer, 
        SETexture* stencilTexture);
    void __FrameBufferEnable(SEFrameBuffer* frameBuffer);
    void __FrameBufferDisable(SEFrameBuffer* frameBuffer);

    // Compute.
    void __ComputeShaderDispatch(SEShaderProgram* program, unsigned int globalX, 
        unsigned int globalY, unsigned int globalZ);
	void __ComputeShaderDispatchIndirect(void* indirect);
	void __DispatchVertex(unsigned int threadCount);
    void __DispatchVertexIndirect(void* indirect);

    // SEBuffer.
    void __DeleteBuffer(SEBuffer* buffer);
    void* __BufferMap(SEBuffer* buffer, SEBufferAccess access);
    void __BufferUnmap(SEBuffer* buffer);
    void __BufferBindIndex(SEBuffer* buffer, unsigned int index);
    void __BufferBindIndexTo(SEBuffer* buffer, unsigned int index, SEBufferView* view);
    void __BufferBind(SEBuffer* buffer);
    void __BufferBindTo(SEBuffer* buffer, SEBufferView* view);
    SEBufferViewHandle* __CreateBufferView(SEBufferView* view);
    void __DeleteBufferView(SEBufferView* view);
    void __BufferUpdateSubData(SEBuffer* buffer, int offset, size_t size, void* data);
    SEBufferHandle* __BufferLoadFromSystemMemory(SEBuffer* buffer, size_t size, 
        void* data, SEBufferUsage usage);
    SEBufferHandle* __BufferLoadImmutableFromSystemMemory(SEBuffer* buffer, 
        size_t size, void* data);
    void __BufferClear(SEBuffer* buffer, SEBufferInternalFormat internalFormat, 
        SEBufferFormat format, SEBufferComponentType type, void* data);

	// Device info query.
	int __GetDeviceMemoryAvailable();

    // Memory barrier.
    void __MemoryBarrier(unsigned int flags);

    // Pipeline state block.
    void __ApplyPipelineStateBlock(SEPipelineStateBlock* psb);

    void __SetPatchVertices(int value);
    void __GetViewport(SEViewportState* dstViewport);
    void __SetViewport(SEViewportState* srcViewport);

    // Drawing.
    void __DrawPrimitive(SEPrimitiveType type, int count);
    void __DrawPrimitiveInstanced(SEPrimitiveType type, int count, int instanceCount);
    void __DrawIndexedPrimitive(SEPrimitiveType type, int count);
    void __DrawIndexedPrimitiveIndirect(SEPrimitiveType type, unsigned int commandOffset);

    void __SetPointSize(float value);

    // Timer stuff.
    SEGPUTimerHandle* __CreateTimer(SEGPUTimer* timer);
    void __DeleteTimer(SEGPUTimer* timer);
    void __TimerStart(SEGPUTimer* timer);
    void __TimerStop(SEGPUTimer* timer);
    double __TimerGetTimeElapsed(SEGPUTimer* timer);

private:
    bool mEnable4xMsaa;
    unsigned int m4xMsaaQuality;
    GLint mAnisFilterLevel;
};

}

#endif