#include "RTLightmapBaker.h"

using namespace Swing;

//----------------------------------------------------------------------------
RTLightmapBaker::RTLightmapBaker(int width, int height)
{
    Width = width;
    Height = height;
    Title = "V-Ray Lightmap Baker";
}
//----------------------------------------------------------------------------
RTLightmapBaker::~RTLightmapBaker()
{
}
//----------------------------------------------------------------------------
void RTLightmapBaker::Initialize(SEApplicationDescription* ApplicationDesc)
{
    // Setup renderer delegate.
    mRayTracingDevice->SetOnRenderStart<RTLightmapBaker, &RTLightmapBaker::OnRenderStart>(*this, nullptr);
    mRayTracingDevice->SetOnImageReady<RTLightmapBaker, &RTLightmapBaker::OnImageReady>(*this, nullptr);
    mRayTracingDevice->SetOnRTImageUpdated<RTLightmapBaker, &RTLightmapBaker::OnRTImageUpdated>(*this, nullptr);
    mRayTracingDevice->SetOnDeviceClose<RTLightmapBaker, &RTLightmapBaker::OnDeviceClose>(*this, nullptr);
    mRayTracingDevice->SetOnDumpMessage<RTLightmapBaker, &RTLightmapBaker::OnDumpMessage>(*this, nullptr);

    const char* sceneFileName = OpenFileDialog("V-Ray scene files (*.vrscene)\0*.vrscene\0All files (*.*)\0*.*\0", "Choose V-Ray scene file to load");
    if( !sceneFileName )
    {
        puts("Scene file not loaded.");
        return;
    }
}
//----------------------------------------------------------------------------
void RTLightmapBaker::FrameFunc()
{
}
//----------------------------------------------------------------------------
void RTLightmapBaker::Terminate()
{
}
//----------------------------------------------------------------------------
void RTLightmapBaker::ProcessInput()
{
}
//----------------------------------------------------------------------------
void RTLightmapBaker::OnSizing(int newWidth, int newHeight)
{
}
//----------------------------------------------------------------------------
void RTLightmapBaker::OnSize(int left, int top, int right, int bottom)
{
}
//----------------------------------------------------------------------------
void RTLightmapBaker::OnWindowClose()
{
}
//----------------------------------------------------------------------------
void RTLightmapBaker::OnPaint()
{
}
//----------------------------------------------------------------------------
void RTLightmapBaker::OnRenderStart(SERayTracingDevice& rtDevice, void*)
{
    mFrameNumber = 0;
    mFinished = false;
    mBmp = nullptr;
    printf("\n Rendering started. \n");
}
//----------------------------------------------------------------------------
void RTLightmapBaker::OnImageReady(SERayTracingDevice& rtDevice, void*)
{
    mFinished = true;
    UpdateImage(nullptr);
    printf("\n Image is ready: %u \n", ++mFrameNumber);
}
//----------------------------------------------------------------------------
void RTLightmapBaker::OnDeviceClose(SERayTracingDevice& rtDevice, void*)
{
    puts("\n Renderer closed.");
}
//----------------------------------------------------------------------------
void RTLightmapBaker::OnRTImageUpdated(SERayTracingDevice& rtDevice, SERayTracingDeviceImage* img, void*)
{
    printf("Image updated: %u \n", ++mFrameNumber);

    UpdateImage(img);
}
//----------------------------------------------------------------------------
void RTLightmapBaker::OnDumpMessage(SERayTracingDevice& rtDevice, const char* msg, int level, void*)
{
    printf("%s (%u)\n", msg, level);
}
//----------------------------------------------------------------------------
void RTLightmapBaker::UpdateImage(SERayTracingDeviceImage* image)
{
}
//----------------------------------------------------------------------------