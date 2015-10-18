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
    // simple polyhedron:每个edge最多被两个三角形共享,三角面没有自相交的情况.

    // 调用者有责任确保传入的数据构成simple polyhedron.
    // 从外侧观看mesh时,三角面的索引顺序应为顺时针方向.
    // bOwner为true时,polyhedron拥有传入数组的所有权.
    // 否则为false时,由调用者负责删除传入数组.
    SEPolyhedron3f(int iVCount, SEVector3f* aVertex, int iTCount,
        int* aiIndex, bool bOwner);

    // 如果传入的polyhedron拥有其数组数据,则为'this'对象复制其数组数据,
    // 从而使'this'对象也拥有其数组数据.
    // 如果传入的polyhedron不拥有其数组数据,则'this'对象也不拥有.只进行指针共享.
    SEPolyhedron3f(const SEPolyhedron3f& rPoly);

    virtual ~SEPolyhedron3f(void);

    // 如果传入的polyhedron拥有其数组数据,则为'this'对象复制其数组数据,
    // 从而使'this'对象也拥有其数组数据.
    // 如果传入的polyhedron不拥有其数组数据,则'this'对象也不拥有.只进行指针共享.
    SEPolyhedron3f& operator = (const SEPolyhedron3f& rPoly);

    int GetVCount(void) const;
    const SEVector3f* GetVertices(void) const;
    const SEVector3f& GetVertex(int i) const;
    int GetTCount(void) const;
    const int* GetIndices(void) const;
    const int* GetTriangle(int i) const;

    // 允许对顶点进行修改.
    // 但不进行任何几何拓扑校验,不保证仍是simple polyhedron.
    virtual void SetVertex(int i, const SEVector3f& rV);
    SEVector3f* GetVertices(void);

    // 计算polyhedron的各种相关信息.
    SEVector3f ComputeVertexAverage(void) const; // 平均顶点中心
    float ComputeSurfaceArea(void) const; // 表面积
    float ComputeVolume(void) const; // 体积

protected:
    int m_iVCount;
    SEVector3f* m_aVertex;
    int m_iTCount;
    int* m_aiIndex;
    bool m_bOwner;
};

}

#endif
