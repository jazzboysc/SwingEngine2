// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SECorePCH.h"
#include "SETaskWorkerThread.h"
#include "SETaskManager.h"

using namespace Swing;

//----------------------------------------------------------------------------
SETaskWorkerThread::SETaskWorkerThread(SETaskManager* pTaskManager)
{
    SE_ASSERT( pTaskManager );
    m_pTaskManager = pTaskManager;
}
//----------------------------------------------------------------------------
SETaskWorkerThread::SETaskWorkerThread()
{
}
//----------------------------------------------------------------------------
SETaskWorkerThread::~SETaskWorkerThread()
{
}
//----------------------------------------------------------------------------
int SETaskWorkerThread::Run()
{
    SE_ASSERT( m_pTaskManager );

    while( true )
    {
        // Wait for task.
        m_pTaskManager->m_pWorkerSemaphore->Wait();

        // Try to get task from task queue.
        SETask* pMyTask = 0;
        { 
            // Mutex scope begins.
            SEMutexLock lock(*m_pTaskManager->m_pTaskQueueMutex);

            if( m_pTaskManager->m_TaskQueue.size() == 0 )
            {
                // No more work to do, it's time to quit.
                break;
            }

            pMyTask = m_pTaskManager->m_TaskQueue.back();
            m_pTaskManager->m_TaskQueue.pop_back();
            // Mutex scope ends.
        }

        pMyTask->DoWork();

        // Work is done, decrease the number of unfinished task.
        // If all the tasks have been finished, inform the task manager.
        m_pTaskManager->m_pTasksRunningCondition->Lock();
        int unfinished = --m_pTaskManager->m_uiUnfinishedTasksCount;
        if( unfinished == 0 )
        {
            m_pTaskManager->m_pTasksRunningCondition->Signal();
        }
        m_pTaskManager->m_pTasksRunningCondition->Unlock();
    }

    return 0;
}
//----------------------------------------------------------------------------