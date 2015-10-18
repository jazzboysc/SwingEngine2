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

#ifndef Swing_Spatial_H
#define Swing_Spatial_H

#include "SERenderingEngineLIB.h"
#include "SEPlatforms.h"
#include "SEAdvancedObject.h"
#include "SEBoundingVolume.h"
#include "SETransformation.h"

namespace Swing
{

class SECuller;

//----------------------------------------------------------------------------
// Description:
// Date:20080707
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SESpatial : public SEAdvancedObject
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    // Abstract base class.
    virtual ~SESpatial(void);

    // Local和World变换.
    // 某些情况下你也许需要跳过SESpatial::UpdateGS()模块从而直接设置world变换,
    // 此时应设置WorldIsCurrent为true.
    // 比如IK controller和Skin controller都需要这种能力.
    SETransformation Local;
    SETransformation World;
    bool WorldIsCurrent;

    // 世界空间BV.
    // 某些情况下你也许需要跳过SESpatial::UpdateGS()模块从而直接设置世界空间BV,
    // 此时应设置WorldBoundIsCurrent为true.
    SEBoundingVolumePtr WorldBound;
    bool WorldBoundIsCurrent;

    // 注意:
    // SESpatial不需要体现模型空间BV.

    // 剔除模式
    enum CullingMode
    {
        // 根据世界空间BV与世界空间剔除平面,进行动态剔除.
        CULL_DYNAMIC,

        // 强制剔除,如果是一个SENode节点,则其整个子树都将被剔除.
        CULL_ALWAYS,

        // 强制不剔除,如果是一个SENode节点,则其整个子树都不被剔除.
        // 当首次遇到此SENode节点后,bNoCull参数被设置为true,
        // 并传递给GetUnculledSet/OnGetUnculledSet AB递归链,
        // 从而使递归路径上的所有子节点都不会被剔除.
        CULL_NEVER,

        MAX_CULLING_MODE
    };

    CullingMode Culling;

    // Geometric state和Controller更新入口.
    // UpdateGS函数向下计算每个节点的世界变换,向上计算世界BV.
    // UpdateBS函数只向上计算世界BV,当模型数据改变时,只需改变模型BV和世界BV,
    // 无需重新计算空间变换.
    void UpdateGS(double dAppTime = -SEMathd::MAX_REAL,
        bool bInitiator = true);
    void UpdateBS(void);

    // Parent access.
    inline SESpatial* GetParent(void);

protected:
    // Streaming support.
    SESpatial(void);

    // Support for the geometric update.
    // 派生类SENode实现向下进行AB递归.
    // 向上计算世界空间BV,派生类实现具体行为.
    virtual void UpdateWorldData(double dAppTime);
    virtual void UpdateWorldBound(void) = 0;
    void PropagateBoundToRoot(void);

    SESpatial* m_pParent;

private:
    static const char* ms_aacCullingMode[MAX_CULLING_MODE];

// Internal use
public:
    inline void SetParent(SESpatial* pParent);

    // Support for hierarchical culling.
    void OnGetUnculledSet(SECuller& rCuller, bool bNoCull);
    virtual void GetUnculledSet(SECuller& rCuller, bool bNoCull) = 0;
};

typedef SESmartPointer<SESpatial> SESpatialPtr;

#include "SESpatial.inl"

}

#endif
