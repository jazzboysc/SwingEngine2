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

    // 对象访问.
    const SEBox3f& GetBox0(void) const;
    const SEBox3f& GetBox1(void) const;

    // static test-intersection查询.
    virtual bool Test(void);

    // dynamic test-intersection查询.计算first contact time(如果有的话),
    // 但没有任何关于contact set的信息.
    virtual bool Test(float fTMax, const SEVector3f& rVelocity0,
        const SEVector3f& rVelocity1);

private:
    // 用于支持dynamic查询.  
    // 输入参数包括: 两个box在一个潜在分离轴上的各自投影区间,
    // 两区间运动的相对速度值,以及查询所指定的最大时间值.
    // 输出参数包括: first/last contact time.
    // 当这个函数针对不同的潜在分离轴被频繁调用时,相应的输出参数也被不断更新.
    // 输出的first contact time只有在大于输入的first contact time时才更新.
    // 输出的last contact time只有在小于输入的last contact time时才更新.
    bool IsSeparated(float fMin0, float fMax0, float fMin1, float fMax1,
        float fSpeed, float fTMax, float& rfTFirst, float& rfTLast);

    // 待检查对象.
    const SEBox3f* m_pBox0;
    const SEBox3f* m_pBox1;
};

}

#endif
