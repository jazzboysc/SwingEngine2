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

#ifndef Swing_Frustum3_H
#define Swing_Frustum3_H

#include "SECoreLIB.h"
#include "SEVector3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20090115
//----------------------------------------------------------------------------
class SE_CORE_API SEFrustum3f
{
public:
    // 规范正交截投体.设E为原点,R为右向量,U为上向量,D为方向向量.
    // 设r > 0且u > 0,分别为R轴和U轴方向的延展长度.
    // 设n和f为D轴方向的延展长度,且0 < n < f.
    // 近截投面(near plane)上的四角顶点表示为E + s0*r*R + s1*u*U + n*D,
    // 其中 |s0| = |s1| = 1(表示四种选择).
    // 远截投面(far plane)上的四角顶点表示为E + (f/n)*(s0*r*R + s1*u*U) + f*D,
    // 其中 |s0| = |s1| = 1(表示四种选择).

    // 默认值为: E = (0,0,0), R = (1,0,0), U = (0,1,0), D = (0,0,1),
    // RBound = 1, UBound = 1, DMin = 1, DMax = 2.
    SEFrustum3f(void);
    SEFrustum3f(const SEVector3f& rOrigin, const SEVector3f& rRVector,
        const SEVector3f& rUVector, const SEVector3f& rDVector,
        float fRBound, float fUBound, float fDMin, float fDMax);

    // 当RBound,UBound,Dmin,Dmax发生改变时,必须调用Update()函数.
    // DRatio,MTwoUF,MTwoRF将随之改变,因此应在Update()之后访问这三个变量.
    void Update(void);
    float GetDRatio(void) const;
    float GetMTwoUF(void) const;
    float GetMTwoRF(void) const;

    void ComputeVertices(SEVector3f aVertex[8]) const;

    SEVector3f Origin, RVector, UVector, DVector;
    float RBound, UBound, DMin, DMax;

protected:
    // 相关导出数据.
    float m_fDRatio, m_fMTwoUF, m_fMTwoRF;
};

}

#endif
