// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015
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
#include "SEProgressReporter.h"

#if defined(SE_IS_WINDOWS)
#include <windows.h>
#if (_MSC_VER >= 1400)
#include <stdio.h>
#define snprintf _snprintf
#pragma warning(disable:4996)
#endif
#else
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#endif

using namespace Swing;

//----------------------------------------------------------------------------
int TerminalWidth()
{
#if defined(SE_IS_WINDOWS)
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    if( h == INVALID_HANDLE_VALUE || h == NULL )
    {
        fprintf(stderr, "GetStdHandle() call failed");
        return 80;
    }

    CONSOLE_SCREEN_BUFFER_INFO bufferInfo = { 0 };
    GetConsoleScreenBufferInfo(h, &bufferInfo);
    return bufferInfo.dwSize.X;
#else
    struct winsize w;
    if( ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) < 0 )
    {
        fprintf(stderr, "Error in ioctl() in TerminalWidth(): %d", errno);
        return 80;
    }
    return w.ws_col;
#endif
}
//----------------------------------------------------------------------------
SEProgressReporter::SEProgressReporter(int iTotalWork, 
    const std::string& rTitle, int iBarLength)
    :
    m_iTotalWork(iTotalWork)
{
    if( iBarLength <= 0 )
    {
        iBarLength = TerminalWidth() - 28;
    }

    int tempValue = iBarLength - (int)rTitle.size();
    m_iTotalPlusses = SE_MAX(2, tempValue);
    m_pMutex = SEMutex::Create();
    m_iPlussesPrinted = 0;
    m_iWorkDone = 0;
    m_pTimer = SE_NEW SETimer;
    m_pTimer->Start();
    m_pOutFile = stdout;

    // Initialize progress string.
    const int bufLen = rTitle.size() + m_iTotalPlusses + 64;
    m_pBuf = SE_NEW char[bufLen];
    snprintf(m_pBuf, bufLen, "\r%s: [", rTitle.c_str());
    m_pCurSpace = m_pBuf + strlen(m_pBuf);
    char* s = m_pCurSpace;
    for( int i = 0; i < m_iTotalPlusses; ++i )
    {
        *s++ = ' ';
    }
    *s++ = ']';
    *s++ = ' ';
    *s++ = '\0';

    fputs(m_pBuf, m_pOutFile);
    fflush(m_pOutFile);
}
//----------------------------------------------------------------------------
SEProgressReporter::~SEProgressReporter()
{
    SE_DELETE[] m_pBuf;
    SE_DELETE m_pTimer;
    SEMutex::Destroy(m_pMutex);
}
//----------------------------------------------------------------------------
void SEProgressReporter::Update(int iAmount)
{
    if( iAmount == 0 )
    {
        return;
    }

    SEMutexLock lock(*m_pMutex);
    m_iWorkDone += iAmount;
    float percentDone = float(m_iWorkDone) / float(m_iTotalWork);
    int plussesNeeded = SE_RoundToInt(m_iTotalPlusses * percentDone);
    if( plussesNeeded > m_iTotalPlusses )
    {
        plussesNeeded = m_iTotalPlusses;
    }

    while( m_iPlussesPrinted < plussesNeeded )
    {
        *m_pCurSpace++ = '+';
        ++m_iPlussesPrinted;
    }
    fputs(m_pBuf, m_pOutFile);

    // Update elapsed time and estimated time to completion.
    float seconds = (float)m_pTimer->GetTimeElapsed();
    float estRemaining = seconds / percentDone - seconds;
    if( percentDone == 1.0f )
    {
        fprintf(m_pOutFile, " (%.1fs)       ", seconds);
    }
    else
    {
        fprintf(m_pOutFile, " (%.1fs|%.1fs)  ", seconds, 
            SE_MAX(0.0f, estRemaining));
    }
    fflush(m_pOutFile);
}
//----------------------------------------------------------------------------
void SEProgressReporter::Done()
{
    SEMutexLock lock(*m_pMutex);
    while( m_iPlussesPrinted++ < m_iTotalPlusses )
    {
        *m_pCurSpace++ = '+';
    }
    fputs(m_pBuf, m_pOutFile);
    float seconds = (float)m_pTimer->GetTimeElapsed();
    fprintf(m_pOutFile, " (%.1fs)       \n", seconds);
    fflush(m_pOutFile);
}
//----------------------------------------------------------------------------