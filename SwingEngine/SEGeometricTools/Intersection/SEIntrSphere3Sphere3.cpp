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
#include "SEIntrSphere3Sphere3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEIntrSphere3Sphere3f::SEIntrSphere3Sphere3f(const SESphere3f& rSphere0,
    const SESphere3f& rSphere1)
    :
    m_pSphere0(&rSphere0),
    m_pSphere1(&rSphere1)
{
}
//----------------------------------------------------------------------------
const SESphere3f& SEIntrSphere3Sphere3f::GetSphere0() const
{
    return *m_pSphere0;
}
//----------------------------------------------------------------------------
const SESphere3f& SEIntrSphere3Sphere3f::GetSphere1() const
{
    return *m_pSphere1;
}
//----------------------------------------------------------------------------
bool SEIntrSphere3Sphere3f::Test()
{
    SEVector3f vec3fDiff = m_pSphere1->Center - m_pSphere0->Center;
    float fRSum = m_pSphere0->Radius + m_pSphere1->Radius;

    return vec3fDiff.GetSquaredLength() <= fRSum*fRSum;
}
//----------------------------------------------------------------------------
bool SEIntrSphere3Sphere3f::Find()
{
    // 相交平面必有法线N.
    m_Normal = m_pSphere1->Center - m_pSphere0->Center;
    float fNSqrLen = m_Normal.GetSquaredLength();
    float fRSum = m_pSphere0->Radius + m_pSphere1->Radius;
    if( fNSqrLen > fRSum*fRSum )
    {
        return false;
    }

    float fR0Sqr = m_pSphere0->Radius*m_pSphere0->Radius;
    float fR1Sqr = m_pSphere1->Radius*m_pSphere1->Radius;
    float fInvNSqrLen = 1.0f / fNSqrLen;
    float fT = 0.5f * (1.0f + (fR0Sqr-fR1Sqr)*fInvNSqrLen);
    if( fT < 0.0f || fT > 1.0f )
    {
        return false;
    }

    float fRSqr = fR0Sqr - fT*fT*fNSqrLen;
    if( fRSqr < 0.0f )
    {
        return false;
    }

    // 相交圆的中心和半径.
    m_Center = m_pSphere0->Center + fT*m_Normal;
    m_fRadius = SEMathf::Sqrt(fRSqr);

    // 计算相交圆所在平面的U和V向量.
    m_Normal *= SEMathf::Sqrt(fInvNSqrLen);
    SEVector3f::GetOrthonormalBasis(m_UAxis, m_VAxis, m_Normal, true);

    return true;
}
//----------------------------------------------------------------------------
bool SEIntrSphere3Sphere3f::Test(float fTMax,
    const SEVector3f& rVelocity0, const SEVector3f& rVelocity1)
{
    SEVector3f vec3fVDiff = rVelocity1 - rVelocity0;
    float fA = vec3fVDiff.GetSquaredLength();
    SEVector3f vec3fCDiff = m_pSphere1->Center - m_pSphere0->Center;
    float fC = vec3fCDiff.GetSquaredLength();
    float fRSum = m_pSphere0->Radius + m_pSphere1->Radius;
    float fRSumSqr = fRSum*fRSum;

    if( fA > 0.0f )
    {
        float fB = vec3fCDiff.Dot(vec3fVDiff);
        if( fB <= 0.0f )
        {
            if( -fTMax*fA <= fB )
            {
                return fA*fC - fB*fB <= fA*fRSumSqr;
            }
            else
            {
                return fTMax*(fTMax*fA + 2.0f*fB) + fC <= fRSumSqr;
            }
        }
    }

    return fC <= fRSumSqr;
}
//----------------------------------------------------------------------------
bool SEIntrSphere3Sphere3f::Find(float fTMax,
    const SEVector3f& rVelocity0, const SEVector3f& rVelocity1)
{
    SEVector3f vec3fVDiff = rVelocity1 - rVelocity0;
    float fA = vec3fVDiff.GetSquaredLength();
    SEVector3f vec3fCDiff = m_pSphere1->Center - m_pSphere0->Center;
    float fC = vec3fCDiff.GetSquaredLength();
    float fRSum = m_pSphere0->Radius + m_pSphere1->Radius;
    float fRSumSqr = fRSum*fRSum;

    if( fA > 0.0f )
    {
        float fB = vec3fCDiff.Dot(vec3fVDiff);
        if( fB <= 0.0f )
        {
            if( -fTMax*fA <= fB
            || fTMax*(fTMax*fA + 2.0f*fB) + fC <= fRSumSqr )
            {
                float fCDiff = fC - fRSumSqr;
                float fDiscr = fB*fB - fA*fCDiff;
                if( fDiscr >= 0.0f )
                {
                    if( fCDiff <= 0.0f )
                    {
                        // 两球初始时相交.计算球心距离的中点作为相交点.
                        m_fContactTime = 0.0f;
                        m_ContactPoint = 0.5f * (m_pSphere0->Center +
                            m_pSphere1->Center);
                    }
                    else
                    {
                        // 第一次接触时间在[0,fTMax].
                        m_fContactTime = -(fB + SEMathf::Sqrt(fDiscr))/fA;
                        if( m_fContactTime < 0.0f )
                        {
                            m_fContactTime = 0.0f;
                        }
                        else if( m_fContactTime > fTMax )
                        {
                            m_fContactTime = fTMax;
                        }

                        SEVector3f vec3fNewCDiff = vec3fCDiff +
                            m_fContactTime*vec3fVDiff;

                        m_ContactPoint = m_pSphere0->Center +
                            m_fContactTime*rVelocity0 +
                            (m_pSphere0->Radius/fRSum)*vec3fNewCDiff;
                    }
                    return true;
                }
            }
            return false;
        }
    }

    if( fC <= fRSumSqr )
    {
        // 两球初始时相交.计算球心距离的中点作为相交点.
        m_fContactTime = 0.0f;
        m_ContactPoint = 0.5f * (m_pSphere0->Center +
            m_pSphere1->Center);

        return true;
    }

    return false;
}
//----------------------------------------------------------------------------
const SEVector3f& SEIntrSphere3Sphere3f::GetCenter() const
{
    return m_Center;
}
//----------------------------------------------------------------------------
const SEVector3f& SEIntrSphere3Sphere3f::GetUAxis() const
{
    return m_UAxis;
}
//----------------------------------------------------------------------------
const SEVector3f& SEIntrSphere3Sphere3f::GetVAxis() const
{
    return m_VAxis;
}
//----------------------------------------------------------------------------
const SEVector3f& SEIntrSphere3Sphere3f::GetNormal() const
{
    return m_Normal;
}
//----------------------------------------------------------------------------
float SEIntrSphere3Sphere3f::GetRadius() const
{
    return m_fRadius;
}
//----------------------------------------------------------------------------
const SEVector3f& SEIntrSphere3Sphere3f::GetContactPoint() const
{
    return m_ContactPoint;
}
//----------------------------------------------------------------------------

