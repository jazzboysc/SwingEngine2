// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_D3D12Device_H
#define Swing_D3D12Device_H

#include "SED3D12DeviceLIB.h"
#include "SEGPUDevice.h"
#include "SED3D12Common.h"

namespace Swing
{

class SE_D3D12_DEVICE_API SED3D12Device : public SEGPUDevice
{
public:
    SED3D12Device(HWND mainWindow);
    ~SED3D12Device();

private:
    void __Initialize(SEGPUDeviceDescription* deviceDesc);
    void __Terminate();
    void __OnResize(unsigned int width, unsigned int height);
    SEShaderHandle* __CreateShader(SEShader* shader);

private:
    HWND mMainWindow;
    bool mEnable4xMsaa;
    UINT m4xMsaaQuality;
};

}

#endif