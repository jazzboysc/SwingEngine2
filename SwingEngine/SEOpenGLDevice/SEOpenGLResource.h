// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_OpenGLResource_H
#define Swing_OpenGLResource_H

#include "SEOpenGLDeviceLIB.h"
#include "SEGPUResource.h"

#include <GL/glew.h>

namespace Swing
{

struct SE_OPENGL_DEVICE_API SEOpenGLShaderHandle : public SEShaderHandle
{
    GLuint mShader;
};

struct SE_OPENGL_DEVICE_API SEOpenGLShaderProgramHandle : public SEShaderProgramHandle
{
    GLuint mProgram;
};

struct SE_OPENGL_DEVICE_API SEOpenGLShaderUniformHandle : public SEShaderUniformHandle
{
    GLint mUniform;
};

struct SE_OPENGL_DEVICE_API SEOpenGLTextureHandle : public SETextureHandle
{
    GLuint mTexture;
};

struct SE_OPENGL_DEVICE_API SEOpenGLFBOHandle : public SEFBOHandle
{
    GLuint mFBO;
};

struct SE_OPENGL_DEVICE_API SEOpenGLBufferHandle : public SEBufferHandle
{
    GLuint mBuffer;
};

struct SE_OPENGL_DEVICE_API SEOpenGLBufferViewHandle : public SEBufferViewHandle
{
};

struct SE_OPENGL_DEVICE_API SEOpenGLTimerHandle :public SEGPUTimerHandle
{
    GLuint mTimer;
};

}

#endif