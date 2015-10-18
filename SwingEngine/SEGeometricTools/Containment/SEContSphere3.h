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

#ifndef Swing_ContSphere3_H
#define Swing_ContSphere3_H

#include "SEGeometricToolsLIB.h"
#include "SESphere3.h"
#include "SEVector3.h"
#include "SEMath.h"

namespace Swing
{

// ������������ɶ���,��������������СAABBox��BV��.
SE_GEOMETRICTOOLS_API SESphere3f ContSphereOfAABBf(int iCount, const SEVector3f* aPoint);

// ������������ɶ���,������BV��,�����Ƕ����ƽ��ֵ.
SE_GEOMETRICTOOLS_API SESphere3f ContSphereAveragef(int iCount, const SEVector3f* aPoint);

// ����ָ�������Ƿ����������.
SE_GEOMETRICTOOLS_API bool IsInSpheref(const SEVector3f& rPoint, const SESphere3f& rSphere);

// ��������������BV��.
SE_GEOMETRICTOOLS_API SESphere3f MergeSpheresf(const SESphere3f& rSphere0, 
    const SESphere3f& rSphere1);

// ������������ɶ���,��������СBV��,
// �㷨����������붥������˳��,��˹���ʱ��Ԥ��ΪO(n).
class SE_GEOMETRICTOOLS_API SEMinSphere3f
{
public:
    SEMinSphere3f(int iCount, const SEVector3f* aPoint, SESphere3f& rMinimal, 
        float fEpsilon = 1.0e-03f);

private:
    // ��ǰ��С��Ķ�������֧��.
    class SE_GEOMETRICTOOLS_API SESupport
    {
    public:
        // �������붥���Ƿ�����support����,
        // ������붥����뼯����ĳ�����С�ڸ���epsilon,����Ϊ���ڼ���.
        bool Contains(int iIndex, SEVector3f** apPoint, float fEpsilon)
        {
            for( int i = 0; i < Count; i++ )
            {
                SEVector3f vec3fDiff = *apPoint[iIndex] - *apPoint[Index[i]];
                if( vec3fDiff.GetSquaredLength() < fEpsilon )
                {
                    return true;
                }
            }

            return false;
        }

        int Count;
        int Index[4];
    };

    // ���Զ���P�Ƿ��������S��.
    bool Contains(const SEVector3f& rPoint, const SESphere3f& rSphere, 
        float& rfDistDiff);

    SESphere3f ExactSphere1(const SEVector3f& rPoint);
    SESphere3f ExactSphere2(const SEVector3f& rPoint0, const SEVector3f& rPoint1);
    SESphere3f ExactSphere3(const SEVector3f& rPoint0, const SEVector3f& rPoint1, 
        const SEVector3f& rPoint2);
    SESphere3f ExactSphere4(const SEVector3f& rPoint0, const SEVector3f& rPoint1, 
        const SEVector3f& rPoint2, const SEVector3f& rPoint3);

    SESphere3f UpdateSupport1(int i, SEVector3f** apPermute, SESupport& rSupport);
    SESphere3f UpdateSupport2(int i, SEVector3f** apPermute, SESupport& rSupport);
    SESphere3f UpdateSupport3(int i, SEVector3f** apPermute, SESupport& rSupport);
    SESphere3f UpdateSupport4(int i, SEVector3f** apPermute, SESupport& rSupport);

    typedef SESphere3f (SEMinSphere3f::*UpdateFunction)(int, SEVector3f**, SESupport&);

    float m_fEpsilon, m_fOnePlusEpsilon;
    UpdateFunction m_aoUpdate[5];
};

}

#endif