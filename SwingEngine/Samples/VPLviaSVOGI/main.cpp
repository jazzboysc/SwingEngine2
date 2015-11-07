#include "VPLviaSVOGI.pch.h"
#include "VPLviaSVOGIApp.h"
#include "SEOpenGLDevice.h"

using namespace Swing;

Swing::SEWglApplication* app = 0;
//----------------------------------------------------------------------------
int main(int, char **)
{
	unsigned int width, height;
#ifdef DEMO_SPONZA_SCENE
	width = 1280;
	height = 720;
#else
	width = 768;
	height = 768;
#endif

	// Create GPU device.
	SEOpenGLDevice* gpuDevice = new SEOpenGLDevice();

	// Initialize application.
	app = new VPLviaSVOGI(width, height);
    app->Swing::SEWglApplication::Initialize(gpuDevice);
    app->Swing::SEWglApplication::Run();
    app->Swing::SEWglApplication::Terminate();
	delete app;

	return 0;
}
//----------------------------------------------------------------------------