// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_VRayWin32ApplicationLIB_H
#define Swing_VRayWin32ApplicationLIB_H

#ifdef SE_VRAY_WIN32_APPLICATION_DLL_EXPORT
#define SE_VRAY_WIN32_APPLICATION_API __declspec(dllexport)

#else
#ifdef SE_VRAY_WIN32_APPLICATION_DLL_IMPORT
#define SE_VRAY_WIN32_APPLICATION_API __declspec(dllimport)

#else
#define SE_VRAY_WIN32_APPLICATION_API

#endif
#endif

#endif
