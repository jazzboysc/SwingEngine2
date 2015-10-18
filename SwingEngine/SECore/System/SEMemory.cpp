// Swing Engine Version 2 Source Code 
// Most of techniques in the engine are mainly based on David Eberly's
// Wild Magic 4 and 5 open-source game engine. The author of Swing Engine 
// learned a lot from Eberly's experience of architecture and algorithm.
// Several subsystems are totally new, and others are reorganized or
// reimplimented based on Wild Magic's subsystems.
// Copyright (c) 2007-2011
//
// David Eberly's permission:
// Geometric Tools, LLC
// Copyright (c) 1998-2010
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt
// http://www.geometrictools.com/License/Boost/LICENSE_1_0.txt

#include "SECorePCH.h"
#include "SEMemory.h"

using namespace Swing;

#ifdef SE_MEMORY_MANAGER

// 如果想进行new/delete[]或new[]/delete的不匹配校验,则打开下面的注释
//#define SE_ENABLE_NEW_DELETE_MISMATCH_ASSERT

// 如果想在SEMemory::GenerateReport中进行内存一致性检测,则打开下面的注释
//#define SE_ENABLE_CONSISTENCY_CHECK

#include <cassert>
#include <cstdlib>
#include <fstream>

size_t SEMemory::CurNumNewCalls = 0;
size_t SEMemory::CurNumDeleteCalls = 0;
size_t SEMemory::CurMaxAllowedBytes = 0;
size_t SEMemory::CurNumBlocks = 0;
size_t SEMemory::CurNumBytes = 0;
SEMemory::SEMemoryBlock* SEMemory::MemBlockHead = 0;
SEMemory::SEMemoryBlock* SEMemory::MemBlockTail = 0;
bool SEMemory::IsTrackSizes = false;
size_t SEMemory::CurMaxAllocatedBytes = 0;
size_t SEMemory::CurMaxBlockSize = 0;
size_t SEMemory::Histogram[32] =
{
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0
};

