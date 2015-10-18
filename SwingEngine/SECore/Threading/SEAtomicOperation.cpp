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
#include "SEAtomicOperation.h"

#if defined(__APPLE__)
    #if !(defined(__i386__) || defined(__amd64__))
    #include <libkern/OSAtomic.h>
    #endif
#endif

#if defined(_WIN32)
#include <windows.h>
#else
#include <pthread.h>
#include <semaphore.h>
#endif

//----------------------------------------------------------------------------
#if defined(_WIN32)

    #if _MSC_VER >= 1300
        extern "C" void _ReadWriteBarrier();
        #pragma intrinsic(_ReadWriteBarrier)
    #else
        #define _ReadWriteBarrier()
    #endif

#endif
//----------------------------------------------------------------------------

using namespace Swing;

//----------------------------------------------------------------------------
SE_Int32 Swing::SE_AtomicAdd(SE_AtomicInt32* pV, SE_Int32 iDelta)
{
#if defined(_WIN32)

    // Do atomic add with MSVC inline assembly
#if (SE_POINTER_SIZE == 8)

    return InterlockedAdd(pV, iDelta);

#else

    SE_Int32 iRes;

    _ReadWriteBarrier();
    __asm
    {
        __asm mov edx, pV
        __asm mov eax, iDelta
        __asm lock xadd [edx], eax
        __asm mov iRes, eax
    }
    _ReadWriteBarrier();

    return iRes + iDelta;

#endif

#elif defined(__APPLE__)

    #if !(defined(__i386__) || defined(__amd64__))
        return OSAtomicAdd32Barrier(iDelta, pV);
    #endif

#else
    // Do atomic add with gcc x86 inline assembly
    SE_Int32 iOrigValue;
    __asm__ __volatile__("lock\n"
                         "xaddl %0,%1"
                         : "=r"(iOrigValue), "=m"(*pV) : "0"(iDelta)
                         : "memory");

    return iOrigValue + iDelta;
#endif
}
//----------------------------------------------------------------------------
SE_Int32 Swing::SE_AtomicCompareAndSwap(SE_AtomicInt32* pV, 
    SE_Int32 iNewValue, SE_Int32 iOldValue)
{
#if defined(_WIN32)

    return InterlockedCompareExchange((volatile LONG*)pV, (LONG)iNewValue, 
        (LONG)iOldValue);

#elif defined(__APPLE__)

    #if !(defined(__i386__) || defined(__amd64__))
        return OSAtomicCompareAndSwap32Barrier(iOldValue, iNewValue, pV);
    #endif

#else

    SE_Int32 iRes;
    __asm__ __volatile__("lock\ncmpxchgl %2,%1"
                          : "=a"(iRes), "=m"(*pV)
                          : "q"(iNewValue), "0"(iOldValue)
                          : "memory");
    return iRes;

#endif
}
//----------------------------------------------------------------------------
float Swing::SE_AtomicAdd(volatile float* pV, float fDelta)
{
    union bits
    {
        float f;
        SE_Int32 i;
    };

    bits oldVal, newVal;
    do
    {
        // On IA32/x64, adding a PAUSE instruction in compare/exchange loops
        // is recommended to improve performance.  (And it does!)
#if (defined(__i386__) || defined(__amd64__))
        __asm__ __volatile__ ("pause\n");
#endif

        oldVal.f = *pV;
        newVal.f = oldVal.f + fDelta;
    }while( SE_AtomicCompareAndSwap(((SE_AtomicInt32*)pV), newVal.i, 
        oldVal.i) != oldVal.i );

    return newVal.f;
}
//----------------------------------------------------------------------------