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

#ifndef Swing_UnculledSet_H
#define Swing_UnculledSet_H

#include "SERenderingEngineLIB.h"
#include "SEUnculledObject.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20080707
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEUnculledSet
{
public:
    SEUnculledSet(int iMaxCount = 0, int iGrowBy = 0);
    virtual ~SEUnculledSet(void);

    inline int GetCount(void) const;
    inline SEUnculledObject* GetUnculled(void);
    inline SEUnculledObject& GetUnculled(int i);

    // 根据输入数据创建一个SEUnculledObject并放在可见对象集合尾部
    void Insert(SESpatial* pObject);
    // 并不释放已分配数据
    inline void Clear(void);
    void Resize(int iMaxCount, int iGrowBy);

private:
    enum
    {
        US_DEFAULT_MAX_COUNT = 32,
        US_DEFAULT_GROWBY = 32,
    };

    int m_iMaxCount, m_iGrowBy, m_iCount;
    SEUnculledObject* m_pUnculled;
};

#include "SEUnculledSet.inl"

}

#endif
