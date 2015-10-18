// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_AssetUtilityLIB_H
#define Swing_AssetUtilityLIB_H

#ifdef SE_ASSET_UTILITY_DLL_EXPORT
#define SE_ASSET_UTILITY_API __declspec(dllexport)

#else
#ifdef SE_ASSET_UTILITY_DLL_IMPORT
#define SE_ASSET_UTILITY_API __declspec(dllimport)

#else
#define SE_ASSET_UTILITY_API

#endif
#endif

#endif
