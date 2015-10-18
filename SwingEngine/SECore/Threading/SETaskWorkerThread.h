// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015


#ifndef Swing_TaskWorkerThread_H
#define Swing_TaskWorkerThread_H

#include "SECoreLIB.h"
#include "SEThread.h"

namespace Swing
{

class SETaskManager;
//----------------------------------------------------------------------------
// Description:
// Date:20130414
//----------------------------------------------------------------------------
class SE_CORE_API SETaskWorkerThread : public SEThread
{
public:
    SETaskWorkerThread(SETaskManager* pTaskManager);
    ~SETaskWorkerThread();

    virtual int Run();

private:
    SETaskWorkerThread();

    SETaskManager* m_pTaskManager;
};

}

#endif
