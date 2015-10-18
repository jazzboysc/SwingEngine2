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

#ifndef Swing_LightNode_H
#define Swing_LightNode_H

#include "SERenderingEngineLIB.h"
#include "SELight.h"
#include "SENode.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20090224
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SELightNode : public SENode
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    // 节点的世界平移变换用来作为light的位置点.
    // 节点的世界旋转变换用来作为light的R,U,D三轴.
    // 其中,世界旋转矩阵的Row0是R轴,Row1是U轴,Row2是D轴.
    //
    // 在构造时,节点的局部变换被设置为light的当前坐标体系.
    //   local translation       = light location
    //   local rotation row 0 = light right
    //   local rotation row 1 = light up
    //   local rotation row 2 = light direction
    SELightNode(SELight* pLight = 0);
    virtual ~SELightNode(void);

    // 当设置light时,节点的局部变换被设置为light的当前坐标体系.
    // 节点的世界变换被计算出来,然后light的坐标体系被设置为使用节点的世界变换.
    void SetLight(SELight* pLight);

    inline SELight* GetLight(void);
    inline const SELight* GetLight(void) const;

protected:
    // geometric updates
    virtual void UpdateWorldData(double dAppTime);

    SELightPtr m_spLight;
};

typedef SESmartPointer<SELightNode> SELightNodePtr;

#include "SELightNode.inl"

}

#endif
