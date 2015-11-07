// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_D3D12DeviceLIB_H
#define Swing_D3D12DeviceLIB_H

#ifdef SE_D3D12_DEVICE_DLL_EXPORT
#define SE_D3D12_DEVICE_API __declspec(dllexport)

#else
#ifdef SE_D3D12_DEVICE_DLL_IMPORT
#define SE_D3D12_DEVICE_API __declspec(dllimport)

#else
#define SE_D3D12_DEVICE_API

#endif
#endif

#endif
