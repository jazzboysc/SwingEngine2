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
#include "SESystem.h"

using namespace Swing;

// 支持Load
#include <sys/stat.h>

// 支持GetTime
#ifdef __APPLE__
#include <sys/time.h>
static timeval SystemInitialTime;
static bool InitializedTimeFlag = false;
#else
#include <sys/timeb.h>
static long InitialSec = 0;
static long InitialUSec = 0;
static bool InitializedTimeFlag = false;
#endif

// 支持应用程序目录定位
#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif

char SESystem::Path[SYSTEM_MAX_PATH];
char SESystem::EnvVar[SYSTEM_MAX_ENVVAR]; // 环境变量值
std::vector<std::string>* SESystem::Directories = 0;
char SESystem::SE_PATH[SYSTEM_MAX_ENVVAR];

int SESystem::CPUCoresCount = 0;

//----------------------------------------------------------------------------
void SESystem::SwapBytes(int iSize, void* pValue)
{
    // iSize必须是偶数
    SE_ASSERT( iSize >= 2 && (iSize & 1) == 0 );

    char* pBytes = (char*)pValue;
    for( int i0 = 0, i1 = iSize - 1; i0 < iSize / 2; i0++, i1-- )
    {
        char cTemp = pBytes[i0];
        pBytes[i0] = pBytes[i1];
        pBytes[i1] = cTemp;
    }
}
//----------------------------------------------------------------------------
void SESystem::SwapBytes(int iSize, int iCount, void* pValue)
{
    // iSize必须是偶数
    SE_ASSERT( iSize >= 2 && (iSize & 1) == 0 );

    char* pBytes = (char*)pValue;
    for( int i = 0; i < iCount; i++, pBytes += iSize )
    {
        for( int i0 = 0, i1 = iSize - 1; i0 < iSize / 2; i0++, i1-- )
        {
            char cTemp = pBytes[i0];
            pBytes[i0] = pBytes[i1];
            pBytes[i1] = cTemp;
        }
    }
}
//----------------------------------------------------------------------------
bool SESystem::IsBigEndian()
{
    int iInt = 1;
    char* pChar = (char*)&iInt;

    return !(*pChar);
}
//----------------------------------------------------------------------------
void SESystem::EndianCopy(int iSize, const void* pSrc, void* pDst)
{
    size_t uiSize = (size_t)iSize;
    Memcpy(pDst, uiSize, pSrc, uiSize);

#ifdef SE_BIG_ENDIAN
    SwapBytes(iSize, pDst);
#endif
}
//----------------------------------------------------------------------------
void SESystem::EndianCopy(int iSize, int iCount, const void* pSrc, 
    void* pDst)
{
    size_t uiSize = (size_t)(iSize * iCount);
    Memcpy(pDst, uiSize, pSrc, uiSize);

#ifdef SE_BIG_ENDIAN
    SwapBytes(iSize, iCount, pDst);
#endif
}
//----------------------------------------------------------------------------
double SESystem::GetTime()
{
#ifdef __APPLE__
    if( !InitializedTimeFlag )
    {
        InitializedTimeFlag = true;
        gettimeofday(&SystemInitialTime, 0);
    }

    struct timeval CurTime;
    gettimeofday(&CurTime, 0);
    
    struct timeval DeltaTime;
    timersub(&CurTime, &SystemInitialTime, &DeltaTime);

    return 0.001*(double)(1000*DeltaTime.tv_sec + DeltaTime.tv_usec/1000);
#else
    struct timeb TB;

    if( !InitializedTimeFlag )
    {
        InitializedTimeFlag = true;
        ftime(&TB);
        InitialSec = (long)TB.time;
        InitialUSec = 1000*TB.millitm;
    }

    ftime(&TB);
    long lCurrentSec = (long)TB.time;
    long lCurrentUSec = 1000*TB.millitm;
    long lDeltaSec = lCurrentSec - InitialSec;
    long lDeltaUSec = lCurrentUSec -InitialUSec;
    if( lDeltaUSec < 0)
    {
        lDeltaUSec += 1000000;
        lDeltaSec--;
    }

    return 0.001*(double)(1000*lDeltaSec + lDeltaUSec/1000);
#endif
}
//----------------------------------------------------------------------------
bool SESystem::Load(const char* pFileName, char*& rDstBuffer, int& riSize)
{
    struct stat FileStat;

    if( stat(pFileName, &FileStat) != 0 )
    {
        // 文件不存在
        rDstBuffer = 0;
        riSize = 0;
        return false;
    }

    FILE* pFile = SESystem::Fopen(pFileName, "rb");

    SE_ASSERT( pFile );

    if( !pFile )
    { 
        rDstBuffer = 0;
        riSize = 0;
        return false;
    }

    riSize = FileStat.st_size;
    rDstBuffer = SE_NEW char[riSize];
    int iRead = (int)fread(rDstBuffer, sizeof(char), riSize, pFile);
    // 如果文件不能正常关闭或者读取字节数不等于文件字节数
    if( SESystem::Fclose(pFile) != 0 || iRead != riSize )
    {
        SE_ASSERT( false );

        SE_DELETE[] rDstBuffer;
        rDstBuffer = 0;
        riSize = 0;
        return false;
    }

    return true;
}
//----------------------------------------------------------------------------
bool SESystem::Save(const char* pFileName, const char* pSrcBuffer, 
    int iSize)
{
    if( !pSrcBuffer || iSize <= 0 )
    {
        SE_ASSERT( false );

        return false;
    }

    FILE* pFile = SESystem::Fopen(pFileName, "wb");
    if( !pFile )
    {
        return false;
    }

    int iWrite = (int)fwrite(pSrcBuffer, sizeof(char), iSize, pFile);
    // 如果文件不能正常关闭或者写入字节数不等于期望字节数
    if( SESystem::Fclose(pFile) != 0 || iWrite != iSize )
    {
        SE_ASSERT( false );

        return false;
    }

    return true;
}
//----------------------------------------------------------------------------
bool SESystem::Append(const char* pFileName, const char* pSrcBuffer, 
    int iSize)
{
    if( !pSrcBuffer || iSize <= 0 )
    {
        SE_ASSERT( false );

        return false;
    }

    FILE* pFile = SESystem::Fopen(pFileName, "ab");
    if( !pFile )
    {
        return false;
    }

    int iWrite = (int)fwrite(pSrcBuffer, sizeof(char), iSize, pFile);
    // 如果文件不能正常关闭或者写入字节数不等于期望字节数
    if( SESystem::Fclose(pFile) != 0 || iWrite != iSize )
    {
        SE_ASSERT( false );

        return false;
    }

    return true;
}
//----------------------------------------------------------------------------
int SESystem::Read1(const char* pSrcBuffer, int iCount, void* pDstData)
{
    SE_ASSERT( pSrcBuffer && iCount > 0 && pDstData );

    size_t uiSize = (size_t)iCount;
    Memcpy(pDstData, uiSize, pSrcBuffer, uiSize);

    return iCount;
}
//----------------------------------------------------------------------------
int SESystem::Write1(char* pDstBuffer, int iCount, const void* pSrcData)
{
    SE_ASSERT( pDstBuffer && iCount > 0 && pSrcData );

    size_t uiSize = (size_t)iCount;
    Memcpy(pDstBuffer, uiSize, pSrcData, uiSize);

    return iCount;
}
//----------------------------------------------------------------------------
int SESystem::Read1(FILE* pSrcFile, int iCount, void* pDstData)
{
    SE_ASSERT( pSrcFile && iCount > 0 && pDstData );

    fread(pDstData, 1, iCount, pSrcFile);

    return iCount;
}
//----------------------------------------------------------------------------
int SESystem::Write1(FILE* pDstFile, int iCount, const void* pSrcData)
{
    SE_ASSERT( pDstFile && iCount > 0 && pSrcData );

    fwrite(pSrcData, 1, iCount, pDstFile);

    return iCount;
}
//----------------------------------------------------------------------------
int SESystem::Read2le(const char* pSrcBuffer, int iCount, void* pDstData)
{
    SE_ASSERT( pSrcBuffer && iCount > 0 && pDstData );

    int iNumBytes = 2*iCount;
    size_t uiSize = (size_t)iNumBytes;
    Memcpy(pDstData, uiSize, pSrcBuffer, uiSize);
#ifdef SE_BIG_ENDIAN
    SwapBytes(2, iCount, pDstData);
#endif

    return iNumBytes;
}
//----------------------------------------------------------------------------
int SESystem::Read4le(const char* pSrcBuffer, int iCount, void* pDstData)
{
    SE_ASSERT( pSrcBuffer && iCount > 0 && pDstData );

    int iNumBytes = 4*iCount;
    size_t uiSize = (size_t)iNumBytes;
    Memcpy(pDstData, uiSize, pSrcBuffer, uiSize);
#ifdef SE_BIG_ENDIAN
    SwapBytes(4, iCount, pDstData);
#endif

    return iNumBytes;
}
//----------------------------------------------------------------------------
int SESystem::Read8le(const char* pSrcBuffer, int iCount, void* pDstData)
{
    SE_ASSERT( pSrcBuffer && iCount > 0 && pDstData );

    int iNumBytes = 8*iCount;
    size_t uiSize = (size_t)iNumBytes;
    Memcpy(pDstData, uiSize, pSrcBuffer, uiSize);
#ifdef SE_BIG_ENDIAN
    SwapBytes(8, iCount, pDstData);
#endif

    return iNumBytes;
}
//----------------------------------------------------------------------------
int SESystem::Write2le(char* pDstBuffer, int iCount, const void* pSrcData)
{
    SE_ASSERT( pDstBuffer && iCount > 0 && pSrcData );

    int iNumBytes = 2*iCount;
    size_t uiSize = (size_t)iNumBytes;
    Memcpy(pDstBuffer, uiSize, pSrcData, uiSize);
#ifdef SE_BIG_ENDIAN
    SwapBytes(2, iCount, pDstBuffer);
#endif

    return iNumBytes;
}
//----------------------------------------------------------------------------
int SESystem::Write4le(char* pDstBuffer, int iCount, const void* pSrcData)
{
    SE_ASSERT( pDstBuffer && iCount > 0 && pSrcData );

    int iNumBytes = 4*iCount;
    size_t uiSize = (size_t)iNumBytes;
    Memcpy(pDstBuffer, uiSize, pSrcData, uiSize);
#ifdef SE_BIG_ENDIAN
    SwapBytes(4, iCount, pDstBuffer);
#endif

    return iNumBytes;
}
//----------------------------------------------------------------------------
int SESystem::Write8le(char* pDstBuffer, int iCount, const void* pSrcData)
{
    SE_ASSERT( pDstBuffer && iCount > 0 && pSrcData );

    int iNumBytes = 8*iCount;
    size_t uiSize = (size_t)iNumBytes;
    Memcpy(pDstBuffer, uiSize, pSrcData, uiSize);
#ifdef SE_BIG_ENDIAN
    SwapBytes(8, iCount, pDstBuffer);
#endif

    return iNumBytes;
}
//----------------------------------------------------------------------------
int SESystem::Read2le(FILE* pSrcFile, int iCount, void* pDstData)
{
    SE_ASSERT( pSrcFile && iCount > 0 && pDstData );

    fread(pDstData, 2, iCount, pSrcFile);
#ifdef SE_BIG_ENDIAN
    SwapBytes(2, iCount, pDstData);
#endif

    return 2*iCount;
}
//----------------------------------------------------------------------------
int SESystem::Read4le(FILE* pSrcFile, int iCount, void* pDstData)
{
    SE_ASSERT( pSrcFile && iCount > 0 && pDstData);

    fread(pDstData, 4, iCount, pSrcFile);
#ifdef SE_BIG_ENDIAN
    SwapBytes(4, iCount, pDstData);
#endif

    return 4*iCount;
}
//----------------------------------------------------------------------------
int SESystem::Read8le(FILE* pSrcFile, int iCount, void* pDstData)
{
    SE_ASSERT( pSrcFile && iCount > 0 && pDstData );

    fread(pDstData, 8, iCount, pSrcFile);
#ifdef SE_BIG_ENDIAN
    SwapBytes(8, iCount, pDstData);
#endif

    return 8*iCount;
}
//----------------------------------------------------------------------------
int SESystem::Write2le(FILE* pDstFile, int iCount, const void* pSrcData)
{
    SE_ASSERT( pDstFile && iCount > 0 && pSrcData );

#ifdef SE_BIG_ENDIAN
    const short* pTempData = (const short*)pSrcData;
    for( int i = 0; i < iCount; i++ )
    {
        short sTemp = *pTempData++;
        SwapBytes(2, &sTemp);
        fwrite(&sTemp, 2, 1, pDstFile);
    }
#else
    fwrite(pSrcData, 2, iCount, pDstFile);
#endif

    return 2*iCount;
}
//----------------------------------------------------------------------------
int SESystem::Write4le(FILE* pDstFile, int iCount, const void* pSrcData)
{
    SE_ASSERT( pDstFile && iCount > 0 && pSrcData);

#ifdef SE_BIG_ENDIAN
    const int* pTempData = (const int*)pSrcData;
    for( int i = 0; i < iCount; i++ )
    {
        int iTemp = *pTempData++;
        SwapBytes(4, &iTemp);
        fwrite(&iTemp, 4, 1, pDstFile);
    }
#else
    fwrite(pSrcData, 4, iCount, pDstFile);
#endif

    return 4*iCount;
}
//----------------------------------------------------------------------------
int SESystem::Write8le(FILE* pDstFile, int iCount, const void* pSrcData)
{
    SE_ASSERT( pDstFile && iCount > 0 && pSrcData );

#ifdef SE_BIG_ENDIAN
    const double* pTempData = (const double*)pSrcData;
    for( int i = 0; i < iCount; i++ )
    {
        double dTemp = *pTempData++;
        SwapBytes(8, &dTemp);
        fwrite(&dTemp, 8, 1, pDstFile);
    }
#else
    fwrite(pSrcData, 8, iCount, pDstFile);
#endif

    return 8*iCount;
}
//----------------------------------------------------------------------------
int SESystem::Read2be(const char* pSrcBuffer, int iCount, void* pDstData)
{
    SE_ASSERT( pSrcBuffer && iCount > 0 && pDstData );

    int iNumBytes = 2*iCount;
    size_t uiSize = (size_t)iNumBytes;
    Memcpy(pDstData, uiSize, pSrcBuffer, uiSize);
#ifndef SE_BIG_ENDIAN
    SwapBytes(2, iCount, pDstData);
#endif

    return iNumBytes;
}
//----------------------------------------------------------------------------
int SESystem::Read4be(const char* pSrcBuffer, int iCount, void* pDstData)
{
    SE_ASSERT( pSrcBuffer && iCount > 0 && pDstData );

    int iNumBytes = 4*iCount;
    size_t uiSize = (size_t)iNumBytes;
    Memcpy(pDstData, uiSize, pSrcBuffer, uiSize);
#ifndef SE_BIG_ENDIAN
    SwapBytes(4, iCount, pDstData);
#endif

    return iNumBytes;
}
//----------------------------------------------------------------------------
int SESystem::Read8be(const char* pSrcBuffer, int iCount, void* pDstData)
{
    SE_ASSERT( pSrcBuffer && iCount > 0 && pDstData );

    int iNumBytes = 8*iCount;
    size_t uiSize = (size_t)iNumBytes;
    Memcpy(pDstData, uiSize, pSrcBuffer, uiSize);
#ifndef SE_BIG_ENDIAN
    SwapBytes(8, iCount, pDstData);
#endif

    return iNumBytes;
}
//----------------------------------------------------------------------------
int SESystem::Write2be(char* pDstBuffer, int iCount, const void* pSrcData)
{
    SE_ASSERT( pDstBuffer && iCount > 0 && pSrcData );

    int iNumBytes = 2*iCount;
    size_t uiSize = (size_t)iNumBytes;
    Memcpy(pDstBuffer, uiSize, pSrcData, uiSize);
#ifndef SE_BIG_ENDIAN
    SwapBytes(2, iCount, pDstBuffer);
#endif

    return iNumBytes;
}
//----------------------------------------------------------------------------
int SESystem::Write4be(char* pDstBuffer, int iCount, const void* pSrcData)
{
    SE_ASSERT( pDstBuffer && iCount > 0 && pSrcData );

    int iNumBytes = 4*iCount;
    size_t uiSize = (size_t)iNumBytes;
    Memcpy(pDstBuffer, uiSize, pSrcData, uiSize);
#ifndef SE_BIG_ENDIAN
    SwapBytes(4, iCount, pDstBuffer);
#endif

    return iNumBytes;
}
//----------------------------------------------------------------------------
int SESystem::Write8be(char* pDstBuffer, int iCount, const void* pSrcData)
{
    SE_ASSERT( pDstBuffer && iCount > 0 && pSrcData );

    int iNumBytes = 8*iCount;
    size_t uiSize = (size_t)iNumBytes;
    Memcpy(pDstBuffer, uiSize, pSrcData, uiSize);
#ifndef SE_BIG_ENDIAN
    SwapBytes(8, iCount, pDstBuffer);
#endif

    return iNumBytes;
}
//----------------------------------------------------------------------------
int SESystem::Read2be(FILE* pSrcFile, int iCount, void* pDstData)
{
    SE_ASSERT( pSrcFile && iCount > 0 && pDstData );

    fread(pDstData, 2, iCount, pSrcFile);
#ifndef SE_BIG_ENDIAN
    SwapBytes(2, iCount, pDstData);
#endif

    return 2*iCount;
}
//----------------------------------------------------------------------------
int SESystem::Read4be(FILE* pSrcFile, int iCount, void* pDstData)
{
    SE_ASSERT( pSrcFile && iCount > 0 && pDstData );

    fread(pDstData, 4, iCount, pSrcFile);
#ifndef SE_BIG_ENDIAN
    SwapBytes(4, iCount, pDstData);
#endif

    return 4*iCount;
}
//----------------------------------------------------------------------------
int SESystem::Read8be(FILE* pSrcFile, int iCount, void* pDstData)
{
    SE_ASSERT( pSrcFile && iCount > 0 && pDstData );

    fread(pDstData, 8, iCount, pSrcFile);
#ifndef SE_BIG_ENDIAN
    SwapBytes(8, iCount, pDstData);
#endif

    return 8*iCount;
}
//----------------------------------------------------------------------------
int SESystem::Write2be(FILE* pDstFile, int iCount, const void* pSrcData)
{
    SE_ASSERT( pDstFile && iCount > 0 && pSrcData );

#ifndef SE_BIG_ENDIAN
    const short* pTempData = (const short*)pSrcData;
    for( int i = 0; i < iCount; i++ )
    {
        short sTemp = *pTempData++;
        SwapBytes(2, &sTemp);
        fwrite(&sTemp, 2, 1, pDstFile);
    }
#else
    fwrite(pSrcData, 2, iCount, pDstFile);
#endif

    return 2*iCount;
}
//----------------------------------------------------------------------------
int SESystem::Write4be(FILE* pDstFile, int iCount, const void* pSrcData)
{
    SE_ASSERT( pDstFile && iCount > 0 && pSrcData );

#ifndef SE_BIG_ENDIAN
    const int* pTempData = (const int*)pSrcData;
    for( int i = 0; i < iCount; i++ )
    {
        int iTemp = *pTempData++;
        SwapBytes(4, &iTemp);
        fwrite(&iTemp, 4, 1, pDstFile);
    }
#else
    fwrite(pSrcData, 4, iCount, pDstFile);
#endif

    return 4*iCount;
}
//----------------------------------------------------------------------------
int SESystem::Write8be(FILE* pDstFile, int iCount, const void* pSrcData)
{
    SE_ASSERT( pDstFile && iCount > 0 && pSrcData );

#ifndef SE_BIG_ENDIAN
    const double* pTempData = (const double*)pSrcData;
    for( int i = 0; i < iCount; i++ )
    {
        double dTemp = *pTempData++;
        SwapBytes(8, &dTemp);
        fwrite(&dTemp, 8, 1, pDstFile);
    }
#else
    fwrite(pSrcData, 8, iCount, pDstFile);
#endif

    return 8*iCount;
}
//----------------------------------------------------------------------------
const char* SESystem::GetPath(const char* pDirectory, 
    const char* pFileName)
{
#ifdef __APPLE__
    // An application-relative path is needed for the applications to be able
    // to find the input data sets.  Unfortunately, there is no exact way to
    // predict which directory the application is run from, since this depends
    // on whether it was launched by the Finder, Xcode, gdb, the console, etc.
    // To work around this, the following code switches to the application
    // directory.  Beware of your applications themselves changing directory,
    // which could cause the input data sets not to be found.

    CFBundleRef tempBundle;
    CFURLRef tempBundleURL;
    CFStringRef tempFSPath;

    tempBundle = CFBundleGetMainBundle();
    SE_ASSERT( tempBundle );

    tempBundleURL = CFBundleCopyBundleURL(tempBundle);
    SE_ASSERT( tempBundleURL );

    tempFSPath = CFURLCopyFileSystemPath(tempBundleURL, kCFURLPOSIXPathStyle);
    SE_ASSERT( tempFSPath );

    bool bRes = CFStringGetCString(tempFSPath, Path, SYSTEM_MAX_PATH, 
        kCFStringEncodingASCII);
    SE_ASSERT( bRes );
    (void)bRes;

    // with every CF function, there are two simple rules: 
    // if the function name includes "Create", you should release the result.
    // if the function name includes "Get", you should NOT release the result.
    // so, main bundle should not be released.
    CFRelease(tempBundleURL);
    CFRelease(tempFSPath);

    // Application bundle is the working directory we want to go.
#if 0
    // which directory we want to go?
    // 0 ${ApplicationPath}/build/Default/executableFile
    // 1 ${ApplicationPath}/build/Default
    // 2 ${ApplicationPath}/build
    // 3 ${ApplicationPath}
    char* pEnd;
    for( int i = 0; i < 1; i++ )
    {
        if( pEnd = strrchr(Path, '/') )
        {
            // go to ${ApplicationPath}/build/Default
            *pEnd = '\0';
        }
    }
#endif

    int iRes = chdir(Path);
    SE_ASSERT( iRes == 0 );
    (void)iRes;
#endif

    size_t uiDLength = strlen(pDirectory);
    size_t uiFLength = strlen(pFileName);
    if( uiDLength + uiFLength + 1 <= SYSTEM_MAX_PATH )
    {
        SESystem::Strcpy(Path, SYSTEM_MAX_PATH, pDirectory);
        SESystem::Strcat(Path, SYSTEM_MAX_PATH, pFileName);

        return Path;
    }

    return 0;
}
//----------------------------------------------------------------------------
void SESystem::Initialize()
{
    // 创建资源磁盘目录
    SE_ASSERT( Directories == 0 );

    Directories = SE_NEW std::vector<std::string>;

    const char* pSEPath = GetEnv("SE_PATH");
    if( pSEPath )
    {
        Strcpy(SE_PATH, SYSTEM_MAX_ENVVAR, pSEPath);
    }
    else
    {
        SE_PATH[0] = 0;
    }
}
//----------------------------------------------------------------------------
void SESystem::Terminate()
{
    SE_DELETE Directories;
    Directories = 0;
}
//----------------------------------------------------------------------------
int SESystem::GetDirectoryCount()
{
    if( !Directories )
    {
        Initialize();
    }

    return (int)Directories->size();
}
//----------------------------------------------------------------------------
const char* SESystem::GetDirectory(int i)
{
    if( !Directories )
    {
        Initialize();
    }

    if( 0 <= i && i < (int)Directories->size() )
    {
        return (*Directories)[i].c_str();
    }

    return 0;
}
//----------------------------------------------------------------------------
bool SESystem::InsertDirectory(const char* pDirectory)
{
    if( !Directories )
    {
        Initialize();
    }

    std::string NewDirectory = std::string(pDirectory) + "/";
    for( int i = 0; i < (int)Directories->size(); i++ )
    {
        if( NewDirectory == (*Directories)[i] )
        {
            return false;
        }
    }
    Directories->push_back(NewDirectory);

    return true;
}
//----------------------------------------------------------------------------
bool SESystem::RemoveDirectory(const char* pDirectory)
{
    if( !Directories )
    {
        Initialize();
    }

    std::string CurDirectory = std::string(pDirectory) + "/";
    std::vector<std::string>::iterator Iter = Directories->begin();
    for( /**/; Iter != Directories->end(); Iter++ )
    {
        if( CurDirectory == *Iter)
        {
            Directories->erase(Iter);

            return true;
        }
    }

    return false;
}
//----------------------------------------------------------------------------
void SESystem::RemoveAllDirectories()
{
    if( !Directories )
    {
        Initialize();
    }

    Directories->clear();
}
//----------------------------------------------------------------------------
const char* SESystem::GetPath(const char* pFileName, int eMode)
{
    if( !Directories )
    {
        Initialize();
    }

    for( int i = 0; i < (int)Directories->size(); i++ )
    {
        const char* pDecorated = SESystem::GetPath(
            (*Directories)[i].c_str(), pFileName);

        if( !pDecorated )
        {
            return 0;
        }

        FILE* pFile;
        if( eMode == SM_READ )
        {
            pFile = SESystem::Fopen(pDecorated, "r");
        }
        else if( eMode == SM_WRITE )
        {
            pFile = SESystem::Fopen(pDecorated, "w");
        }
        else // eMode == SM_READ_WRITE
        {
            pFile = SESystem::Fopen(pDecorated, "r+");
        }

        if( pFile )
        {
            SESystem::Fclose(pFile);

            return pDecorated;
        }
    }

    return 0;
}
//----------------------------------------------------------------------------
unsigned int SESystem::MakeRGB(unsigned char ucR, unsigned char ucG, 
    unsigned char ucB)
{
#ifdef SE_BIG_ENDIAN
    return (0xFF | (ucB << 8) | (ucG << 16) | (ucR << 24));
#else
    return (ucR | (ucG << 8) | (ucB << 16) | (0xFF << 24));
#endif
}
//----------------------------------------------------------------------------
unsigned int SESystem::MakeRGBA(unsigned char ucR, unsigned char ucG, 
    unsigned char ucB, unsigned char ucA)
{
#ifdef SE_BIG_ENDIAN
    return (ucA | (ucB << 8) | (ucG << 16) | (ucR << 24));
#else
    return (ucR | (ucG << 8) | (ucB << 16) | (ucA << 24));
#endif
}
//----------------------------------------------------------------------------
FILE* SESystem::Fopen(const char* pFileName, const char* pMode)
{
    // Do not use SE_ASSERT macro in this function.

#if defined(SE_USING_VC80) || defined(SE_USING_VC90)
    FILE* pFile;
    errno_t uiError = fopen_s(&pFile, pFileName, pMode);
    if( uiError == 0 )
    {
        return pFile;
    }
    else
    {
        return 0;
    }
#else
    return fopen(pFileName, pMode);
#endif
}
//----------------------------------------------------------------------------
int SESystem::Fprintf(FILE* pFile, const char* pFormat, ...)
{
    // Do not use SE_ASSERT macro in this function.

    if( !pFile || !pFormat )
    {
        return -1;
    }

    va_list Args;
    va_start(Args, pFormat);

#if defined(SE_USING_VC80) || defined(SE_USING_VC90)
    int iNumWritten = vfprintf_s(pFile, pFormat, Args);
#else
    int iNumWritten = vfprintf(pFile, pFormat, Args);
#endif

    va_end(Args);
    return iNumWritten;
}
//----------------------------------------------------------------------------
int SESystem::Fclose(FILE* pFile)
{
    // Do not use SE_ASSERT macro in this function.

    return fclose(pFile);
}
//----------------------------------------------------------------------------
const char* SESystem::GetEnv(const char* pEnvVarName)
{
#if defined(SE_USING_VC80) || defined(SE_USING_VC90)
   size_t uiRequiredSize;
   errno_t uiError = getenv_s(&uiRequiredSize, 0, 0, pEnvVarName);
   if( uiError > 0 )
   {
       return 0;
   }
   getenv_s(&uiRequiredSize, EnvVar, SYSTEM_MAX_ENVVAR, pEnvVarName);
#else
    char* pEnvVar = getenv(pEnvVarName);
    if( pEnvVar == 0 )
    {
        return 0;
    }
    SESystem::Strcpy(EnvVar, SYSTEM_MAX_ENVVAR, getenv(pEnvVarName));
#endif
    return EnvVar;
}
//----------------------------------------------------------------------------
void* SESystem::Memcpy(void* pDst, size_t uiDstSize, const void* pSrc, 
    size_t uiSrcSize)
{
#if defined(SE_USING_VC80) || defined(SE_USING_VC90)
    errno_t uiError = memcpy_s(pDst, uiDstSize, pSrc, uiSrcSize);
    if( uiError == 0 )
    {
        return pDst;
    }
    else
    {
        return 0;
    }
#else
    if( !pDst || uiDstSize == 0 || !pSrc || uiSrcSize == 0 )
    {
        // 与memcpy_s的行为一致,支持更安全的校验
        return 0;
    }

    if( uiSrcSize > uiDstSize )
    {
        // 源内存大于目标内存,拷贝失败,与memcpy_s一致
        return 0;
    }
    memcpy(pDst, pSrc, uiSrcSize);
    return pDst;
#endif
}
//----------------------------------------------------------------------------
int SESystem::Sprintf(char* pDst, size_t uiDstSize, const char* pFormat, 
    ...)
{
    if( !pDst || uiDstSize == 0 || !pFormat )
    {
        return -1;
    }

    va_list Args;
    va_start(Args, pFormat);

#if defined(SE_USING_VC80) || defined(SE_USING_VC90)
    int iNumWritten = vsprintf_s(pDst, uiDstSize, pFormat, Args);
#else
    int iNumWritten = vsprintf(pDst, pFormat, Args);
#endif

    va_end(Args);
    return iNumWritten;
}
//----------------------------------------------------------------------------
char* SESystem::Strcpy(char* pDst, size_t uiDstSize, const char* pSrc)
{
#if defined(SE_USING_VC80) || defined(SE_USING_VC90)
    errno_t uiError = strcpy_s(pDst, uiDstSize, pSrc);
    if( uiError == 0 )
    {
        return pDst;
    }
    else
    {
        return 0;
    }
#else
    if( !pDst || uiDstSize == 0 || !pSrc )
    {
        // 与strcpy_s行为一致
        return 0;
    }

    size_t uiSrcLen = strlen(pSrc);
    if( uiSrcLen + 1 > uiDstSize )
    {
        return 0;
    }
    strncpy(pDst, pSrc, uiSrcLen);
    pDst[uiSrcLen] = 0;

    return pDst;
#endif
}
//----------------------------------------------------------------------------
char* SESystem::Strcat(char* pDst, size_t uiDstSize, const char* pSrc)
{
#if defined(SE_USING_VC80) || defined(SE_USING_VC90)
    errno_t uiError = strcat_s(pDst, uiDstSize, pSrc);
    if( uiError == 0 )
    {
        return pDst;
    }
    else
    {
        return 0;
    }
#else
    if( !pDst || uiDstSize == 0 || !pSrc )
    {
        return 0;
    }

    size_t uiSrcLen = strlen(pSrc);
    size_t uiDstLen = strlen(pDst);
    size_t uiSumLen = uiSrcLen + uiDstLen;
    if( uiSumLen + 1 > uiDstSize )
    {
        return 0;
    }
    strncat(pDst, pSrc, uiSrcLen);
    pDst[uiSumLen] = 0;

    return pDst;
#endif
}
//----------------------------------------------------------------------------
char* SESystem::Strncpy(char* pDst, size_t uiDstSize, const char* pSrc, 
    size_t uiSrcSize)
{
#if defined(SE_USING_VC80) || defined(SE_USING_VC90)
    errno_t uiError = strncpy_s(pDst, uiDstSize, pSrc, uiSrcSize);
    if( uiError == 0 )
    {
        return pDst;
    }
    else
    {
        return 0;
    }
#else
    if( !pDst || uiDstSize == 0 || !pSrc || uiSrcSize == 0 )
    {
        return 0;
    }

    if( uiSrcSize + 1 > uiDstSize )
    {
        return 0;
    }
    strncpy(pDst, pSrc, uiSrcSize);

    return pDst;
#endif
}
//----------------------------------------------------------------------------
char* SESystem::Strtok(char* pToken, const char* pDelimiters, 
    char*& rpNextToken)
{
#if defined(SE_USING_VC80) || defined(SE_USING_VC90)
    return strtok_s(pToken, pDelimiters, &rpNextToken);
#else
    (void)rpNextToken;  // 避免release编译时产生warning
    return strtok(pToken, pDelimiters);
#endif
}
//----------------------------------------------------------------------------

#ifdef SE_IS_WINDOWS
#include <windows.h>
#endif

//----------------------------------------------------------------------------
int SESystem::GetCPUCoresCount()
{
#if defined(SE_IS_WINDOWS)

    if( CPUCoresCount == 0 )
    {
        SYSTEM_INFO sysinfo;
        GetSystemInfo(&sysinfo);
        CPUCoresCount = sysinfo.dwNumberOfProcessors;
    }

    return CPUCoresCount;

#else
    // TODO:
    SE_ASSERT( false );
    return 0;
#endif
}
//----------------------------------------------------------------------------