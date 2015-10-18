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

#ifndef Swing_Culler_H
#define Swing_Culler_H

#include "SERenderingEngineLIB.h"
#include "SECamera.h"
#include "SEPlane3.h"
#include "SESpatial.h"
#include "SEUnculledSet.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20080707
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SECuller
{
public:
    // 前两个参数用于创建潜在可视听对象集容器.
    // 如果没有传递camera,稍后应在使用ComputeUnculledSet之前,
    // 通过SetCamera设置camera.
    SECuller(int iMaxCount = 0, int iGrowBy = 0, const SECamera* pCamera = 0);
    virtual ~SECuller(void);

    // 访问camera,frustum copy,潜在可视听对象容器.
    inline void SetCamera(const SECamera* pCamera);
    inline const SECamera* GetCamera(void) const;
    void SetFrustum(const float* afFrustum);
    inline const float* GetFrustum(void) const;
    inline SEUnculledSet& GetVisibleSet(void);
    inline SEUnculledSet& GetAudibleSet(void);

    // 基类默认行为是根据传入参数创建一个SEUnculledObject,
    // 并将其添加到SEUnculledObject数组末尾.
    // 派生类可以重载此函数,比如SEUnculledObject数组可以被维护为有序数组,
    // 从而使渲染状态改变最小化,
    // 或者也可以成为一个对象unique list供portal系统使用.
    virtual void Insert(SESpatial* pObject, bool bIsAudible = false);

    // 世界空间剔除平面堆栈访问.
    // 用户可以在截投体平面基础上添加用户自定义平面,
    // PushPlane传入参数必须是世界空间平面.
    enum { US_MAX_PLANE_COUNT = 32 };
    inline int GetPlaneCount(void) const;
    inline const SEPlane3f* GetPlanes(void) const;
    inline void SetPlaneState(unsigned int uiPlaneState);
    inline unsigned int GetPlaneState(void) const;
    inline void PushPlane(const SEPlane3f& rPlane);
    inline void PopPlane(void);

    // 把对象的世界空间BV和各个剔除平面比较,
    // 并记录各个平面与之比较的结果,供剔除系统在场景剔除遍历过程中使用.
    // 只有SESpatial类需要调用这个函数.
    bool IsInFrustum(const SEBoundingVolume* pBound);

    // 支持Portal::GetUnculledSet.
    bool IsInFrustum(int iVertexCount, const SEVector3f* aVertex,
        bool bIgnoreNearPlane);

    // 把对象的世界空间BV和各个剔除平面比较,
    // 只判断该对象是否在frustum中,不记录比较结果.
    // 支持SERoamTerrainPage::Simplify.
    bool IsSingleInFrustum(const SEBoundingVolume* pBound) const;

    // 支持BspNode::GetUnculledSet.
    // 判断截投体与平面的位置情况,
    // 在平面正半空间返回+1,负半空间返回-1,相交返回0.
    // 传入平面为世界空间平面.
    int OnWhichSide(const SEPlane3f& rPlane) const;

    // 剔除系统入口.
    // 通过遍历传入的场景视图树,创建潜在可视听对象集合.
    void ComputeUnculledSet(SESpatial* pScene);

protected:
    // 剔除系统需要知道当前摄像机的相关信息.
    const SECamera* m_pCamera;

    // 传入摄像机截投体参数的拷贝值.
    // 这将允许各种子系统在剔除过程中(比如portal系统),
    // 改变截投体参数而不影响当前摄像机.
    float m_Frustum[SECamera::VF_COUNT];

    // 世界空间平面数组,包含6个截投体平面,允许26个用户自定义平面.
    // m_uiPlaneState的32位对应32个剔除平面的状态,
    // 状态位为1时表明该平面应该参与剔除比较,为0时不用参与.
    // 这样做的目的在于提高剔除系统的效率,
    // 比如某父节点的BV完全处于某平面的一个半空间内,
    // 则可断定其所有子节点也均处在该平面的该半空间内,
    // 从而避免子节点的重复性剔除比较.
    int m_iPlaneCount;
    SEPlane3f m_aPlane[US_MAX_PLANE_COUNT];
    unsigned int m_uiPlaneState;

    // 调用ComputeUnculledSet后产生的潜在可视对象集合.
    SEUnculledSet m_VisibleSet;

    // 调用ComputeUnculledSet后产生的潜在可听对象集合.
    SEUnculledSet m_AudibleSet;
};

#include "SECuller.inl"

}

#endif
