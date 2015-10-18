// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_Application_H
#define Swing_Application_H

#include "SEApplicationLIB.h"
#include "SEGPUDevice.h"

// TODO split window specific impl into windowsApplication class
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
struct GLFWwindow;

#include <string>

namespace Swing
{
//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 09/14/2013
//----------------------------------------------------------------------------
class SERTGICamera;
class SE_APPLICATION_API SEApplication
{
public:
	SEApplication();
	virtual ~SEApplication();

	virtual void Initialize(SEGPUDevice* device);
	virtual void Run();
	virtual void Terminate();
	virtual void ProcessInput();
	virtual void FrameFunc();
	void UpdateMainCamera();

	virtual void ProcessInput(int key, int scancode, int action, int mods);
	static void KeyboardCallbackWrapper(
		GLFWwindow* window, int key, int scancode, int action, int mods);

	static SEApplication* GetInstance();

	GLFWwindow* Window;
	int Width = 1024, Height = 768;
	std::string Title = "Default Application";
    unsigned int FrameCounter;

protected:
	SEGPUDevicePtr mDevice;
	SERTGICamera* mMainCamera;
	float mMainCameraSpeed;
	bool mInitialized = false;

#ifdef _WIN32
	HGLRC mOpenGLContext;
	HDC mWindowsDeviceContext;
#else
#ifdef __APPLE__
	CGLContextObj mOpenGLContext;
	CGLShareGroupObj mShareGroup;
#endif
#endif

	static SEApplication* mInstance;
};

}

#endif