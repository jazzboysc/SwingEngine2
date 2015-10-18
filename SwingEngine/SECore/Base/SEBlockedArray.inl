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

//----------------------------------------------------------------------------
template <typename T, int LogBlockSize> 
SEBlockedArray<T, LogBlockSize>::SEBlockedArray(SE_UInt32 uiURes, 
    SE_UInt32 uiVRes, const T* pData)
{
    m_uiURes = uiURes;
    m_uiVRes = uiVRes;

    uBlocks = RoundUp(m_uiURes) >> LogBlockSize;
    SE_UInt32 uiCount = RoundUp(m_uiURes) * RoundUp(m_uiVRes);

    m_Data = SE_AllocateAligned<T>(uiCount);
    for( SE_UInt32 i = 0; i < uiCount; ++i )
    {
        new (&m_Data[i]) T();
    }

    if( pData )
    {
        for( SE_UInt32 v = 0; v < m_uiVRes; ++v )
        {
            for( SE_UInt32 u = 0; u < m_uiURes; ++u )
            {
                (*this)(u, v) = pData[v * m_uiURes + u];
            }
        }
    }
}
//----------------------------------------------------------------------------
template <typename T, int LogBlockSize> 
SEBlockedArray<T, LogBlockSize>::~SEBlockedArray()
{
    for( SE_UInt32 i = 0; i < m_uiURes * m_uiVRes; ++i )
    {
        m_Data[i].~T();
    }

    SE_FreeAligned(m_Data);
    m_Data = 0;
}
//----------------------------------------------------------------------------
template <typename T, int LogBlockSize> 
SE_UInt32 SEBlockedArray<T, LogBlockSize>::BlockSize() const
{
    return 1 << LogBlockSize;
}
//----------------------------------------------------------------------------
template <typename T, int LogBlockSize> 
SE_UInt32 SEBlockedArray<T, LogBlockSize>::RoundUp(SE_UInt32 x) const
{
    return (x + BlockSize() - 1) & ~(BlockSize() - 1);
}
//----------------------------------------------------------------------------
template <typename T, int LogBlockSize> 
SE_UInt32 SEBlockedArray<T, LogBlockSize>::USize() const
{
    return m_uiURes;
}
//----------------------------------------------------------------------------
template <typename T, int LogBlockSize> 
SE_UInt32 SEBlockedArray<T, LogBlockSize>::VSize() const
{
    return m_uiVRes;
}
//----------------------------------------------------------------------------
template <typename T, int LogBlockSize> 
SE_UInt32 SEBlockedArray<T, LogBlockSize>::Block(SE_UInt32 a) const
{
    return a >> LogBlockSize;
}
//----------------------------------------------------------------------------
template <typename T, int LogBlockSize> 
SE_UInt32 SEBlockedArray<T, LogBlockSize>::Offset(SE_UInt32 a) const
{
    return (a & (BlockSize() - 1));
}
//----------------------------------------------------------------------------
template <typename T, int LogBlockSize> 
T& SEBlockedArray<T, LogBlockSize>::operator()(SE_UInt32 u, SE_UInt32 v)
{
    SE_UInt32 bu = Block(u), bv = Block(v);
    SE_UInt32 ou = Offset(u), ov = Offset(v);
    SE_UInt32 offset = BlockSize() * BlockSize() * (uBlocks * bv + bu);
    offset += BlockSize() * ov + ou;

    return m_Data[offset];
}
//----------------------------------------------------------------------------
template <typename T, int LogBlockSize> 
const T& SEBlockedArray<T, LogBlockSize>::operator()(SE_UInt32 u, 
    SE_UInt32 v) const
{
    SE_UInt32 bu = Block(u), bv = Block(v);
    SE_UInt32 ou = Offset(u), ov = Offset(v);
    SE_UInt32 offset = BlockSize() * BlockSize() * (uBlocks * bv + bu);
    offset += BlockSize() * ov + ou;

    return m_Data[offset];
}
//----------------------------------------------------------------------------
template <typename T, int LogBlockSize> 
void SEBlockedArray<T, LogBlockSize>::GetLinearArray(T* a) const
{
    for( SE_UInt32 v = 0; v < m_uiVRes; ++v )
    {
        for( SE_UInt32 u = 0; u < m_uiURes; ++u )
        {
            *a++ = (*this)(u, v);
        }
    }
}
//----------------------------------------------------------------------------