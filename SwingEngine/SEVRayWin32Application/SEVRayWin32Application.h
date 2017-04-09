// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_VRayWin32Application_H
#define Swing_VRayWin32Application_H

#include "SEVRayWin32ApplicationLIB.h"
#include "SEApplication.h"

#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <Windowsx.h>
#include <Commdlg.h>
#include <Commctrl.h>

#define _USE_MATH_DEFINES
#include <cmath>

#include <string>

namespace Swing
{

class SE_VRAY_WIN32_APPLICATION_API SEVRayWin32Application : public SEApplication
{
public:
	SEVRayWin32Application();
	virtual ~SEVRayWin32Application();

    // Implement base class interface.
	virtual void Initialize(SEApplicationDescription* ApplicationDesc) override;
	virtual void Run() override;
	virtual void Terminate() override;
	virtual void ProcessInput() override;
	virtual void FrameFunc() override;
    virtual void UpdateMainCamera() override;
	virtual void ProcessInput(int key, int scancode, int action, int mods) override;
    virtual const char* OpenFileDialog(const char* filter, const char* title) override;
    virtual void Refresh(bool updateWindowSize = true) override;
};

}

#endif