// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SECorePCH.h"
#include "SEThread.h"
#include "SEAssert.h"

using namespace Swing;

#if defined(_WIN32)
//----------------------------------------------------------------------------
#include <windows.h>
//----------------------------------------------------------------------------
static DWORD WINAPI FuncThreadEntry(LPVOID pArgs)
{
    SE_ASSERT( pArgs );

    SEThread* pMyThread = (SEThread*)pArgs;
    int iRes = pMyThread->Run();

    return (DWORD)iRes;
}
//----------------------------------------------------------------------------
SEThread::SEThread()
{
    m_Thread = 0;
}
//----------------------------------------------------------------------------
SEThread::~SEThread()
{
    if( m_Thread )
    {
        CloseHandle((HANDLE)m_Thread);
        m_Thread = 0;
    }
}
//----------------------------------------------------------------------------
void SEThread::Start()
{
    m_Thread = CreateThread(NULL, 0, FuncThreadEntry, (LPVOID)this, 0, NULL);
    SE_ASSERT( m_Thread );
}
//----------------------------------------------------------------------------
void SEThread::Wait()
{
    WaitForSingleObject((HANDLE)m_Thread, INFINITE);
}
//----------------------------------------------------------------------------
#else
#error Other platforms not yet implemented.
#endif
//----------------------------------------------------------------------------
