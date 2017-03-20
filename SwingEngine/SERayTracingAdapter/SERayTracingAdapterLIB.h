// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_RayTracingAdapterLIB_H
#define Swing_RayTracingAdapterLIB_H

#ifdef SE_RAY_TRACING_ADAPTER_DLL_EXPORT
#define SE_RAY_TRACING_ADAPTER_API __declspec(dllexport)

#else
#ifdef SE_RAY_TRACING_ADAPTER_DLL_IMPORT
#define SE_RAY_TRACING_ADAPTER_API __declspec(dllimport)

#else
#define SE_RAY_TRACING_ADAPTER_API

#endif
#endif

#endif
