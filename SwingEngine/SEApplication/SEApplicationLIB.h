// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_ApplicationLIB_H
#define Swing_ApplicationLIB_H

#ifdef SE_APPLICATION_DLL_EXPORT
#define SE_APPLICATION_API __declspec(dllexport)

#else
#ifdef SE_APPLICATION_DLL_IMPORT
#define SE_APPLICATION_API __declspec(dllimport)

#else
#define SE_APPLICATION_API

#endif
#endif

#endif
