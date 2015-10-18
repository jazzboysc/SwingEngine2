#include "SwingEngineCore.h"
#include "SwingEngineAssetUtility.h"
#include "SwingEngineRenderingEngine.h"
#include "SwingEngineD3D11Device.h"
#include "D3D11Sample.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Test glm.
#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/simd_mat4.hpp"
#include "glm/gtx/simd_quat.hpp"
#include "glm/gtx/simd_vec4.hpp"
#include "glm/gtx/norm.hpp"
using namespace glm;
// Test glm.

#include <windows.h>
#include <windowsx.h>

using namespace Swing;

SEApplicationLayer* app = 0;
SED3D11Device* d3d11Device = 0;

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_SIZE:
		if (app)
		{
			app->OnReshape(LOWORD(lParam), HIWORD(lParam));
			return 0;
		}
		break;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		if (app)
		{
			app->OnMouse(wParam, 0, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
		}
		break;

    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
		if (app)
		{
			app->OnMouse(wParam, 1, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
		}
		break;

    case WM_MOUSEMOVE:
		if (app)
		{
			app->OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
		}
		break;

    default:
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

//----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine,
    int showCmd)
{
    (void)prevInstance;
    (void)cmdLine;
    (void)showCmd;

    WNDCLASS wc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = MainWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(0, IDI_APPLICATION);
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = "D3DWndClassName";

    if( !RegisterClass(&wc) )
    {
        MessageBox(0, "RegisterClass Failed.", 0, 0);
        return false;
    }

    // Compute window rectangle dimensions based on requested client area dimensions.
    RECT R = { 0, 0, 512, 512 };
    AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
    int width = R.right - R.left;
    int height = R.bottom - R.top;

    HWND mhMainWnd = CreateWindow("D3DWndClassName", "D3D11Sample",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, hInstance, 0);
    if( !mhMainWnd )
    {
        MessageBox(0, "CreateWindow Failed.", 0, 0);
        return false;
    }

    ShowWindow(mhMainWnd, SW_SHOW);
    UpdateWindow(mhMainWnd);

	// Test glfw.
	if( !glfwInit() )
	{
		return false;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);

	// Open a window and create its OpenGL context
	GLFWwindow* window = glfwCreateWindow(512, 512, "Test glfw", NULL, NULL);
	if( window == NULL )
	{
		glfwTerminate();
		return false;
	}

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(window, (mode->width - 512) / 2, (mode->height - 512) / 2);

	glfwMakeContextCurrent(window);
	// Test glfw.

	// Test glew.
	if( glewInit() != GLEW_OK )
	{
		return false;
	}
	// Test glew.

	// Test glm.
	glm::mat4 rotM;
	rotM = rotate(mat4(), radians(-60.0f), vec3(0, 1, 0));
	// Test glm.

	// Test png.
	png_image png;
	memset(&png, 0, sizeof(png_image));
	png.version = PNG_IMAGE_VERSION;

	if( png_image_begin_read_from_file(&png, "pointLight.png") )
	{
		png_bytep buffer;

		png.format = PNG_FORMAT_RGBA;
		buffer = (png_bytep)malloc(PNG_IMAGE_SIZE(png));

		png_image_finish_read(&png, 0, buffer, 0, 0);
	    free(buffer);
	}
	// Test png.

    // Initialize D3D11 device.
    d3d11Device = SE_NEW SED3D11Device(mhMainWnd);
    SEGPUDeviceDescription dd;
    dd.EnableMSAA = true;
    dd.FramebufferWidth = width;
    dd.FramebufferHeight = height;
    SE_GPU_DEVICE_FUNC(d3d11Device, Initialize)(&dd);

    // Initialize application.
    app = SE_NEW D3D11Sample(d3d11Device, width, height);
    app->Initialize();

    MSG msg = { 0 };
    while( msg.message != WM_QUIT )
    {
        if( PeekMessage(&msg, 0, 0, 0, PM_REMOVE) )
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            app->Run();
        }
    }

    // Terminate application.
    app->Terminate();
    SE_DELETE app;

    // Terminate D3D11 device.
    SE_GPU_DEVICE_FUNC(d3d11Device, Terminate)();
    SE_DELETE d3d11Device;

    return (int)msg.wParam;
}
//----------------------------------------------------------------------------