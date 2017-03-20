// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_RenderingEngineLIB_H
#define Swing_RenderingEngineLIB_H

#ifdef SE_RENDERING_ENGINE_DLL_EXPORT
#define SE_RENDERING_ENGINE_API __declspec(dllexport)

#else
#ifdef SE_RENDERING_ENGINE_DLL_IMPORT
#define SE_RENDERING_ENGINE_API __declspec(dllimport)

#else
#define SE_RENDERING_ENGINE_API

#endif
#endif

#endif
