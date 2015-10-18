#include "VPLviaSVOGI.pch.h"
#include "VPLviaSVOGIApp.h"
#include "SEOpenGLDevice.h"

using namespace Swing;

Swing::SEApplication* app = 0;
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
    app->Swing::SEApplication::Initialize(gpuDevice);
    app->Swing::SEApplication::Run();
    app->Swing::SEApplication::Terminate();
	delete app;

	return 0;
}
//----------------------------------------------------------------------------