// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_D3D12Application_H
#define Swing_D3D12Application_H

#include "SED3D12ApplicationLIB.h"
#include "SEApplication.h"

#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <string>

namespace Swing
{

class SE_D3D12_APPLICATION_API SED3D12Application : public SEApplication
{
public:
    SED3D12Application(HINSTANCE instance, HINSTANCE prevInstance,
        PSTR cmdLine, int showCmd);
	virtual ~SED3D12Application();

    // Implement base class interface.
	virtual void Initialize(SEApplicationDescription* ApplicationDesc) override;
	virtual void Run() override;
	virtual void Terminate() override;
	virtual void ProcessInput() override;
	virtual void FrameFunc() override;
    virtual void UpdateMainCamera() override;
	virtual void ProcessInput(int key, int scancode, int action, int mods) override;

protected:
    HINSTANCE mAppInstance;
    HWND mhMainWnd;
};

}

#endif