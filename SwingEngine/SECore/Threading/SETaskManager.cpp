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
#include "SETaskManager.h"

using namespace Swing;

//----------------------------------------------------------------------------
SETaskManager::SETaskManager(bool bUseWorkerThreads)
{
    m_pTaskQueueMutex = SEMutex::Create();
    m_pWorkerSemaphore = SE_NEW SESemaphore;
    m_pTasksRunningCondition = SE_NEW SEConditionVariable;

    m_uiUnfinishedTasksCount = 0;
    m_uiWorkerThreadsCount = SESystem::GetCPUCoresCount();
    m_bUseWorkerThreads = bUseWorkerThreads;
}
//----------------------------------------------------------------------------
SETaskManager::~SETaskManager()
{
    SEMutex::Destroy(m_pTaskQueueMutex);
    m_pTaskQueueMutex = 0;

    SE_DELETE m_pWorkerSemaphore;
    m_pWorkerSemaphore = 0;

    SE_DELETE m_pTasksRunningCondition;
    m_pTasksRunningCondition = 0;
}
//----------------------------------------------------------------------------
void SETaskManager::InitializeWorkerThreads()
{
    if( !m_bUseWorkerThreads )
    {
        return;
    }

    for( int i = 0; i < (int)m_uiWorkerThreadsCount; ++i )
    {
        SETaskWorkerThread* pThread = SE_NEW SETaskWorkerThread(this);
        pThread->Start();

        m_WorkerThreads.push_back(pThread);
    }
}
//----------------------------------------------------------------------------
void SETaskManager::Cleanup()
{
    if( !m_bUseWorkerThreads )
    {
        return;
    }

    {
        // Mutex scope begins.
        SEMutexLock lock(*m_pTaskQueueMutex);
        SE_ASSERT( m_TaskQueue.size() == 0 );
        // Mutex scope ends.
    }

    m_pWorkerSemaphore->Post(m_uiWorkerThreadsCount);

    for( int i = 0; i < (int)m_WorkerThreads.size(); ++i )
    {
        m_WorkerThreads[i]->Wait();
        SE_DELETE m_WorkerThreads[i];
    }
    m_WorkerThreads.clear();
}
//----------------------------------------------------------------------------
void SETaskManager::WaitForAllTasks()
{
    // Enqueue just runs them immediately in this case.
    if( !m_bUseWorkerThreads )
    {
        return;
    }

    if( m_WorkerThreads.size() == 0 )
    {
        // No tasks have been enqueued, InitializeWorkerThreads never called.
        return;
    }

    m_pTasksRunningCondition->Lock();
    while( m_uiUnfinishedTasksCount > 0 )
    {
        m_pTasksRunningCondition->Wait();
    }
    m_pTasksRunningCondition->Unlock();
}
//----------------------------------------------------------------------------
void SETaskManager::EnqueueTasks(const std::vector<SETask*>& rTasks)
{
    if( !m_bUseWorkerThreads )
    {
        for( int i = 0; i < (int)rTasks.size(); ++i )
        {
            rTasks[i]->DoWork();
        }

        return;
    }

    if(  m_WorkerThreads.size() == 0 )
    {
        InitializeWorkerThreads();
    }

    {
        // Mutex scope begins.
        SEMutexLock lock(*m_pTaskQueueMutex);
        for( int i = 0; i < (int)rTasks.size(); ++i )
        {
            m_TaskQueue.push_back(rTasks[i]);
        }
        // Mutex scope ends.
    }

    m_pTasksRunningCondition->Lock();
    m_uiUnfinishedTasksCount += rTasks.size();
    m_pTasksRunningCondition->Unlock();

    m_pWorkerSemaphore->Post((int)rTasks.size());
}
//----------------------------------------------------------------------------