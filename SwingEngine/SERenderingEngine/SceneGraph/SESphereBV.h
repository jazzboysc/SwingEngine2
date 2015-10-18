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

#ifndef Swing_SphereBV_H
#define Swing_SphereBV_H

#include "SERenderingEngineLIB.h"
#include "SEPlatforms.h"
#include "SEBoundingVolume.h"
#include "SESphere3.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20080726
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SESphereBV : public SEBoundingVolume
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    SESphereBV(void);  // center (0,0,0), radius 0
    SESphereBV(const SESphere3f& rSphere);

    virtual int GetBVType(void) const;

    // 所有BV都要定义中点和半径.
    virtual void SetCenter(const SEVector3f& rCenter);
    virtual void SetRadius(float fRadius);
    virtual SEVector3f GetCenter(void) const;
    virtual float GetRadius(void) const;

    inline SESphere3f& Sphere(void);
    inline const SESphere3f& GetSphere(void) const;

    // 根据传入顶点集合创建BV.
    virtual void ComputeFromData(const SEVector3fArray* pVertices);

    // 变换BV(从模型空间到世界空间).
    virtual void TransformBy(const SETransformation& rTransform, 
        SEBoundingVolume* pResult);

    // 判断BV是否在平面正半空间(平面法线所指向的空间),相交,负半空间,
    // 相应的返回值为+1,0,-1.
    virtual int OnWhichSide(const SEPlane3f& rPlane) const;

    // 测试BV是否和射线相交,不计算交点,射线方向必须为单位向量.
    virtual bool TestIntersection(const SERay3f& rRay) const;

    // 测试是否和另一个BV相交.
    virtual bool TestIntersection(const SEBoundingVolume* pInput) const;

    // 用另一个BV复制出自己.
    virtual void CopyFrom(const SEBoundingVolume* pInput);

    // 当前BV增长,包含传入的BV和之前的自己.
    virtual void GrowToContain(const SEBoundingVolume* pInput);

    // 是否包含传入点.
    virtual bool Contains(const SEVector3f& rPoint) const;

protected:
    SESphere3f m_Sphere;
};

typedef SESmartPointer<SESphereBV> SESphereBVPtr;

#include "SESphereBV.inl"

}

#endif
