// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_D3D12ApplicationLIB_H
#define Swing_D3D12ApplicationLIB_H

#ifdef SE_D3D12_APPLICATION_DLL_EXPORT
#define SE_D3D12_APPLICATION_API __declspec(dllexport)

#else
#ifdef SE_D3D12_APPLICATION_DLL_IMPORT
#define SE_D3D12_APPLICATION_API __declspec(dllimport)

#else
#define SE_D3D12_APPLICATION_API

#endif
#endif

#endif
