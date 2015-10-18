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

#ifndef Swing_Memory_H
#define Swing_Memory_H

#include "SECoreLIB.h"
#include "SEPlatforms.h"

#ifndef SE_MEMORY_MANAGER

#define SE_NEW new
#define SE_DELETE delete

#else

// 重载new和delete,实现内存追踪

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20080202
//----------------------------------------------------------------------------
class SE_CORE_API SEMemory
{
public:
    struct SEMemoryBlock
    {
        size_t Size;
        const char* File;
        unsigned int Line;
        bool IsArray;
        SEMemoryBlock* Prev;
        SEMemoryBlock* Next;
    };

    static size_t& MaxAllowedBytes(void);
    static bool& TrackSizes(void);

    static size_t GetNumNewCalls(void);
    static size_t GetNumDeleteCalls(void);
    static size_t GetNumBlocks(void);
    static size_t GetNumBytes(void);
    static size_t GetMaxAllocatedBytes(void);
    static size_t GetMaxBlockSize(void);
    static size_t GetHistogram(int i);

    // 用于迭代当前内存list
    static const SEMemoryBlock* GetHead(void);
    static const SEMemoryBlock* GetTail(void);

    // 生成当前内存报告
    static void GenerateReport(const char* pFileName);

private:
    // 记录当前运行期间new和delete的调用次数
    static size_t CurNumNewCalls;
    static size_t CurNumDeleteCalls;

    // 如果希望限制内存使用量,则应用程序可设置此值,
    // 当应用程序超过指定内存使用量后,断言提示
    // 默认值为0,此时不进行内存使用量比较
    static size_t CurMaxAllowedBytes;

    // 当前已分配的内存块数量
    static size_t CurNumBlocks;

    // 当前已分配的字节总数(不包括SEMemoryBlock结构占用的字节总数)
    static size_t CurNumBytes;

    // 内存块list头,尾
    static SEMemoryBlock* MemBlockHead;
    static SEMemoryBlock* MemBlockTail;

    // 设置为true时追踪CurMaxBlockSize,Histogram
    static bool IsTrackSizes;

    // 内存字节总数历史最大值(不包括SEMemoryBlock结构占用的字节总数)
    static size_t CurMaxAllocatedBytes;

    // 当前已分配的最大内存块字节数
    static size_t CurMaxBlockSize;

    // 追踪已经分配的各种尺寸的内存块的各自数量,
    // Histogram[0]记录已经分配的1字节的内存块数量,
    // Histogram[31]记录已经分配的大于等于pow(2,30)字节的内存块数量
    // 其余Histogram[i]记录已经分配的N字节内存块数量,
    // 其中N满足:pow(2,i-1) < N <= pow(2,i)
    static size_t Histogram[32];

public:
    // Internal use
    static void* Allocate(size_t uiSize, char* pFile, unsigned int uiLine,
        bool bIsArray);
    static void Deallocate(char* pAddr, bool bIsArray);
    static void InsertBlock(SEMemoryBlock* pBlock);
    static void RemoveBlock(SEMemoryBlock* pBlock);
};

#include "SEMemory.inl"

}

#define SE_NEW new(__FILE__,__LINE__)
#define SE_DELETE delete

void* operator new(size_t uiSize);
void* operator new[](size_t uiSize);
void* operator new(size_t uiSize, char* pFile, unsigned int uiLine);
void* operator new[](size_t uiSize, char* pFile, unsigned int uiLine);
void operator delete(void* pAddr);
void operator delete[](void* pAddr);
void operator delete(void* pAddr, char* pFile, unsigned int uiLine);
void operator delete[](void* pAddr, char* pFile, unsigned int uiLine);

#endif

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20130415
//----------------------------------------------------------------------------
SE_CORE_API void* SE_AllocateAligned(size_t uiSize);

//----------------------------------------------------------------------------
// Description:
// Date:20130415
//----------------------------------------------------------------------------
template <typename T>
T* SE_AllocateAligned(SE_UInt32 uiCount)
{
    return (T*)SE_AllocateAligned(uiCount * sizeof(T));
}

//----------------------------------------------------------------------------
// Description:
// Date:20130415
//----------------------------------------------------------------------------
SE_CORE_API void SE_FreeAligned(void* pAddr);

//----------------------------------------------------------------------------
// Description: Stack memory allocation.
// Date:20130524
//----------------------------------------------------------------------------
#if defined(SE_IS_WINDOWS)
#include <malloc.h>
#else
#include <alloca.h>
#endif
#define SE_ALLOCA(TYPE, COUNT) (TYPE*)alloca((COUNT) * sizeof(TYPE))

}

#endif
