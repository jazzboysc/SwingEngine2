#pragma once

#include "SEVRayWin32Application.h"

namespace Swing
{

class RTLightmapBaker : public SEVRayWin32Application
{
public:
    RTLightmapBaker(int width = 1024, int height = 768);
    ~RTLightmapBaker();

    // Override SEVRayWin32Application interface.
	void Initialize(SEApplicationDescription* ApplicationDesc) override;
	void ProcessInput() override;
	void FrameFunc() override;
	void Terminate() override;


};

}