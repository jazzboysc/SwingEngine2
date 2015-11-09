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
    SEOpenGLDevice* gpuDevice = new SEOpenGLDevice();

    // Initialize application.
	app = new ObjLoadingTestApp(width, height);
    app->Swing::SEWglApplication::Initialize(gpuDevice);
    app->Swing::SEWglApplication::Run();
    app->Swing::SEWglApplication::Terminate();
    delete app;

    return 0;
}
//----------------------------------------------------------------------------