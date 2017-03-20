// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_OpenGLDeviceLIB_H
#define Swing_OpenGLDeviceLIB_H

#ifdef SE_OPENGL_DEVICE_DLL_EXPORT
#define SE_OPENGL_DEVICE_API __declspec(dllexport)

#else
#ifdef SE_OPENGL_DEVICE_DLL_IMPORT
#define SE_OPENGL_DEVICE_API __declspec(dllimport)

#else
#define SE_OPENGL_DEVICE_API

#endif
#endif

#endif
