#ifndef Swing_D3D12Sample_H
#define Swing_D3D12Sample_H

#include "SED3D12Application.h"
#include "SEGPUDevice.h"
#include "SEShaderProgram.h"

namespace Swing
{

class D3D12Sample : public SED3D12Application
{
public:
    D3D12Sample(HINSTANCE instance, HINSTANCE prevInstance,
        PSTR cmdLine, int showCmd, int width = 768, int height = 768);
    ~D3D12Sample();

    // Override SED3D12Application interface.
    void Initialize(SEGPUDeviceBase* device) override;
    void ProcessInput() override;
    void FrameFunc() override;
    void Terminate() override;
};

}

#endif