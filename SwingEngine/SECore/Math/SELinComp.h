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

    // linear component����ʾΪP + t*D����ʽ,����P��ԭ��,D�ǵ�λ���ȷ�������.
    // �û�����ȷ����������.tֵ���������ֱ��,����,�߶�,��,�ռ��Ժ�����.

    // ����.
    enum
    {
        CT_EMPTY,
        CT_POINT,
        CT_SEGMENT,
        CT_RAY,
        CT_LINE
    };

    int GetType(void) const;

    // tֵ���������,�����������������.
    // ����SetInterval(min, max)��������tֵ����.
    // ����������Ĳ��Դ�����ֿ��ܵĴ���ֵ:
    //   CT_LINE:
    //     [-MAX_REAL,MAX_REAL]
    //   CT_RAY:
    //     [min,MAX_REAL], ����min��finiteֵ
    //     [-MAX_REAL,max], ����max��finiteֵ
    //   CT_SEGMENT:
    //     [min,max], ����min��max��finiteֵ, ��min < max
    //   CT_POINT:
    //     [min,max], ����min��max��finiteֵ, �� min = max
    //   CT_EMPTY:
    //     [min,max], ����min > max��min = max = MAX_REAL��
    //                min = max = -MAX_REAL
    void SetInterval(Real fMin, Real fMax);

    // ���贴��SELinComp����Ϳɵ�֪������������.
    static int GetTypeFromInterval(Real fMin, Real fMax);

    // ����linear component���͵Ĺ淶������Ϊ:
    // ֱ����[-MAX_REAL,MAX_REAL],������[0,MAX_REAL],�߶���[-e,e],����e > 0,
    // ����[0,0],�ռ���[MAX_REAL,-MAX_REAL].�������������[min,max],
    // �������µ���.
    //
    // CT_RAY: ���max��MAX_REAL��min����,��P�㱻��ΪP' = P + min*D,
    // �Ӷ����߱���ʾΪP' + t*D,����t >= 0.
    // ���min��-MAX_REAL��max��finiteֵ,��P���D��������ΪP' = P + max*D
    // ��D' = -D.
    //
    // CT_SEGMENT: ���min������-max,��P����Ϊ
    // P' = P + ((min + max)/2)*D ���߶���չ������e' = (max - min)/2.
    //
    // CT_POINT: ���min��������, ��P�㱻��ΪP' = P + min*D.
    //
    // CT_EMPTY: ����maxΪ-MAX_REAL��minΪMAX_REAL.
    //
    // ��һ���������麯��,��Ϊ����Ĺ淶�����������������ռ��ά��.
    virtual void MakeCanonical(void) = 0;
    bool IsCanonical(void) const;

    // ������� [min,max].
    Real GetMin(void) const;
    Real GetMax(void) const;

    // �ж�ָ���Ĳ����Ƿ��ڵ�ǰ������.
    bool Contains(Real fParam) const;

protected:
    SELinComp(void);  // default : CT_NONE

    // ��ֵ���������.
    SELinComp& operator = (const SELinComp& rComponent);

    // component type.
    int m_iType;

    // tֵ��������.
    Real m_fMin, m_fMax;
};

#include "SELinComp.inl"

typedef SELinComp<float> SELinCompf;
typedef SELinComp<double> SELinCompd;

}

#endif
