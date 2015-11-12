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
void D3D12Sample::Initialize(SEGPUDeviceBase* device)
{
    SEVertexShader* vShader = SE_NEW SEVertexShader("D3D12Sample/vTest.hlsl");
    vShader->CreateDeviceResource(device);
    SEFragmentShader* fShader = SE_NEW SEFragmentShader("D3D12Sample/fTest.hlsl");
    fShader->CreateDeviceResource(device);
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