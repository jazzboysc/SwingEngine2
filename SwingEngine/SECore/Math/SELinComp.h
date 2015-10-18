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

#ifndef Swing_LinComp_H
#define Swing_LinComp_H

#include "SEPlatforms.h"
#include "SEMath.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20081219
//----------------------------------------------------------------------------
template <class Real>
class SELinComp
{
public:
    // Abstract base class.
    virtual ~SELinComp(void);

    // linear component被表示为P + t*D的形式,其中P是原点,D是单位长度方向向量.
    // 用户必须确保以上条件.t值的区间针对直线,射线,线段,点,空集稍后描述.

    // 类型.
    enum
    {
        CT_EMPTY,
        CT_POINT,
        CT_SEGMENT,
        CT_RAY,
        CT_LINE
    };

    int GetType(void) const;

    // t值区间的限制,如上面所定义的类型.
    // 函数SetInterval(min, max)用来设置t值区间.
    // 它根据下面的策略处理各种可能的传入值:
    //   CT_LINE:
    //     [-MAX_REAL,MAX_REAL]
    //   CT_RAY:
    //     [min,MAX_REAL], 其中min是finite值
    //     [-MAX_REAL,max], 其中max是finite值
    //   CT_SEGMENT:
    //     [min,max], 其中min和max是finite值, 且min < max
    //   CT_POINT:
    //     [min,max], 其中min和max是finite值, 且 min = max
    //   CT_EMPTY:
    //     [min,max], 其中min > max或min = max = MAX_REAL或
    //                min = max = -MAX_REAL
    void SetInterval(Real fMin, Real fMax);

    // 无需创建SELinComp对象就可得知区间所属类型.
    static int GetTypeFromInterval(Real fMin, Real fMax);

    // 各种linear component类型的规范化区间为:
    // 直线是[-MAX_REAL,MAX_REAL],射线是[0,MAX_REAL],线段是[-e,e],其中e > 0,
    // 点是[0,0],空集是[MAX_REAL,-MAX_REAL].如果传入区间是[min,max],
    // 则做如下调整.
    //
    // CT_RAY: 如果max是MAX_REAL且min非零,则P点被改为P' = P + min*D,
    // 从而射线被表示为P' + t*D,其中t >= 0.
    // 如果min是-MAX_REAL且max是finite值,则P点和D向量被改为P' = P + max*D
    // 且D' = -D.
    //
    // CT_SEGMENT: 如果min不等于-max,则P被改为
    // P' = P + ((min + max)/2)*D 且线段延展长度是e' = (max - min)/2.
    //
    // CT_POINT: 如果min不等于零, 则P点被改为P' = P + min*D.
    //
    // CT_EMPTY: 设置max为-MAX_REAL且min为MAX_REAL.
    //
    // 第一个函数是虚函数,因为具体的规范化操作依赖于向量空间的维数.
    virtual void MakeCanonical(void) = 0;
    bool IsCanonical(void) const;

    // 区间访问 [min,max].
    Real GetMin(void) const;
    Real GetMax(void) const;

    // 判断指定的参数是否在当前区间里.
    bool Contains(Real fParam) const;

protected:
    SELinComp(void);  // default : CT_NONE

    // 赋值运算符重载.
    SELinComp& operator = (const SELinComp& rComponent);

    // component type.
    int m_iType;

    // t值区间限制.
    Real m_fMin, m_fMax;
};

#include "SELinComp.inl"

typedef SELinComp<float> SELinCompf;
typedef SELinComp<double> SELinCompd;

}

#endif
