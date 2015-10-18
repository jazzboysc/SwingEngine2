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

#ifndef Swing_Stream_H
#define Swing_Stream_H

#include "SECoreLIB.h"
#include "SESystem.h"
#include "SEStreamVersion.h"
#include "SEStreamMCR.h"
#include "SESmartPointer.h"
#include "SEHashTable.h"

// 需要被streaming的class成员
#include "SEColorRGB.h"
#include "SEColorRGBA.h"
#include "SEFloat4.h"
#include "SEMatrix2.h"
#include "SEMatrix3.h"
#include "SEMatrix4.h"
#include "SEPlane3.h"
#include "SEQuaternion.h"
#include "SETransformation.h"
#include "SEVector2.h"
#include "SEVector3.h"
#include "SEVector4.h"

namespace Swing
{

class SEObject;

//----------------------------------------------------------------------------
// Description:
// Date:20080329
//----------------------------------------------------------------------------
class SE_CORE_API SEStream
{
public:
    SEStream(void);
    ~SEStream(void);

    // top level object操作
    bool Insert(SEObject* pObject);
    bool Remove(SEObject* pObject);
    void RemoveAll(void);
    int GetObjectCount(void);
    SEObject* GetObjectAt(int i) const;
    bool IsTopLevel(const SEObject* pObject);

    // 内存load/save
    bool Load(char* pBuffer, int iSize);
    bool Save(char*& rpBuffer, int& riSize);

    // 文件load/save
    bool Load(const char* pFileName);
    bool Save(const char* pFileName);

    // 对unique object列表的访问,列表元素顺序依据register阶段的深度优先遍历顺序,
    // 可以在load或save调用后访问该列表,
    // 如果在load之后访问,则通过GetOrderedObject对列表元素的修改是有效的,
    // 如果在save之后访问,则通过GetOrderedObject对列表元素的修改将不会影响
    // save结果,而只改变原始对象实例.
    inline int GetOrderedCount(void) const;
    inline SEObject* GetOrderedObject(int i) const;

    // 支持disk usage
    int GetDiskUsed(void) const;

    // 返回最近一次文件load操作的文件版本,
    // 如果没有load过文件,则返回值为-1
    inline SEStreamVersion GetVersion(void) const;

    // link类
    class SE_CORE_API SELink
    {
    public:
        SELink(SEObject* pObject);

	public:
        void SetObject(SEObject* pObject);
        SEObject* GetObject(void);

        int GetCount(void) const;
        SEObject* GetLinkID(void);

        void Add(SEObject* pLinkID);

    protected:
        SEObject* mObject;
        int m_iCurrent;
        std::vector<SEObject*> m_LinkID;
    };

protected:
    friend class SEObject;

    bool InsertInMap(SEObject* pObject, SELink* pLink);
    void InsertInOrdered(SEObject* pObject);

    // 最近一次文件读取的版本
    SEStreamVersion m_Version;

    // top level object列表
    std::vector<SEObject*> m_pTopLevel;

    // save时使用的对象注册表,
    // 使用此表的目的在于快速检索出该对象是否已经注册过
    mutable SEHashTable<SEObject*, SELink*> m_Map;

    // 唯一对象列表,由scene graph在register阶段生成,
    // 用于确保对象基于深度优先的遍历顺序存储,
    // 如果对象基于hash表顺序存储(m_Map),
    // 则对象顺序将会因每次程序运行时内存地址分配问题造成顺序不唯一
    mutable std::vector<SEObject*> m_Ordered;

    // read/write操作总是应用于内存buffer,
    // 目的在于减少磁盘反复读写操作开销
    int m_iBufferSize, m_iBufferNext;
    char* m_pBuffer;

    static const char* ms_pTopLevel;

// Internal use
public:
    // 支持linking
    SEObject* GetFromMap(SEObject* pLinkID);

    // 支持debugging
    inline int GetBufferSize(void) const;
    inline int GetBufferNext(void) const;

