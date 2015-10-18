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

#ifndef Swing_Polyhedron3_H
#define Swing_Polyhedron3_H

#include "SECoreLIB.h"
#include "SEVector3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20081124
//----------------------------------------------------------------------------
class SE_CORE_API SEPolyhedron3f
{
public:
    // simple polyhedron:ÿ��edge��౻���������ι���,������û�����ཻ�����.

    // ������������ȷ����������ݹ���simple polyhedron.
    // �����ۿ�meshʱ,�����������˳��ӦΪ˳ʱ�뷽��.
    // bOwnerΪtrueʱ,polyhedronӵ�д������������Ȩ.
    // ����Ϊfalseʱ,�ɵ����߸���ɾ����������.
    SEPolyhedron3f(int iVCount, SEVector3f* aVertex, int iTCount,
        int* aiIndex, bool bOwner);

    // ��������polyhedronӵ������������,��Ϊ'this'����������������,
    // �Ӷ�ʹ'this'����Ҳӵ������������.
    // ��������polyhedron��ӵ������������,��'this'����Ҳ��ӵ��.ֻ����ָ�빲��.
    SEPolyhedron3f(const SEPolyhedron3f& rPoly);

    virtual ~SEPolyhedron3f(void);

    // ��������polyhedronӵ������������,��Ϊ'this'����������������,
    // �Ӷ�ʹ'this'����Ҳӵ������������.
    // ��������polyhedron��ӵ������������,��'this'����Ҳ��ӵ��.ֻ����ָ�빲��.
    SEPolyhedron3f& operator = (const SEPolyhedron3f& rPoly);

    int GetVCount(void) const;
    const SEVector3f* GetVertices(void) const;
    const SEVector3f& GetVertex(int i) const;
    int GetTCount(void) const;
    const int* GetIndices(void) const;
    const int* GetTriangle(int i) const;

    // ����Զ�������޸�.
    // ���������κμ�������У��,����֤����simple polyhedron.
    virtual void SetVertex(int i, const SEVector3f& rV);
    SEVector3f* GetVertices(void);

    // ����polyhedron�ĸ��������Ϣ.
    SEVector3f ComputeVertexAverage(void) const; // ƽ����������
    float ComputeSurfaceArea(void) const; // �����
    float ComputeVolume(void) const; // ���

protected:
    int m_iVCount;
    SEVector3f* m_aVertex;
    int m_iTCount;
    int* m_aiIndex;
    bool m_bOwner;
};

}

#endif
