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
#include "SEMutex.h"
#include "SEAssert.h"

using namespace Swing;

#if defined(_WIN32)
//----------------------------------------------------------------------------
#include <windows.h>
//----------------------------------------------------------------------------
SERWMutex* SERWMutex::Create()
{
    return SE_NEW SERWMutex;
}
//----------------------------------------------------------------------------
void SERWMutex::Destroy(SERWMutex* pM)
{
    SE_DELETE pM;
}
//----------------------------------------------------------------------------
SERWMutex::SERWMutex()
{
    m_RWMutex.WritersWaitingCount = 0;
    m_RWMutex.ReadersWaitingCount = 0;
    m_RWMutex.ActiveWriterReaders = 0;

    m_RWMutex.pCriticalSection = (void*)SE_NEW CRITICAL_SECTION;
    InitializeCriticalSection((LPCRITICAL_SECTION)m_RWMutex.pCriticalSection);

    m_RWMutex.hReadyToRead = (void*)CreateEvent(NULL, TRUE, FALSE, NULL);
    SE_ASSERT( m_RWMutex.hReadyToRead != NULL );

    m_RWMutex.hReadyToWrite = (void*)CreateSemaphore(NULL, 0, 1, NULL);
    if( m_RWMutex.hReadyToWrite == NULL )
    {
        CloseHandle((HANDLE)m_RWMutex.hReadyToRead);
        SE_ASSERT( false );
    }
}
//----------------------------------------------------------------------------
SERWMutex::~SERWMutex()
{
    if( m_RWMutex.hReadyToRead )
    {
        CloseHandle((HANDLE)m_RWMutex.hReadyToRead);
    }

    if( m_RWMutex.hReadyToWrite )
    {
        CloseHandle((HANDLE)m_RWMutex.hReadyToWrite);
    }

    DeleteCriticalSection((LPCRITICAL_SECTION)m_RWMutex.pCriticalSection);
    SE_DELETE m_RWMutex.pCriticalSection;
    m_RWMutex.pCriticalSection = 0;
}
//----------------------------------------------------------------------------
void SERWMutex::AcquireRead()
{
    bool bNotifyReaders = false;

    EnterCriticalSection((LPCRITICAL_SECTION)m_RWMutex.pCriticalSection);

    if( (m_RWMutex.WritersWaitingCount > 0) || 
        (HIWORD(m_RWMutex.ActiveWriterReaders) > 0) )
    {
        ++m_RWMutex.ReadersWaitingCount;

        while( true )
        {
            ResetEvent(m_RWMutex.hReadyToRead);
            LeaveCriticalSection((LPCRITICAL_SECTION)m_RWMutex.pCriticalSection);
            WaitForSingleObject(m_RWMutex.hReadyToRead, INFINITE);
            EnterCriticalSection((LPCRITICAL_SECTION)m_RWMutex.pCriticalSection);

            // The reader is only allowed to read if there aren't
            // any writers waiting and if a writer doesn't own the
            // lock.
            if( (m_RWMutex.WritersWaitingCount == 0) && 
                (HIWORD(m_RWMutex.ActiveWriterReaders) == 0) )
            {
                break;
            }
        }

        // Reader is done waiting.
        --m_RWMutex.ReadersWaitingCount;

        // Reader can read.
        ++m_RWMutex.ActiveWriterReaders;
    }
    else
    {
        // Reader can read.
        if( (++m_RWMutex.ActiveWriterReaders == 1) && 
            (m_RWMutex.ReadersWaitingCount != 0) )
        {
            // Set flag to notify other waiting readers
            // outside of the critical section
            // so that they don't when the threads
            // are dispatched by the scheduler they
            // don't immediately block on the critical
            // section that this thread is holding.
            bNotifyReaders = true;
        }
    }

    SE_ASSERT( HIWORD(m_RWMutex.ActiveWriterReaders) == 0 );
    LeaveCriticalSection((LPCRITICAL_SECTION)m_RWMutex.pCriticalSection);

    if( bNotifyReaders )
    {
        SetEvent(m_RWMutex.hReadyToRead);
    }
}
//----------------------------------------------------------------------------
void SERWMutex::AcquireWrite()
{
    EnterCriticalSection((LPCRITICAL_SECTION)m_RWMutex.pCriticalSection);

    // Are there active readers?
    if( m_RWMutex.ActiveWriterReaders != 0 )
    {
        ++m_RWMutex.WritersWaitingCount;

        LeaveCriticalSection((LPCRITICAL_SECTION)m_RWMutex.pCriticalSection);
        WaitForSingleObject((HANDLE)m_RWMutex.hReadyToWrite, INFINITE);

        // Upon wakeup theirs no need for the writer
        // to acquire the critical section.  It
        // already has been transfered ownership of the
        // lock by the signaler.
    }
    else
    {
        SE_ASSERT( m_RWMutex.ActiveWriterReaders == 0 );

        // Set that the writer owns the lock.
        m_RWMutex.ActiveWriterReaders = MAKELONG(0, 1);

        LeaveCriticalSection((LPCRITICAL_SECTION)m_RWMutex.pCriticalSection);
    }
}
//----------------------------------------------------------------------------
void SERWMutex::ReleaseRead()
{
    EnterCriticalSection((LPCRITICAL_SECTION)m_RWMutex.pCriticalSection);

    // Assert that the lock isn't held by a writer.
    SE_ASSERT( HIWORD(m_RWMutex.ActiveWriterReaders) == 0 );

    // Assert that the lock is held by readers.
    SE_ASSERT( LOWORD(m_RWMutex.ActiveWriterReaders > 0) );

    // Decrement the number of active readers.
    if( --m_RWMutex.ActiveWriterReaders == 0 )
    {
        ResetEvent((HANDLE)m_RWMutex.hReadyToRead);
    }

    // if writers are waiting and this is the last reader
    // hand owneership over to a writer.
    if( (m_RWMutex.WritersWaitingCount != 0) && 
        (m_RWMutex.ActiveWriterReaders == 0) )
    {
        // Decrement the number of waiting writers
        --m_RWMutex.WritersWaitingCount;

        // SEPass ownership to a writer thread.
        m_RWMutex.ActiveWriterReaders = (SE_UInt32)MAKELONG(0, 1);
        ReleaseSemaphore((HANDLE)m_RWMutex.hReadyToWrite, 1, NULL);
    }

    LeaveCriticalSection((LPCRITICAL_SECTION)m_RWMutex.pCriticalSection);
}
//----------------------------------------------------------------------------
void SERWMutex::ReleaseWrite()
{
    bool bNotifyWriter = false;
    bool bNotifyReaders = false;

    EnterCriticalSection((LPCRITICAL_SECTION)m_RWMutex.pCriticalSection);

    // Assert that the lock is owned by a writer.
    SE_ASSERT( HIWORD(m_RWMutex.ActiveWriterReaders) == 1 );

    // Assert that the lock isn't owned by one or more readers
    SE_ASSERT( LOWORD(m_RWMutex.ActiveWriterReaders) == 0 );

    if( m_RWMutex.WritersWaitingCount != 0 )
    {
        // Writers waiting, decrement the number of
        // waiting writers and release the semaphore
        // which means ownership is passed to the thread
        // that has been released.
        --m_RWMutex.WritersWaitingCount;
        bNotifyWriter = true;
    }
    else
    {
        // There aren't any writers waiting
        // Release the exclusive hold on the lock.
        m_RWMutex.ActiveWriterReaders = 0;

        // if readers are waiting set the flag
        // that will cause the readers to be notified
        // once the critical section is released.  This
        // is done so that an awakened reader won't immediately
        // block on the critical section which is still being
        // held by this thread.
        if( m_RWMutex.ReadersWaitingCount != 0 )
        {
            bNotifyReaders = true;
        }
    }

    LeaveCriticalSection((LPCRITICAL_SECTION)m_RWMutex.pCriticalSection);

    if( bNotifyWriter )
    {
        ReleaseSemaphore((HANDLE)m_RWMutex.hReadyToWrite, 1, NULL);
    }
    else if( bNotifyReaders )
    {
        SetEvent((HANDLE)m_RWMutex.hReadyToRead);
    }
}
//----------------------------------------------------------------------------
SERWMutexLock::SERWMutexLock(SERWMutex& rM, LockType eType)
    : m_eType(eType), m_rMutex(rM)
{
    if( m_eType == LT_READ)
    {
        m_rMutex.AcquireRead();
    }
    else
    {
        m_rMutex.AcquireWrite();
    }
}
//----------------------------------------------------------------------------
SERWMutexLock::~SERWMutexLock()
{
    if( m_eType == LT_READ )
    {
        m_rMutex.ReleaseRead();
    }
    else
    {
        m_rMutex.ReleaseWrite();
    }
}
//----------------------------------------------------------------------------
void SERWMutexLock::UpgradeToWrite()
{
    SE_ASSERT( m_eType == LT_READ );

    m_rMutex.ReleaseRead();
    m_rMutex.AcquireWrite();
    m_eType = LT_WRITE;
}
//----------------------------------------------------------------------------
void SERWMutexLock::DowngradeToRead()
{
    SE_ASSERT( m_eType == LT_WRITE );

    m_rMutex.ReleaseWrite();
    m_rMutex.AcquireRead();
    m_eType = LT_READ;
}
//----------------------------------------------------------------------------
#elif defined(__LINUX__) || defined(__APPLE__)
//----------------------------------------------------------------------------
#error Other platforms not yet implemented.
//----------------------------------------------------------------------------
#else
#error Other platforms not yet implemented.
#endif
//----------------------------------------------------------------------------
