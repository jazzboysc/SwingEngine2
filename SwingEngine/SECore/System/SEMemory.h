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

// ����new��delete,ʵ���ڴ�׷��

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

    // ���ڵ�����ǰ�ڴ�list
    static const SEMemoryBlock* GetHead(void);
    static const SEMemoryBlock* GetTail(void);

    // ���ɵ�ǰ�ڴ汨��
    static void GenerateReport(const char* pFileName);

private:
    // ��¼��ǰ�����ڼ�new��delete�ĵ��ô���
    static size_t CurNumNewCalls;
    static size_t CurNumDeleteCalls;

    // ���ϣ�������ڴ�ʹ����,��Ӧ�ó�������ô�ֵ,
    // ��Ӧ�ó��򳬹�ָ���ڴ�ʹ������,������ʾ
    // Ĭ��ֵΪ0,��ʱ�������ڴ�ʹ�����Ƚ�
    static size_t CurMaxAllowedBytes;

    // ��ǰ�ѷ�����ڴ������
    static size_t CurNumBlocks;

    // ��ǰ�ѷ�����ֽ�����(������SEMemoryBlock�ṹռ�õ��ֽ�����)
    static size_t CurNumBytes;

    // �ڴ��listͷ,β
    static SEMemoryBlock* MemBlockHead;
    static SEMemoryBlock* MemBlockTail;

    // ����Ϊtrueʱ׷��CurMaxBlockSize,Histogram
    static bool IsTrackSizes;

    // �ڴ��ֽ�������ʷ���ֵ(������SEMemoryBlock�ṹռ�õ��ֽ�����)
    static size_t CurMaxAllocatedBytes;

    // ��ǰ�ѷ��������ڴ���ֽ���
    static size_t CurMaxBlockSize;

    // ׷���Ѿ�����ĸ��ֳߴ���ڴ��ĸ�������,
    // Histogram[0]��¼�Ѿ������1�ֽڵ��ڴ������,
    // Histogram[31]��¼�Ѿ�����Ĵ��ڵ���pow(2,30)�ֽڵ��ڴ������
    // ����Histogram[i]��¼�Ѿ������N�ֽ��ڴ������,
    // ����N����:pow(2,i-1) < N <= pow(2,i)
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
