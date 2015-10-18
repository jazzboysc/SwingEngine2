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

#ifndef Swing_ConvexPolyhedron3_H
#define Swing_ConvexPolyhedron3_H

#include "SECoreLIB.h"
#include "SEPolyhedron3.h"
#include "SEPlane3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20081124
//----------------------------------------------------------------------------
class SE_CORE_API SEConvexPolyhedron3f : public SEPolyhedron3f
{
public:
    // ������������ȷ����������ݹ���͹������.
    // �����ۿ�meshʱ,�����������˳��ӦΪ˳ʱ�뷽��.
    // bOwnerΪtrueʱ,convex polyhedronӵ�д������������Ȩ.
    // ����Ϊfalseʱ,�ɵ����߸���ɾ����������.
    //
    // ����洢���������Ӧ��ƽ������,ƽ�淨��ָ��������ڲ�.
    // ����ͨ�����캯������ƽ������,�Ӷ�bOwnerҲ��������������Ȩ.
    // ���û�д���ƽ������,��������Լ����𴴽�������,�Ӷ�����bOwnerӰ��.
    SEConvexPolyhedron3f(int iVCount, SEVector3f* aVertex,
        int iTCount, int* aiIndex, SEPlane3f* aPlane, bool bOwner);

    // ��������polyhedronӵ������������,��Ϊ'this'����������������,
    // �Ӷ�ʹ'this'����Ҳӵ������������.
    // ��������polyhedron��ӵ������������,��'this'����Ҳ��ӵ��.ֻ����ָ�빲��.
    SEConvexPolyhedron3f(const SEConvexPolyhedron3f& rPoly);

    virtual ~SEConvexPolyhedron3f(void);

    // ��������polyhedronӵ������������,��Ϊ'this'����������������,
    // �Ӷ�ʹ'this'����Ҳӵ������������.
    // ��������polyhedron��ӵ������������,��'this'����Ҳ��ӵ��.ֻ����ָ�빲��.
    SEConvexPolyhedron3f& operator = (const SEConvexPolyhedron3f& rPoly);

    const SEPlane3f* GetPlanes(void) const;
    const SEPlane3f& GetPlane(int i) const;

    // ����Զ�������޸�.
    // �����߱���ȷ��mesh��Ϊconvex ployhedron.
    // �޸Ķ������Ҫ����UpdatePlanes()����.
    // ��������޸Ķ����������ͨ��SetVertex������ɵ�,
    // ��ֻ���ܵ�Ӱ����������plane�ᱻ���¼���.
    // ���������ͨ��GetVertices�����޸ĵĶ���,
    // ���ཫ�޷���֪�����޸ĵ���Ϣ,�Ӷ�ʹUpdatePlanes()��������ȫ��plane.
    virtual void SetVertex(int i, const SEVector3f& rV);
    void UpdatePlanes(void);

    // ͹�����Լ��.
    // ���������������������������,
    // ����֤���ж����嶥�㶼�����ڸ�������ķǸ��ռ�.
    // ʹ�ô����ŵľ������,�Ӷ���һ�����㴦�������渺��ռ�ʱ,
    // ���������d < 0.
    // ͬʱΪ�˱�����ֵ�����ɵĴ����ж�,����ʹ��һ�������ٽ�ֵt,
    // �Ӷ�ʹ�����жϳ�Ϊ d < t < 0.
    bool IsConvex(float fThreshold = 0.0f) const;

    // Point-in-polyhedron����.
    // �ٽ�ֵt��IsConvex�����е���;һ��.
    bool ContainsPoint(const SEVector3f& rP, float fThreshold = 0.0f)
        const;

protected:
    SEPlane3f* m_aPlane;
    bool m_bPlaneOwner;

    // ֧�ָ�Ч����ƽ������.
    // set�洢�˸��Ķ�������ʱ��Ӱ�쵽�������������.
    // �Ӷ�����ֻ����޸Ĺ�����������и���.
    void UpdatePlane(int i, const SEVector3f& rAverage);
    std::set<int> m_TModified;
};

}

#endif
