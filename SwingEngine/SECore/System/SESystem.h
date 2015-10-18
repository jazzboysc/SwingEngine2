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

#ifndef Swing_System_H
#define Swing_System_H

#include <cassert>
#include <cctype>
#include <cfloat>
#include <cmath>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <fstream>

#include <algorithm>
#include <list>
#include <map>
#include <unordered_map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "SECoreLIB.h"
#include "SEPlatforms.h"
#include "SEAssert.h"
#include "SEMemory.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20080202
//----------------------------------------------------------------------------
class SE_CORE_API SESystem
{
public:
    // ����һ��C�����������ݵ��ֽ��ڴ��ŷ�λ��(iSize = 2,4,8)
    static void SwapBytes(int iSize, void* pValue);
	// ����һ��C�����������ݵ��ֽ��ڴ��ŷ�λ��(iSize = 2,4,8)
    static void SwapBytes(int iSize, int iCount, void* pValue);

    // ͳһ�������Ϊlittle endian��ʽ
    static bool IsBigEndian(void);
    static void EndianCopy(int iSize, const void* pSrc, void* pDst);
    static void EndianCopy(int iSize, int iCount, const void* pSrc, void* 
        pDst);

    // ���ص�λΪ��
    static double GetTime(void);

    // Get the number of CPU cores.
    static int GetCPUCoresCount();

    // ֧���ļ���load,save
    static bool Load(const char* pFileName, char*& rDstBuffer, int& riSize);
    static bool Save(const char* pFileName, const char* pSrcBuffer, int 
        iSize);
    static bool Append(const char* pFileName, const char* pSrcBuffer, int 
        iSize);

    // buffer��д,�ļ���д,���ֽ�������,endianness�޹�
    static int Read1(const char* pSrcBuffer, int iCount, void* pDstData);
    static int Write1(char* pDstBuffer, int iCount, const void* pSrcData);
    static int Read1(FILE* pSrcFile, int iCount, void* pDstData);
    static int Write1(FILE* pDstFile, int iCount, const void* pSrcData);

    // buffer��д,�ļ���д,2,4,8�ֽ�������,Դ����Ϊlittle endian
    static int Read2le(const char* pSrcBuffer, int iCount, void* pDstData);
    static int Read4le(const char* pSrcBuffer, int iCount, void* pDstData);
    static int Read8le(const char* pSrcBuffer, int iCount, void* pDstData);
    static int Write2le(char* pDstBuffer, int iCount, const void* pSrcData);
    static int Write4le(char* pDstBuffer, int iCount, const void* pSrcData);
    static int Write8le(char* pDstBuffer, int iCount, const void* pSrcData);
    static int Read2le(FILE* pSrcFile, int iCount, void* pDstData);
    static int Read4le(FILE* pSrcFile, int iCount, void* pDstData);
    static int Read8le(FILE* pSrcFile, int iCount, void* pDstData);
    static int Write2le(FILE* pDstFile, int iCount, const void* pSrcData);
    static int Write4le(FILE* pDstFile, int iCount, const void* pSrcData);
    static int Write8le(FILE* pDstFile, int iCount, const void* pSrcData);

    // buffer��д,�ļ���д,2,4,8�ֽ�������,Դ����Ϊbig endian
    static int Read2be(const char* pSrcBuffer, int iCount, void* pDstData);
    static int Read4be(const char* pSrcBuffer, int iCount, void* pDstData);
    static int Read8be(const char* pSrcBuffer, int iCount, void* pDstData);
    static int Write2be(char* pDstBuffer, int iCount, const void* pSrcData);
    static int Write4be(char* pDstBuffer, int iCount, const void* pSrcData);
    static int Write8be(char* pDstBuffer, int iCount, const void* pSrcData);
    static int Read2be(FILE* pSrcFile, int iCount, void* pDstData);
    static int Read4be(FILE* pSrcFile, int iCount, void* pDstData);
    static int Read8be(FILE* pSrcFile, int iCount, void* pDstData);
    static int Write2be(FILE* pDstFile, int iCount, const void* pSrcData);
    static int Write4be(FILE* pDstFile, int iCount, const void* pSrcData);
    static int Write8be(FILE* pDstFile, int iCount, const void* pSrcData);

