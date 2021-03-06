// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_WglApplication_H
#define Swing_WglApplication_H

#include "SEWglApplicationLIB.h"
#include "SEApplication.h"

#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
struct GLFWwindow;

#include <string>

namespace Swing
{

class SE_WGL_APPLICATION_API SEWglApplication : public SEApplication
{
public:
	SEWglApplication();
	virtual ~SEWglApplication();

    // Implement base class interface.
	virtual void Initialize(SEApplicationDescription* ApplicationDesc) override;
	virtual void Run() override;
	virtual void Terminate() override;
	virtual void ProcessInput() override;
	virtual void FrameFunc() override;
    virtual void UpdateMainCamera() override;
	virtual void ProcessInput(int key, int scancode, int action, int mods) override;

	GLFWwindow* Window;

protected:

#ifdef _WIN32
	HGLRC mOpenGLContext;
	HDC mWindowsDeviceContext;
#else
#ifdef __APPLE__
	CGLContextObj mOpenGLContext;
	CGLShareGroupObj mShareGroup;
#endif
#endif
};

}

#endif