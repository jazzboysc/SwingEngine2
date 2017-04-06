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
class SEVRayWin32Window
{
public:
    SEVRayWin32Window(/*VRayRenderer& renderer*/) : finished(false), stop(), notFirst(), negative(), fullscreen(), exposure(), contrast(),
        lButtonDown(), /*img(), bmp(), renderer(renderer), movement(renderer),*/ timerID(), zoomLevel(1)
    {
        InitializeCriticalSection(&cs_bmp);
        InitializeCriticalSection(&cs_img);
    }

    ~SEVRayWin32Window()
    {
        EnterCriticalSection(&cs_img);
        LeaveCriticalSection(&cs_img);
        DeleteCriticalSection(&cs_img);
        DeleteCriticalSection(&cs_bmp);
        //delete bmp;
        //delete img;
    }

    void Create(int _width, int _height, HWND parent = NULL) 
    {
        this->width = _width ? _width : 300;
        this->height = _height ? _height : 150;

        static const TCHAR windowClass[] = TEXT("vray_window_class");
        HINSTANCE hInst = GetModuleHandle(NULL);

        WNDCLASSEX wcex;
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
        wcex.lpfnWndProc = &SEVRayWin32Window::MainWndProc;
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
        SEVRayWin32Window& me = *reinterpret_cast<SEVRayWin32Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        if( !&me ) 
        {
            if( message == WM_NCCREATE ) {
                SEVRayWin32Window* self = reinterpret_cast<SEVRayWin32Window*>(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams);
                self->hWnd = hWnd;
                SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
            }
            return DefWindowProc(hWnd, message, wParam, lParam);
        }

        switch( message ) 
        {

        case WM_PAINT: 
        {
            //static const LOGFONT logfont = {
            //    24, 0, 0, 0, FW_THIN, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, TEXT("Arial")
            //};

            //RECT rect;
            //PAINTSTRUCT ps;
            //GetClientRect(hWnd, &rect);

            //if( !me.bmp ) 
            //{
            //    ExtTextOut(BeginPaint(hWnd, &ps), 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
            //    HFONT font = CreateFontIndirect(&logfont);
            //    HFONT old_font = (HFONT)SelectObject(ps.hdc, font);
            //    SetTextColor(ps.hdc, 0x00FF00);
            //    SetTextAlign(ps.hdc, TA_TOP | TA_LEFT);
            //    SetBkMode(ps.hdc, TRANSPARENT);
            //    TextOut(ps.hdc, 20, 20, TEXT("Waiting..."), 10);
            //    DeleteObject(SelectObject(ps.hdc, old_font));
            //    EndPaint(hWnd, &ps);
            //}
            //else if( TryEnterCriticalSection(&me.cs_bmp) ) 
            //{
            //    SetDIBitsToDevice(BeginPaint(hWnd, &ps), 0, 0, rect.right, rect.bottom, 0, 0, 0, rect.bottom, me.bmp->getPixels(),
            //        reinterpret_cast<BITMAPINFO*>(static_cast<BitmapInfoHeader*>(me.bmp)), DIB_RGB_COLORS);
            //    LeaveCriticalSection(&me.cs_bmp);

            //    HFONT font = CreateFontIndirect(&logfont);
            //    HFONT old_font = (HFONT)SelectObject(ps.hdc, font);
            //    SetTextColor(ps.hdc, 0x00FF00);
            //    SetTextAlign(ps.hdc, TA_TOP | TA_LEFT);
            //    SetBkMode(ps.hdc, TRANSPARENT);
            //    TCHAR buf[32];
            //    TextOut(ps.hdc, 20, 20, buf, wsprintf(buf, TEXT("Frame number: %u"), me.n));
            //    if( me.finished ) 
            //    {
            //        SetTextColor(ps.hdc, 0x0000FF);
            //        TextOut(ps.hdc, 20, 50, TEXT("Finished Rendering."), 19);
            //    }
            //    else if( me.timerID ) 
            //    {
            //        TextOut(ps.hdc, rect.right - 180, 20, TEXT("Auto-rotating..."), 16);
            //    }
            //    DeleteObject(SelectObject(ps.hdc, old_font));
            //    EndPaint(hWnd, &ps);
            //}
        }
        break;

        case WM_KEYDOWN:
            if( wParam == VK_PAUSE ) 
            {
                //if( me.renderer.getState() == RENDERING_PAUSED ) 
                //{
                //    me.renderer.resume();
                //    puts("Pause OFF");
                //}
                //else 
                //{
                //    me.renderer.pause();
                //    puts("Pause ON");
                //}
            }
            break;

        case WM_CHAR:
            if( wParam == ' ' ) me.exposure = 0, me.contrast = 0;
            else if( wParam == '[' ) me.exposure -= 0.1f;
            else if( wParam == ']' ) me.exposure += 0.1f;
            else if( wParam == '\\' ) me.exposure = 0;
            else if( wParam == 'w' || wParam == 'W' ) me.contrast += 0.1f;
            else if( wParam == 'q' || wParam == 'Q' ) me.contrast -= 0.1f;
            else if( wParam == 'e' || wParam == 'E' ) me.contrast = 0;
            else if( wParam == 'n' || wParam == 'N' ) me.negative ^= true;
            else if( wParam == 'x' || wParam == 'X' ) 
            {
                //me.renderer.setRenderRegion(me.width / 4, me.height / 4, me.width / 2, me.height / 2);
            }
            else if( wParam == 'z' || wParam == 'Z' ) 
            {
                //float& z = me.zoomLevel;
                //const float sqrt2 = sqrtf(2);
                //z *= sqrt2;
                //if( z > 5 ) z = sqrt2 / 2;
                //float srcW = z * me.width;
                //float srcH = z * me.height;
                //me.renderer.setCropRegion(roundInt(srcW), roundInt(srcH), (srcW - me.width) / 2, (srcH - me.height) / 2);
            }
            else if( wParam == 'b' || wParam == 'B' ) 
            {
                //bool ok;
                //Box box = me.renderer.getBoundingBox(ok);
                //if( ok ) printf("%s\n", box.toString().c_str());
            }
            else if( wParam == 'f' || wParam == 'F' || wParam == 0x1B ) 
            {
                me.fullscreen ^= true;
                if( me.fullscreen ) 
                {
                    GetWindowPlacement(hWnd, &me.placement);
                    SetWindowLong(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
                    HMONITOR hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
                    MONITORINFO mi = { sizeof(mi) };

                    if( GetMonitorInfo(hmon, &mi) ) 
                    {
                        SetWindowPos(hWnd, HWND_TOPMOST, mi.rcMonitor.left, mi.rcMonitor.top,
                            mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top,
                            SWP_FRAMECHANGED);
                    }

                    //me.renderer.setImageSize(mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top);
                    //delete me.img;
                    //delete me.bmp;
                    //me.img = NULL;
                    //me.bmp = NULL;
                }
                else 
                {
                    //SetWindowLong(hWnd, GWL_STYLE, style);
                    //SetWindowPlacement(hWnd, &me.placement);
                    //int dx, dy;
                    //me.GetBorders(dx, dy);
                    //me.renderer.setImageSize(me.placement.rcNormalPosition.right - me.placement.rcNormalPosition.left - dx,
                    //    me.placement.rcNormalPosition.bottom - me.placement.rcNormalPosition.top - dy);
                }
            }
            else break;
            if( me.exposure < -6 ) me.exposure = -6;
            else if( me.exposure > 6 ) me.exposure = 6;
            if( me.contrast < -1 ) me.contrast = -1;
            else if( me.contrast > 0.9f ) me.contrast = 0.9f;
            SendMessage(hWnd, WM_SIZE, 0, 0);
            InvalidateRect(hWnd, NULL, FALSE);
            UpdateWindow(hWnd);
            break;

        case WM_KILLFOCUS:
            SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            break;

        case WM_LBUTTONDOWN:
            if( !me.timerID ) 
            {
                if( wParam & MK_SHIFT ) 
                {
                    //RECT rect;
                    //GetClientRect(hWnd, &rect);
                    //POINT current = {
                    //    MulDiv(GET_X_LPARAM(lParam), me.width, rect.right),
                    //    MulDiv(GET_Y_LPARAM(lParam), me.height, rect.bottom)
                    //};
                    ////Plugin node = me.renderer.pickPlugin(current.x, current.y);
                    ////if (node) printf("Node Name = %s\n", node.getName().c_str());
                    //vector<PluginDistancePair> plugins = me.renderer.pickPlugins(current.x, current.y);
                    //puts("");
                    //for( unsigned i = 0; i < plugins.size(); ++i ) {
                    //    printf("%u. Node Name = %s, Distance = %f\n", i + 1, plugins[i].plugin.getName(), plugins[i].distance);
                    //}
                    //puts("");
                }
                else 
                {
                    me.ctrlDown = !!(wParam & MK_CONTROL);
                    me.lButtonDown = true;
                    me.previous.x = me.cursor.x = GET_X_LPARAM(lParam);
                    me.previous.y = me.cursor.y = GET_Y_LPARAM(lParam);
                    me.moveTime.reset();
                    SetCapture(hWnd);
                }
            }
            break;

        case WM_MOUSEMOVE:
            //if( me.lButtonDown && !me.timerID ) 
            //{
            //    POINT current = {
            //        GET_X_LPARAM(lParam),
            //        GET_Y_LPARAM(lParam)
            //    };

            //    if( current.x != me.previous.x || current.y != me.previous.y ) 
            //    {
            //        RECT rect;
            //        GetClientRect(hWnd, &rect);

            //        POINT delta = {
            //            current.x - me.cursor.x,
            //            current.y - me.cursor.y
            //        };

            //        bool ctrlDown = !!(wParam & MK_CONTROL);

            //        if( me.ctrlDown != ctrlDown ) 
            //        {
            //            me.ctrlDown = ctrlDown;
            //            me.movement.resetTransform();
            //            me.cursor = current;
            //        }

            //        if( ctrlDown ) 
            //        {
            //            me.movement.move(float(-delta.x) / rect.right, float(delta.y) / rect.bottom);
            //        }
            //        else 
            //        {
            //            float theta = float(M_PI / 2)*delta.x / rect.right;
            //            me.movement.rotateHorizontal(theta);
            //            theta = float(M_PI / 2)*delta.y / rect.bottom;
            //            me.movement.rotateVertical(theta);
            //        }
            //        me.moveTime.update();
            //        if( me.moveTime.interval0() >= TIMEOUT ) 
            //        {
            //            me.movement.setTransform();
            //            KillTimer(hWnd, TIMER2_ID);
            //            me.moveTime.reset();
            //        }
            //        else 
            //        {
            //            SetTimer(hWnd, TIMER2_ID, TIMEOUT, NULL);
            //        }

            //        me.time.reset();
            //        me.previous = current;
            //    }
            //}
            break;

        case WM_LBUTTONUP:
            //if( me.lButtonDown && !me.timerID ) 
            //{
            //    me.lButtonDown = false;
            //    me.movement.resetTransform();
            //    ReleaseCapture();
            //}
            break;

        case WM_RBUTTONDOWN:
            //me.renderer.addHosts("localhost");
            break;

        case WM_MOUSEWHEEL: 
        {
            //const double amount = *((short*)(&wParam) + 1) / double(-WHEEL_DELTA);
            //const double factor = pow(ZOOM_FACTOR, amount);
            //if( wParam & (MK_CONTROL | MK_LBUTTON) )
            //    me.movement.setFov(factor);
            //else {
            //    me.movement.move(0, 0, amount);
            //    me.movement.setTransform();
            //    me.movement.resetTransform();
            //}
            //me.time.reset();
        }
        break;

        case WM_SIZE:
        {
            //RECT rect;
            //GetClientRect(hWnd, &rect);
            //EnterCriticalSection(&me.cs_img);
            //LocalVRayImage img_tmp(me.img->downscale(rect.right, rect.bottom));
            //LeaveCriticalSection(&me.cs_img);
            //if( me.negative ) img_tmp->makeNegative();
            //img_tmp->changeExposure(me.exposure);
            //img_tmp->changeContrast(me.contrast);
            //Bmp* bmp_tmp = img_tmp->getBmp();
            //EnterCriticalSection(&me.cs_bmp);
            //delete me.bmp;
            //me.bmp = bmp_tmp;
            //LeaveCriticalSection(&me.cs_bmp);
        }
        break;

        case WM_SIZING: 
        {
            //RECT& wrect = *reinterpret_cast<RECT*>(lParam);
            //int dwx = wrect.right - wrect.left;
            //int dwy = wrect.bottom - wrect.top;
            //int dx, dy;
            //me.GetBorders(dx, dy);
            //me.renderer.setImageSize(dwx - dx, dwy - dy);
        }
        break;

        case WM_LBUTTONDBLCLK:
            if( me.timerID )
            {
                KillTimer(hWnd, me.timerID);
                me.timerID = 0;
            }
            else
            {
                me.timerID = SetTimer(hWnd, 1, 1000, NULL);
            }
            InvalidateRect(hWnd, NULL, FALSE);
            break;

        case WM_TIMER:
            //if( wParam == TIMER2_ID )
            //{
            //    me.movement.setTransform();
            //    KillTimer(hWnd, TIMER2_ID);
            //}
            //else 
            //{
            //    me.movement.rotateHorizontal(float(M_PI / 8));
            //    me.movement.setTransform();
            //    me.movement.resetTransform();
            //    me.time.reset();
            //}
            break;

        case WM_CLOSE:
            me.stop = true;
            DestroyWindow(hWnd);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }

    void GetBorders(int &dx, int &dy)
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

    //void OnRenderStart(VRayRenderer& r, void*)
    //{
    //    n = 0;
    //    finished = false;
    //    //bmp = NULL;
    //    time.update();
    //    printf("\n\nRendering Started. Time Elapsed: %u ms\n\n", time.interval0());
    //}

    //void OnImageReady(VRayRenderer& r, void*)
    //{
    //    finished = true;
    //    time.update();
    //    UpdateImage(r.getImage());
    //    printf("\nImage is Ready: %u\n", ++n);
    //    printf("Time elapsed: %u ms\n", time.interval0());
    //};

    //void OnRTimageUpdated(VRayRenderer& r, VRayImage* img, void*)
    //{
    //    time.update();
    //    printf("Image Updated: %u, time elapsed: %u ms, time dif: %u\n", ++n, time.interval0(), time.interval1());

    //    UpdateImage(img);
    //}

    //void UpdateImage(VRayImage* img)
    //{
    //    if( stop ) return;

    //    int w, h;
    //    img->getSize(w, h);
    //    VRayImage* oldImage = this->img;

    //    EnterCriticalSection(&cs_img);
    //    this->img = img;
    //    width = w;
    //    height = h;
    //    LeaveCriticalSection(&cs_img);

    //    delete oldImage;

    //    if( n == 1 && !notFirst )
    //    {
    //        notFirst = true;
    //        Resize();
    //    }
    //    SendMessage(hWnd, WM_SIZE, 0, 0);
    //    InvalidateRect(hWnd, NULL, FALSE);
    //}

    //static void OnRendererClose(VRayRenderer& r, void*)
    //{
    //    puts("\nRenderer closed.");
    //};

    //static void OnDumpMessage(VRayRenderer& r, const char* msg, int level, void*)
    //{
    //    printf("%s (%u)\n", msg, level);
    //}

private:
    unsigned n;
    bool finished : 1;
    bool stop : 1;
    bool notFirst : 1;
    bool lButtonDown : 1;
    bool ctrlDown : 1;
    bool negative : 1;
    bool fullscreen : 1;
    float exposure;
    float contrast;
    HWND hWnd;
    //VRayImage *img;
    //Bmp *bmp;
    int width, height;
    WINDOWPLACEMENT placement;
    CRITICAL_SECTION cs_bmp, cs_img;
    TimeInterval time;
    TimeInterval moveTime;
    //VRayRenderer& renderer;
    //Movement movement;
    POINT cursor, previous;
    UINT_PTR timerID;
    float zoomLevel;
    static const DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX;
};
//----------------------------------------------------------------------------


SEVRayWin32Window* gVRayWin32Window = nullptr;
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

    mGraphicsFeature = AGF_RayTracer;
    mGPUDevice = nullptr;
    mRayTracingDevice = ApplicationDesc->RayTracingDevice;
    
    SERayTracingDeviceDescription deviceDesc;
    deviceDesc.RenderMode = RTDRM_RT_GPU_CUDA;
    deviceDesc.ImageWidth = Width;
    deviceDesc.ImageHeight = Height;
    mRayTracingDevice->Initialize(&deviceDesc);

    // Create rendering window.
    gVRayWin32Window = new SEVRayWin32Window();
    gVRayWin32Window->Create(Width, Height, GetConsoleWindow());

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

    delete gVRayWin32Window;
    gVRayWin32Window = nullptr;

	this->Terminate();
	SE_DELETE mMainCamera;
}
//----------------------------------------------------------------------------
void SEVRayWin32Application::ProcessInput(int, int, int, int)
{
}
//----------------------------------------------------------------------------