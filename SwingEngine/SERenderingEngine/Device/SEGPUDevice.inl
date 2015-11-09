// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

//----------------------------------------------------------------------------
void SEGPUDevice::BufferClear(SEBuffer* buffer, 
    SEBufferInternalFormat internalFormat, SEBufferFormat format, 
    SEBufferComponentType type, void* data)
{
    (this->*_BufferClear)(buffer, internalFormat, format, type, data);
}
//----------------------------------------------------------------------------
SEBufferHandle* SEGPUDevice::BufferLoadImmutableFromSystemMemory(SEBuffer* buffer, 
    size_t size, void* data)
{
    return (this->*_BufferLoadImmutableFromSystemMemory)(buffer, size, data);
}
//----------------------------------------------------------------------------
SEBufferHandle* SEGPUDevice::BufferLoadFromSystemMemory(SEBuffer* buffer, 
    size_t size, void* data, SEBufferUsage usage)
{
    return (this->*_BufferLoadFromSystemMemory)(buffer, size, data, usage);
}
//----------------------------------------------------------------------------
void SEGPUDevice::BufferUpdateSubData(SEBuffer* buffer, int offset, size_t size, 
    void* data)
{
    (this->*_BufferUpdateSubData)(buffer, offset, size, data);
}
//----------------------------------------------------------------------------
void SEGPUDevice::BufferBind(SEBuffer* buffer)
{
    (this->*_BufferBind)(buffer);
}
//----------------------------------------------------------------------------
void SEGPUDevice::BufferBindTo(SEBuffer* buffer, SEBufferView* view)
{
    (this->*_BufferBindTo)(buffer, view);
}
//----------------------------------------------------------------------------
SEBufferViewHandle* SEGPUDevice::CreateBufferView(SEBufferView* view)
{
    return (this->*_CreateBufferView)(view);
}
//----------------------------------------------------------------------------
void SEGPUDevice::DeleteBufferView(SEBufferView* view)
{
    (this->*_DeleteBufferView)(view);
}
//----------------------------------------------------------------------------
void SEGPUDevice::BufferBindIndex(SEBuffer* buffer, unsigned int index)
{
    (this->*_BufferBindIndex)(buffer, index);
}
//----------------------------------------------------------------------------
void SEGPUDevice::BufferBindIndexTo(SEBuffer* buffer, unsigned int index, 
    SEBufferView* view)
{
    (this->*_BufferBindIndexTo)(buffer, index, view);
}
//----------------------------------------------------------------------------
void SEGPUDevice::BufferUnmap(SEBuffer* buffer)
{
    (this->*_BufferUnmap)(buffer);
}
//----------------------------------------------------------------------------
void* SEGPUDevice::BufferMap(SEBuffer* buffer, SEBufferAccess access)
{
    return (this->*_BufferMap)(buffer, access);
}
//----------------------------------------------------------------------------
void SEGPUDevice::DeleteBuffer(SEBuffer* buffer)
{
    (this->*_DeleteBuffer)(buffer);
}
//----------------------------------------------------------------------------
void SEGPUDevice::ComputeShaderDispatch(SEShaderProgram* program,
	unsigned int globalX, unsigned int globalY, unsigned int globalZ)
{
	(this->*_ComputeShaderDispatch)(program, globalX, globalY, globalZ);
}
//----------------------------------------------------------------------------
void SEGPUDevice::ComputeShaderDispatchIndirect(void* indirect)
{
	(this->*_ComputeShaderDispatchIndirect)(indirect);
}
//----------------------------------------------------------------------------
void SEGPUDevice::DispatchVertexIndirect(void* indirect)
{
    (this->*_DispatchVertexIndirect)(indirect);
}
//----------------------------------------------------------------------------
void SEGPUDevice::DispatchVertex(unsigned int threadCount)
{
    (this->*_DispatchVertex)(threadCount);
}
//----------------------------------------------------------------------------
void SEGPUDevice::FrameBufferDisable(SEFrameBuffer* frameBuffer)
{
    (this->*_FrameBufferDisable)(frameBuffer);
}
//----------------------------------------------------------------------------
void SEGPUDevice::FrameBufferEnable(SEFrameBuffer* frameBuffer)
{
    (this->*_FrameBufferEnable)(frameBuffer);
}
//----------------------------------------------------------------------------
void SEGPUDevice::FrameBufferSetRenderTargets(SEFrameBuffer* frameBuffer,
    unsigned int colorTextureCount, SETexture** colorTextures,
    SETexture* depthTexture, SETexture* stencilTexture)
{
    (this->*_FrameBufferSetRenderTargets)(frameBuffer, colorTextureCount,
        colorTextures, depthTexture, stencilTexture);
}
//----------------------------------------------------------------------------
void SEGPUDevice::FrameBufferSetColorTarget(SEFrameBuffer* frameBuffer, 
    unsigned int attachPoint, SETexture* colorTexture)
{
    (this->*_FrameBufferSetColorTarget)(frameBuffer, attachPoint, 
        colorTexture);
}
//----------------------------------------------------------------------------
void SEGPUDevice::FrameBufferSetDepthTarget(SEFrameBuffer* frameBuffer, 
    SETexture* depthTexture)
{
    (this->*_FrameBufferSetDepthTarget)(frameBuffer, depthTexture);
}
//----------------------------------------------------------------------------
void SEGPUDevice::FrameBufferSetStencilTarget(SEFrameBuffer* frameBuffer, 
    SETexture* stencilTexture)
{
    (this->*_FrameBufferSetStencilTarget)(frameBuffer, stencilTexture);
}
//----------------------------------------------------------------------------
void SEGPUDevice::DeleteFrameBuffer(SEFrameBuffer* frameBuffer)
{
    (this->*_DeleteFrameBuffer)(frameBuffer);
}
//----------------------------------------------------------------------------
SEFBOHandle* SEGPUDevice::CreateFrameBuffer(SEFrameBuffer* frameBuffer)
{
    return (this->*_CreateFrameBuffer)(frameBuffer);
}
//----------------------------------------------------------------------------
SETextureHandle* SEGPUDevice::TextureCubeLoadFromSystemMemory(SETexture* texture,
    SEBufferInternalFormat internalFormat, int width, int height,
    SEBufferFormat format, SEBufferComponentType type, bool mipMap,
    void* pixelsPX, void* pixelsNX, void* pixelsPY, void* pixelsNY,
    void* pixelsPZ, void* pixelsNZ)
{
    return (this->*_TextureCubeLoadFromSystemMemory)(texture, internalFormat, 
        width, height, format, type, mipMap, pixelsPX, pixelsNX, pixelsPY, 
        pixelsNY, pixelsPZ, pixelsNZ);
}
//----------------------------------------------------------------------------
SETextureHandle* SEGPUDevice::BufferTextureLoadFromTextureBuffer(SETexture* texture,
    SETextureBuffer* textureBuffer, SEBufferInternalFormat internalFormat)
{
    return (this->*_BufferTextureLoadFromTextureBuffer)(texture, 
        textureBuffer, internalFormat);
}
//----------------------------------------------------------------------------
void SEGPUDevice::Texture3DUpdateFromPixelBuffer(SETexture* texture, 
    SEPixelBuffer* pixelBuffer)
{
    (this->*_Texture3DUpdateFromPixelBuffer)(texture, pixelBuffer);
}
//----------------------------------------------------------------------------
SETextureHandle* SEGPUDevice::Texture3DLoadFromSystemMemory(SETexture* texture, 
    SEBufferInternalFormat internalFormat, int width, int height, int depth, 
    SEBufferFormat format, SEBufferComponentType type, void* pixels)
{
    return (this->*_Texture3DLoadFromSystemMemory)(texture, internalFormat, 
        width, height, depth, format, type, pixels);
}
//----------------------------------------------------------------------------
SETextureHandle* SEGPUDevice::Tex2DArrayLoadFromSystemMemory(SETexture* texture, 
    SEBufferInternalFormat internalFormat, int width, int height, int depth, 
	SEBufferFormat format, SEBufferComponentType type, bool mipmap, void* pixels)
{
    return (this->*_Tex2DArrayLoadFromSystemMemory)(texture, internalFormat, 
        width, height, depth, format, type, mipmap, pixels);
}
//----------------------------------------------------------------------------
void SEGPUDevice::Texture2DGetImageData(SETexture* texture, void* dstPixels)
{
    (this->*_Texture2DGetImageData)(texture, dstPixels);
}
//----------------------------------------------------------------------------
void SEGPUDevice::Texture2DUpdateFromPixelBuffer(SETexture* texture, 
    SEPixelBuffer* pixelBuffer)
{
    (this->*_Texture2DUpdateFromPixelBuffer)(texture, pixelBuffer);
}
//----------------------------------------------------------------------------
SETextureHandle* SEGPUDevice::Texture2DLoadFromTextureBuffer(SETexture* texture, 
    SETextureBuffer* textureBuffer, SEBufferInternalFormat internalFormat)
{
    return (this->*_Texture2DLoadFromTextureBuffer)(texture, textureBuffer, 
        internalFormat);
}
//----------------------------------------------------------------------------
SETextureHandle* SEGPUDevice::Texture2DLoadFromSystemMemory(SETexture* texture, 
    SEBufferInternalFormat internalFormat, int width, int height, 
    SEBufferFormat format, SEBufferComponentType type, bool mipMap, 
    SESamplerDesc* sampler, void* pixels)
{
    return (this->*_Texture2DLoadFromSystemMemory)(texture, internalFormat, 
        width, height, format, type, mipMap, sampler, pixels);
}
//----------------------------------------------------------------------------
void SEGPUDevice::Texture1DGetDataFromGPUMemory(SETexture* texture, void* dstData)
{
    (this->*_Texture1DGetDataFromGPUMemory)(texture, dstData);
}
//----------------------------------------------------------------------------
void SEGPUDevice::TextureBindToSampler(SETexture* texture, unsigned int index, 
    SESamplerDesc* sampler)
{
    (this->*_TextureBindToSampler)(texture, index, sampler);
}
//----------------------------------------------------------------------------
void SEGPUDevice::TextureGenerateMipmap(SETexture* texture)
{
    (this->*_TextureGenerateMipmap)(texture);
}
//----------------------------------------------------------------------------
void SEGPUDevice::TextureBindToImageUnit(SETexture* texture, unsigned int unit, 
	bool layered, SEBufferAccess access)
{
	(this->*_TextureBindToImageUnit)(texture, unit, layered, access);
}
//----------------------------------------------------------------------------
void SEGPUDevice::Texture1DUpdateFromPixelBuffer(SETexture* texture, 
    SEPixelBuffer* pixelBuffer)
{
    (this->*_Texture1DUpdateFromPixelBuffer)(texture, pixelBuffer);
}
//----------------------------------------------------------------------------
SETextureHandle* SEGPUDevice::Texture1DLoadFromSystemMemory(SETexture* texture, 
    SEBufferInternalFormat internalFormat, int width, SEBufferFormat format, 
    SEBufferComponentType type, void* pixels)
{
    return (this->*_Texture1DLoadFromSystemMemory)(texture, internalFormat, 
        width, format, type, pixels);
}
//----------------------------------------------------------------------------
void SEGPUDevice::DeleteTexture(SETexture* texture)
{
    (this->*_DeleteTexture)(texture);
}
//----------------------------------------------------------------------------
void SEGPUDevice::SetProgramParameterInt(SEShaderProgram* program,
    SEShaderProgramParameter pname, int value)
{
    (this->*_SetProgramParameterInt)(program, pname, value);
}
//----------------------------------------------------------------------------
void SEGPUDevice::SetUniformValueFloat2(SEShaderUniform* uniform, 
    const float* value)
{
    (this->*_SetUniformValueFloat2)(uniform, value);
}
//----------------------------------------------------------------------------
void SEGPUDevice::SetUniformValueFloat(SEShaderUniform* uniform, float value)
{
    (this->*_SetUniformValueFloat)(uniform, value);
}
//----------------------------------------------------------------------------
void SEGPUDevice::SetUniformValueInt(SEShaderUniform* uniform, int value)
{
    (this->*_SetUniformValueInt)(uniform, value);
}
//----------------------------------------------------------------------------
void SEGPUDevice::SetUniformValueUInt(SEShaderUniform* uniform, 
    unsigned int value)
{
    (this->*_SetUniformValueUInt)(uniform, value);
}
//----------------------------------------------------------------------------
void SEGPUDevice::SetUniformValueVec3(SEShaderUniform* uniform, 
    const SEVector3f* value)
{
    (this->*_SetUniformValueVec3)(uniform, value);
}
//----------------------------------------------------------------------------
void SEGPUDevice::SetUniformValueVec4(SEShaderUniform* uniform,
	const SEVector4f* value)
{
	(this->*_SetUniformValueVec4)(uniform, value);
}
//----------------------------------------------------------------------------
void SEGPUDevice::SetUniformValueMat4(SEShaderUniform* uniform, 
    const SEMatrix4f* value)
{
    (this->*_SetUniformValueMat4)(uniform, value);
}
//----------------------------------------------------------------------------
void SEGPUDevice::GetUniformLocation(SEShaderProgram* program,
    SEShaderUniform* uniform, const char* name)
{
    (this->*_GetUniformLocation)(program, uniform, name);
}
//----------------------------------------------------------------------------
void SEGPUDevice::DeletePassInfo(SEPassInfo* passInfo)
{
    (this->*_DeletePassInfo)(passInfo);
}
//----------------------------------------------------------------------------
SEPassInfoHandle* SEGPUDevice::CreatePassInfo(SEPassInfo* passInfo, 
    SEShaderProgram* program, SEPrimitive* primitive, bool hasNormal, 
    bool hasTCoord, int vertexComponentCount)
{
    return (this->*_CreatePassInfo)(passInfo, program, primitive, hasNormal, 
        hasTCoord, vertexComponentCount);
}
//----------------------------------------------------------------------------
void SEGPUDevice::EnablePassInfo(SEPassInfo* passInfo)
{
    (this->*_EnablePassInfo)(passInfo);
}
//----------------------------------------------------------------------------
void SEGPUDevice::DisablePassInfo(SEPassInfo* passInfo)
{
    (this->*_DisablePassInfo)(passInfo);
}
//----------------------------------------------------------------------------
void SEGPUDevice::DisableProgram(SEShaderProgram* program)
{
    (this->*_DisableProgram)(program);
}
//----------------------------------------------------------------------------
void SEGPUDevice::EnableProgram(SEShaderProgram* program)
{
    (this->*_EnableProgram)(program);
}
//----------------------------------------------------------------------------
void SEGPUDevice::DeleteProgram(SEShaderProgram* program)
{
    (this->*_DeleteProgram)(program);
}
//----------------------------------------------------------------------------
SEShaderProgramHandle* SEGPUDevice::CreateProgram(SEShaderProgram* program)
{
    return (this->*_CreateProgram)(program);
}
//----------------------------------------------------------------------------
void SEGPUDevice::DeleteShader(SEShader* shader)
{
    (this->*_DeleteShader)(shader);
}
//----------------------------------------------------------------------------
SEShaderHandle* SEGPUDevice::CreateShader(SEShader* shader)
{
    return (this->*_CreateShader)(shader);
}
//----------------------------------------------------------------------------
const SEGPUDeviceDescription& SEGPUDevice::GetDeviceDescription() const
{
	return mDeviceDesc;
}
//----------------------------------------------------------------------------
const SEGPUDeviceInfo& SEGPUDevice::GetDeviceInfo() const
{
	return mDeviceInfo;
}
//----------------------------------------------------------------------------
int SEGPUDevice::GetDeviceMemoryAvailable()
{
	return (this->*_GetDeviceMemoryAvailable)();
}
//----------------------------------------------------------------------------
SEGPUDeviceInspector* SEGPUDevice::GetInspector()
{
    return mInspector;
}
//----------------------------------------------------------------------------
void SEGPUDevice::MemoryBarrier(unsigned int flags)
{
    (this->*_MemoryBarrier)(flags);
}
//----------------------------------------------------------------------------
void SEGPUDevice::ApplyPipelineStateBlock(SEPipelineStateBlock* psb)
{
    (this->*_ApplyPipelineStateBlock)(psb);
}
//----------------------------------------------------------------------------
inline void SEGPUDevice::SetPatchVertices(int value)
{
    (this->*_SetPatchVertices)(value);
}
//----------------------------------------------------------------------------
inline void SEGPUDevice::DrawPrimitive(SEPrimitiveType type, int count)
{
    (this->*_DrawPrimitive)(type, count);
}
//----------------------------------------------------------------------------
inline void SEGPUDevice::DrawPrimitiveInstanced(SEPrimitiveType type, int count,
    int instanceCount)
{
    (this->*_DrawPrimitiveInstanced)(type, count, instanceCount);
}
//----------------------------------------------------------------------------
inline void SEGPUDevice::DrawIndexedPrimitive(SEPrimitiveType type, int count)
{
    (this->*_DrawIndexedPrimitive)(type, count);
}
//----------------------------------------------------------------------------
inline void SEGPUDevice::DrawIndexedPrimitiveIndirect(SEPrimitiveType type,
    unsigned int commandOffset)
{
    (this->*_DrawIndexedPrimitiveIndirect)(type, commandOffset);
}
//----------------------------------------------------------------------------
inline void SEGPUDevice::SetPointSize(float value)
{
    (this->*_SetPointSize)(value);
}
//----------------------------------------------------------------------------
inline void SEGPUDevice::GetViewport(SEViewportState* dstViewport)
{
    (this->*_GetViewport)(dstViewport);
}
//----------------------------------------------------------------------------
inline void SEGPUDevice::SetViewport(SEViewportState* srcViewport)
{
    (this->*_SetViewport)(srcViewport);
}
//----------------------------------------------------------------------------
inline SEGPUTimerHandle* SEGPUDevice::CreateTimer(SEGPUTimer* timer)
{
    return (this->*_CreateTimer)(timer);
}
//----------------------------------------------------------------------------
inline void SEGPUDevice::DeleteTimer(SEGPUTimer* timer)
{
    (this->*_DeleteTimer)(timer);
}
//----------------------------------------------------------------------------
inline void SEGPUDevice::TimerStart(SEGPUTimer* timer)
{
    (this->*_TimerStart)(timer);
}
//----------------------------------------------------------------------------
inline void SEGPUDevice::TimerStop(SEGPUTimer* timer)
{
    (this->*_TimerStop)(timer);
}
//----------------------------------------------------------------------------
inline double SEGPUDevice::TimerGetTimeElapsed(SEGPUTimer* timer)
{
    return (this->*_TimerGetTimeElapsed)(timer);
}
//----------------------------------------------------------------------------