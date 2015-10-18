//----------------------------------------------------------------------------
// Graphics framework for real-time GI study.
// Che Sun at Worcester Polytechnic Institute, Fall 2013.
//----------------------------------------------------------------------------

#ifndef Swing_GPUTimer_H
#define Swing_GPUTimer_H

#include "SERenderingEngineLIB.h"
#include "SEGPUDeviceResident.h"
#include "SEGPUResource.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 10/27/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEGPUTimer : public SEGPUDeviceResident
{
public:
    SEGPUTimer();
    ~SEGPUTimer();

    void CreateDeviceResource(SEGPUDevice* device);
    void Start();
    void Stop();
    double GetTimeElapsed();

    SEGPUTimerHandle* GetTimerHandle(){ return mTimerHandle; };

private:
    SEGPUTimerHandle* mTimerHandle;
    bool mStarted;
};

typedef SESmartPointer<SEGPUTimer> SEGPUTimerPtr;

}

#endif