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
    // �ڵ������ƽ�Ʊ任������Ϊlight��λ�õ�.
    // �ڵ��������ת�任������Ϊlight��R,U,D����.
    // ����,������ת�����Row0��R��,Row1��U��,Row2��D��.
    //
    // �ڹ���ʱ,�ڵ�ľֲ��任������Ϊlight�ĵ�ǰ������ϵ.
    //   local translation       = light location
    //   local rotation row 0 = light right
    //   local rotation row 1 = light up
    //   local rotation row 2 = light direction
    SELightNode(SELight* pLight = 0);
    virtual ~SELightNode(void);

    // ������lightʱ,�ڵ�ľֲ��任������Ϊlight�ĵ�ǰ������ϵ.
    // �ڵ������任���������,Ȼ��light��������ϵ������Ϊʹ�ýڵ������任.
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
