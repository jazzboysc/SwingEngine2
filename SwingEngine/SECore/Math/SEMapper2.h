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

#ifndef Swing_Mapper2_H
#define Swing_Mapper2_H

#include "SECoreLIB.h"
#include "SEVector2.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20081202
//----------------------------------------------------------------------------
class SE_CORE_API SEMapper2f
{
public:
    // �����㴫�붥�㼯�ϵ�ά��ʱ,epsilonֵ����������.
    SEMapper2f(int iVCount, const SEVector2f* aVertex, float fEpsilon);
    ~SEMapper2f(void);

    // ���붥�㼯�ϵ�AABBox.
    const SEVector2f& GetMin(void) const;
    const SEVector2f& GetMax(void) const;
    float GetMaxRange(void) const;

    // ���ϵ�ά��(0, 1, ��2).
    int GetDimension(void) const;

    // ����ϵͳ.�����κ�ά��d,origin����Ч.
    // ��λ����������0 <= i < d��Χ����Ч.
    // extreme index�봫�붥�������й�,��0 <= i < d��Χ����Ч.
    // ���d = 0,�����ж��㶼��ͬ,����ʹ����epsilon,��extreme index���ܲ���0.
    // ���d = 1,�����ж��㶼��һ���߶���.extreme index��ʾ�����߶εĶ˵�.
    // ���d = 2,��ǰ����extreme index��ʾһ���߶�.
    // ֮���extreme index��ʾ�ڴ�ֱ�������߶εķ�����,������߶���Զ�Ķ���.
    const SEVector2f& GetOrigin(void) const;
    const SEVector2f& GetDirection(int i) const;
    int GetExtremeIndex(int i) const;

    // ���d = 2,��������{U0,U1}������������ϵ.
    // ������ֵ���㹹��һ��������.�����������������Ƿ�Ϊ��ʱ������˳��.
    bool GetExtremeCCW(void) const;

private:
    // ���붥���AABBox.
    // maximum range��max[0]-min[0]��max[1]-min[1]�еĽϴ���.
    SEVector2f m_Min, m_Max;
    float m_fMaxRange;

    // ���붥�㼯�ϵĹ���ά��.
    // ���ж�ά��ʱ,�������캯���Ĳ���fEpsilon�����ṩһ��������ֵ.
    int m_iDimension;

    // ��������maximum dimensional extents������ֵ.
    // m_aiExtreme[0]��m_aiExtreme[1]��������������,
    // ���������㶨���˾߱����extent����������.
    // �������ά����2,��m_aiExtreme[2]����һ����Ч�Ķ�������,
    // �ö��㶨���˴�ֱ��m_aiExtreme[0]��m_aiExtreme[1]���������ϵ����extent.
    // ������<V[extreme0],V[extreme1],V[extreme2]>������˳ʱ�������ʱ������
    // ˳��, ��m_bExtremeCCW����.
    int m_aiExtreme[3];
    bool m_bExtremeCCW;

    SEVector2f m_Origin;
    SEVector2f m_aDirection[2];
};

}

#endif
