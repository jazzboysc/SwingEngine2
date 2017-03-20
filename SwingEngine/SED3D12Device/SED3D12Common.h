// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_D3D12Common_H
#define Swing_D3D12Common_H

#include <windows.h>
#include <d3d12.h>
#include <D3Dcompiler.h>
#include <dxgi1_4.h>
#include <wrl.h>

using namespace Microsoft::WRL;

#define SE_RELEASE_COM(x) { if(x){ x->Release(); x = 0; } }

inline void ThrowIfFailed(HRESULT hr)
{
    if( FAILED(hr) )
    {
        throw;
    }
}

#endif