    void Read(SEObject*& rpValue);
    void Read(bool& rValue);
    void Read(int iCount, bool* pValue);
    void Read(char& rValue);
    void Read(int iCount, char* pValue);
    void Read(unsigned char& rValue);
    void Read(int iCount, unsigned char* pValue);
    void Read(short& rValue);
    void Read(int iCount, short* pValue);
    void Read(unsigned short& rValue);
    void Read(int iCount, unsigned short* pValue);
    void Read(int& rValue);
    void Read(int iCount, int* pValue);
    void Read(unsigned int& rValue);
    void Read(int iCount, unsigned int* pValue);
    void Read(long& rValue);
    void Read(int iCount, long* pValue);
    void Read(unsigned long& rValue);
    void Read(int iCount, unsigned long* pValue);
    void Read(float& rValue);
    void Read(int iCount, float* pValue);
    void Read(double& rValue);
    void Read(int iCount, double* pValue);

    void Read(std::string& rValue);
    void Read(int iCount, std::string* pValue);
    void Read(SEColorRGB& rValue);
    void Read(int iCount, SEColorRGB* pValue);
    void Read(SEColorRGBA& rValue);
    void Read(int iCount, SEColorRGBA* pValue);
    void Read(SEFloat4& rValue);
    void Read(int iCount, SEFloat4* pValue);
    void Read(SEMatrix2f& rValue);
    void Read(int iCount, SEMatrix2f* pValue);
    void Read(SEMatrix3f& rValue);
    void Read(int iCount, SEMatrix3f* pValue);
    void Read(SEMatrix4f& rValue);
    void Read(int iCount, SEMatrix4f* pValue);
    void Read(SEPlane3f& rValue);
    void Read(int iCount, SEPlane3f* pValue);
    void Read(SEQuaternionf& rValue);
    void Read(int iCount, SEQuaternionf* pValue);
    void Read(SEVector2f& rValue);
    void Read(int iCount, SEVector2f* pValue);
    void Read(SEVector3f& rValue);
    void Read(int iCount, SEVector3f* pValue);
    void Read(SEVector4f& rValue);
    void Read(int iCount, SEVector4f* pValue);
    void Read(SETransformation& rValue);
    void Read(int iCount, SETransformation* pValue);

    void Write(const SEObject* pValue);
    void Write(bool bValue);
    void Write(int iCount, const bool* pValue);
    void Write(char cValue);
    void Write(int iCount, const char* pValue);
    void Write(unsigned char ucValue);
    void Write(int iCount, const unsigned char* pValue);
    void Write(short sValue);
    void Write(int iCount, const short* pValue);
    void Write(unsigned short usValue);
    void Write(int iCount, const unsigned short* pValue);
    void Write(int iValue);
    void Write(int iCount, const int* pValue);
    void Write(unsigned int uiValue);
    void Write(int iCount, const unsigned int* pValue);
    void Write(long lValue);
    void Write(int iCount, const long* pValue);
    void Write(unsigned long ulValue);
    void Write(int iCount, const unsigned long* pValue);
    void Write(float fValue);
    void Write(int iCount, const float* pValue);
    void Write(double dValue);
    void Write(int iCount, const double* pValue);

    void Write(const std::string& rValue);
    void Write(int iCount, const std::string* pValue);
    void Write(const SEColorRGB& rValue);
    void Write(int iCount, const SEColorRGB* pValue);
    void Write(const SEColorRGBA& rValue);
    void Write(int iCount, const SEColorRGBA* pValue);
    void Write(const SEFloat4& rValue);
    void Write(int iCount, const SEFloat4* pValue);
    void Write(const SEMatrix2f& rValue);
    void Write(int iCount, const SEMatrix2f* pValue);
    void Write(const SEMatrix3f& rValue);
    void Write(int iCount, const SEMatrix3f* pValue);
    void Write(const SEMatrix4f& rValue);
    void Write(int iCount, const SEMatrix4f* pValue);
    void Write(const SEPlane3f& rValue);
    void Write(int iCount, const SEPlane3f* pValue);
    void Write(const SEQuaternionf& rValue);
    void Write(int iCount, const SEQuaternionf* pValue);
    void Write(const SEVector2f& rValue);
    void Write(int iCount, const SEVector2f* pValue);
    void Write(const SEVector3f& rValue);
    void Write(int iCount, const SEVector3f* pValue);
    void Write(const SEVector4f& rValue);
    void Write(int iCount, const SEVector4f* pValue);
    void Write(const SETransformation& rValue);
    void Write(int iCount, const SETransformation* pValue);
};

typedef SEObject* (*FactoryFunction)(SEStream&);

#include "SEStream.inl"

}

#endif
