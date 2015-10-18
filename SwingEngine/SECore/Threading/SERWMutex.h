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

#ifndef Swing_RWMutex_H
#define Swing_RWMutex_H

#include "SECoreLIB.h"
#include "SERWMutexType.h"

namespace Swing
{

struct SERWMutexLock;

//----------------------------------------------------------------------------
// Description:
// Date:20130411
//----------------------------------------------------------------------------
class SE_CORE_API SERWMutex
{
public:
    static SERWMutex* Create();
    static void Destroy(SERWMutex* pM);

private:
    SERWMutex();
    ~SERWMutex();

    friend struct SERWMutexLock;
    SERWMutex(SERWMutex&);
    SERWMutex& operator=(const SERWMutex&);

    // System-dependent rw mutex implementation
#if defined(_WIN32)
    void AcquireRead();
    void ReleaseRead();
    void AcquireWrite();
    void ReleaseWrite();
#endif

    SERWMutexType m_RWMutex;
};

//----------------------------------------------------------------------------
// Description:
// Date:20130411
//----------------------------------------------------------------------------
struct SE_CORE_API SERWMutexLock
{
    enum LockType
    {
        LT_READ,
        LT_WRITE
    };

    SERWMutexLock(SERWMutex& rM, LockType eType);
    ~SERWMutexLock();

    void UpgradeToWrite();
    void DowngradeToRead();

private:
    SERWMutexLock(const SERWMutexLock&);
    SERWMutexLock& operator=(const SERWMutexLock&);

    LockType m_eType;
    SERWMutex& m_rMutex;
};

}

#endif
