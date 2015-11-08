#include "D3D12Sample.h"

using namespace Swing;

//----------------------------------------------------------------------------
D3D12Sample::D3D12Sample(HINSTANCE instance, HINSTANCE prevInstance,
    PSTR cmdLine, int showCmd, int width, int height)
    :
    SED3D12Application(instance, prevInstance, cmdLine, showCmd)
{
    Width = width;
    Height = height;
    Title = "D3D12 Sample";
}
//----------------------------------------------------------------------------
D3D12Sample::~D3D12Sample()
{
}
//----------------------------------------------------------------------------
void D3D12Sample::Initialize(SEGPUDevice*)
{
}
//----------------------------------------------------------------------------
void D3D12Sample::ProcessInput()
{
}
//----------------------------------------------------------------------------
void D3D12Sample::FrameFunc()
{
}
//----------------------------------------------------------------------------
void D3D12Sample::Terminate()
{
}
//----------------------------------------------------------------------------