#include "RTLightmapBaker.h"
#include "SEVRayRTDevice.h"

using namespace Swing;

SEVRayWin32Application* app = 0;
//----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	unsigned int width, height;
	width = 1280;
	height = 720;

    // Create ray tracing device.
    SEVRayRTDevice* rtDevice = SE_NEW SEVRayRTDevice();

    // Initialize application.
	app = SE_NEW RTLightmapBaker(width, height);
    SEApplicationDescription appDesc;
    appDesc.GraphicsFeature = AGF_RayTracer;
    appDesc.RayTracingDevice = rtDevice;
    app->Swing::SEVRayWin32Application::Initialize(&appDesc);
    app->Swing::SEVRayWin32Application::Run();
    app->Swing::SEVRayWin32Application::Terminate();
    SE_DELETE app;

    return 0;
}
//----------------------------------------------------------------------------