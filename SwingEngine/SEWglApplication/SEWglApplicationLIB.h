// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_WglApplicationLIB_H
#define Swing_WglApplicationLIB_H

#ifdef SE_WGL_APPLICATION_DLL_EXPORT
#define SE_WGL_APPLICATION_API __declspec(dllexport)

#else
#ifdef SE_WGL_APPLICATION_DLL_IMPORT
#define SE_WGL_APPLICATION_API __declspec(dllimport)

#else
#define SE_WGL_APPLICATION_API

#endif
#endif

#endif
