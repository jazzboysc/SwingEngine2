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

// 设3维平面的法线向量为单位向量N,且已知平面上一点P0,平面上任意一点P,
// 则平面方程可表示为Dot(N,(P - P0)) = 0;进一步表示为Dot(N,P) = c.

// 平面采用左手坐标系与左手原则,设平面上3点为P0,P1,P2,则平面法线N的
// 方向为左手顺时针索引P0,P1,P2的叉积向量方向.
// 注意DirectX采用左手顺时针索引,而OpenGL采用右手逆时针索引,
// 设DirectX方式的三角面<P0,P1,P2>,则OpenGL中应为<P0,P2,P1>.

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
    // 用单位向量法线和常量构建
    SEPlane3f(const SEVector3f& rNormal, float fConstant);
    // 用单位向量法线和平面上一点构建
    SEPlane3f(const SEVector3f& rNormal, const SEVector3f& rP0);
    // 用平面上3点构建
    SEPlane3f(const SEVector3f& rP0, const SEVector3f& rP1, const SEVector3f& 
        rP2);

    SEPlane3f& operator = (const SEPlane3f& rPlane);

    // 点P在平面正面返回1,在平面上返回0,在平面负面返回-1
    inline int OnWhichSide(const SEVector3f& rP) const;
    // 计算点P到平面的距离
    inline float GetDistance(const SEVector3f& rP) const;

public:
    SEVector3f Normal;
    float Constant;
};

#include "SEPlane3.inl"

}

#endif