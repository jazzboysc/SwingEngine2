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

#ifndef Swing_SharedArray_H
#define Swing_SharedArray_H

#include "SEPlatforms.h"
#include "SEObject.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20080315
//----------------------------------------------------------------------------
template <class Type>
class SESharedArray : public SEObject
{
    SE_DECLARE_TEMPLATE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_TEMPLATE_STREAM;

public:
    SESharedArray(int iCount = 0, Type* pArray = 0);
    SESharedArray(const SESharedArray& rShared);
    virtual ~SESharedArray(void);

    SESharedArray& operator = (const SESharedArray& rShared);

    int GetCount(void) const;
    Type* GetData(void) const;

    operator const Type* (void) const;
    operator Type* (void);
    const Type& operator [] (int i) const;
    Type& operator [] (int i);

    // 这个函数不会造成数组重新分配大小,意图是允许数组部分数据处于激活状态从而被访问,
    // 调用者有责任记住该数组的实际大小
    void SetActiveCount(int iActiveCount);

protected:
    int m_iCount;
    Type* m_pArray;
};

#include "SESharedArray.inl"

}

#endif
