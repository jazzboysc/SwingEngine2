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
    // �淶������Ͷ��.��EΪԭ��,RΪ������,UΪ������,DΪ��������.
    // ��r > 0��u > 0,�ֱ�ΪR���U�᷽�����չ����.
    // ��n��fΪD�᷽�����չ����,��0 < n < f.
    // ����Ͷ��(near plane)�ϵ��ĽǶ����ʾΪE + s0*r*R + s1*u*U + n*D,
    // ���� |s0| = |s1| = 1(��ʾ����ѡ��).
    // Զ��Ͷ��(far plane)�ϵ��ĽǶ����ʾΪE + (f/n)*(s0*r*R + s1*u*U) + f*D,
    // ���� |s0| = |s1| = 1(��ʾ����ѡ��).

    // Ĭ��ֵΪ: E = (0,0,0), R = (1,0,0), U = (0,1,0), D = (0,0,1),
    // RBound = 1, UBound = 1, DMin = 1, DMax = 2.
    SEFrustum3f(void);
    SEFrustum3f(const SEVector3f& rOrigin, const SEVector3f& rRVector,
        const SEVector3f& rUVector, const SEVector3f& rDVector,
        float fRBound, float fUBound, float fDMin, float fDMax);

    // ��RBound,UBound,Dmin,Dmax�����ı�ʱ,�������Update()����.
    // DRatio,MTwoUF,MTwoRF����֮�ı�,���Ӧ��Update()֮���������������.
    void Update(void);
    float GetDRatio(void) const;
    float GetMTwoUF(void) const;
    float GetMTwoRF(void) const;

    void ComputeVertices(SEVector3f aVertex[8]) const;

    SEVector3f Origin, RVector, UVector, DVector;
    float RBound, UBound, DMin, DMax;

protected:
    // ��ص�������.
    float m_fDRatio, m_fMTwoUF, m_fMTwoRF;
};

}

#endif
