// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_GPUDevice_H
#define Swing_GPUDevice_H

#include "SERenderingEngineLIB.h"
#include "SEGPUDeviceBase.h"

namespace Swing
{

//----------------------------------------------------------------------------
#define SE_INSERT_GPU_DEVICE_FUNC(function, device) \
    _##function = (GPUDevice##function)&##device::__##function
//----------------------------------------------------------------------------

class SEGPUDevice;

typedef SEShaderHandle* (SEGPUDevice::*GPUDeviceCreateShader)(SEShader* shader);
typedef void (SEGPUDevice::*GPUDeviceDeleteShader)(SEShader* shader);
typedef SEShaderProgramHandle* (SEGPUDevice::*GPUDeviceCreateProgram)(
    SEShaderProgram* program);
typedef void (SEGPUDevice::*GPUDeviceDeleteProgram)(SEShaderProgram* program);
typedef void (SEGPUDevice::*GPUDeviceEnableProgram)(SEShaderProgram* program);
typedef void (SEGPUDevice::*GPUDeviceDisableProgram)(SEShaderProgram* program);
typedef SEPassInfoHandle* (SEGPUDevice::*GPUDeviceCreatePassInfo)(
    SEPassInfo* passInfo, SEShaderProgram* program, SEPrimitive* primitive, 
    bool hasNormal, bool hasTCoord, int vertexComponentCount);
typedef void (SEGPUDevice::*GPUDeviceDeletePassInfo)(SEPassInfo* passInfo);
typedef void (SEGPUDevice::*GPUDeviceEnablePassInfo)(SEPassInfo* passInfo);
typedef void (SEGPUDevice::*GPUDeviceDisablePassInfo)(SEPassInfo* passInfo);
typedef void (SEGPUDevice::*GPUDeviceGetUniformLocation)(SEShaderProgram* program, 
    SEShaderUniform* uniform, const char* name);
typedef void (SEGPUDevice::*GPUDeviceSetUniformValueMat4)(SEShaderUniform* uniform, 
    const SEMatrix4f* value);
typedef void (SEGPUDevice::*GPUDeviceSetUniformValueVec3)(SEShaderUniform* uniform, 
    const SEVector3f* value);
typedef void (SEGPUDevice::*GPUDeviceSetUniformValueVec4)(SEShaderUniform* uniform,
	const SEVector4f* value);
typedef void (SEGPUDevice::*GPUDeviceSetUniformValueInt)(SEShaderUniform* uniform, 
    int value);
typedef void (SEGPUDevice::*GPUDeviceSetUniformValueUInt)(SEShaderUniform* uniform,
    unsigned int value);
typedef void (SEGPUDevice::*GPUDeviceSetUniformValueFloat)(
    SEShaderUniform* uniform, float value);
typedef void (SEGPUDevice::*GPUDeviceSetUniformValueFloat2)(
    SEShaderUniform* uniform, const float* value);
typedef void (SEGPUDevice::*GPUDeviceSetProgramParameterInt)(
    SEShaderProgram* program, SEShaderProgramParameter pname, int value);
typedef void (SEGPUDevice::*GPUDeviceDeleteTexture)(SETexture* texture);
typedef SETextureHandle* (SEGPUDevice::*GPUDeviceTexture1DLoadFromSystemMemory)(
    SETexture* texture, SEBufferInternalFormat internalFormat, int width, 
    SEBufferFormat format, SEBufferComponentType type, void* pixels);
typedef void (SEGPUDevice::*GPUDeviceTexture1DUpdateFromPixelBuffer)(
    SETexture* texture, SEPixelBuffer* pixelBuffer);
typedef void (SEGPUDevice::*GPUDeviceTextureBindToImageUnit)(SETexture* texture, 
	unsigned int unit, bool layered, SEBufferAccess access);
typedef void (SEGPUDevice::*GPUDeviceTextureBindToSampler)(SETexture* texture, 
    unsigned int index, SESamplerDesc* sampler);
typedef void (SEGPUDevice::*GPUDeviceTextureGenerateMipmap)(SETexture* texture);
typedef void (SEGPUDevice::*GPUDeviceTexture1DGetDataFromGPUMemory)(
    SETexture* texture, void* dstData);
typedef SETextureHandle* (SEGPUDevice::*GPUDeviceTexture2DLoadFromSystemMemory)(
    SETexture* texture, SEBufferInternalFormat internalFormat, int width, 
    int height, SEBufferFormat format, SEBufferComponentType type, bool mipMap, 
    SESamplerDesc* sampler, void* pixels);
typedef SETextureHandle* (SEGPUDevice::*GPUDeviceTexture2DLoadFromTextureBuffer)(
    SETexture* texture, SETextureBuffer* textureBuffer, 
    SEBufferInternalFormat internalFormat);
typedef void (SEGPUDevice::*GPUDeviceTexture2DUpdateFromPixelBuffer)(
    SETexture* texture, SEPixelBuffer* pixelBuffer);
typedef void (SEGPUDevice::*GPUDeviceTexture2DGetImageData)(SETexture* texture, 
    void* dstPixels);
typedef SETextureHandle* (SEGPUDevice::*GPUDeviceTex2DArrayLoadFromSystemMemory)(
    SETexture* texture, SEBufferInternalFormat internalFormat, int width,
    int height, int depth, SEBufferFormat format, SEBufferComponentType type,
	bool mipMap, void* pixels);
typedef SETextureHandle* (SEGPUDevice::*GPUDeviceTexture3DLoadFromSystemMemory)(
    SETexture* texture, SEBufferInternalFormat internalFormat, int width, 
    int height, int depth, SEBufferFormat format, SEBufferComponentType type, 
    void* pixels);
typedef void (SEGPUDevice::*GPUDeviceTexture3DUpdateFromPixelBuffer)(
    SETexture* texture, SEPixelBuffer* pixelBuffer);
typedef SETextureHandle* (SEGPUDevice::*GPUDeviceTextureCubeLoadFromSystemMemory)(
    SETexture* texture, SEBufferInternalFormat internalFormat, int width,
    int height, SEBufferFormat format, SEBufferComponentType type, bool mipMap,
    void* pixelsPX, void* pixelsNX, void* pixelsPY, void* pixelsNY, 
    void* pixelsPZ, void* pixelsNZ);
typedef SETextureHandle* (SEGPUDevice::*GPUDeviceBufferTextureLoadFromTextureBuffer)(
    SETexture* texture, SETextureBuffer* textureBuffer, 
    SEBufferInternalFormat internalFormat);
typedef SEFBOHandle* (SEGPUDevice::*GPUDeviceCreateFrameBuffer)(
    SEFrameBuffer* frameBuffer);
typedef void (SEGPUDevice::*GPUDeviceDeleteFrameBuffer)(
    SEFrameBuffer* frameBuffer);
typedef void (SEGPUDevice::*GPUDeviceFrameBufferSetRenderTargets)(
    SEFrameBuffer* frameBuffer, unsigned int colorTextureCount, 
    SETexture** colorTextures, SETexture* depthTexture, SETexture* stencilTexture);
typedef void (SEGPUDevice::*GPUDeviceFrameBufferSetColorTarget)(SEFrameBuffer* frameBuffer,
    unsigned int attachPoint, SETexture* colorTexture);
typedef void (SEGPUDevice::*GPUDeviceFrameBufferSetDepthTarget)(SEFrameBuffer* frameBuffer, 
    SETexture* depthTexture);
typedef void (SEGPUDevice::*GPUDeviceFrameBufferSetStencilTarget)(SEFrameBuffer* frameBuffer, 
    SETexture* stencilTexture);
typedef void (SEGPUDevice::*GPUDeviceFrameBufferEnable)(
    SEFrameBuffer* frameBuffer);
typedef void (SEGPUDevice::*GPUDeviceFrameBufferDisable)(
    SEFrameBuffer* frameBuffer);
typedef void (SEGPUDevice::*GPUDeviceComputeShaderDispatch)(
    SEShaderProgram* program, unsigned int globalX, unsigned int globalY, 
    unsigned int globalZ);
typedef void (SEGPUDevice::*GPUDeviceComputeShaderDispatchIndirect)(void* indirect);
typedef void (SEGPUDevice::*GPUDeviceDispatchVertex)(unsigned int threadCount);
typedef void (SEGPUDevice::*GPUDeviceDispatchVertexIndirect)(void* indirect);
typedef void (SEGPUDevice::*GPUDeviceDeleteBuffer)(SEBuffer* buffer);
typedef void* (SEGPUDevice::*GPUDeviceBufferMap)(SEBuffer* buffer,
    SEBufferAccess access);
typedef void (SEGPUDevice::*GPUDeviceBufferUnmap)(SEBuffer* buffer);
typedef void (SEGPUDevice::*GPUDeviceBufferBindIndex)(SEBuffer* buffer, 
    unsigned int index);
typedef void (SEGPUDevice::*GPUDeviceBufferBindIndexTo)(SEBuffer* buffer,
    unsigned int index, SEBufferView* view);
typedef void (SEGPUDevice::*GPUDeviceBufferBind)(SEBuffer* buffer);
typedef void (SEGPUDevice::*GPUDeviceBufferBindTo)(SEBuffer* buffer, SEBufferView* view);
typedef SEBufferViewHandle* (SEGPUDevice::*GPUDeviceCreateBufferView)(SEBufferView* view);
typedef void (SEGPUDevice::*GPUDeviceDeleteBufferView)(SEBufferView* view);
typedef void (SEGPUDevice::*GPUDeviceBufferUpdateSubData)(SEBuffer* buffer, 
    int offset, size_t size, void* data);
typedef SEBufferHandle* (SEGPUDevice::*GPUDeviceBufferLoadFromSystemMemory)(
    SEBuffer* buffer, size_t size, void* data, SEBufferUsage usage);
typedef SEBufferHandle* 
    (SEGPUDevice::*GPUDeviceBufferLoadImmutableFromSystemMemory)(SEBuffer* buffer, 
    size_t size, void* data);
typedef void (SEGPUDevice::*GPUDeviceBufferClear)(SEBuffer* buffer, 
    SEBufferInternalFormat internalFormat, SEBufferFormat format, 
    SEBufferComponentType type, void* data);
typedef int (SEGPUDevice::*GPUDeviceGetDeviceMemoryAvailable)();
typedef void (SEGPUDevice::*GPUDeviceMemoryBarrier)(unsigned int flags);
typedef void (SEGPUDevice::*GPUDeviceApplyPipelineStateBlock)(SEPipelineStateBlock* psb);
typedef void (SEGPUDevice::*GPUDeviceSetPatchVertices)(int value);
typedef void (SEGPUDevice::*GPUDeviceDrawPrimitive)(SEPrimitiveType type, int count);
typedef void (SEGPUDevice::*GPUDeviceDrawPrimitiveInstanced)(SEPrimitiveType type, 
    int count, int instanceCount);
typedef void (SEGPUDevice::*GPUDeviceDrawIndexedPrimitive)(SEPrimitiveType type, int count);
typedef void (SEGPUDevice::*GPUDeviceDrawIndexedPrimitiveIndirect)(SEPrimitiveType type,
    unsigned int commandOffset);
typedef void (SEGPUDevice::*GPUDeviceSetPointSize)(float value);
typedef void (SEGPUDevice::*GPUDeviceGetViewport)(SEViewportState* dstViewport);
typedef void (SEGPUDevice::*GPUDeviceSetViewport)(SEViewportState* srcViewport);
typedef SEGPUTimerHandle* (SEGPUDevice::*GPUDeviceCreateTimer)(SEGPUTimer* timer);
typedef void (SEGPUDevice::*GPUDeviceDeleteTimer)(SEGPUTimer* timer);
typedef void (SEGPUDevice::*GPUDeviceTimerStart)(SEGPUTimer* timer);
typedef void (SEGPUDevice::*GPUDeviceTimerStop)(SEGPUTimer* timer);
typedef double (SEGPUDevice::*GPUDeviceTimerGetTimeElapsed)(SEGPUTimer* timer);

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/29/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEGPUDevice : public SEGPUDeviceBase
{
public:
    ~SEGPUDevice();

protected:
    // Abstract base class.
    SEGPUDevice();

public:
    // Shader stuff.
    inline 	SEShaderHandle* CreateShader(SEShader* shader);
    inline 	void DeleteShader(SEShader* shader);
    inline 	SEShaderProgramHandle* CreateProgram(SEShaderProgram* program);
    inline 	void DeleteProgram(SEShaderProgram* program);
    inline 	void EnableProgram(SEShaderProgram* program);
    inline 	void DisableProgram(SEShaderProgram* program);

    // Pass info stuff.
    inline 	SEPassInfoHandle* CreatePassInfo(SEPassInfo* passInfo, 
        SEShaderProgram* program, SEPrimitive* primitive, bool hasNormal, 
        bool hasTCoord, int vertexComponentCount);
    inline 	void DeletePassInfo(SEPassInfo* passInfo);
    inline void EnablePassInfo(SEPassInfo* passInfo);
    inline void DisablePassInfo(SEPassInfo* passInfo);

    // Uniform stuff.
    inline 	void GetUniformLocation(SEShaderProgram* program, 
        SEShaderUniform* uniform, const char* name);
    inline 	void SetUniformValueMat4(SEShaderUniform* uniform,
        const SEMatrix4f* value);
    inline 	void SetUniformValueVec3(SEShaderUniform* uniform,
        const SEVector3f* value);
	inline 	void SetUniformValueVec4(SEShaderUniform* uniform,
		const SEVector4f* value);
    inline 	void SetUniformValueInt(SEShaderUniform* uniform,
        int value);
    inline 	void SetUniformValueUInt(SEShaderUniform* uniform,
        unsigned int value);
    inline 	void SetUniformValueFloat(SEShaderUniform* uniform, float value);
    inline 	void SetUniformValueFloat2(SEShaderUniform* uniform, 
        const float* value);

    inline 	void SetProgramParameterInt(SEShaderProgram* program, 
        SEShaderProgramParameter pname, int value);

    // Texture stuff.
    inline 	void DeleteTexture(SETexture* texture);
    inline SETextureHandle* Texture1DLoadFromSystemMemory(SETexture* texture, 
        SEBufferInternalFormat internalFormat, int width, SEBufferFormat format, 
        SEBufferComponentType type, void* pixels);
    inline 	void Texture1DUpdateFromPixelBuffer(SETexture* texture, 
        SEPixelBuffer* pixelBuffer);
    inline 	void TextureBindToImageUnit(SETexture* texture, unsigned int unit,
		bool layered, SEBufferAccess access);
    inline 	void TextureBindToSampler(SETexture* texture, unsigned int index, 
        SESamplerDesc* sampler);
    inline void TextureGenerateMipmap(SETexture* texture);
    inline  void Texture1DGetDataFromGPUMemory(SETexture* texture, 
        void* dstData);
    inline 	SETextureHandle* Texture2DLoadFromSystemMemory(SETexture* texture, 
        SEBufferInternalFormat internalFormat, int width, int height, 
        SEBufferFormat format, SEBufferComponentType type, bool mipMap, 
        SESamplerDesc* sampler, void* pixels);
    inline 	SETextureHandle* Texture2DLoadFromTextureBuffer(SETexture* texture, 
        SETextureBuffer* textureBuffer, SEBufferInternalFormat internalFormat);
    inline 	void Texture2DUpdateFromPixelBuffer(SETexture* texture, 
        SEPixelBuffer* pixelBuffer);
    inline 	void Texture2DGetImageData(SETexture* texture, void* dstPixels);
    inline 	SETextureHandle* Tex2DArrayLoadFromSystemMemory(SETexture* texture, 
        SEBufferInternalFormat internalFormat, int width, int height, 
        int depth, SEBufferFormat format, SEBufferComponentType type, 
		bool mipMap, void* pixels);
    inline 	SETextureHandle* Texture3DLoadFromSystemMemory(SETexture* texture, 
        SEBufferInternalFormat internalFormat, int width, int height, 
        int depth, SEBufferFormat format, SEBufferComponentType type, 
        void* pixels);
    inline 	void Texture3DUpdateFromPixelBuffer(SETexture* texture, 
        SEPixelBuffer* pixelBuffer);
    inline 	SETextureHandle* TextureCubeLoadFromSystemMemory(SETexture* texture, 
        SEBufferInternalFormat internalFormat, int width, int height, 
        SEBufferFormat format, SEBufferComponentType type, bool mipMap,
        void* pixelsPX, void* pixelsNX, void* pixelsPY, void* pixelsNY,
        void* pixelsPZ, void* pixelsNZ);
    inline  SETextureHandle* BufferTextureLoadFromTextureBuffer(
        SETexture* texture, SETextureBuffer* textureBuffer,
        SEBufferInternalFormat internalFormat);

    // Framebuffer stuff.
    inline 	SEFBOHandle* CreateFrameBuffer(SEFrameBuffer* frameBuffer);
    inline 	void DeleteFrameBuffer(SEFrameBuffer* frameBuffer);
    inline 	void FrameBufferSetRenderTargets(SEFrameBuffer* frameBuffer, 
        unsigned int colorTextureCount, SETexture** colorTextures, 
        SETexture* depthTexture, SETexture* stencilTexture);
    inline  void FrameBufferSetColorTarget(SEFrameBuffer* frameBuffer, 
        unsigned int attachPoint, SETexture* colorTexture);
    inline  void FrameBufferSetDepthTarget(SEFrameBuffer* frameBuffer, 
        SETexture* depthTexture);
    inline  void FrameBufferSetStencilTarget(SEFrameBuffer* frameBuffer, 
        SETexture* stencilTexture);
    inline 	void FrameBufferEnable(SEFrameBuffer* frameBuffer);
    inline 	void FrameBufferDisable(SEFrameBuffer* frameBuffer);

    // Compute shader stuff.
    inline 	void ComputeShaderDispatch(SEShaderProgram* program, 
        unsigned int globalX, unsigned int globalY, unsigned int globalZ);
    inline 	void ComputeShaderDispatchIndirect(void* indirect);
    inline 	void DispatchVertex(unsigned int threadCount);
    inline 	void DispatchVertexIndirect(void* indirect);

    // Buffer stuff.
    inline 	void DeleteBuffer(SEBuffer* buffer);
    inline 	void* BufferMap(SEBuffer* buffer, SEBufferAccess access);
    inline 	void BufferUnmap(SEBuffer* buffer);
    inline 	void BufferBindIndex(SEBuffer* buffer, unsigned int index);
    inline  void BufferBindIndexTo(SEBuffer* buffer, unsigned int index, SEBufferView* view);
    inline 	void BufferBind(SEBuffer* buffer);
    inline  void BufferBindTo(SEBuffer* buffer, SEBufferView* view);
    inline  SEBufferViewHandle* CreateBufferView(SEBufferView* view);
    inline  void DeleteBufferView(SEBufferView* view);
    inline 	void BufferUpdateSubData(SEBuffer* buffer, int offset, size_t size, 
        void* data);
    inline 	SEBufferHandle* BufferLoadFromSystemMemory(SEBuffer* buffer, 
        size_t size, void* data, SEBufferUsage usage);
    inline 	SEBufferHandle* BufferLoadImmutableFromSystemMemory(SEBuffer* buffer,
        size_t size, void* data);
    inline 	void BufferClear(SEBuffer* buffer, 
        SEBufferInternalFormat internalFormat, SEBufferFormat format,
        SEBufferComponentType type, void* data);

    // Device query stuff.
    inline const SEGPUDeviceDescription& GetDeviceDescription() const;
    inline const SEGPUDeviceInfo& GetDeviceInfo() const;
    inline int GetDeviceMemoryAvailable();

    // Inspector stuff.
    void SetInspector(SEGPUDeviceInspector* inspector);
    inline SEGPUDeviceInspector* GetInspector();

    // Memory barrier stuff.
    inline void MemoryBarrier(unsigned int flags);

    // Pipeline state block stuff.
    inline void ApplyPipelineStateBlock(SEPipelineStateBlock* psb);

    inline void SetPatchVertices(int value);

    // Drawing stuff.
    inline void DrawPrimitive(SEPrimitiveType type, int count);
    inline void DrawPrimitiveInstanced(SEPrimitiveType type, int count, int instanceCount);
    inline void DrawIndexedPrimitive(SEPrimitiveType type, int count);
    inline void DrawIndexedPrimitiveIndirect(SEPrimitiveType type, unsigned int commandOffset);

    inline void SetPointSize(float value);

    inline void GetViewport(SEViewportState* dstViewport);
    inline void SetViewport(SEViewportState* srcViewport);

    // Timer stuff.
    inline SEGPUTimerHandle* CreateTimer(SEGPUTimer* timer);
    inline void DeleteTimer(SEGPUTimer* timer);
    inline void TimerStart(SEGPUTimer* timer);
    inline void TimerStop(SEGPUTimer* timer);
    inline double TimerGetTimeElapsed(SEGPUTimer* timer);

protected:
    GPUDeviceCreateShader                         _CreateShader;
    GPUDeviceDeleteShader                         _DeleteShader;
    GPUDeviceCreateProgram                        _CreateProgram;
    GPUDeviceDeleteProgram                        _DeleteProgram;
    GPUDeviceEnableProgram                        _EnableProgram;
    GPUDeviceDisableProgram                       _DisableProgram;
    GPUDeviceCreatePassInfo                       _CreatePassInfo;
    GPUDeviceDeletePassInfo                       _DeletePassInfo;
    GPUDeviceEnablePassInfo                       _EnablePassInfo;
    GPUDeviceDisablePassInfo                      _DisablePassInfo;
    GPUDeviceGetUniformLocation                   _GetUniformLocation;
    GPUDeviceSetUniformValueMat4                  _SetUniformValueMat4;
    GPUDeviceSetUniformValueVec3                  _SetUniformValueVec3;
    GPUDeviceSetUniformValueVec4                  _SetUniformValueVec4;
    GPUDeviceSetUniformValueInt                   _SetUniformValueInt;
    GPUDeviceSetUniformValueUInt                  _SetUniformValueUInt;
    GPUDeviceSetUniformValueFloat                 _SetUniformValueFloat;
    GPUDeviceSetUniformValueFloat2                _SetUniformValueFloat2;
    GPUDeviceSetProgramParameterInt               _SetProgramParameterInt;
    GPUDeviceDeleteTexture                        _DeleteTexture;
    GPUDeviceTexture1DLoadFromSystemMemory        _Texture1DLoadFromSystemMemory;
    GPUDeviceTexture1DUpdateFromPixelBuffer       _Texture1DUpdateFromPixelBuffer;
    GPUDeviceTextureBindToImageUnit               _TextureBindToImageUnit;
    GPUDeviceTextureBindToSampler                 _TextureBindToSampler;
    GPUDeviceTextureGenerateMipmap                _TextureGenerateMipmap;
    GPUDeviceTexture1DGetDataFromGPUMemory        _Texture1DGetDataFromGPUMemory;
    GPUDeviceTexture2DLoadFromSystemMemory        _Texture2DLoadFromSystemMemory;
    GPUDeviceTexture2DLoadFromTextureBuffer       _Texture2DLoadFromTextureBuffer;
    GPUDeviceTexture2DUpdateFromPixelBuffer       _Texture2DUpdateFromPixelBuffer;
    GPUDeviceTexture2DGetImageData                _Texture2DGetImageData;
    GPUDeviceTex2DArrayLoadFromSystemMemory       _Tex2DArrayLoadFromSystemMemory;
    GPUDeviceTexture3DLoadFromSystemMemory        _Texture3DLoadFromSystemMemory;
    GPUDeviceTexture3DUpdateFromPixelBuffer       _Texture3DUpdateFromPixelBuffer;
    GPUDeviceTextureCubeLoadFromSystemMemory      _TextureCubeLoadFromSystemMemory;
    GPUDeviceBufferTextureLoadFromTextureBuffer   _BufferTextureLoadFromTextureBuffer;
    GPUDeviceCreateFrameBuffer                    _CreateFrameBuffer;
    GPUDeviceDeleteFrameBuffer                    _DeleteFrameBuffer;
    GPUDeviceFrameBufferSetRenderTargets          _FrameBufferSetRenderTargets;
    GPUDeviceFrameBufferSetColorTarget            _FrameBufferSetColorTarget;
    GPUDeviceFrameBufferSetDepthTarget            _FrameBufferSetDepthTarget;
    GPUDeviceFrameBufferSetStencilTarget          _FrameBufferSetStencilTarget;
    GPUDeviceFrameBufferEnable                    _FrameBufferEnable;
    GPUDeviceFrameBufferDisable                   _FrameBufferDisable;
    GPUDeviceComputeShaderDispatch                _ComputeShaderDispatch;
    GPUDeviceComputeShaderDispatchIndirect        _ComputeShaderDispatchIndirect;
    GPUDeviceDispatchVertex                       _DispatchVertex;
    GPUDeviceDispatchVertexIndirect               _DispatchVertexIndirect;
    GPUDeviceDeleteBuffer                         _DeleteBuffer;
    GPUDeviceBufferMap                            _BufferMap;
    GPUDeviceBufferUnmap                          _BufferUnmap;
    GPUDeviceBufferBindIndex                      _BufferBindIndex;
    GPUDeviceBufferBindIndexTo                    _BufferBindIndexTo;
    GPUDeviceBufferBind                           _BufferBind;
    GPUDeviceBufferBindTo                         _BufferBindTo;
    GPUDeviceCreateBufferView                     _CreateBufferView;
    GPUDeviceDeleteBufferView                     _DeleteBufferView;
    GPUDeviceBufferUpdateSubData                  _BufferUpdateSubData;
    GPUDeviceBufferLoadFromSystemMemory           _BufferLoadFromSystemMemory;
    GPUDeviceBufferLoadImmutableFromSystemMemory  _BufferLoadImmutableFromSystemMemory;
    GPUDeviceBufferClear                          _BufferClear;
    GPUDeviceGetDeviceMemoryAvailable             _GetDeviceMemoryAvailable;
    GPUDeviceMemoryBarrier                        _MemoryBarrier;
    GPUDeviceApplyPipelineStateBlock              _ApplyPipelineStateBlock;
    GPUDeviceSetPatchVertices                     _SetPatchVertices;
    GPUDeviceDrawPrimitive                        _DrawPrimitive;
    GPUDeviceDrawPrimitiveInstanced               _DrawPrimitiveInstanced;
    GPUDeviceDrawIndexedPrimitive                 _DrawIndexedPrimitive;
    GPUDeviceDrawIndexedPrimitiveIndirect         _DrawIndexedPrimitiveIndirect;
    GPUDeviceSetPointSize                         _SetPointSize;
    GPUDeviceGetViewport                          _GetViewport;
    GPUDeviceSetViewport                          _SetViewport;
    GPUDeviceCreateTimer                          _CreateTimer;
    GPUDeviceDeleteTimer                          _DeleteTimer;
    GPUDeviceTimerStart                           _TimerStart;
    GPUDeviceTimerStop                            _TimerStop;
    GPUDeviceTimerGetTimeElapsed                  _TimerGetTimeElapsed;
};

typedef SESmartPointer<SEGPUDevice> SEGPUDevicePtr;

#include "SEGPUDevice.inl"

}

#endif