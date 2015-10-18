// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_D3D11Common_H
#define Swing_D3D11Common_H

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#define SE_RELEASE_COM(x) { if(x){ x->Release(); x = 0; } }

#endif