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

#ifndef Swing_RTTI_H
#define Swing_RTTI_H

#include "SECoreLIB.h"
#include "SEPlatforms.h"
#include "SESystem.h"
#include "SERTTIMCR.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20080202
//----------------------------------------------------------------------------
class SE_CORE_API SERTTI
{
public:
    // 名称必须是全局唯一名称,在Swing名字空间里,类Foo应该使用"Swing.Foo",
    // 如果应用程序有其他名字空间,如SomeName,则类Foo应使用"SomeName.Foo"
    SERTTI(const char* pName, const SERTTI* pBaseType);
    ~SERTTI(void);

    inline const char* GetName(void) const;
    inline int GetDiskUsed(void) const;

    inline bool IsExactly(const SERTTI& rType) const;
    bool IsDerived(const SERTTI& rType) const;

private:
    const char* m_pRTTIName;
    const SERTTI* m_pBaseType;
};

#include "SERTTI.inl"

}

#endif
