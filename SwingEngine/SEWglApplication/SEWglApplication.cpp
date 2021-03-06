// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SEWglApplicationPCH.h"
#include "SEWglApplication.h"
#include "SERTGICamera.h"
#include "SETextureManager.h"
#include "SEVector2.h"
#include "SEVector3.h"

#include <GL/glew.h>
#include <glfw/glfw3.h>
#include <sys/timeb.h>
#include <direct.h>

#pragma warning(disable:4996)

using namespace Swing;

void KeyboardCallbackWrapper(GLFWwindow*, int, int, int, int)
{
}

//----------------------------------------------------------------------------
SEWglApplication::SEWglApplication()
{
	mOpenGLContext = wglGetCurrentContext();
	mWindowsDeviceContext = wglGetCurrentDC();
}
//----------------------------------------------------------------------------
SEWglApplication::~SEWglApplication()
{
}
//----------------------------------------------------------------------------
void SEWglApplication::Initialize(SEApplicationDescription* ApplicationDesc)
{
    if (mInitialized)
    {
        return;
    }

    // Set working directory to resource folder.
    chdir("..\\..\\Bin\\");

    mGraphicsFeature = AGF_Rasterizer;
    mGPUDevice = ApplicationDesc->GPUDevice;
	mMainCamera = new SERTGICamera;

	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(EXIT_FAILURE);
	}

	// TODO: get MSAA from device?
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Open a window and create its OpenGL context
	Window = glfwCreateWindow(Width, Height, Title.c_str(), NULL, NULL);
	if (Window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.\n");
		glfwTerminate();
		return;
	}

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(Window, (mode->width - Width) / 2, (mode->height - Height) / 2);

	HWND hWnd = GetConsoleWindow();
	MoveWindow(hWnd, (mode->width - Width) / 2 - 617, (mode->height - Height) / 2 - 29, 600, 800, TRUE);

	glfwMakeContextCurrent(Window);
	glfwSetKeyCallback(Window, KeyboardCallbackWrapper);

	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		return;
	}

	// Initialize GPU device.
	SEGPUDeviceDescription deviceDesc;
	deviceDesc.FramebufferWidth = Width;
	deviceDesc.FramebufferHeight = Height;
	mGPUDevice->Initialize(&deviceDesc);

	// Anisotropic Filtering
	GLint maxAnisFilterLevel;
	mGPUDevice->GetMaxAnisFilterLevel(&maxAnisFilterLevel);
	mGPUDevice->SetAnisFilterLevel(maxAnisFilterLevel);

    // Initialize texture manager.
    SETextureManager::Initialize();

	// some GL default values
	float color = 0.0f;
	glClearColor(color, color, color, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Unrestricted frame rates.
    glfwSwapInterval(0);

	// Call child class initialize
	this->Initialize(ApplicationDesc);
	this->mInitialized = true;
}
//----------------------------------------------------------------------------
void SEWglApplication::UpdateMainCamera()
{
	static auto lastFrame = 0.0;
	auto currentFrame = glfwGetTime();
	float deltaTime = float(currentFrame - lastFrame);
	lastFrame = currentFrame;

    static auto lastMousePos = SEVector2d(-1, -1);
	static auto mouseStartPos = SEVector2d(0, 0);
	const float mouseSpeed = 0.15f;

    // Cache camera's last frame view and projection transformations.
    mMainCamera->UpdateRenderCache();

	auto camDir = mMainCamera->GetDirection();
	auto camPos = mMainCamera->GetLocation();
	auto camSpeed = mMainCameraSpeed;
	auto horizontalAngle = 0.f;
	auto verticalAngle = 0.f;
	mMainCamera->GetAngle(horizontalAngle, verticalAngle);

	// Get mouse position
	SEVector2d newMousePos;
	glfwGetCursorPos(Window, &newMousePos.X, &newMousePos.Y);

	// Camera Orientation
	// If left click
	if (glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// proceed only if left button is just clicked
        if( lastMousePos == SEVector2d(-1, -1) )
		{
			// Hide cursor
			glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			// Just clicked, do nothing but record initial values
			mouseStartPos = newMousePos;
			lastMousePos = newMousePos;
		}
		else
		{
			// Calculate camera direction with mouse displacement
			auto h = mouseSpeed * float(newMousePos.X - mouseStartPos.X);
			auto v = mouseSpeed * float(newMousePos.Y - mouseStartPos.Y);
			auto rot = SEVector3f(-h, -v, 0);
			mMainCamera->Rotate(rot);
			// Reset mouse position for next frame
			glfwSetCursorPos(Window, mouseStartPos.X, mouseStartPos.Y);
		}
	}
	else
	{
        lastMousePos = SEVector2d(-1, -1);
		// Show the cursor
		glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	auto rightDir = mMainCamera->GetRight();

	// Camera Position
	// Move forward
	if( glfwGetKey(Window, GLFW_KEY_UP) == GLFW_PRESS ||
		glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS )
	{
		camPos += camDir * deltaTime * camSpeed;
	}
	// Move backward
	if( glfwGetKey(Window, GLFW_KEY_DOWN) == GLFW_PRESS ||
		glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS )
	{
		camPos -= camDir * deltaTime * camSpeed;
	}
	// Move up
	if (glfwGetKey(Window, GLFW_KEY_E) == GLFW_PRESS)
	{
		camPos.Y += deltaTime * camSpeed;
	}
	// Move down
	if (glfwGetKey(Window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		camPos.Y -= deltaTime * camSpeed;
	}
	// Strafe right
	if (glfwGetKey(Window, GLFW_KEY_RIGHT) == GLFW_PRESS ||
		glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camPos += rightDir * deltaTime * camSpeed;
	}
	// Strafe left
	if (glfwGetKey(Window, GLFW_KEY_LEFT) == GLFW_PRESS ||
		glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camPos -= rightDir * deltaTime * camSpeed;
	}

	mMainCamera->SetLocation(camPos);
}
//----------------------------------------------------------------------------
void SEWglApplication::ProcessInput()
{
	glfwGetWindowSize(Window, &Width, &Height);
	UpdateMainCamera();
	if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(Window, true);
	}
	this->ProcessInput();
}
//----------------------------------------------------------------------------
void SEWglApplication::Run()
{
	while (!glfwWindowShouldClose(Window))
	{
		this->SEWglApplication::ProcessInput();
		this->SEWglApplication::FrameFunc();
	}
}
//----------------------------------------------------------------------------
void SEWglApplication::FrameFunc()
{
    // Update frame status.
    ++FrameCounter;

	this->FrameFunc();

	// Swap buffers
	glfwSwapBuffers(Window);
	glfwPollEvents();
}
//----------------------------------------------------------------------------
void SEWglApplication::Terminate()
{
	this->Terminate();
	delete mMainCamera;

    // Terminate texture manager.
    SETextureManager::Terminate();

    // Terminate device.
    mGPUDevice->Terminate();

	glfwTerminate();
}
//----------------------------------------------------------------------------
void SEWglApplication::ProcessInput(int, int, int, int)
{
}
//----------------------------------------------------------------------------