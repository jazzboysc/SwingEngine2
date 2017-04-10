// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SEVRayWin32ApplicationPCH.h"
#include "SEVRayWin32Application.h"
#include "SERTGICamera.h"
#include "SETextureManager.h"
#include "SEVector2.h"
#include "SEVector3.h"

#pragma warning(disable:4996)
#pragma warning(disable:4302)

using namespace Swing;

//----------------------------------------------------------------------------
inline int roundInt(float number) {
    return _mm_cvtss_si32(_mm_set_ss(number));
}
//----------------------------------------------------------------------------
inline int roundInt(double number) {
    return _mm_cvtsd_si32(_mm_set_sd(number));
}
//----------------------------------------------------------------------------

const UINT_PTR TIMER2_ID = 2;
const UINT TIMEOUT = 100;

//----------------------------------------------------------------------------
class TimeInterval {
    unsigned long oldTime;
    unsigned long newTime;
    LARGE_INTEGER frequency;
    LARGE_INTEGER counter0;

public:
    TimeInterval() : oldTime(), newTime() {
        QueryPerformanceFrequency(&frequency);
        reset();
    }

    // Resets the internal initial time
    void reset() {
        QueryPerformanceCounter(&counter0);
        newTime = 0;
    }

    void update() {
        LARGE_INTEGER counter1;
        QueryPerformanceCounter(&counter1);
        oldTime = newTime;
        newTime = roundInt(((counter1.QuadPart - counter0.QuadPart)*1000.0) / frequency.QuadPart);
    }

    // Returns the elapsed time between last call to update() and object initialization or last call to reset()  (in miliseconds)
    inline unsigned long interval0() const {
        return newTime;
    }

    // Returns the elapsed time between last and previos call to update()  (in miliseconds)
    inline unsigned long interval1() const {
        return newTime - oldTime;
    }
};
//----------------------------------------------------------------------------
class SEWin32FileDialogHelper : private OPENFILENAME
{
public:
    const char* GetName(HWND hWndOwner = NULL, const char* filter = nullptr, const char* title = nullptr)
    {
        *mFileName = 0;
        lStructSize = sizeof(OPENFILENAME);
        hwndOwner = hWndOwner;
        hInstance = GetModuleHandle(NULL);;
        lpstrFilter = filter;
        lpstrCustomFilter = NULL;
        nMaxCustFilter = 0;
        nFilterIndex = 1;
        lpstrFile = mFileName;
        nMaxFile = MAX_PATH;
        lpstrFileTitle = NULL;
        nMaxFileTitle = 0;
        lpstrInitialDir = NULL;
        lpstrTitle = title;
        Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_LONGNAMES | OFN_EXPLORER | OFN_HIDEREADONLY | OFN_ENABLESIZING;
        lpstrDefExt = NULL;
        lCustData = NULL;
        lpfnHook = NULL;
        lpTemplateName = NULL;
        pvReserved = NULL;
        dwReserved = 0;
        FlagsEx = 0;

        if( GetOpenFileName(this) )
        {
            return mFileName;
        }
        else
        {
            return nullptr;
        }
    }

private:
    char mFileName[MAX_PATH * 2];
};
//----------------------------------------------------------------------------
class SEVRayWin32WindowHelper
{
public:
    SEVRayWin32WindowHelper() : finished(false), notFirst(), negative(), fullscreen(), exposure(), contrast(),
        lButtonDown(), timerID(), zoomLevel(1)
    {
        InitializeCriticalSection(&cs_bmp);
        InitializeCriticalSection(&cs_img);
    }

    ~SEVRayWin32WindowHelper()
    {
        EnterCriticalSection(&cs_img);
        LeaveCriticalSection(&cs_img);
        DeleteCriticalSection(&cs_img);
        DeleteCriticalSection(&cs_bmp);
    }

