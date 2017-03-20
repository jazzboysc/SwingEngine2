// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_Application_H
#define Swing_Application_H

#include "SEApplicationLIB.h"
#include "SEGPUDeviceBase.h"

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

	virtual void Initialize(SEGPUDeviceBase* device) = 0;
	virtual void Run() = 0;
	virtual void Terminate() = 0;
	virtual void ProcessInput() = 0;
	virtual void FrameFunc() = 0;
    virtual void UpdateMainCamera() = 0;
	virtual void ProcessInput(int key, int scancode, int action, int mods) = 0;

	static SEApplication* GetInstance();

	int Width = 1024, Height = 768;
	std::string Title = "Default Application";
    unsigned int FrameCounter;

protected:
    SEGPUDeviceBasePtr mDevice;
	SERTGICamera* mMainCamera;
	float mMainCameraSpeed;
	bool mInitialized = false;

	static SEApplication* mInstance;
};

}

#endif