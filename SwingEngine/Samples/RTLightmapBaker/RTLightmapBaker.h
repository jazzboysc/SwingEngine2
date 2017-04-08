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

    // Override SEApplication interface.
    void OnSizing(int newWidth, int newHeight) override;
    void OnSize(int left, int top, int right, int bottom) override;
    void OnWindowClose() override;
    void OnPaint() override;
};

}