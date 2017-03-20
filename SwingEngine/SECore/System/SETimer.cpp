// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017
//
// This part of Swing Engine is based on PBRT.
/*
    pbrt source code Copyright(c) 1998-2012 Matt Pharr and Greg Humphreys.

    This file is part of pbrt.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */

#include "SECorePCH.h"
#include "SETimer.h"

#if defined (SE_IS_WINDOWS)
#include <windows.h>
#else
#include <sys/time.h>
#endif

#if defined(SE_IS_WINDOWS)
class TimerType
{
public:
    LARGE_INTEGER PerformanceCounter;
    LARGE_INTEGER PerformanceFrequency;
    double OneOverFrequency;
};
#else
class TimerType
{
public:
    struct timeval Timeofday;
};
#endif

using namespace Swing;

//----------------------------------------------------------------------------
SETimer::SETimer()
{
    m_pTimerType = SE_NEW TimerType;

#if defined( SE_IS_WINDOWS )
    // Windows Timer Initialization.
    TimerType* pTimerType = (TimerType*)m_pTimerType;
    QueryPerformanceFrequency(&pTimerType->PerformanceFrequency);
    pTimerType->OneOverFrequency = 
        1.0 / ((double)pTimerType->PerformanceFrequency.QuadPart);
#endif

    m_dTime0 = m_dElapsed = 0.0;
    m_bRunning = false;
}
//----------------------------------------------------------------------------
SETimer::~SETimer()
{
    SE_DELETE m_pTimerType;
}
//----------------------------------------------------------------------------
double SETimer::GetTime()
{
#if defined( SE_IS_WINDOWS )
    // Windows GetTime.
    TimerType* pTimerType = (TimerType*)m_pTimerType;
    QueryPerformanceCounter(&pTimerType->PerformanceCounter);
    return (double) pTimerType->PerformanceCounter.QuadPart * 
        pTimerType->OneOverFrequency;
#else
    // UNIX GetTime.
    TimerType* pTimerType = (TimerType*)m_pTimerType;
    gettimeofday(&pTimerType->Timeofday, 0);
    return pTimerType->Timeofday.tv_sec + 
        pTimerType->Timeofday.tv_usec / 1000000.0;
#endif
}
//----------------------------------------------------------------------------
void SETimer::Start()
{
    SE_ASSERT( !m_bRunning );

    m_bRunning = true;
    m_dTime0 = GetTime();
}
//----------------------------------------------------------------------------
void SETimer::Stop()
{
    SE_ASSERT( m_bRunning );

    m_bRunning = false;
    m_dElapsed += GetTime() - m_dTime0;
}
//----------------------------------------------------------------------------
void SETimer::Reset()
{
    m_bRunning = false;
    m_dElapsed = 0;
}
//----------------------------------------------------------------------------
double SETimer::GetTimeElapsed()
{
    if( m_bRunning )
    {
        Stop();
        Start();
    }

    return m_dElapsed;
}
//----------------------------------------------------------------------------