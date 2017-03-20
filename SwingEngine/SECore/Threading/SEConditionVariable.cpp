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
#include "SEConditionVariable.h"
#include "SEAssert.h"

using namespace Swing;

#if defined(SE_IS_WINDOWS)
//----------------------------------------------------------------------------
#include <windows.h>
//----------------------------------------------------------------------------
SEConditionVariable::SEConditionVariable()
{
    m_ConditionVariableType.WaitersCount = 0;

    m_ConditionVariableType.pCriticalSectionWaitersCountMutex = 
        (void*)SE_NEW CRITICAL_SECTION;
    InitializeCriticalSection((LPCRITICAL_SECTION)
        m_ConditionVariableType.pCriticalSectionWaitersCountMutex);

    m_ConditionVariableType.pCriticalSectionConditionMutex =
        (void*)SE_NEW CRITICAL_SECTION;
    InitializeCriticalSection((LPCRITICAL_SECTION)
        m_ConditionVariableType.pCriticalSectionConditionMutex);

    m_ConditionVariableType.hEvents[SEConditionVariableType::SIGNAL] = 
        (HANDLE)CreateEvent(NULL,  // no security
                            FALSE, // auto-reset event
                            FALSE, // non-signaled initially
                            NULL); // unnamed

    m_ConditionVariableType.hEvents[SEConditionVariableType::BROADCAST] = 
        (HANDLE)CreateEvent(NULL,  // no security
                            TRUE,  // manual-reset
                            FALSE, // non-signaled initially
                            NULL); // unnamed

}
//----------------------------------------------------------------------------
SEConditionVariable::~SEConditionVariable()
{
    CloseHandle((HANDLE)
        m_ConditionVariableType.hEvents[SEConditionVariableType::SIGNAL]);

    CloseHandle((HANDLE)
        m_ConditionVariableType.hEvents[SEConditionVariableType::BROADCAST]);

    DeleteCriticalSection((LPCRITICAL_SECTION)
        m_ConditionVariableType.pCriticalSectionWaitersCountMutex);
    SE_DELETE m_ConditionVariableType.pCriticalSectionWaitersCountMutex;
    m_ConditionVariableType.pCriticalSectionWaitersCountMutex = 0;

    DeleteCriticalSection((LPCRITICAL_SECTION)
        m_ConditionVariableType.pCriticalSectionConditionMutex);
    SE_DELETE m_ConditionVariableType.pCriticalSectionConditionMutex;
    m_ConditionVariableType.pCriticalSectionConditionMutex = 0;
}
//----------------------------------------------------------------------------
void SEConditionVariable::Lock()
{
    EnterCriticalSection((LPCRITICAL_SECTION)
        m_ConditionVariableType.pCriticalSectionConditionMutex);
}
//----------------------------------------------------------------------------
void SEConditionVariable::Unlock()
{
    LeaveCriticalSection((LPCRITICAL_SECTION)
        m_ConditionVariableType.pCriticalSectionConditionMutex);
}
//----------------------------------------------------------------------------
void SEConditionVariable::Wait()
{
    // Avoid race conditions.
    EnterCriticalSection((LPCRITICAL_SECTION)
        m_ConditionVariableType.pCriticalSectionWaitersCountMutex);

    m_ConditionVariableType.WaitersCount++;

    LeaveCriticalSection((LPCRITICAL_SECTION)
        m_ConditionVariableType.pCriticalSectionWaitersCountMutex);

    // It's ok to release the <external_mutex> here since Win32
    // manual-reset events maintain state when used with
    // <SetEvent>.  This avoids the "lost wakeup" bug...
    LeaveCriticalSection((LPCRITICAL_SECTION)
        m_ConditionVariableType.pCriticalSectionConditionMutex);

    // Wait for either event to become signaled due to <pthread_cond_signal>
    // being called or <pthread_cond_broadcast> being called.
    int result = WaitForMultipleObjects(2, m_ConditionVariableType.hEvents, 
        FALSE, INFINITE);

    EnterCriticalSection((LPCRITICAL_SECTION)
        m_ConditionVariableType.pCriticalSectionWaitersCountMutex);

    m_ConditionVariableType.WaitersCount--;
    int last_waiter = 
        (result == WAIT_OBJECT_0 + SEConditionVariableType::BROADCAST) &&
        (m_ConditionVariableType.WaitersCount == 0);

    LeaveCriticalSection((LPCRITICAL_SECTION)
        m_ConditionVariableType.pCriticalSectionWaitersCountMutex);

    // Some thread called <pthread_cond_broadcast>.
    if( last_waiter )
    {
        // We're the last waiter to be notified or to stop waiting, so
        // reset the manual event.
        ResetEvent(m_ConditionVariableType.hEvents[
            SEConditionVariableType::BROADCAST]);
    }

    EnterCriticalSection((LPCRITICAL_SECTION)
        m_ConditionVariableType.pCriticalSectionConditionMutex);
}
//----------------------------------------------------------------------------
void SEConditionVariable::Signal()
{
    EnterCriticalSection((LPCRITICAL_SECTION)
        m_ConditionVariableType.pCriticalSectionWaitersCountMutex);

    int haveWaiters = (m_ConditionVariableType.WaitersCount > 0);

    LeaveCriticalSection((LPCRITICAL_SECTION)
        m_ConditionVariableType.pCriticalSectionWaitersCountMutex);

    if( haveWaiters )
    {
        SetEvent(m_ConditionVariableType.hEvents[
            SEConditionVariableType::SIGNAL]);
    }
}
//----------------------------------------------------------------------------
#else
// TODO:
#error Other platforms not yet implemented.
#endif
//----------------------------------------------------------------------------