    void CreateMainWindow(int _width, int _height, HWND parent = NULL) 
    {
        this->width = _width ? _width : 300;
        this->height = _height ? _height : 150;

        static const TCHAR windowClass[] = TEXT("vray_window_class");
        HINSTANCE hInst = GetModuleHandle(NULL);

        WNDCLASSEX wcex;
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
        wcex.lpfnWndProc = &SEVRayWin32WindowHelper::MainWndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInst;
        wcex.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_ASTERISK));
        wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
        wcex.hbrBackground = 0;
        wcex.lpszMenuName = NULL;
        wcex.lpszClassName = windowClass;
        wcex.hIconSm = NULL;

        RegisterClassEx(&wcex);

        hWnd = CreateWindow(windowClass, TEXT("VRay RT Renderer"), style,
            CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, parent, NULL, hInst, this);

        if( hWnd ) 
        {
            Resize();
            ShowWindow(hWnd, SW_SHOW);
            UpdateWindow(hWnd);
        }
    }

    static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
    {
        SEVRayWin32WindowHelper& me = *reinterpret_cast<SEVRayWin32WindowHelper*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        if( !&me ) 
        {
            if( message == WM_NCCREATE ) {
                SEVRayWin32WindowHelper* self = reinterpret_cast<SEVRayWin32WindowHelper*>(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams);
                self->hWnd = hWnd;
                SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
            }
            return DefWindowProc(hWnd, message, wParam, lParam);
        }

        switch( message ) 
        {

        case WM_PAINT: 
        {
            RECT rect;
            PAINTSTRUCT ps;
            GetClientRect(hWnd, &rect);

            SEApplication::GetInstance()->OnPaint();
        }
        break;

        case WM_SIZE:
        {
            RECT rect;
            GetClientRect(hWnd, &rect);
            SEApplication::GetInstance()->OnSize(rect.left, rect.top, rect.right, rect.bottom);
        }
        break;

        case WM_SIZING: 
        {
            RECT& wrect = *reinterpret_cast<RECT*>(lParam);
            int dwx = wrect.right - wrect.left;
            int dwy = wrect.bottom - wrect.top;
            int dx, dy;
            me.GetBorders(dx, dy);
            SEApplication::GetInstance()->OnSizing(dwx - dx, dwy - dy);
        }
        break;

        case WM_CLOSE:
            SEApplication::GetInstance()->OnWindowClose();
            DestroyWindow(hWnd);
            break;

        case WM_KILLFOCUS:
            SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }

    void GetBorders(int& dx, int& dy)
    {
        RECT wrect, crect;
        GetWindowRect(hWnd, &wrect);
        GetClientRect(hWnd, &crect);
        int dwx = wrect.right - wrect.left;
        int dcx = crect.right - crect.left;
        int dwy = wrect.bottom - wrect.top;
        int dcy = crect.bottom - crect.top;
        dx = dwx - dcx;
        dy = dwy - dcy;
    }

    void Resize()
    {
        int dx, dy;
        GetBorders(dx, dy);
        SetWindowPos(hWnd, NULL, 0, 0, width + dx, height + dy, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
    }

    HWND GetWindowHandle()
    {
        return hWnd;
    }

private:
    unsigned n;
    bool finished : 1;
    bool notFirst : 1;
    bool lButtonDown : 1;
    bool ctrlDown : 1;
    bool negative : 1;
    bool fullscreen : 1;
    float exposure;
    float contrast;
    HWND hWnd;
    int width, height;
    WINDOWPLACEMENT placement;
    CRITICAL_SECTION cs_bmp, cs_img;
    TimeInterval time;
    TimeInterval moveTime;
    POINT cursor, previous;
    UINT_PTR timerID;
    float zoomLevel;
    static const DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX;
};
//----------------------------------------------------------------------------


SEVRayWin32WindowHelper* gVRayWin32WindowHelper = nullptr;
SEWin32FileDialogHelper* gWin32FileDialogHelper = nullptr;
//----------------------------------------------------------------------------
SEVRayWin32Application::SEVRayWin32Application()
{
}
//----------------------------------------------------------------------------
SEVRayWin32Application::~SEVRayWin32Application()
{
}
//----------------------------------------------------------------------------
void SEVRayWin32Application::Initialize(SEApplicationDescription* ApplicationDesc)
{
    if (mInitialized)
    {
        return;
    }

    // Set working directory to resource folder.
    //chdir("..\\..\\Bin\\");

    // Win32 common control initialization.
    InitCommonControls();

    // Create file dialog helper.
    gWin32FileDialogHelper = new SEWin32FileDialogHelper();

    mGraphicsFeature = AGF_RayTracer;
    mGPUDevice = nullptr;
    mRayTracingDevice = ApplicationDesc->RayTracingDevice;
    
    SERayTracingDeviceDescription deviceDesc;
    deviceDesc.RenderMode = RTDRM_RT_GPU_CUDA;
    deviceDesc.ImageWidth = Width;
    deviceDesc.ImageHeight = Height;
    mRayTracingDevice->Initialize(&deviceDesc);

    // Create rendering window.
    gVRayWin32WindowHelper = new SEVRayWin32WindowHelper();
    gVRayWin32WindowHelper->CreateMainWindow(Width, Height, GetConsoleWindow());

	mMainCamera = SE_NEW SERTGICamera;

	// Call child class initialize
	this->Initialize(ApplicationDesc);
	this->mInitialized = true;
}
//----------------------------------------------------------------------------
void SEVRayWin32Application::UpdateMainCamera()
{
}
//----------------------------------------------------------------------------
void SEVRayWin32Application::ProcessInput()
{
	this->ProcessInput();
}
//----------------------------------------------------------------------------
void SEVRayWin32Application::Run()
{
    MSG msg = { 0 };
    // Main message loop:
    while( (unsigned)(GetMessage(&msg, NULL, 0, 0) + 1) > 1 )
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
//----------------------------------------------------------------------------
void SEVRayWin32Application::FrameFunc()
{
    // Update frame status.
    ++FrameCounter;

	this->FrameFunc();
}
//----------------------------------------------------------------------------
void SEVRayWin32Application::Terminate()
{
    mRayTracingDevice = nullptr;

    delete gWin32FileDialogHelper;
    gWin32FileDialogHelper = nullptr;

    delete gVRayWin32WindowHelper;
    gVRayWin32WindowHelper = nullptr;

	this->Terminate();
	SE_DELETE mMainCamera;
}
//----------------------------------------------------------------------------
void SEVRayWin32Application::ProcessInput(int, int, int, int)
{
}
//----------------------------------------------------------------------------
const char* SEVRayWin32Application::OpenFileDialog(const char* filter, const char* title)
{
    if( gWin32FileDialogHelper )
    {
        HWND parentWindow = GetConsoleWindow();
        if( gVRayWin32WindowHelper )
        {
            parentWindow = gVRayWin32WindowHelper->GetWindowHandle();
        }
        return gWin32FileDialogHelper->GetName(parentWindow, filter, title);
    }

    return nullptr;
}
//----------------------------------------------------------------------------
void SEVRayWin32Application::Refresh(bool updateWindowSize)
{
    HWND handle = gVRayWin32WindowHelper->GetWindowHandle();
    if( updateWindowSize )
    {
        SendMessage(handle, WM_SIZE, 0, 0);
    }
    InvalidateRect(handle, NULL, FALSE);
}
//----------------------------------------------------------------------------