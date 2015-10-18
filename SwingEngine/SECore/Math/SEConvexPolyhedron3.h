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
    // 调用者有责任确保传入的数据构成凸多面体.
    // 从外侧观看mesh时,三角面的索引顺序应为顺时针方向.
    // bOwner为true时,convex polyhedron拥有传入数组的所有权.
    // 否则为false时,由调用者负责删除传入数组.
    //
    // 本类存储与三角面对应的平面数组,平面法线指向多面体内部.
    // 可以通过构造函数传入平面数组,从而bOwner也决定该数组所有权.
    // 如果没有传入平面数组,则本类对象自己负责创建该数组,从而不受bOwner影响.
    SEConvexPolyhedron3f(int iVCount, SEVector3f* aVertex,
        int iTCount, int* aiIndex, SEPlane3f* aPlane, bool bOwner);

    // 如果传入的polyhedron拥有其数组数据,则为'this'对象复制其数组数据,
    // 从而使'this'对象也拥有其数组数据.
    // 如果传入的polyhedron不拥有其数组数据,则'this'对象也不拥有.只进行指针共享.
    SEConvexPolyhedron3f(const SEConvexPolyhedron3f& rPoly);

    virtual ~SEConvexPolyhedron3f(void);

    // 如果传入的polyhedron拥有其数组数据,则为'this'对象复制其数组数据,
    // 从而使'this'对象也拥有其数组数据.
    // 如果传入的polyhedron不拥有其数组数据,则'this'对象也不拥有.只进行指针共享.
    SEConvexPolyhedron3f& operator = (const SEConvexPolyhedron3f& rPoly);

    const SEPlane3f* GetPlanes(void) const;
    const SEPlane3f& GetPlane(int i) const;

    // 允许对顶点进行修改.
    // 调用者必须确保mesh仍为convex ployhedron.
    // 修改顶点后需要调用UpdatePlanes()函数.
    // 如果所有修改顶点操作都是通过SetVertex函数完成的,
    // 则只有受到影响的三角面的plane会被重新计算.
    // 否则如果有通过GetVertices函数修改的顶点,
    // 则本类将无法得知具体修改的信息,从而使UpdatePlanes()函数更新全部plane.
    virtual void SetVertex(int i, const SEVector3f& rV);
    void UpdatePlanes(void);

    // 凸面体性检测.
    // 函数将迭代多面体的所有三角面,
    // 并验证所有多面体顶点都必须在该三角面的非负空间.
    // 使用带符号的距离测试,从而当一个顶点处在三角面负半空间时,
    // 其距离满足d < 0.
    // 同时为了避免数值误差造成的错误判断,允许使用一个负数临界值t,
    // 从而使距离判断成为 d < t < 0.
    bool IsConvex(float fThreshold = 0.0f) const;

    // Point-in-polyhedron测试.
    // 临界值t与IsConvex函数中的用途一样.
    bool ContainsPoint(const SEVector3f& rP, float fThreshold = 0.0f)
        const;

protected:
    SEPlane3f* m_aPlane;
    bool m_bPlaneOwner;

    // 支持高效更新平面数组.
    // set存储了更改顶点数据时所影响到的三角面的索引.
    // 从而允许只针对修改过的三角面进行更新.
    void UpdatePlane(int i, const SEVector3f& rAverage);
    std::set<int> m_TModified;
};

}

#endif
