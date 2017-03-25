#include "ObjLoadingTestApp.h"
#include "SEOpenGLDevice.h"

using namespace Swing;

SEWglApplication* app = 0;
//----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	unsigned int width, height;
	width = 1280;
	height = 720;

    // Create GPU device.
    SEOpenGLDevice* gpuDevice = SE_NEW SEOpenGLDevice();

    // Initialize application.
	app = SE_NEW ObjLoadingTestApp(width, height);
    SEApplicationDescription appDesc;
    appDesc.GraphicsFeature = AGF_Rasterizer;
    appDesc.GPUDevice = gpuDevice;
    app->Swing::SEWglApplication::Initialize(&appDesc);
    app->Swing::SEWglApplication::Run();
    app->Swing::SEWglApplication::Terminate();
    SE_DELETE app;

    return 0;
}
//----------------------------------------------------------------------------