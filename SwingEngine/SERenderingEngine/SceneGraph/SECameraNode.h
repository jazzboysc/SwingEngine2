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
    // ���������.
    // �ڵ������ƽ�Ʊ任������Ϊcamera��E��.
    // �ڵ��������ת�任������Ϊcamera��R,U,D����.
    // ����,������ת�����Row0��R��,Row1��U��,Row2��D��.
    //
    // �ڹ���ʱ,�ڵ�ľֲ��任������Ϊcamera�ĵ�ǰ������ϵ.
    //   local translation    = camera location
    //   local rotation row 0 = camera right
    //   local rotation row 1 = camera up
    //   local rotation row 2 = camera direction
    SECameraNode(SECamera* pCamera = 0);
    virtual ~SECameraNode(void);

    // ������cameraʱ,�ڵ�ľֲ��任������Ϊcamera�ĵ�ǰ������ϵ.
    // �ڵ������任���������,Ȼ��camera��������ϵ������Ϊʹ�ýڵ������任.
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
