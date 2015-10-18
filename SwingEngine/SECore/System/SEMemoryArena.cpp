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
#include "SEMemoryArena.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEMemoryArena::SEMemoryArena(SE_UInt32 uiBlockSize)
{
    m_uiBlockSize = uiBlockSize;
    m_uiCurBlockPos = 0;
    m_pCurrentBlock = SE_AllocateAligned<char>(m_uiBlockSize);
}
//----------------------------------------------------------------------------
SEMemoryArena::~SEMemoryArena()
{
    SE_FreeAligned(m_pCurrentBlock);

    for( SE_UInt32 i = 0; i < m_UsedBlocks.size(); ++i )
    {
        SE_FreeAligned(m_UsedBlocks[i]);
    }

    for( SE_UInt32 i = 0; i < m_AvailableBlocks.size(); ++i )
    {
        SE_FreeAligned(m_AvailableBlocks[i]);
    }
}
//----------------------------------------------------------------------------
void* SEMemoryArena::Alloc(SE_UInt32 uiSize)
{
    // Round up uiSize to minimum machine alignment.
    uiSize = ((uiSize + 15) & (~15));

    if( m_uiCurBlockPos + uiSize > m_uiBlockSize )
    {
        // Get new block of memory for _MemoryArena_
        m_UsedBlocks.push_back(m_pCurrentBlock);
        if( m_AvailableBlocks.size() && uiSize <= m_uiBlockSize )
        {
            m_pCurrentBlock = m_AvailableBlocks.back();
            m_AvailableBlocks.pop_back();
        }
        else
        {
            m_pCurrentBlock = SE_AllocateAligned<char>(
                SE_MAX(uiSize, m_uiBlockSize));
        }
        m_uiCurBlockPos = 0;
    }

    void* pRet = m_pCurrentBlock + m_uiCurBlockPos;
    m_uiCurBlockPos += uiSize;

    return pRet;
}
//----------------------------------------------------------------------------
void SEMemoryArena::FreeAll()
{
    m_uiCurBlockPos = 0;
    while( m_UsedBlocks.size() )
    {
        m_AvailableBlocks.push_back(m_UsedBlocks.back());
        m_UsedBlocks.pop_back();
    }
}
//----------------------------------------------------------------------------