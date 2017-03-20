// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017
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

#ifndef Swing_BlockedArray_H
#define Swing_BlockedArray_H

#include "SEPlatforms.h"
#include "SEMemory.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20130524
//----------------------------------------------------------------------------
template <typename T, int LogBlockSize> 
class SEBlockedArray
{
public:
    SEBlockedArray(SE_UInt32 uiURes, SE_UInt32 uiVRes, const T* pData = 0);
    ~SEBlockedArray();

    SE_UInt32 BlockSize() const;
    SE_UInt32 RoundUp(SE_UInt32 x) const;
    SE_UInt32 USize() const;
    SE_UInt32 VSize() const;

    SE_UInt32 Block(SE_UInt32 a) const;
    SE_UInt32 Offset(SE_UInt32 a) const;
    T& operator()(SE_UInt32 u, SE_UInt32 v);
    const T& operator()(SE_UInt32 u, SE_UInt32 v) const;
    void GetLinearArray(T* a) const;

private:
    T* m_Data;
    SE_UInt32 m_uiURes, m_uiVRes, uBlocks;
};

#include "SEBlockedArray.inl"

template <typename T, int LogBlockSize = 2> class SEBlockedArray;

}

#endif