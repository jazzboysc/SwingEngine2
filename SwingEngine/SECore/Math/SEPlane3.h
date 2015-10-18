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

#ifndef Swing_Plane_H
#define Swing_Plane_H

#include "SEVector3.h"

// ��3άƽ��ķ�������Ϊ��λ����N,����֪ƽ����һ��P0,ƽ��������һ��P,
// ��ƽ�淽�̿ɱ�ʾΪDot(N,(P - P0)) = 0;��һ����ʾΪDot(N,P) = c.

// ƽ�������������ϵ������ԭ��,��ƽ����3��ΪP0,P1,P2,��ƽ�淨��N��
// ����Ϊ����˳ʱ������P0,P1,P2�Ĳ����������.
// ע��DirectX��������˳ʱ������,��OpenGL����������ʱ������,
// ��DirectX��ʽ��������<P0,P1,P2>,��OpenGL��ӦΪ<P0,P2,P1>.

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20070522
//----------------------------------------------------------------------------
class SE_CORE_API SEPlane3f
{
public:
    SEPlane3f(void);
    SEPlane3f(const SEPlane3f& rPlane);
    // �õ�λ�������ߺͳ�������
    SEPlane3f(const SEVector3f& rNormal, float fConstant);
    // �õ�λ�������ߺ�ƽ����һ�㹹��
    SEPlane3f(const SEVector3f& rNormal, const SEVector3f& rP0);
    // ��ƽ����3�㹹��
    SEPlane3f(const SEVector3f& rP0, const SEVector3f& rP1, const SEVector3f& 
        rP2);

    SEPlane3f& operator = (const SEPlane3f& rPlane);

    // ��P��ƽ�����淵��1,��ƽ���Ϸ���0,��ƽ�渺�淵��-1
    inline int OnWhichSide(const SEVector3f& rP) const;
    // �����P��ƽ��ľ���
    inline float GetDistance(const SEVector3f& rP) const;

public:
    SEVector3f Normal;
    float Constant;
};

#include "SEPlane3.inl"

}

#endif