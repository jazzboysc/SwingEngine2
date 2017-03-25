#include "VPLviaSVOGI.pch.h"
#include "VPLviaSVOGIApp.h"
#include "SEOpenGLDevice.h"

using namespace Swing;

SEWglApplication* app = 0;
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
	SEOpenGLDevice* gpuDevice =  SE_NEW SEOpenGLDevice();

	// Initialize application.
	app = SE_NEW VPLviaSVOGI(width, height);
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