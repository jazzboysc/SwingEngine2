//----------------------------------------------------------------------------
// Graphics framework for real-time GI study.
// Che Sun at Worcester Polytechnic Institute, Fall 2013.
//----------------------------------------------------------------------------

#include "SERenderingEnginePCH.h"
#include "SEGPUTimer.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEGPUTimer::SEGPUTimer()
    :
    mTimerHandle(0)
{
    mStarted = false;
}
//----------------------------------------------------------------------------
SEGPUTimer::~SEGPUTimer()
{
    if( mTimerHandle )
    {
        mTimerHandle->Device->DeleteTimer(this);
        SE_DELETE mTimerHandle;
        mTimerHandle = 0;
    }
}
//----------------------------------------------------------------------------
void SEGPUTimer::CreateDeviceResource(SEGPUDevice* device)
{
    if( !mTimerHandle )
    {
        mTimerHandle = device->CreateTimer(this);
    }
}
//----------------------------------------------------------------------------
void SEGPUTimer::Start()
{
    if( mTimerHandle )
    {
        mTimerHandle->Device->TimerStart(this);
        mStarted = true;
    }
}
//----------------------------------------------------------------------------
void SEGPUTimer::Stop()
{
    if( mTimerHandle )
    {
        mTimerHandle->Device->TimerStop(this);
    }
}
//----------------------------------------------------------------------------
double SEGPUTimer::GetTimeElapsed()
{
    if( !mStarted )
    {
        return 0.0;
    }

    return mTimerHandle->Device->TimerGetTimeElapsed(this);
}
//----------------------------------------------------------------------------