//----------------------------------------------------------------------------
void* SEMemory::Allocate(size_t uiSize, char* pFile, unsigned int uiLine, 
    bool bIsArray)
{
    CurNumNewCalls++;

    // SE_ASSERT( uiSize > 0 );

    size_t uiExtendedSize = sizeof(SEMemoryBlock) + uiSize;
    char* pAddr = (char*)malloc(uiExtendedSize);

    SEMemoryBlock* pBlock = (SEMemoryBlock*)pAddr;
    pBlock->Size = uiSize;
    pBlock->File = pFile;
    pBlock->Line = uiLine;
    pBlock->IsArray = bIsArray;
    InsertBlock(pBlock);

    // 内存申请者应指向这个位置
    pAddr += sizeof(SEMemoryBlock);

    CurNumBlocks++;
    CurNumBytes += uiSize;

    // 如果超过应用程序指定的内存最大使用临界值
    if( CurMaxAllowedBytes > 0 && CurNumBytes > CurMaxAllowedBytes )
    {
        // 警告应用程序
        SE_ASSERT( false );
    }

    // 记录内存字节总数历史最大值
    if( CurNumBytes > CurMaxAllocatedBytes )
    {
        CurMaxAllocatedBytes = CurNumBytes;
    }

    if( IsTrackSizes )
    {
        if( uiSize > CurMaxBlockSize )
        {
            CurMaxBlockSize = uiSize;
        }

        unsigned int uiTwoPowerI = 1;
        int i;
        for( i = 0; i <= 30; i++, uiTwoPowerI <<= 1 )
        {
            if( uiSize <= uiTwoPowerI )
            {
                Histogram[i]++;

                break;
            }
        }
        if( i == 31)
        {
            Histogram[i]++;
        }
    }

    return (void*)pAddr;
}
//----------------------------------------------------------------------------
void SEMemory::Deallocate(char* pAddr, bool bIsArray)
{
    CurNumDeleteCalls++;

    if( !pAddr)
    {
        return;
    }

    pAddr -= sizeof(SEMemoryBlock);

    // 从双向list中移除此block
    SEMemoryBlock* pBlock = (SEMemoryBlock*)pAddr;
    RemoveBlock(pBlock);

#ifdef SE_ENABLE_NEW_DELETE_MISMATCH_ASSERT
    SE_ASSERT( pBlock->IsArray == bIsArray );
#else
    // Avoid compiler warnings about an unused formal parameter.
    (void)bIsArray;
#endif

    SE_ASSERT(CurNumBlocks > 0 && CurNumBytes >= pBlock->Size);
    CurNumBlocks--;
    CurNumBytes -= pBlock->Size;

    free(pAddr);
}
//----------------------------------------------------------------------------
void SEMemory::InsertBlock(SEMemoryBlock* pBlock)
{
    if( MemBlockTail )
    {
        pBlock->Prev = MemBlockTail;
        pBlock->Next = 0;
        MemBlockTail->Next = pBlock;
        MemBlockTail = pBlock;
    }
    else
    {
        pBlock->Prev = 0;
        pBlock->Next = 0;
        MemBlockHead = pBlock;
        MemBlockTail = pBlock;
    }
}
//----------------------------------------------------------------------------
void SEMemory::RemoveBlock(SEMemoryBlock* pBlock)
{
    if( pBlock->Prev )
    {
        pBlock->Prev->Next = pBlock->Next;
    }
    else
    {
        MemBlockHead = pBlock->Next;
    }
    
    if( pBlock->Next )
    {
        pBlock->Next->Prev = pBlock->Prev;
    }
    else
    {
        MemBlockTail = pBlock->Prev;
    }
}
//----------------------------------------------------------------------------
void SEMemory::GenerateReport(const char* pFileName)
{
    std::ofstream OStr(pFileName);
    SE_ASSERT( OStr );
    if( !OStr )
    {
        return;
    }

    OStr << "Total number of 'new' calls = "
        << (unsigned int)CurNumNewCalls << std::endl;
    OStr << "Total number of 'delete' calls = "
        << (unsigned int)CurNumDeleteCalls << std::endl;
    OStr << "Maximum number of allocated bytes = "
        << (unsigned int)CurMaxAllocatedBytes << std::endl << std::endl;

    OStr << "Remaining number of blocks = "
        << (unsigned int)CurNumBlocks << std::endl;
    OStr << "Remaining number of bytes  = "
        << (unsigned int)CurNumBytes << std::endl << std::endl;

    size_t uiNumKnownBlocks = 0;
    size_t uiNumKnownBytes = 0;
    size_t uiNumUnknownBlocks = 0;
    size_t uiNumUnknownBytes = 0;
    SEMemoryBlock* pBlock = MemBlockHead;
    while( pBlock )
    {
        if( pBlock->File )
        {
            uiNumKnownBlocks++;
            uiNumKnownBytes += pBlock->Size;
        }
        else
        {
            uiNumUnknownBlocks++;
            uiNumUnknownBytes += pBlock->Size;
        }

        pBlock = pBlock->Next;
    }

#ifdef SE_ENABLE_CONSISTENCY_CHECK
    // 内存一致性检测
    SE_ASSERT( uiNumKnownBlocks + uiNumUnknownBlocks == CurNumBlocks );
    SE_ASSERT( uiNumKnownBytes + uiNumUnknownBytes == CurNumBytes );
#endif

    OStr << "Remaining number of known blocks = "
        << (unsigned int)uiNumKnownBlocks << std::endl;
    OStr << "Remaining number of known bytes  = "
        << (unsigned int)uiNumKnownBytes << std::endl << std::endl;

    OStr << "Remaining number of unknown blocks = "
        << (unsigned int)uiNumUnknownBlocks << std::endl;
    OStr << "Remaining number of unknown bytes  = "
        << (unsigned int)uiNumUnknownBytes << std::endl << std::endl;

    // 输出每个块信息
    pBlock = MemBlockHead;
    size_t uiIndex = 0;
    while( pBlock )
    {
        OStr << "block = " << (unsigned int)uiIndex << std::endl;
        OStr << "size  = " << (unsigned int)pBlock->Size << std::endl;
        if( pBlock->File )
        {
            OStr << "file  = " << pBlock->File << std::endl;
            OStr << "line  = " << pBlock->Line << std::endl;
        }
        else
        {
            OStr << "file  = unknown" << std::endl;
            OStr << "line  = unknown" << std::endl;
        }
        OStr << "array = " << pBlock->IsArray << std::endl << std::endl;
        
        pBlock = pBlock->Next;
        uiIndex++;
    }
    OStr.close();

#ifdef SE_ENABLE_CONSISTENCY_CHECK
    SE_ASSERT( uiIndex == CurNumBlocks );
#endif
}
//----------------------------------------------------------------------------
void* operator new(size_t uiSize)
{
    return SEMemory::Allocate(uiSize, 0, 0, false);
}
//----------------------------------------------------------------------------
void* operator new[](size_t uiSize)
{
    return SEMemory::Allocate(uiSize, 0, 0, true);
}
//----------------------------------------------------------------------------
void* operator new(size_t uiSize, char* pFile, unsigned int uiLine)
{
    return SEMemory::Allocate(uiSize, pFile, uiLine, false);
}
//----------------------------------------------------------------------------
void* operator new[](size_t uiSize, char* pFile, unsigned int uiLine)
{
    return SEMemory::Allocate(uiSize, pFile, uiLine, true);
}
//----------------------------------------------------------------------------
void operator delete(void* pAddr)
{
    SEMemory::Deallocate((char*)pAddr, false);
}
//----------------------------------------------------------------------------
void operator delete[](void* pAddr)
{
    SEMemory::Deallocate((char*)pAddr, true);
}
//----------------------------------------------------------------------------
void operator delete(void* pAddr, char*, unsigned int)
{
    SEMemory::Deallocate((char*)pAddr,false);
}
//----------------------------------------------------------------------------
void operator delete[](void* pAddr, char*, unsigned int)
{
    SEMemory::Deallocate((char*)pAddr, true);
}
//----------------------------------------------------------------------------

#endif

#if defined(SE_IS_WINDOWS)
//----------------------------------------------------------------------------
void* Swing::SE_AllocateAligned(size_t uiSize)
{
    return _aligned_malloc(uiSize, SE_L1_CACHE_LINE_SIZE);
}
//----------------------------------------------------------------------------
void Swing::SE_FreeAligned(void* pAddr)
{
    if( !pAddr )
    {
        return;
    }

    _aligned_free(pAddr);
}
//----------------------------------------------------------------------------
#else
#error Other platforms not yet implemented.
#endif