    // Macintoshƽ̨��Ҫ�˺����������临�ӵ�Ӧ�ó���ṹ,
    // �ر��,�˺�������Xcode��Ŀ���Һ���Ŀ¼��,������Ӧ�ó�������
    // �������ļ�����Ŀ����Դ�ļ�����,
    // ������ƽ̨����Ŀ¼�����ļ���,���ֵ�������ྲ̬ȫ���ڴ���(���߳�ʱ��
    // ע��).
    static const char* GetPath(const char* pDirectory, const char* pFileName);

    // ά��һ��Ŀ¼�б�,��Ӧ�ó����ѯ,�б��ʼΪ��,
    // ��Ӧ�ó��������������Ŀ¼,Ŀ¼��������Ŀ¼�ָ�����β(����"/"),
    // InsertDirectory��RemoveDirectory�����Զ�������ַ�
    static void Initialize(void);
    static void Terminate(void);
    static int GetDirectoryCount(void);
    static const char* GetDirectory(int i);
    static bool InsertDirectory(const char* pDirectory);
    static bool RemoveDirectory(const char* pDirectory);
    static void RemoveAllDirectories(void);

    // GetPath����Ŀ¼�б�,���ָ���ļ�������,�򷵻�������·������,���򷵻�
    // null,eMode������ö��ֵ.
    enum
    {
        SM_READ,
        SM_WRITE,
        SM_READ_WRITE
    };
    static const char* GetPath(const char* pFileName, int eMode);

    // ����һ����ɫֵ,����endianϸ��
    static unsigned int MakeRGB(unsigned char ucR, unsigned char ucG, 
        unsigned char ucB);
    static unsigned int MakeRGBA(unsigned char ucR, unsigned char ucG, 
        unsigned char ucB, unsigned char ucA);

    // ��װ��׼I/O�����Ӷ����buffer��д���������,��߰�ȫ����,
    // ĿǰMSVC8.0ʵ�ֵķ�ʽ�������ȵı�׼I/O��
    static FILE* Fopen(const char* pFileName, const char* pMode);
    static int Fprintf(FILE* pFile, const char* pFormat, ...);
    static int Fclose(FILE* pFile);
    static const char* GetEnv(const char* pEnvVarName);
    static void* Memcpy(void* pDst, size_t uiDstSize, const void* pSrc, 
        size_t uiSrcSize);
    static int Sprintf(char* pDst, size_t uiDstSize, const char* pFormat, 
        ...);
    static char* Strcpy(char* pDst, size_t uiDstSize, const char* pSrc);
    static char* Strcat(char* pDst, size_t uiDstSize, const char* pSrc);
    static char* Strncpy(char* pDst, size_t uiDstSize, const char* pSrc, 
        size_t uiSrcSize);
    static char* Strtok(char* pToken, const char* pDelimiters, char*& 
        rpNextToken);

private:
    enum { SYSTEM_MAX_PATH = 1024, SYSTEM_MAX_ENVVAR = 1024 };
    static char Path[SYSTEM_MAX_PATH];
    static char EnvVar[SYSTEM_MAX_ENVVAR];
    static std::vector<std::string>* Directories;   // ϵͳά����Ŀ¼�б�

    // Hardware information.
    static int CPUCoresCount;

public:
    static char SE_PATH[SYSTEM_MAX_ENVVAR];
};

// 2D����ķ������ͷ�,�ͷ�ʱ����ָ�뱻��Ϊnull
template <class T> void Allocate(int iCols, int iRows, T**& raatArray);
template <class T> static void Deallocate(T**& raatArray);

// 3D����ķ������ͷ�,�ͷ�ʱ����ָ�뱻��Ϊnull
template <class T> void Allocate(int iCols, int iRows, int iSlices, T***& 
    raaatArray);
template <class T> void Deallocate(T***& raaatArray);

#include "SESystem.inl"

}

#endif