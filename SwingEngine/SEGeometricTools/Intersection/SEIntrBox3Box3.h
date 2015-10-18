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

#ifndef Swing_IntrBox3Box3_H
#define Swing_IntrBox3Box3_H

#include "SEGeometricToolsLIB.h"
#include "SEIntersector.h"
#include "SEBox3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20081230
//----------------------------------------------------------------------------
class SE_GEOMETRICTOOLS_API SEIntrBox3Box3f : public SEIntersector<float, 
    SEVector3f>
{
public:
    SEIntrBox3Box3f(const SEBox3f& rBox0, const SEBox3f& rBox1);

    // �������.
    const SEBox3f& GetBox0(void) const;
    const SEBox3f& GetBox1(void) const;

    // static test-intersection��ѯ.
    virtual bool Test(void);

    // dynamic test-intersection��ѯ.����first contact time(����еĻ�),
    // ��û���κι���contact set����Ϣ.
    virtual bool Test(float fTMax, const SEVector3f& rVelocity0,
        const SEVector3f& rVelocity1);

private:
    // ����֧��dynamic��ѯ.  
    // �����������: ����box��һ��Ǳ�ڷ������ϵĸ���ͶӰ����,
    // �������˶�������ٶ�ֵ,�Լ���ѯ��ָ�������ʱ��ֵ.
    // �����������: first/last contact time.
    // �����������Բ�ͬ��Ǳ�ڷ����ᱻƵ������ʱ,��Ӧ���������Ҳ�����ϸ���.
    // �����first contact timeֻ���ڴ��������first contact timeʱ�Ÿ���.
    // �����last contact timeֻ����С�������last contact timeʱ�Ÿ���.
    bool IsSeparated(float fMin0, float fMax0, float fMin1, float fMax1,
        float fSpeed, float fTMax, float& rfTFirst, float& rfTLast);

    // ��������.
    const SEBox3f* m_pBox0;
    const SEBox3f* m_pBox1;
};

}

#endif
