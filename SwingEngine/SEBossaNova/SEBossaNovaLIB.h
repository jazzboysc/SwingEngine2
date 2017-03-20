// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_BossaNovaLIB_H
#define Swing_BossaNovaLIB_H

#ifdef SE_BOSSA_NOVA_DLL_EXPORT
#define SE_BOSSA_NOVA_API __declspec(dllexport)

#else
#ifdef SE_BOSSA_NOVA_DLL_IMPORT
#define SE_BOSSA_NOVA_API __declspec(dllimport)

#else
#define SE_BOSSA_NOVA_API

#endif
#endif

#endif
