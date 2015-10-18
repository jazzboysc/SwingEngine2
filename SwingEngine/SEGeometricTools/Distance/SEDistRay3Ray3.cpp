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

#include "SEGeometricToolsPCH.h"
#include "SEDistRay3Ray3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEDistRay3Ray3f::SEDistRay3Ray3f(const SERay3f& rRay0, const SERay3f& rRay1)
    :
    m_pRay0(&rRay0),
    m_pRay1(&rRay1)
{
}
//----------------------------------------------------------------------------
const SERay3f& SEDistRay3Ray3f::GetRay0() const
{
    return *m_pRay0;
}
//----------------------------------------------------------------------------
const SERay3f& SEDistRay3Ray3f::GetRay1() const
{
    return *m_pRay1;
}
//----------------------------------------------------------------------------
float SEDistRay3Ray3f::Get()
{
    float fSqrDist = GetSquared();

    return SEMath<float>::Sqrt(fSqrDist);
}
//----------------------------------------------------------------------------
float SEDistRay3Ray3f::GetSquared()
{
    SEVector3f vec3fDiff = m_pRay0->Origin - m_pRay1->Origin;
    float fA01 = -m_pRay0->Direction.Dot(m_pRay1->Direction);
    float fB0 = vec3fDiff.Dot(m_pRay0->Direction);
    float fC = vec3fDiff.GetSquaredLength();
    float fDet = SEMath<float>::FAbs(1.0f - fA01*fA01);
    float fB1, fS0, fS1, fSqrDist;

    if( fDet >= SEMath<float>::ZERO_TOLERANCE )
    {
        // 两射线不平行.
        fB1 = -vec3fDiff.Dot(m_pRay1->Direction);
        fS0 = fA01*fB1 - fB0;
        fS1 = fA01*fB0 - fB1;

        if( fS0 >= 0.0f )
        {
            if( fS1 >= 0.0f )  // region 0 (interior)
            {
                // 最近点是两射线线上点.
                float fInvDet = 1.0f / fDet;
                fS0 *= fInvDet;
                fS1 *= fInvDet;
                fSqrDist = fS0*(fS0 + fA01*fS1 + 2.0f*fB0) +
                    fS1*(fA01*fS0 + fS1 + 2.0f*fB1) + fC;
            }
            else  // region 3 (side)
            {
                fS1 = 0.0f;
                if( fB0 >= 0.0f )
                {
                    fS0 = 0.0f;
                    fSqrDist = fC;
                }
                else
                {
                    fS0 = -fB0;
                    fSqrDist = fB0*fS0 + fC;
                }
            }
        }
        else
        {
            if( fS1 >= 0.0f )  // region 1 (side)
            {
                fS0 = 0.0f;
                if( fB1 >= 0.0f )
                {
                    fS1 = 0.0f;
                    fSqrDist = fC;
                }
                else
                {
                    fS1 = -fB1;
                    fSqrDist = fB1*fS1 + fC;
                }
            }
            else  // region 2 (corner)
            {
                if( fB0 < 0.0f )
                {
                    fS0 = -fB0;
                    fS1 = 0.0f;
                    fSqrDist = fB0*fS0 + fC;
                }
                else
                {
                    fS0 = 0.0f;
                    if( fB1 >= 0.0f )
                    {
                        fS1 = 0.0f;
                        fSqrDist = fC;
                    }
                    else
                    {
                        fS1 = -fB1;
                        fSqrDist = fB1*fS1 + fC;
                    }
                }
            }
        }
    }
    else
    {
        // 两射线平行.
        if( fA01 > 0.0f )
        {
            // 方向向量的方向相反.
            fS1 = 0.0f;
            if( fB0 >= 0.0f )
            {
                fS0 = 0.0f;
                fSqrDist = fC;
            }
            else
            {
                fS0 = -fB0;
                fSqrDist = fB0*fS0 + fC;
            }
        }
        else
        {
            // 方向向量的方向相同.
            if( fB0 >= 0.0f )
            {
                fB1 = -vec3fDiff.Dot(m_pRay1->Direction);
                fS0 = 0.0f;
                fS1 = -fB1;
                fSqrDist = fB1*fS1 + fC;
            }
            else
            {
                fS0 = -fB0;
                fS1 = 0.0f;
                fSqrDist = fB0*fS0 + fC;
            }
        }
    }

    m_ClosestPoint0 = m_pRay0->Origin + fS0*m_pRay0->Direction;
    m_ClosestPoint1 = m_pRay1->Origin + fS1*m_pRay1->Direction;
    m_fRay0Parameter = fS0;
    m_fRay1Parameter = fS1;

    return SEMath<float>::FAbs(fSqrDist);
}
//----------------------------------------------------------------------------
float SEDistRay3Ray3f::Get(float fT, const SEVector3f& rVelocity0,
    const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMOrigin0 = m_pRay0->Origin + fT*rVelocity0;
    SEVector3f vec3fMOrigin1 = m_pRay1->Origin + fT*rVelocity1;
    SERay3f tempMRay0(vec3fMOrigin0, m_pRay0->Direction);
    SERay3f tempMRay1(vec3fMOrigin1, m_pRay1->Direction);

    return SEDistRay3Ray3f(tempMRay0, tempMRay1).Get();
}
//----------------------------------------------------------------------------
float SEDistRay3Ray3f::GetSquared(float fT, const SEVector3f& rVelocity0, 
    const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMOrigin0 = m_pRay0->Origin + fT*rVelocity0;
    SEVector3f vec3fMOrigin1 = m_pRay1->Origin + fT*rVelocity1;
    SERay3f tempMRay0(vec3fMOrigin0, m_pRay0->Direction);
    SERay3f tempMRay1(vec3fMOrigin1, m_pRay1->Direction);

    return SEDistRay3Ray3f(tempMRay0, tempMRay1).GetSquared();
}
//----------------------------------------------------------------------------
float SEDistRay3Ray3f::GetRay0Parameter() const
{
    return m_fRay0Parameter;
}
//----------------------------------------------------------------------------
float SEDistRay3Ray3f::GetRay1Parameter() const
{
    return m_fRay1Parameter;
}
//----------------------------------------------------------------------------
