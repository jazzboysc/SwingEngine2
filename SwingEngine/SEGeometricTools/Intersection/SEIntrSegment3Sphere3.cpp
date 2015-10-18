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
#include "SEIntrSegment3Sphere3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEIntrSegment3Sphere3f::SEIntrSegment3Sphere3f(const SESegment3f& rSegment, 
    const SESphere3f& rSphere)
    :
    m_pSegment(&rSegment),
    m_pSphere(&rSphere)
{
    m_iCount = 0;
    ZeroThreshold = SEMath<float>::ZERO_TOLERANCE;
}
//----------------------------------------------------------------------------
const SESegment3f& SEIntrSegment3Sphere3f::GetSegment() const
{
    return *m_pSegment;
}
//----------------------------------------------------------------------------
const SESphere3f& SEIntrSegment3Sphere3f::GetSphere() const
{
    return *m_pSphere;
}
//----------------------------------------------------------------------------
bool SEIntrSegment3Sphere3f::Test()
{
    SEVector3f vec3fDiff = m_pSegment->Origin - m_pSphere->Center;
    float fA0 = vec3fDiff.Dot(vec3fDiff) - m_pSphere->Radius*m_pSphere->Radius;
    float fA1 = m_pSegment->Direction.Dot(vec3fDiff);
    float fDiscr = fA1*fA1 - fA0;
    if( fDiscr < 0.0f )
    {
        return false;
    }

    float fTmp0 = m_pSegment->Extent*m_pSegment->Extent + fA0;
    float fTmp1 = 2.0f*fA1*m_pSegment->Extent;
    float fQM = fTmp0 - fTmp1;
    float fQP = fTmp0 + fTmp1;
    if( fQM*fQP <= 0.0f )
    {
        return true;
    }

    return fQM > 0.0f && SEMath<float>::FAbs(fA1) < m_pSegment->Extent;
}
//----------------------------------------------------------------------------
bool SEIntrSegment3Sphere3f::Find()
{
    // 待检查.
    // 线段端点在球上的情况过于复杂.

    SEVector3f vec3fDiff = m_pSegment->Origin - m_pSphere->Center;
    float fA0 = vec3fDiff.Dot(vec3fDiff) - m_pSphere->Radius*m_pSphere->Radius;
    float fA1 = m_pSegment->Direction.Dot(vec3fDiff);
    float fDiscr = fA1*fA1 - fA0;
    if( fDiscr < 0.0f )
    {
        // 判别式小于零,线段所在直线与球没有交点.
        m_iCount = 0;

        return false;
    }

    float fTmp0 = m_pSegment->Extent*m_pSegment->Extent + fA0;
    float fTmp1 = 2.0f*fA1*m_pSegment->Extent;
    float fQM = fTmp0 - fTmp1;
    float fQP = fTmp0 + fTmp1;
    float fQMmQP = fQM*fQP;
    float fRoot;
    if( fQMmQP < 0.0f )
    {
        // 线段两端点一个在球内一个在球外,必与球有一个交点.

        fRoot = SEMath<float>::Sqrt(fDiscr);
        m_afSegmentT[0] = (fQM > 0.0f ? -fA1 - fRoot : -fA1 + fRoot);
        m_aPoint[0] = m_pSegment->Origin + m_afSegmentT[0] *
            m_pSegment->Direction;
        m_iCount = 1;

        return true;
    }

    if( fQMmQP == 0.0f )
    {
        // 线段两端点至少有一个在球上,至少与球有一个交点.

        if( fQM == 0.0f && fQP == 0.0f )
        {
            m_afSegmentT[0] = -m_pSegment->Extent;
            m_afSegmentT[1] = m_pSegment->Extent;
            m_aPoint[0] = m_pSegment->Origin + m_afSegmentT[0] *
                m_pSegment->Direction;
            m_aPoint[1] = m_pSegment->Origin + m_afSegmentT[1] *
                m_pSegment->Direction;
            m_iCount = 2;
        }
        else if( fQM == 0.0f )
        {
            if( fQP < 0.0f )
            {
                m_afSegmentT[0] = -m_pSegment->Extent;
                m_aPoint[0] = m_pSegment->Origin + m_afSegmentT[0] *
                    m_pSegment->Direction;
                m_iCount = 1;
            }
            else
            {
                if( fDiscr >= ZeroThreshold )
                {
                    if( fA1 < m_pSegment->Extent )
                    {
                        fRoot = SEMath<float>::Sqrt(fDiscr);
                        m_afSegmentT[0] = -m_pSegment->Extent;
                        m_afSegmentT[1] = -fA1 + fRoot;
                        m_aPoint[0] = m_pSegment->Origin + m_afSegmentT[0] *
                            m_pSegment->Direction;
                        m_aPoint[1] = m_pSegment->Origin + m_afSegmentT[1] *
                            m_pSegment->Direction;
                        m_iCount = 2;
                    }
                    else
                    {
                        m_afSegmentT[0] = -m_pSegment->Extent;
                        m_aPoint[0] = m_pSegment->Origin + m_afSegmentT[0] *
                            m_pSegment->Direction;
                        m_iCount = 1;
                    }
                }
                else
                {
                    m_afSegmentT[0] = -m_pSegment->Extent;
                    m_aPoint[0] = m_pSegment->Origin + m_afSegmentT[0] *
                        m_pSegment->Direction;
                    m_iCount = 1;
                }
            }
        }
        else  // fQP == 0.0f
        {
            if( fQM < 0.0f )
            {
                m_afSegmentT[0] = m_pSegment->Extent;
                m_aPoint[0] = m_pSegment->Origin + m_afSegmentT[0] *
                    m_pSegment->Direction;
                m_iCount = 1;
            }
            else
            {
                if( fDiscr >= ZeroThreshold )
                {
                    if( -fA1 < m_pSegment->Extent )
                    {
                        fRoot = SEMath<float>::Sqrt(fDiscr);
                        m_afSegmentT[0] = -fA1 - fRoot;
                        m_afSegmentT[1] = m_pSegment->Extent;
                        m_aPoint[0] = m_pSegment->Origin + m_afSegmentT[0] *
                            m_pSegment->Direction;
                        m_aPoint[1] = m_pSegment->Origin + m_afSegmentT[1] *
                            m_pSegment->Direction;
                        m_iCount = 2;
                    }
                    else
                    {
                        m_afSegmentT[0] = m_pSegment->Extent;
                        m_aPoint[0] = m_pSegment->Origin + m_afSegmentT[0] *
                            m_pSegment->Direction;
                        m_iCount = 1;
                    }
                }
                else
                {
                    m_afSegmentT[0] = m_pSegment->Extent;
                    m_aPoint[0] = m_pSegment->Origin + m_afSegmentT[0] *
                        m_pSegment->Direction;
                    m_iCount = 1;
                }
            }
        }

        return true;
    }

    if( fQM > 0.0f && SEMath<float>::FAbs(fA1) < m_pSegment->Extent )
    {
        // 线段两端点都在球外,且二次曲线极小值对应的t值在线段上.

        if( fDiscr >= ZeroThreshold )
        {
            fRoot = SEMath<float>::Sqrt(fDiscr);
            m_afSegmentT[0] = -fA1 - fRoot;
            m_afSegmentT[1] = -fA1 + fRoot;
            m_aPoint[0] = m_pSegment->Origin + m_afSegmentT[0] *
                m_pSegment->Direction;
            m_aPoint[1] = m_pSegment->Origin + m_afSegmentT[1] *
                m_pSegment->Direction;
            m_iCount = 2;
        }
        else
        {
            m_afSegmentT[0] = -fA1;
            m_aPoint[0] = m_pSegment->Origin + m_afSegmentT[0] *
                m_pSegment->Direction;
            m_iCount = 1;
        }
    }
    else
    {
        m_iCount = 0;
    }

    return m_iCount > 0;
}
//----------------------------------------------------------------------------
bool SEIntrSegment3Sphere3f::Test(float, const SEVector3f&, const SEVector3f&)
{
    // 待实现.
    return false;
}
//----------------------------------------------------------------------------
bool SEIntrSegment3Sphere3f::Find(float, const SEVector3f&, const SEVector3f&)
{
    // 待实现.
    return false;
}
//----------------------------------------------------------------------------
int SEIntrSegment3Sphere3f::GetCount() const
{
    return m_iCount;
}
//----------------------------------------------------------------------------
const SEVector3f& SEIntrSegment3Sphere3f::GetPoint(int i) const
{
    SE_ASSERT( 0 <= i && i < m_iCount );

    return m_aPoint[i];
}
//----------------------------------------------------------------------------
float SEIntrSegment3Sphere3f::GetSegmentT(int i) const
{
    SE_ASSERT( 0 <= i && i < m_iCount );

    return m_afSegmentT[i];
}
//----------------------------------------------------------------------------
