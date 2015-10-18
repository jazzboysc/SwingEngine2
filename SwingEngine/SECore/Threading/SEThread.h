// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015


#ifndef Swing_Thread_H
#define Swing_Thread_H

#include "SECoreLIB.h"
#include "SEThreadType.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20130414
//----------------------------------------------------------------------------
class SE_CORE_API SEThread
{
public:
    virtual ~SEThread();

    void Start();
    void Wait();

    // This function is called automatically after the thread starts.
    virtual int Run() = 0;

protected:
    // Abstract base class.
    SEThread();

private:
    SEThreadType m_Thread;
};

}

#endif
