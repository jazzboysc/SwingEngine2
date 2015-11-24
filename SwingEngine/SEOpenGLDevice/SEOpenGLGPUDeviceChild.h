// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_OpenGLGPUDeviceChild_H
#define Swing_OpenGLGPUDeviceChild_H

#include "SEOpenGLDeviceLIB.h"
#include "SEGPUDeviceBaseChild.h"

namespace Swing
{

struct SE_OPENGL_DEVICE_API SEOpenGLRenderPassInfoHandle : public SERenderPassInfoHandle
{
    GLuint mVAO;
};

}

#endif