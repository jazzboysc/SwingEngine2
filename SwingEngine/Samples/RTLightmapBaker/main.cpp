#include "RTLightmapBaker.h"
#include "SEVRayRTDevice.h"

using namespace Swing;

SEVRayWin32Application* app = 0;
//----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	unsigned int width, height;
	width = 512;
	height = 512;

    // Create ray tracing device.
    SEVRayRTDevice* rtDevice = SE_NEW SEVRayRTDevice();

    SEApplicationDescription appDesc;
    appDesc.GraphicsFeature = AGF_RayTracer;
    appDesc.RayTracingDevice = rtDevice;
    appDesc.GPUDevice = nullptr;
    appDesc.RayTracingDeviceDescription.RenderMode = RTDRM_RT_GPU_CUDA;
    appDesc.RayTracingDeviceDescription.ImageWidth = width;
    appDesc.RayTracingDeviceDescription.ImageHeight = height;

    // Initialize application.
	app = SE_NEW RTLightmapBaker(width, height);
    app->Swing::SEVRayWin32Application::Initialize(&appDesc);
    app->Swing::SEVRayWin32Application::Run();
    app->Swing::SEVRayWin32Application::Terminate();
    SE_DELETE app;

    return 0;
}
//----------------------------------------------------------------------------