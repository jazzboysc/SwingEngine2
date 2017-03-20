// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SEOpenGLDevicePCH.h"
#include "SEOpenGLDevice.h"

using namespace Swing;

//----------------------------------------------------------------------------
void SEOpenGLDevice::InsertGPUDeviceBaseFunctions()
{
    SE_INSERT_GPU_DEVICE_BASE_FUNC(Initialize, SEOpenGLDevice);
    SE_INSERT_GPU_DEVICE_BASE_FUNC(Terminate, SEOpenGLDevice);
    SE_INSERT_GPU_DEVICE_BASE_FUNC(GetMaxAnisFilterLevel, SEOpenGLDevice);
    SE_INSERT_GPU_DEVICE_BASE_FUNC(SetAnisFilterLevel, SEOpenGLDevice);
    SE_INSERT_GPU_DEVICE_BASE_FUNC(CreateShader, SEOpenGLDevice);
    SE_INSERT_GPU_DEVICE_BASE_FUNC(DeleteShader, SEOpenGLDevice);
    SE_INSERT_GPU_DEVICE_BASE_FUNC(CreateRenderPassInfo, SEOpenGLDevice);
    SE_INSERT_GPU_DEVICE_BASE_FUNC(DeleteRenderPassInfo, SEOpenGLDevice);
    SE_INSERT_GPU_DEVICE_BASE_FUNC(EnableRenderPassInfo, SEOpenGLDevice);
    SE_INSERT_GPU_DEVICE_BASE_FUNC(DisableRenderPassInfo, SEOpenGLDevice);
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__Initialize(SEGPUDeviceDescription*)
{
    // TODO:
    //  Should create OpenGL context here. Nothing to do now since we are
    // using GLFW.

    // Set device info.
    // TODO:
    // Only support NVIDIA for now.
    const GLubyte* str = glGetString(GL_VENDOR);
    std::string vendor((char*)str);
    if( vendor.find("NVIDIA") != std::string::npos )
    {
        mDeviceInfo.Vendor = DV_NVIDIA;
        glGetIntegerv(GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX,
            (GLint*)&mDeviceInfo.GPUTotalMemory);
        mDeviceInfo.GPUTotalMemory = mDeviceInfo.GPUTotalMemory >> 10;
        glGetIntegerv(GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX,
            (GLint*)&mDeviceInfo.GPUDedicatedMemory);
        mDeviceInfo.GPUDedicatedMemory = mDeviceInfo.GPUDedicatedMemory >> 10;
        mDeviceInfo.Valid = true;
    }

    // Default global states.
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glFrontFace(GL_CW);
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__Terminate()
{
    // TODO:
    //  Should destroy OpenGL context here. Nothing to do now since we are
    // using GLFW.
}
//----------------------------------------------------------------------------
SEShaderHandle* SEOpenGLDevice::__CreateShader(SEShader* shader)
{
    SEOpenGLShaderHandle* shaderHandle = SE_NEW SEOpenGLShaderHandle();
    shaderHandle->Device = this;

    const char* programText = shader->GetSource().c_str();
    GLenum shaderType = gsShaderType[(int)shader->GetType()];
    const std::string fileName = shader->GetShaderFileName();

    shaderHandle->mShader = glCreateShader(shaderType);
    glShaderSource(shaderHandle->mShader, 1, &programText, 0);
    glCompileShader(shaderHandle->mShader);

    GLint compiled;
    glGetShaderiv(shaderHandle->mShader, GL_COMPILE_STATUS, &compiled);
    if( !compiled )
    {
        GLint iInfoLen = 0;
        glGetShaderiv(shaderHandle->mShader, GL_INFO_LOG_LENGTH, &iInfoLen);
        if( iInfoLen > 1 )
        {
            char* acInfoLog = SE_NEW char[iInfoLen];
            glGetShaderInfoLog(shaderHandle->mShader, iInfoLen, 0, acInfoLog);
            SETerminal::Output(SETerminal::OC_Error, "Failed compiling %s\n%s\n",
                fileName.c_str(), acInfoLog);

            SE_DELETE[] acInfoLog;
        }

        SE_ASSERT(false);
        SE_DELETE  shaderHandle;
        return 0;
    }

#ifdef SE_OUTPUT_SHADER_RESOURCE_LOADING
    SETerminal::Output(SETerminal::OC_Success, "Loading shader %s finished\n",
        fileName.c_str());
#endif

    SE_OPENGL_DEVICE_CHECK_ERROR;

    return shaderHandle;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__DeleteShader(SEShader* shader)
{
    SEOpenGLShaderHandle* shaderHandle =
        (SEOpenGLShaderHandle*)shader->GetShaderHandle();
    glDeleteShader(shaderHandle->mShader);

    SE_OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
SERenderPassInfoHandle* SEOpenGLDevice::__CreateRenderPassInfo(SERenderPassInfo*,
    SEShaderProgram* program, SEGeometryAttributes* geometryAttr,
    SEPipelineStateBlock*, SERootSignature*, SERenderPassTargetsInfo*)
{
    SEOpenGLRenderPassInfoHandle* passInfoHandle = SE_NEW SEOpenGLRenderPassInfoHandle();
    passInfoHandle->DeviceBase = this;

    SEOpenGLShaderProgramHandle* programHandle =
        (SEOpenGLShaderProgramHandle*)program->GetProgramHandle();
    SE_ASSERT(programHandle);

    glGenVertexArrays(1, &passInfoHandle->mVAO);
    glBindVertexArray(passInfoHandle->mVAO);

    if( geometryAttr->Prim->IB )
    {
        geometryAttr->Prim->IB->Bind();
    }
    if( geometryAttr->Prim->VB )
    {
        geometryAttr->Prim->VB->Bind();
    }

    // Specify vertex attributes.
    // TODO:
    // Remove this naive input layout specification system.
    if( !geometryAttr->HasNormal && !geometryAttr->HasTCoord )
    {
        GLint loc = glGetAttribLocation(programHandle->mProgram, "vPosition");
        if( loc != -1 )
        {
            glEnableVertexAttribArray(loc);
            glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
        }
    }
    else if( geometryAttr->HasNormal && !geometryAttr->HasTCoord )
    {
        GLint loc = glGetAttribLocation(programHandle->mProgram, "vPosition");
        if( loc != -1 )
        {
            glEnableVertexAttribArray(loc);
            glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE,
                geometryAttr->VertexComponentCount*sizeof(float), 0);
        }

        loc = glGetAttribLocation(programHandle->mProgram, "vNormal");
        if( loc != -1 )
        {
            glEnableVertexAttribArray(loc);
            glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE,
                geometryAttr->VertexComponentCount*sizeof(float), (void*)12);
        }
    }
    else if( geometryAttr->HasNormal && geometryAttr->HasTCoord )
    {
        GLint loc = glGetAttribLocation(programHandle->mProgram, "vPosition");
        if( loc != -1 )
        {
            glEnableVertexAttribArray(loc);
            glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE,
                geometryAttr->VertexComponentCount*sizeof(float), 0);
        }

        loc = glGetAttribLocation(programHandle->mProgram, "vTCoord");
        if( loc != -1 )
        {
            glEnableVertexAttribArray(loc);
            glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE,
                geometryAttr->VertexComponentCount*sizeof(float), (void*)12);
        }

        loc = glGetAttribLocation(programHandle->mProgram, "vNormal");
        if( loc != -1 )
        {
            glEnableVertexAttribArray(loc);
            glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE,
                geometryAttr->VertexComponentCount*sizeof(float), (void*)20);
        }
    }
    else
    {
        GLint loc = glGetAttribLocation(programHandle->mProgram, "vPosition");
        if( loc != -1 )
        {
            glEnableVertexAttribArray(loc);
            glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE,
                geometryAttr->VertexComponentCount*sizeof(float), 0);
        }

        loc = glGetAttribLocation(programHandle->mProgram, "vTCoord");
        if( loc != -1 )
        {
            glEnableVertexAttribArray(loc);
            glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE,
                geometryAttr->VertexComponentCount*sizeof(float), (void*)12);
        }
    }

    glBindVertexArray(0);

    SE_OPENGL_DEVICE_CHECK_ERROR;

    return passInfoHandle;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__DeleteRenderPassInfo(SERenderPassInfo* renderPassInfo)
{
    SEOpenGLRenderPassInfoHandle* passInfoHandle =
        (SEOpenGLRenderPassInfoHandle*)renderPassInfo->GetPassInfoHandle();
    glDeleteVertexArrays(1, &passInfoHandle->mVAO);

    SE_OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__EnableRenderPassInfo(SERenderPassInfo* renderPassInfo)
{
    SEOpenGLRenderPassInfoHandle* passInfoHandle =
        (SEOpenGLRenderPassInfoHandle*)renderPassInfo->GetPassInfoHandle();
    glBindVertexArray(passInfoHandle->mVAO);

    SE_OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__DisableRenderPassInfo(SERenderPassInfo*)
{
    glBindVertexArray(0);

    SE_OPENGL_DEVICE_CHECK_ERROR;
}
//----------------------------------------------------------------------------
SECommandQueueHandle* SEOpenGLDevice::__CreateCommandQueue(SECommandQueue*)
{
    SEOpenGLCommandQueueHandle* commandQueueHandle
        = SE_NEW SEOpenGLCommandQueueHandle();
    commandQueueHandle->DeviceBase = this;

    return commandQueueHandle;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__DeleteCommandQueue(SECommandQueue*)
{
    // Nothing to do.
}
//----------------------------------------------------------------------------
SECommandAllocatorHandle* SEOpenGLDevice::__CreateCommandAllocator(
    SECommandAllocator*, SECommandList*)
{
    SEOpenGLCommandAllocatorHandle* commandAllocatorHandle
        = SE_NEW SEOpenGLCommandAllocatorHandle();
    commandAllocatorHandle->DeviceBase = this;

    return commandAllocatorHandle;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__DeleteCommandAllocator(
    SECommandAllocator*, SECommandList*)
{
    // Nothing to do. 
}
//----------------------------------------------------------------------------
SECommandListHandle* SEOpenGLDevice::__CreateCommandList(SECommandList*,
    SECommandAllocator*)
{
    SEOpenGLCommandListHandle* commandListHandle = SE_NEW SEOpenGLCommandListHandle();
    commandListHandle->DeviceBase = this;

    return commandListHandle;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__DeleteCommandList(SECommandList*, SECommandAllocator*)
{
    // Nothing to do. 
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__ResetRenderCommandList(SERenderCommandList*,
    SERenderPassInfo*)
{
    // Nothing to do. 
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__CloseRenderCommandList(SERenderCommandList*)
{
    // Nothing to do. 
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__RenderCommandListSetRootSignature(
    SERenderCommandList*, SERootSignature*)
{
    // Nothing to do. 
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__RenderCommandListSetViewport(SERenderCommandList*,
    SEViewportState* srcViewport)
{
    this->__SetViewport(srcViewport);
}
//----------------------------------------------------------------------------
SERootSignatureHandle* SEOpenGLDevice::__CreateRootSignature(SERootSignature*)
{
    SEOpenGLRootSignatureHandle* rootSignatureHandle = SE_NEW SEOpenGLRootSignatureHandle();
    rootSignatureHandle->DeviceBase = this;

    return rootSignatureHandle;
}
//----------------------------------------------------------------------------
void SEOpenGLDevice::__DeleteRootSignature(SERootSignature*)
{
    // Nothing to do. 
}
//----------------------------------------------------------------------------