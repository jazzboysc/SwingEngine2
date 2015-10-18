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
#include "SEObject.h"
#include "SEStream.h"

using namespace Swing;

const char* SEStream::ms_pTopLevel = "Top Level";

//----------------------------------------------------------------------------
SEStream::SEStream()
    :
    m_Map(1024)
{
    m_iBufferSize = 0;
    m_iBufferNext = 0;
    m_pBuffer = 0;
}
//----------------------------------------------------------------------------
SEStream::~SEStream()
{
    m_Map.RemoveAll();
    m_Ordered.clear();
    RemoveAll();
}
//----------------------------------------------------------------------------
bool SEStream::Insert(SEObject* pObject)
{
    if( pObject )
    {
        // 只能插入一次
        for( int i = 0; i < (int)m_pTopLevel.size(); i++ )
        {
            if( pObject == m_pTopLevel[i] )
            {
                return false;
            }
        }

        m_pTopLevel.push_back(pObject);
        pObject->IncrementReferenceCount();

        return true;
    }

    return false;
}
//----------------------------------------------------------------------------
bool SEStream::Remove(SEObject* pObject)
{
    if( pObject )
    {
        std::vector<SEObject*>::iterator pIter = m_pTopLevel.begin();
        for( /**/; pIter != m_pTopLevel.end(); pIter++ )
        {
            if( pObject == *pIter )
            {
                m_pTopLevel.erase(pIter);
                pObject->DecrementReferenceCount();

                return true;
            }
        }
    }

    return false;
}
//----------------------------------------------------------------------------
void SEStream::RemoveAll()
{
    for( int i = 0; i < (int)m_pTopLevel.size(); i++ )
    {
        m_pTopLevel[i]->DecrementReferenceCount();
    }

    m_pTopLevel.clear();
}
//----------------------------------------------------------------------------
int SEStream::GetObjectCount()
{
    return (int)m_pTopLevel.size();
}
//----------------------------------------------------------------------------
SEObject* SEStream::GetObjectAt(int i) const
{
    if( 0 <= i && i < (int)m_pTopLevel.size() )
    {
        return m_pTopLevel[i];
    }

    return 0;
}
//----------------------------------------------------------------------------
bool SEStream::IsTopLevel(const SEObject* pObject)
{
    for( int i = 0; i < (int)m_pTopLevel.size(); i++ )
    {
        if( pObject == m_pTopLevel[i] )
        {
            return true;
        }
    }

    return false;
}
//----------------------------------------------------------------------------
bool SEStream::Load(char* pBuffer, int iSize)
{
    if( !SEObject::msFactoryFunctionTable )
    {
        // 必须首先创建工厂函数表
        SE_ASSERT( false );

        return false;
    }

    m_Map.RemoveAll();
    m_Ordered.clear();
    // 清除之前所有top level object
    RemoveAll();

    // 初始化内部stream buffer,m_iBufferNext已经初始化为0
    m_iBufferSize = iSize;
    m_pBuffer = pBuffer;

    // 装载unique object列表
    std::string StrTopLevel(ms_pTopLevel);
    SEObject* pObject;
    while( m_iBufferNext < m_iBufferSize )
    {
        // 期望遇到"Top Level"或RTTI name
        std::string StrTemp;
        Read(StrTemp);
        bool bTopLevel = (StrTemp == StrTopLevel);
        if( bTopLevel )
        {
            // 期望读取RTTI name
            Read(StrTemp);
        }

        // RTTI name必须是"Swing.xxx"
        SE_ASSERT( ((int)StrTemp.length()) >= 7 );

        // 根据RTTI name,找到即将读取的object的工厂函数
        FactoryFunction* pFactory = SEObject::msFactoryFunctionTable->Find(StrTemp);
        if( !pFactory )
        {
            // 无法找到该类型的工厂函数,
            // 必须确保该类调用过SE_REGISTER_STREAM(someclass)
            SE_ASSERT( false );

            return false;
        }

        // 装载该object
        pObject = (*pFactory)(*this);

        // 为应用程序使用而追踪所有top level object,
        // 加入top level对象表
        if( bTopLevel )
        {
            Insert(pObject);
        }
    }

    // link阶段,为所有载入的对象建立连接关系,构成原始scene graph,
    // 维护一个允许应用程序使用的对象列表,
    // 线性迭代唯一对象hash表,为所有对象建立连接关系
    SELink** ppLink = m_Map.GetFirst(&pObject);
    while( ppLink )
    {
        pObject = (*ppLink)->GetObject();  // 获取该对象当前内存地址
        pObject->Link(*this, *ppLink);     // 为该对象建立连接关系
        m_Ordered.push_back(pObject);      // 此时的顺序为迭代hash表的访问顺序
        ppLink = m_Map.GetNext(&pObject);
    }

    // 不再需要link数据
    ppLink = m_Map.GetFirst(&pObject);
    while( ppLink )
    {
        SE_DELETE *ppLink;
        ppLink = m_Map.GetNext(&pObject);
    }

    // 重置内部数据
    m_pBuffer = 0;
    m_iBufferSize = 0;
    m_iBufferNext = 0;

    return true;
}
//----------------------------------------------------------------------------
bool SEStream::Save(char*& rpBuffer, int& riSize)
{
    m_Map.RemoveAll();
    m_Ordered.clear();

    // register阶段,
    // 建立唯一对象列表
    int i;
    for( i = 0; i < (int)m_pTopLevel.size(); i++ )
    {
        m_pTopLevel[i]->Register(*this);
    }

    // 统计所有对象的磁盘字节数
    std::string StrTopLevel(ms_pTopLevel);
    int iTLGetDiskUsed = sizeof(int) + (int)StrTopLevel.length();
    m_iBufferSize = ((int)m_pTopLevel.size())*iTLGetDiskUsed;
    for( i = 0; i < (int)m_Ordered.size(); i++ )
    {
        m_iBufferSize += m_Ordered[i]->GetDiskUsed(SEStreamVersion::CURRENT);
    }

    // 申请所需内存buffer
    m_pBuffer = SE_NEW char[m_iBufferSize];
    m_iBufferNext = 0;

    // 保存唯一对象列表到内存buffer中
    for( i = 0; i < (int)m_Ordered.size(); i++ )
    {
        const SEObject* pObject = m_Ordered[i];
        if( IsTopLevel(pObject) )
        {
            Write(StrTopLevel);
        }
        pObject->Save(*this);
    }

    // 确保内存buffer填充字节总数与对象报告总数吻合
    SE_ASSERT( m_iBufferNext == m_iBufferSize );
    if( m_iBufferNext != m_iBufferSize )
    {
        // 出现不吻合,
        // 要么是某对象错误报告了所需磁盘字节数(通过GetDiskUsed函数),
        // 要么是某对象写入成员数据时出错
        SE_ASSERT( false );

        SE_DELETE[] m_pBuffer;
        rpBuffer = 0;
        riSize = 0;

        return false;
    }

    // 把内存buffer所属权传递给本函数调用者
    rpBuffer = m_pBuffer;
    riSize = m_iBufferSize;

    // 重置内部数据
    m_pBuffer = 0;
    m_iBufferSize = 0;
    m_iBufferNext = 0;

    return true;
}
//----------------------------------------------------------------------------
bool SEStream::Load(const char* pFileName)
{
    // 从磁盘载入到内存
    char* pBuffer;
    int iSize;
    if( !SESystem::Load(pFileName, pBuffer, iSize) )
    {
        return false;
    }

    // 获取文件版本
    if( iSize < SEStreamVersion::LENGTH )
    {
        // 文件不存在或没有足够大小
        SE_DELETE[] pBuffer;

        return false;
    }
    m_Version = SEStreamVersion(pBuffer);
    if( !m_Version.IsValid() )
    {
        // TO DO.  This should not abort if the input stream version is less
        // than current version.

        SE_DELETE[] pBuffer;

        return false;
    }

    // 偏移到数据区,重构scene graph
    iSize -= SEStreamVersion::LENGTH;
    if( !Load(pBuffer+SEStreamVersion::LENGTH, iSize) )
    {
        SE_DELETE[] pBuffer;

        return false;
    }

    SE_DELETE[] pBuffer;

    return true;
}
//----------------------------------------------------------------------------
bool SEStream::Save(const char* pFileName)
{
    // 保存文件版本到磁盘
    if( !SESystem::Save(pFileName, SEStreamVersion::LABEL, 
        SEStreamVersion::LENGTH) )
    {
        return false;
    }

    // 分解scene graph并存储到内存buffer
    char* pBuffer;
    int iSize;
    if( !Save(pBuffer, iSize) )
    {
        return false;
    }

    // 把scene graph从内存存储到磁盘
    if( !SESystem::Append(pFileName, pBuffer, iSize) )
    {
        SE_DELETE[] pBuffer;

        return false;
    }

    SE_DELETE[] pBuffer;

    return true;
}
//----------------------------------------------------------------------------
bool SEStream::InsertInMap(SEObject* pObject, SELink* pLink)
{
    return m_Map.Insert(pObject, pLink);
}
//----------------------------------------------------------------------------
SEObject* SEStream::GetFromMap(SEObject* pLinkID)
{
    SELink** ppLink = m_Map.Find(pLinkID);

    return (ppLink ? (*ppLink)->GetObject() : 0);
}
//----------------------------------------------------------------------------
void SEStream::InsertInOrdered(SEObject* pObject)
{
    m_Ordered.push_back(pObject);
}
//----------------------------------------------------------------------------
int SEStream::GetDiskUsed() const
{
    int i;
    for( i = 0; i < (int)m_pTopLevel.size(); i++ )
    {
        m_pTopLevel[i]->Register((SEStream&)*this);
    }

    int iSize = 0;
    for( i = 0; i < (int)m_Ordered.size(); i++ )
    {
        iSize += m_Ordered[i]->GetDiskUsed(SEStreamVersion::CURRENT);
    }

    m_Map.RemoveAll();
    m_Ordered.clear();

    return iSize;
}
//----------------------------------------------------------------------------
SEStream::SELink::SELink(SEObject* pObject)
{
    mObject = pObject;
    m_iCurrent = 0;
}
//----------------------------------------------------------------------------
void SEStream::SELink::Add(SEObject* pLinkID)
{
    m_LinkID.push_back(pLinkID);
}
//----------------------------------------------------------------------------
void SEStream::SELink::SetObject(SEObject* pObject)
{
    mObject = pObject;
}
//----------------------------------------------------------------------------
SEObject* SEStream::SELink::GetObject()
{
    return mObject;
}
//----------------------------------------------------------------------------
int SEStream::SELink::GetCount() const
{
    return (int)m_LinkID.size();
}
//----------------------------------------------------------------------------
SEObject* SEStream::SELink::GetLinkID()
{
    SE_ASSERT( m_iCurrent < (int)m_LinkID.size() );

    return m_LinkID[m_iCurrent++];
}
//----------------------------------------------------------------------------
void SEStream::Read(SEObject*& rpValue)
{
    // To support 32-bit and 64-bit systems.
    char acTemp[8];

    m_iBufferNext += SESystem::Read8le(m_pBuffer+m_iBufferNext, 1, 
        &acTemp);
    SE_ASSERT( m_iBufferNext <= m_iBufferSize );

#ifdef SE_BIG_ENDIAN
    memcpy(&rpValue, acTemp+8-sizeof(SEObject*), sizeof(SEObject*));
#else
    memcpy(&rpValue, acTemp, sizeof(SEObject*));
#endif
}
//----------------------------------------------------------------------------
void SEStream::Read(bool& rValue)
{
    char* pSrc = m_pBuffer + m_iBufferNext;
    m_iBufferNext++;

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );

    rValue = (*pSrc != 0 ? true : false);
}
//----------------------------------------------------------------------------
void SEStream::Read(int iCount, bool* pValue)
{
    if( iCount > 0 )
    {
        char* pSrc = m_pBuffer + m_iBufferNext;
        m_iBufferNext += iCount;

        SE_ASSERT( m_iBufferNext <= m_iBufferSize );

        for( int i = 0; i < iCount; i++ )
        {
            pValue[i] = (pSrc[i] != 0 ? true : false);
        }
    }
}
//----------------------------------------------------------------------------
void SEStream::Read(char& rValue)
{
    char* pSrc = m_pBuffer + m_iBufferNext;
    m_iBufferNext++;

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );

    rValue = *pSrc;
}
//----------------------------------------------------------------------------
void SEStream::Read(int iCount, char* pValue)
{
    if( iCount > 0 )
    {
        char* pSrc = m_pBuffer + m_iBufferNext;
        m_iBufferNext += iCount;

        SE_ASSERT( m_iBufferNext <= m_iBufferSize );

        size_t uiSize = (size_t)iCount;
        SESystem::Memcpy(pValue, uiSize, pSrc, uiSize);
    }
}
//----------------------------------------------------------------------------
void SEStream::Read(unsigned char& rValue)
{
    unsigned char* pSrc = (unsigned char*)(m_pBuffer + m_iBufferNext);
    m_iBufferNext++;

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );

    rValue = *pSrc;
}
//----------------------------------------------------------------------------
void SEStream::Read(int iCount, unsigned char* pValue)
{
    if( iCount > 0 )
    {
        unsigned char* pSrc = (unsigned char*)(m_pBuffer + m_iBufferNext);
        m_iBufferNext += iCount;

        SE_ASSERT( m_iBufferNext <= m_iBufferSize );

        size_t uiSize = (size_t)iCount;
        SESystem::Memcpy(pValue, uiSize, pSrc, uiSize);
    }
}
//----------------------------------------------------------------------------
void SEStream::Read (short& rValue)
{
    m_iBufferNext += SESystem::Read2le(m_pBuffer+m_iBufferNext,1,&rValue);
    SE_ASSERT(m_iBufferNext <= m_iBufferSize);
}
//----------------------------------------------------------------------------
void SEStream::Read (int iCount, short* pValue)
{
    m_iBufferNext += SESystem::Read2le(m_pBuffer+m_iBufferNext,iCount,
        pValue);
    SE_ASSERT(m_iBufferNext <= m_iBufferSize);
}
//----------------------------------------------------------------------------
void SEStream::Read(unsigned short& rValue)
{
    m_iBufferNext += SESystem::Read2le(m_pBuffer+m_iBufferNext, 1, &rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(int iCount, unsigned short* pValue)
{
    m_iBufferNext += SESystem::Read2le(m_pBuffer+m_iBufferNext, iCount, 
        pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(int& rValue)
{
    m_iBufferNext += SESystem::Read4le(m_pBuffer+m_iBufferNext, 1, &rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(int iCount, int* pValue)
{
    m_iBufferNext += SESystem::Read4le(m_pBuffer+m_iBufferNext, iCount, 
        pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(unsigned int& rValue)
{
    m_iBufferNext += SESystem::Read4le(m_pBuffer+m_iBufferNext, 1, &rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(int iCount, unsigned int* pValue)
{
    m_iBufferNext += SESystem::Read4le(m_pBuffer+m_iBufferNext, iCount, 
        pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(long& rValue)
{
    m_iBufferNext += SESystem::Read4le(m_pBuffer+m_iBufferNext, 1, &rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(int iCount, long* pValue)
{
    m_iBufferNext += SESystem::Read4le(m_pBuffer+m_iBufferNext, iCount, 
        pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(unsigned long& rValue)
{
    m_iBufferNext += SESystem::Read4le(m_pBuffer+m_iBufferNext, 1, &rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(int iCount, unsigned long* pValue)
{
    m_iBufferNext += SESystem::Read4le(m_pBuffer+m_iBufferNext, iCount, 
        pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(float& rValue)
{
    m_iBufferNext += SESystem::Read4le(m_pBuffer+m_iBufferNext, 1, &rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(int iCount, float* pValue)
{
    m_iBufferNext += SESystem::Read4le(m_pBuffer+m_iBufferNext, iCount, 
        pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(double& rValue)
{
    m_iBufferNext += SESystem::Read8le(m_pBuffer+m_iBufferNext, 1, &rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(int iCount, double* pValue)
{
    m_iBufferNext += SESystem::Read8le(m_pBuffer+m_iBufferNext, iCount, 
        pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(std::string& rValue)
{
    int iLength;
    Read(iLength);

    if( iLength > 0 )
    {
        char* pText = m_pBuffer + m_iBufferNext;
        m_iBufferNext += iLength;
        SE_ASSERT( m_iBufferNext <= m_iBufferSize );

        rValue.assign(pText, iLength);
    }
    else
    {
        rValue.clear();
    }
}
//----------------------------------------------------------------------------
void SEStream::Read(int iCount, std::string* pValue)
{
    for( int i = 0; i < iCount; i++ )
    {
        Read(pValue[i]);
    }
}
//----------------------------------------------------------------------------
void SEStream::Read(SEColorRGB& rValue)
{
    m_iBufferNext += SESystem::Read4le(m_pBuffer+m_iBufferNext, 3, 
        (float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(int iCount, SEColorRGB* pValue)
{
    m_iBufferNext += SESystem::Read4le(m_pBuffer+m_iBufferNext, 3*iCount, 
        (float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(SEColorRGBA& rValue)
{
    m_iBufferNext += SESystem::Read4le(m_pBuffer+m_iBufferNext, 4, 
        (float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(int iCount, SEColorRGBA* pValue)
{
    m_iBufferNext += SESystem::Read4le(m_pBuffer+m_iBufferNext, 4*iCount, 
        (float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(SEFloat4& rValue)
{
    m_iBufferNext += SESystem::Read4le(m_pBuffer+m_iBufferNext, 4, 
        (float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(int iCount, SEFloat4* pValue)
{
    m_iBufferNext += SESystem::Read4le(m_pBuffer+m_iBufferNext, 4*iCount, 
        (float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(SEMatrix2f& rValue)
{
    m_iBufferNext += SESystem::Read4le(m_pBuffer+m_iBufferNext, 4, 
        (float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(int iCount, SEMatrix2f* pValue)
{
    m_iBufferNext += SESystem::Read4le(m_pBuffer+m_iBufferNext, 4*iCount, 
        (float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(SEMatrix3f& rValue)
{
    m_iBufferNext += SESystem::Read4le(m_pBuffer+m_iBufferNext, 9, 
        (float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(int iCount, SEMatrix3f* pValue)
{
    m_iBufferNext += SESystem::Read4le(m_pBuffer+m_iBufferNext, 
        9*iCount, (float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(SEMatrix4f& rValue)
{
    m_iBufferNext += SESystem::Read4le(m_pBuffer+m_iBufferNext, 16, 
        (float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(int iCount, SEMatrix4f* pValue)
{
    m_iBufferNext += SESystem::Read4le(m_pBuffer+m_iBufferNext, 16*iCount, 
        (float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(SEPlane3f& rValue)
{
    Read(rValue.Normal);
    Read(rValue.Constant);
}
//----------------------------------------------------------------------------
void SEStream::Read(int iCount, SEPlane3f* pValue)
{
    for( int i = 0; i < iCount; i++ )
    {
        Read(pValue[i].Normal);
        Read(pValue[i].Constant);
    }
}
//----------------------------------------------------------------------------
void SEStream::Read(SEQuaternionf& rValue)
{
    m_iBufferNext += SESystem::Read4le(m_pBuffer+m_iBufferNext, 4, 
        (float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(int iCount, SEQuaternionf* pValue)
{
    m_iBufferNext += SESystem::Read4le(m_pBuffer+m_iBufferNext, 4*iCount, 
        (float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(SEVector2f& rValue)
{
    m_iBufferNext += SESystem::Read4le(m_pBuffer+m_iBufferNext, 2, 
        (float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(int iCount, SEVector2f* pValue)
{
    m_iBufferNext += SESystem::Read4le(m_pBuffer+m_iBufferNext, 2*iCount, 
        (float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(SEVector3f& rValue)
{
    m_iBufferNext += SESystem::Read4le(m_pBuffer+m_iBufferNext, 3, 
        (float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(int iCount, SEVector3f* pValue)
{
    m_iBufferNext += SESystem::Read4le(m_pBuffer+m_iBufferNext, 3*iCount, 
        (float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(SEVector4f& rValue)
{
    m_iBufferNext += SESystem::Read4le(m_pBuffer+m_iBufferNext, 4, 
        (float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(int iCount, SEVector4f* pValue)
{
    m_iBufferNext += SESystem::Read4le(m_pBuffer+m_iBufferNext, 4*iCount, 
        (float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Read(SETransformation& rValue)
{
    Read(rValue.m_Matrix);
    Read(rValue.m_Translate);
    Read(rValue.m_Scale);
    Read(rValue.m_bIsIdentity);
    Read(rValue.m_bIsSRMatrix);
    Read(rValue.m_bIsUniformScale);
}
//----------------------------------------------------------------------------
void SEStream::Read(int iCount, SETransformation* pValue)
{
    for( int i = 0; i < iCount; i++ )
    {
        Read(pValue[i].m_Matrix);
        Read(pValue[i].m_Translate);
        Read(pValue[i].m_Scale);
        Read(pValue[i].m_bIsIdentity);
        Read(pValue[i].m_bIsSRMatrix);
        Read(pValue[i].m_bIsUniformScale);
    }
}
//----------------------------------------------------------------------------
void SEStream::Write(const SEObject* pValue)
{
    // To support 32-bit and 64-bit systems.
    char acTemp[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    memcpy(acTemp, &pValue, sizeof(SEObject*));
    m_iBufferNext += SESystem::Write8le(m_pBuffer+m_iBufferNext, 1, 
        &acTemp);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(bool bValue)
{
    char* pDst = m_pBuffer + m_iBufferNext;
    m_iBufferNext++;

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );

    *pDst = (bValue ? 1 : 0);
}
//----------------------------------------------------------------------------
void SEStream::Write(int iCount, const bool* pValue)
{
    if( iCount > 0 )
    {
        char* pDst = m_pBuffer + m_iBufferNext;
        m_iBufferNext += iCount;

        SE_ASSERT( m_iBufferNext <= m_iBufferSize );

        for( int i = 0; i < iCount; i++ )
        {
            pDst[i] = (pValue[i] ? 1 : 0);
        }
    }
}
//----------------------------------------------------------------------------
void SEStream::Write(char cValue)
{
    char* pDst = m_pBuffer + m_iBufferNext;
    m_iBufferNext++;

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );

    *pDst = cValue;
}
//----------------------------------------------------------------------------
void SEStream::Write(int iCount, const char* pValue)
{
    if( iCount > 0 )
    {
        char* pDst = m_pBuffer + m_iBufferNext;
        m_iBufferNext += iCount;

        SE_ASSERT( m_iBufferNext <= m_iBufferSize );

        size_t uiSize = (size_t)iCount;
        SESystem::Memcpy(pDst, uiSize, pValue, uiSize);
    }
}
//----------------------------------------------------------------------------
void SEStream::Write(unsigned char ucValue)
{
    unsigned char* pDst = (unsigned char*)(m_pBuffer + m_iBufferNext);
    m_iBufferNext++;

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );

    *pDst = ucValue;
}
//----------------------------------------------------------------------------
void SEStream::Write(int iCount, const unsigned char* pValue)
{
    if( iCount > 0 )
    {
        unsigned char* pDst = (unsigned char*)(m_pBuffer + m_iBufferNext);
        m_iBufferNext += iCount;

        SE_ASSERT( m_iBufferNext <= m_iBufferSize );

        size_t uiSize = (size_t)iCount;
        SESystem::Memcpy(pDst, uiSize, pValue, uiSize);
    }
}
//----------------------------------------------------------------------------
void SEStream::Write(short sValue)
{
    m_iBufferNext += SESystem::Write2le(m_pBuffer+m_iBufferNext, 1, 
        &sValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(int iCount, const short* pValue)
{
    m_iBufferNext += SESystem::Write2le(m_pBuffer+m_iBufferNext, iCount, 
        pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(unsigned short usValue)
{
    m_iBufferNext += SESystem::Write2le(m_pBuffer+m_iBufferNext, 1, 
        &usValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(int iCount, const unsigned short* pValue)
{
    m_iBufferNext += SESystem::Write2le(m_pBuffer+m_iBufferNext, iCount, 
        pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(int iValue)
{
    m_iBufferNext += SESystem::Write4le(m_pBuffer+m_iBufferNext, 1, 
        &iValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(int iCount, const int* pValue)
{
    m_iBufferNext += SESystem::Write4le(m_pBuffer+m_iBufferNext, iCount, 
        pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(unsigned int uiValue)
{
    m_iBufferNext += SESystem::Write4le(m_pBuffer+m_iBufferNext, 1, 
        &uiValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(int iCount, const unsigned int* pValue)
{
    m_iBufferNext += SESystem::Write4le(m_pBuffer+m_iBufferNext, iCount, 
        pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(long lValue)
{
    m_iBufferNext += SESystem::Write4le(m_pBuffer+m_iBufferNext, 1, 
        &lValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(int iCount, const long* pValue)
{
    m_iBufferNext += SESystem::Write4le(m_pBuffer+m_iBufferNext, iCount, 
        pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(unsigned long ulValue)
{
    m_iBufferNext += SESystem::Write4le(m_pBuffer+m_iBufferNext, 1, 
        &ulValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(int iCount, const unsigned long* pValue)
{
    m_iBufferNext += SESystem::Write4le(m_pBuffer+m_iBufferNext, iCount, 
        pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(float fValue)
{
    m_iBufferNext += SESystem::Write4le(m_pBuffer+m_iBufferNext, 1, 
        &fValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(int iCount, const float* pValue)
{
    m_iBufferNext += SESystem::Write4le(m_pBuffer+m_iBufferNext, iCount, 
        pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(double dValue)
{
    m_iBufferNext += SESystem::Write8le(m_pBuffer+m_iBufferNext, 1, 
        &dValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(int iCount, const double* pValue)
{
    m_iBufferNext += SESystem::Write8le(m_pBuffer+m_iBufferNext, iCount, 
        pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(const std::string& rValue)
{
    Write((int)rValue.length());
    Write((int)rValue.length(), rValue.c_str());
}
//----------------------------------------------------------------------------
void SEStream::Write(int iCount, const std::string* pValue)
{
    for( int i = 0; i < iCount; i++ )
    {
        Write(pValue[i]);
    }
}
//----------------------------------------------------------------------------
void SEStream::Write(const SEColorRGB& rValue)
{
    m_iBufferNext += SESystem::Write4le(m_pBuffer+m_iBufferNext, 3, 
        (const float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(int iCount, const SEColorRGB* pValue)
{
    m_iBufferNext += SESystem::Write4le(m_pBuffer+m_iBufferNext, 3*iCount, 
        (const float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(const SEColorRGBA& rValue)
{
    m_iBufferNext += SESystem::Write4le(m_pBuffer+m_iBufferNext, 4, 
        (const float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(int iCount, const SEColorRGBA* pValue)
{
    m_iBufferNext += SESystem::Write4le(m_pBuffer+m_iBufferNext, 4*iCount, 
        (const float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(const SEFloat4& rValue)
{
    m_iBufferNext += SESystem::Write4le(m_pBuffer+m_iBufferNext, 4, 
        (const float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(int iCount, const SEFloat4* pValue)
{
    m_iBufferNext += SESystem::Write4le(m_pBuffer+m_iBufferNext, 4*iCount, 
        (const float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(const SEMatrix2f& rValue)
{
    m_iBufferNext += SESystem::Write4le(m_pBuffer+m_iBufferNext, 4, 
        (const float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(int iCount, const SEMatrix2f* pValue)
{
    m_iBufferNext += SESystem::Write4le(m_pBuffer+m_iBufferNext, 4*iCount, 
        (const float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(const SEMatrix3f& rValue)
{
    m_iBufferNext += SESystem::Write4le(m_pBuffer+m_iBufferNext, 9, 
        (const float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(int iCount, const SEMatrix3f* pValue)
{
    m_iBufferNext += SESystem::Write4le(m_pBuffer+m_iBufferNext, 
        9*iCount, (const float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(const SEMatrix4f& rValue)
{
    m_iBufferNext += SESystem::Write4le(m_pBuffer+m_iBufferNext, 16, 
        (const float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(int iCount, const SEMatrix4f* pValue)
{
    m_iBufferNext += SESystem::Write4le(m_pBuffer+m_iBufferNext, 16*iCount, 
        (const float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(const SEPlane3f& rValue)
{
    Write(rValue.Normal);
    Write(rValue.Constant);
}
//----------------------------------------------------------------------------
void SEStream::Write(int iCount, const SEPlane3f* pValue)
{
    for( int i = 0; i < iCount; i++ )
    {
        Write(pValue[i].Normal);
        Write(pValue[i].Constant);
    }
}
//----------------------------------------------------------------------------
void SEStream::Write(const SEQuaternionf& rValue)
{
    m_iBufferNext += SESystem::Write4le(m_pBuffer+m_iBufferNext, 4, 
        (const float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(int iCount, const SEQuaternionf* pValue)
{
    m_iBufferNext += SESystem::Write4le(m_pBuffer+m_iBufferNext, 4*iCount, 
        (const float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(const SEVector2f& rValue)
{
    m_iBufferNext += SESystem::Write4le(m_pBuffer+m_iBufferNext, 2, 
        (const float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(int iCount, const SEVector2f* pValue)
{
    m_iBufferNext += SESystem::Write4le(m_pBuffer+m_iBufferNext, 2*iCount, 
        (const float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(const SEVector3f& rValue)
{
    m_iBufferNext += SESystem::Write4le(m_pBuffer+m_iBufferNext, 3, 
        (const float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(int iCount, const SEVector3f* pValue)
{
    m_iBufferNext += SESystem::Write4le(m_pBuffer+m_iBufferNext, 3*iCount, 
        (const float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(const SEVector4f& rValue)
{
    m_iBufferNext += SESystem::Write4le(m_pBuffer+m_iBufferNext, 4, 
        (const float*)rValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(int iCount, const SEVector4f* pValue)
{
    m_iBufferNext += SESystem::Write4le(m_pBuffer+m_iBufferNext, 4*iCount, 
        (const float*)pValue);

    SE_ASSERT( m_iBufferNext <= m_iBufferSize );
}
//----------------------------------------------------------------------------
void SEStream::Write(const SETransformation& rValue)
{
    Write(rValue.m_Matrix);
    Write(rValue.m_Translate);
    Write(rValue.m_Scale);
    Write(rValue.m_bIsIdentity);
    Write(rValue.m_bIsSRMatrix);
    Write(rValue.m_bIsUniformScale);
}
//----------------------------------------------------------------------------
void SEStream::Write(int iCount, const SETransformation* pValue)
{
    for( int i = 0; i < iCount; i++ )
    {
        Write(pValue[i].m_Matrix);
        Write(pValue[i].m_Translate);
        Write(pValue[i].m_Scale);
        Write(pValue[i].m_bIsIdentity);
        Write(pValue[i].m_bIsSRMatrix);
        Write(pValue[i].m_bIsUniformScale);
    }
}
//----------------------------------------------------------------------------
