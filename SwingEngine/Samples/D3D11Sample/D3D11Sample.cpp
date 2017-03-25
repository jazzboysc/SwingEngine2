#include "D3D11Sample.h"

using namespace Swing;

//----------------------------------------------------------------------------
D3D11Sample::D3D11Sample(SEGPUDevice* device, int width, int height)
    :
    mWidth(width),
    mHeight(height),
    mWindowTitle("D3D11 Sample"),
    mGPUDevice(device)
{
}
//----------------------------------------------------------------------------
D3D11Sample::~D3D11Sample()
{
}
//----------------------------------------------------------------------------
void D3D11Sample::Initialize()
{
    SEVertexShader* vShader = SE_NEW SEVertexShader("test.hlsl");
    vShader->CreateDeviceResource(mGPUDevice);
}
//----------------------------------------------------------------------------
void D3D11Sample::Run()
{
}
//----------------------------------------------------------------------------
void D3D11Sample::Terminate()
{
}
//----------------------------------------------------------------------------
void D3D11Sample::OnKeyboard(unsigned char, int, int)
{
}
//----------------------------------------------------------------------------
void D3D11Sample::OnKeyboardUp(unsigned char, int, int)
{
}
//----------------------------------------------------------------------------
void D3D11Sample::OnMouse(int, int, int, int)
{
}
//----------------------------------------------------------------------------
void D3D11Sample::OnMouseMove(int, int)
{
}
//----------------------------------------------------------------------------
void D3D11Sample::OnReshape(int, int)
{
}
//----------------------------------------------------------------------------