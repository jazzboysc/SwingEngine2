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

#ifndef Swing_TaskManager_H
#define Swing_TaskManager_H

#include "SECoreLIB.h"
#include "SESystem.h"
#include "SETask.h"
#include "SEMutex.h"
#include "SESemaphore.h"
#include "SEConditionVariable.h"
#include "SETaskWorkerThread.h"

namespace Swing
{
//----------------------------------------------------------------------------
// Description:
// Date:20130414
//----------------------------------------------------------------------------
class SE_CORE_API SETaskManager
{
public:
    SETaskManager(bool bUseWorkerThreads = true);
    ~SETaskManager();

    void InitializeWorkerThreads();
    void Cleanup();
    void WaitForAllTasks();
    void EnqueueTasks(const std::vector<SETask*>& rTasks);

private:
    friend class SETaskWorkerThread;

    SEMutex* m_pTaskQueueMutex;
    SESemaphore* m_pWorkerSemaphore;
    SEConditionVariable* m_pTasksRunningCondition;

    SE_UInt32 m_uiUnfinishedTasksCount;
    SE_UInt32 m_uiWorkerThreadsCount;
    std::vector<SETaskWorkerThread*> m_WorkerThreads;
    std::vector<SETask*> m_TaskQueue;
    bool m_bUseWorkerThreads;
};

}

#endif
