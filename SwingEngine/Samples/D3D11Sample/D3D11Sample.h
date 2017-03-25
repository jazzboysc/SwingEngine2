#ifndef Swing_D3D11Sample_H
#define Swing_D3D11Sample_H

#include "SEApplicationLayer.h"
#include "SEGPUDevice.h"
#include "SEShaderProgram.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/13/2014
//----------------------------------------------------------------------------
class D3D11Sample : public SEApplicationLayer
{
public:
    D3D11Sample(SEGPUDevice* device, int width, int height);
    ~D3D11Sample();

    void Initialize();
    void Run();
    void Terminate();

    // Event handlers.
    void OnKeyboard(unsigned char key, int x, int y);
    void OnKeyboardUp(unsigned char key, int x, int y);
    void OnMouse(int button, int state, int x, int y);
    void OnMouseMove(int x, int y);
    void OnReshape(int x, int y);


private:
    int mWidth, mHeight;
    std::string mWindowTitle;
    SEGPUDevice* mGPUDevice;
};

}

#endif