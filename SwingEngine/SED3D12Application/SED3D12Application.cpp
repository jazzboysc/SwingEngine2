// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SED3D12ApplicationPCH.h"
#include "SED3D12Application.h"
#include "SED3D12Device.h"
#include "SERTGICamera.h"
#include "SETextureManager.h"

#include <direct.h>

#pragma warning(disable:4996)

using namespace Swing;

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch( msg )
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_SIZE:
        //if( app )
        //{
        //    app->OnReshape(LOWORD(lParam), HIWORD(lParam));
        //    return 0;
        //}
        break;

    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
        //if( app )
        //{
        //    app->OnMouse(wParam, 0, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        //    return 0;
        //}
        break;

    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
        //if( app )
        //{
        //    app->OnMouse(wParam, 1, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        //    return 0;
        //}
        break;

    case WM_MOUSEMOVE:
        //if( app )
        //{
        //    app->OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        //    return 0;
        //}
        break;

    default:
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

//----------------------------------------------------------------------------
SED3D12Application::SED3D12Application(HINSTANCE instance, HINSTANCE, PSTR, 
    int)
    :
    mAppInstance(instance),
    mhMainWnd(0)
{
}
//----------------------------------------------------------------------------
SED3D12Application::~SED3D12Application()
{
    // TODO:
}
//----------------------------------------------------------------------------
void SED3D12Application::Initialize(SEGPUDeviceBase* device)
{
    // Set working directory to resource folder.
    chdir("..\\..\\Bin\\");

    mDevice = device;
    mMainCamera = new SERTGICamera();

    // Create app window.

    WNDCLASS wc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = MainWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = mAppInstance;
    wc.hIcon = LoadIcon(0, IDI_APPLICATION);
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = "D3DWndClassName";

    if( !RegisterClass(&wc) )
    {
        MessageBox(0, "RegisterClass Failed.", 0, 0);
        return;
    }

    // Compute window rectangle dimensions based on requested client area dimensions.
    RECT R = { 0, 0, Width, Height };
    AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
    int width = R.right - R.left;
    int height = R.bottom - R.top;

    mhMainWnd = CreateWindow("D3DWndClassName", Title.c_str(), 
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 
        0, mAppInstance, 0);
    if( !mhMainWnd )
    {
        MessageBox(0, "CreateWindow Failed.", 0, 0);
        return;
    }

    ShowWindow(mhMainWnd, SW_SHOW);
    UpdateWindow(mhMainWnd);

    // Initialize GPU device.
    SEGPUDeviceDescription deviceDesc;
    deviceDesc.FramebufferWidth = Width;
    deviceDesc.FramebufferHeight = Height;
    ((SED3D12Device*)(SEGPUDeviceBase*)mDevice)->SetMainWindow(mhMainWnd);
    mDevice->Initialize(&deviceDesc);

    // Anisotropic Filtering
    int maxAnisFilterLevel;
    mDevice->GetMaxAnisFilterLevel(&maxAnisFilterLevel);
    mDevice->SetAnisFilterLevel(maxAnisFilterLevel);

    // Initialize texture manager.
    SETextureManager::Initialize();

    // Call child class initialize
    this->Initialize(device);
    this->mInitialized = true;
}
//----------------------------------------------------------------------------
void SED3D12Application::UpdateMainCamera()
{
    // TODO:
}
//----------------------------------------------------------------------------
void SED3D12Application::ProcessInput()
{
    // TODO:
}
//----------------------------------------------------------------------------
void SED3D12Application::Run()
{
    MSG msg = { 0 };
    while( msg.message != WM_QUIT )
    {
        if( PeekMessage(&msg, 0, 0, 0, PM_REMOVE) )
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            this->SED3D12Application::ProcessInput();
            this->SED3D12Application::FrameFunc();
        }
    }
}
//----------------------------------------------------------------------------
void SED3D12Application::FrameFunc()
{
    // Update frame status.
    ++FrameCounter;

    this->FrameFunc();
}
//----------------------------------------------------------------------------
void SED3D12Application::Terminate()
{
    this->Terminate();
    delete mMainCamera;

    // Terminate texture manager.
    SETextureManager::Terminate();

    // Terminate device.
    mDevice->Terminate();
}
//----------------------------------------------------------------------------
void SED3D12Application::ProcessInput(int, int, int, int)
{
    // TODO:
}
//----------------------------------------------------------------------------