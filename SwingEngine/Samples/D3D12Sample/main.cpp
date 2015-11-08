#include "D3D12Sample.h"
#include "SED3D12Device.h"

using namespace Swing;

SED3D12Application* app = 0;

//----------------------------------------------------------------------------
int main(int, char **)
{
    unsigned int width, height;
    width = 1280;
    height = 720;

    // Create GPU device.
    SED3D12Device* gpuDevice = new SED3D12Device(0);

    // Initialize application.
    app = new D3D12Sample(width, height);
    app->Swing::SED3D12Application::Initialize(gpuDevice);
    app->Swing::SED3D12Application::Run();
    app->Swing::SED3D12Application::Terminate();
    delete app;

    return 0;
}
//----------------------------------------------------------------------------