#include "ObjLoadingTestApp.h"
#include "SEOpenGLDevice.h"

using namespace Swing;

Swing::SEApplication* app = 0;
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
    app->Swing::SEApplication::Initialize(gpuDevice);
    app->Swing::SEApplication::Run();
    app->Swing::SEApplication::Terminate();
    delete app;

    return 0;
}
//----------------------------------------------------------------------------