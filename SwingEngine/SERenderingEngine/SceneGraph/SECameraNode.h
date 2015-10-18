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

#ifndef Swing_CameraNode_H
#define Swing_CameraNode_H

#include "SERenderingEngineLIB.h"
#include "SECamera.h"
#include "SENode.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20090224
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SECameraNode : public SENode
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    // 构造和析构.
    // 节点的世界平移变换用来作为camera的E点.
    // 节点的世界旋转变换用来作为camera的R,U,D三轴.
    // 其中,世界旋转矩阵的Row0是R轴,Row1是U轴,Row2是D轴.
    //
    // 在构造时,节点的局部变换被设置为camera的当前坐标体系.
    //   local translation    = camera location
    //   local rotation row 0 = camera right
    //   local rotation row 1 = camera up
    //   local rotation row 2 = camera direction
    SECameraNode(SECamera* pCamera = 0);
    virtual ~SECameraNode(void);

    // 当设置camera时,节点的局部变换被设置为camera的当前坐标体系.
    // 节点的世界变换被计算出来,然后camera的坐标体系被设置为使用节点的世界变换.
    void SetCamera(SECamera* pCamera);

    inline SECamera* GetCamera(void);
    inline const SECamera* GetCamera(void) const;

protected:
    // geometric updates
    virtual void UpdateWorldData(double dAppTime);

    SECameraPtr m_spCamera;
};

typedef SESmartPointer<SECameraNode> SECameraNodePtr;

#include "SECameraNode.inl"

}

#endif
