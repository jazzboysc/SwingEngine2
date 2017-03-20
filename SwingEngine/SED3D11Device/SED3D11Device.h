// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_D3D11Device_H
#define Swing_D3D11Device_H

#include "SED3D11DeviceLIB.h"
#include "SEGPUDevice.h"
#include "SED3D11Common.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/14/2014
//----------------------------------------------------------------------------
class SE_D3D11_DEVICE_API SED3D11Device : public SEGPUDevice
{
public:
    SED3D11Device(HWND mainWindow);
    ~SED3D11Device();

private:
    void __Initialize(SEGPUDeviceDescription* deviceDesc);
    void __Terminate();
    void __OnResize(unsigned int width, unsigned int height);
    SEShaderHandle* __CreateShader(SEShader* shader);

private:
    HWND mMainWindow;
    bool mEnable4xMsaa;
    UINT m4xMsaaQuality;

	ID3D11Device* md3dDevice;
	ID3D11DeviceContext* md3dImmediateContext;
	IDXGISwapChain* mSwapChain;
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11DepthStencilView* mDepthStencilView;
	D3D11_VIEWPORT mScreenViewport;
};

}

#endif