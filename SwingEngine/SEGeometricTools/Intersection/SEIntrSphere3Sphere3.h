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

#ifndef Swing_IntrSphere3Sphere3_H
#define Swing_IntrSphere3Sphere3_H

#include "SEGeometricToolsLIB.h"
#include "SEPlatforms.h"
#include "SEIntersector.h"
#include "SESphere3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20081219
//----------------------------------------------------------------------------
class SE_GEOMETRICTOOLS_API SEIntrSphere3Sphere3f : public SEIntersector<float, 
    SEVector3f>
{
public:
    SEIntrSphere3Sphere3f(const SESphere3f& rSphere0, const SESphere3f& 
        rSphere1);

    // �������.
    const SESphere3f& GetSphere0(void) const;
    const SESphere3f& GetSphere1(void) const;

    // ��̬test-intersection��ѯ.
    virtual bool Test(void);

    // ��̬find-intersection��ѯ.
    virtual bool Find(void);

    // ��̬find-intersection��ѯ���ཻ��.
    const SEVector3f& GetCenter(void) const;
    const SEVector3f& GetUAxis(void) const;
    const SEVector3f& GetVAxis(void) const;
    const SEVector3f& GetNormal(void) const;
    float GetRadius(void) const;

    // ��̬test-intersection��ѯ.
    virtual bool Test(float fTMax, const SEVector3f& rVelocity0,
        const SEVector3f& rVelocity1);

    // ��̬find-intersection��ѯ.
    virtual bool Find(float fTMax, const SEVector3f& rVelocity0,
        const SEVector3f& rVelocity1);

    // ��̬find-intersection��ѯ���ཻ��.
    const SEVector3f& GetContactPoint(void) const;

private:
    using SEIntersector<float, SEVector3f>::m_fContactTime;

    // ������Ƿ��ཻ�Ķ���.
    const SESphere3f* m_pSphere0;
    const SESphere3f* m_pSphere1;

    // ��̬����ཻԲ.
    // Բ����C,Բ����ƽ����������λ����U��Vȷ��.
    // ƽ�淨���ǵ�λ����N.Բ�뾶��R.
    SEVector3f m_Center, m_UAxis, m_VAxis, m_Normal;
    float m_fRadius;

    // ��̬����ĽӴ���.
    SEVector3f m_ContactPoint;
};

}

#endif
