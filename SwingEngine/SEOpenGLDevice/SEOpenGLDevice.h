// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_OpenGLDevice_H
#define Swing_OpenGLDevice_H

#include "SEOpenGLDeviceLIB.h"
#include "SEGPUDevice.h"
#include "SEOpenGLResource.h"
#include "SEOpenGLGPUDeviceChild.h"
#include "SEGPUDeviceInspector.h"
#include "SETerminal.h"
#include "SEShaderProgram.h"
#include "SEBufferView.h"
#include "SEBufferBase.h"
#include "SETexture.h"
#include "SETexture1D.h"
#include "SETexture2D.h"
#include "SETexture2DArray.h"
#include "SETexture3D.h"
#include "SEPixelBuffer.h"
#include "SEFrameBuffer.h"
#include "SEPrimitive.h"
#include "SERenderPassInfo.h"
#include "SEPipelineStateBlock.h"
#include "SEGPUTimer.h"
#include "SEVector3.h"
#include "SEMatrix4.h"

#include <string>
#include <GL/glew.h>

#ifdef _DEBUG
//----------------------------------------------------------------------------
#define SE_OPENGL_DEVICE_CHECK_ERROR  \
    {  \
        GLenum res = glGetError();  \
        SE_ASSERT(res == GL_NO_ERROR);  \
    }
//----------------------------------------------------------------------------
#else
#define SE_OPENGL_DEVICE_CHECK_ERROR
#endif

#ifdef SE_GPU_MEMORY_INSPECTION
//----------------------------------------------------------------------------
#define SE_OPENGL_DEVICE_BEGIN_MEM_INSPECTION(inspector, resident)  \
    int mem1 = 0;  \
    if( inspector )  \
    {  \
        mem1 = GetDeviceMemoryAvailable();  \
    }
//----------------------------------------------------------------------------
#define SE_OPENGL_DEVICE_END_MEM_INSPECTION(inspector, resident)  \
    int mem2 = 0;  \
    if( inspector )  \
    {  \
        mem2 = GetDeviceMemoryAvailable();  \
        SEDeviceMemOp op;  \
        op.Resident = resident;  \
        op.DeviceMemPrint = mem1 - mem2;  \
        inspector->OnDeviceMemoryOperation(&op);  \
    }
//----------------------------------------------------------------------------
#else
#define SE_OPENGL_DEVICE_BEGIN_MEM_INSPECTION(inspector, resident) (void)resident
#define SE_OPENGL_DEVICE_END_MEM_INSPECTION(inspector, resident) (void)resident
#endif
//----------------------------------------------------------------------------

#define GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX          0x9047
#define GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX    0x9048
#define GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX  0x9049
#define GPU_MEMORY_INFO_EVICTION_COUNT_NVX            0x904A
#define GPU_MEMORY_INFO_EVICTED_MEMORY_NVX            0x904B

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
    void InsertGPUDeviceBaseFunctions();
    void InsertGPUDeviceFunctions();

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

    // Render pass info stuff.
    SERenderPassInfoHandle* __CreateRenderPassInfo(SERenderPassInfo* renderPassInfo, 
        SEShaderProgram* program, SEGeometryAttributes* geometryAttr, 
        SEPipelineStateBlock* psb, SERootSignature* rootSignature, 
        SERenderPassTargetsInfo* targetsInfo);
    void __DeleteRenderPassInfo(SERenderPassInfo* renderPassInfo);
    void __EnableRenderPassInfo(SERenderPassInfo* renderPassInfo);
    void __DisableRenderPassInfo(SERenderPassInfo* renderPassInfo);

    // Command queue stuff.
    SECommandQueueHandle* __CreateCommandQueue(SECommandQueue* commandQueue);
    void __DeleteCommandQueue(SECommandQueue* commandQueue);

    // Command allocator stuff.
    SECommandAllocatorHandle* __CreateCommandAllocator(
        SECommandAllocator* commandAllocator, SECommandList* commandList);
    void __DeleteCommandAllocator(
        SECommandAllocator* commandAllocator, SECommandList* commandList);

    // Command list stuff.
    SECommandListHandle* __CreateCommandList(SECommandList* commandList,
        SECommandAllocator* commandAllocator);
    void __DeleteCommandList(SECommandList* commandList,
        SECommandAllocator* commandAllocator);
    void __ResetRenderCommandList(SERenderCommandList* renderCommandList,
        SERenderPassInfo* renderPassInfo);
    void __CloseRenderCommandList(SERenderCommandList* renderCommandList);
    void __RenderCommandListSetRootSignature(
        SERenderCommandList* renderCommandList, SERootSignature* rootSignature);
    void __RenderCommandListSetViewport(SERenderCommandList* renderCommandList,
        SEViewportState* srcViewport);

    // Root signature stuff.
    SERootSignatureHandle* __CreateRootSignature(SERootSignature* rootSignature);
    void __DeleteRootSignature(SERootSignature* rootSignature);

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

    // Texture.
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

    // FrameBuffer.
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

    // Buffer.
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
    void __GPUMemoryBarrier(unsigned int flags);

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

private:
    static const GLenum gsShaderType[ShaderType_Max];
    static const GLenum gsShaderProgramParams[SPP_Max];
    static const GLenum gsBufferTargets[BufferType_Max];
    static const GLenum gsBufferBindings[BufferType_Max];
    static const GLenum gsBufferFormat[BufferFormat_Max];
    static const GLint  gsBufferInternalFormat[BufferInternalFormat_Max];
    static const GLenum gsBufferComponentType[BufferComponentType_Max];
    static const GLenum gsBufferAccess[BufferAccess_Max];
    static const GLenum gsBufferUsage[BufferUsage_Max];
    static const GLenum gsTextureTargets[TextureType_Max];
    static const GLenum gsFilterType[FilterType_Max];
    static const GLenum gsWrapType[WrapType_Max];
    static const GLenum gsPrimitiveType[PrimitiveType_Max];
};

}

#endif