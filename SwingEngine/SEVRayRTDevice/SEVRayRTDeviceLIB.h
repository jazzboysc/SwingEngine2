// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_VRayRTDeviceLIB_H
#define Swing_VRayRTDeviceLIB_H

#ifdef SE_VRAY_RT_DEVICE_DLL_EXPORT
#define SE_VRAY_RT_DEVICE_API __declspec(dllexport)

#else
#ifdef SE_VRAY_RT_DEVICE_DLL_IMPORT
#define SE_VRAY_RT_DEVICE_API __declspec(dllimport)

#else
#define SE_VRAY_RT_DEVICE_API

#endif
#endif

#endif
