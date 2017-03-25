#include "D3D12Sample.h"
#include "SED3D12Device.h"

using namespace Swing;

SED3D12Application* app = 0;

//----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
    PSTR cmdLine, int showCmd)
{
    unsigned int width, height;
    width = 1280;
    height = 720;

    // Create GPU device.
    SED3D12Device* gpuDevice = new SED3D12Device(0);

    // Initialize application.
    app = new D3D12Sample(hInstance, prevInstance, cmdLine, showCmd, width, 
        height);
    SEApplicationDescription appDesc;
    appDesc.GraphicsFeature = AGF_Rasterizer;
    appDesc.GPUDevice = gpuDevice;
    app->Swing::SED3D12Application::Initialize(&appDesc);
    app->Swing::SED3D12Application::Run();
    app->Swing::SED3D12Application::Terminate();
    delete app;

    return 0;
}
//----------------------------------------------------------------------------