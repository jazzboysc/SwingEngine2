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
#include "SEDistSegment2Segment2.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEDistSegment2Segment2f::SEDistSegment2Segment2f(const SESegment2f& rSegment0, 
    const SESegment2f& rSegment1)
    :
    m_pSegment0(&rSegment0),
    m_pSegment1(&rSegment1)
{
}
//----------------------------------------------------------------------------
const SESegment2f& SEDistSegment2Segment2f::GetSegment0() const
{
    return *m_pSegment0;
}
//----------------------------------------------------------------------------
const SESegment2f& SEDistSegment2Segment2f::GetSegment1() const
{
    return *m_pSegment1;
}
//----------------------------------------------------------------------------
float SEDistSegment2Segment2f::Get()
{
    float fSqrDist = GetSquared();

    return SEMath<float>::Sqrt(fSqrDist);
}
//----------------------------------------------------------------------------
float SEDistSegment2Segment2f::GetSquared()
{
    SEVector2f vec2fDiff = m_pSegment0->Origin - m_pSegment1->Origin;
    float fA01 = -m_pSegment0->Direction.Dot(m_pSegment1->Direction);
    float fB0 = vec2fDiff.Dot(m_pSegment0->Direction);
    float fB1 = -vec2fDiff.Dot(m_pSegment1->Direction);
    float fC = vec2fDiff.GetSquaredLength();
    float fDet = SEMath<float>::FAbs(1.0f - fA01*fA01);
    float fS0, fS1, fSqrDist, fExtDet0, fExtDet1, fTmpS0, fTmpS1;

    if( fDet >= SEMath<float>::ZERO_TOLERANCE )
    {
        // ���߶β�ƽ��.
        fS0 = fA01*fB1 - fB0;
        fS1 = fA01*fB0 - fB1;
        fExtDet0 = m_pSegment0->Extent*fDet;
        fExtDet1 = m_pSegment1->Extent*fDet;

        if( fS0 >= -fExtDet0 )
        {
            if( fS0 <= fExtDet0 )
            {
                if( fS1 >= -fExtDet1 )
                {
                    if( fS1 <= fExtDet1 )  // region 0 (interior)
                    {
                        // ����������߶����ϵ�.
                        float fInvDet = 1.0f / fDet;
                        fS0 *= fInvDet;
                        fS1 *= fInvDet;
                        fSqrDist = 0.0f;
                    }
                    else  // region 3 (side)
                    {
                        fS1 = m_pSegment1->Extent;
                        fTmpS0 = -(fA01*fS1 + fB0);
                        if( fTmpS0 < -m_pSegment0->Extent )
                        {
                            fS0 = -m_pSegment0->Extent;
                            fSqrDist = fS0*(fS0 - 2.0f*fTmpS0) +
                                fS1*(fS1 + 2.0f*fB1) + fC;
                        }
                        else if( fTmpS0 <= m_pSegment0->Extent )
                        {
                            fS0 = fTmpS0;
                            fSqrDist = -fS0*fS0 + fS1*(fS1 + 2.0f*fB1) + fC;
                        }
                        else
                        {
                            fS0 = m_pSegment0->Extent;
                            fSqrDist = fS0*(fS0 - 2.0f*fTmpS0) +
                                fS1*(fS1 + 2.0f*fB1) + fC;
                        }
                    }
                }
                else  // region 7 (side)
                {
                    fS1 = -m_pSegment1->Extent;
                    fTmpS0 = -(fA01*fS1 + fB0);
                    if( fTmpS0 < -m_pSegment0->Extent )
                    {
                        fS0 = -m_pSegment0->Extent;
                        fSqrDist = fS0*(fS0 - 2.0f*fTmpS0) +
                            fS1*(fS1 + 2.0f*fB1) + fC;
                    }
                    else if( fTmpS0 <= m_pSegment0->Extent )
                    {
                        fS0 = fTmpS0;
                        fSqrDist = -fS0*fS0 + fS1*(fS1 + 2.0f*fB1) + fC;
                    }
                    else
                    {
                        fS0 = m_pSegment0->Extent;
                        fSqrDist = fS0*(fS0 - 2.0f*fTmpS0) +
                            fS1*(fS1 + 2.0f*fB1) + fC;
                    }
                }
            }
            else
            {
                if( fS1 >= -fExtDet1 )
                {
                    if( fS1 <= fExtDet1 )  // region 1 (side)
                    {
                        fS0 = m_pSegment0->Extent;
                        fTmpS1 = -(fA01*fS0 + fB1);
                        if( fTmpS1 < -m_pSegment1->Extent )
                        {
                            fS1 = -m_pSegment1->Extent;
                            fSqrDist = fS1*(fS1 - 2.0f*fTmpS1) +
                                fS0*(fS0 + 2.0f*fB0) + fC;
                        }
                        else if( fTmpS1 <= m_pSegment1->Extent )
                        {
                            fS1 = fTmpS1;
                            fSqrDist = -fS1*fS1 + fS0*(fS0 + 2.0f*fB0) + fC;
                        }
                        else
                        {
                            fS1 = m_pSegment1->Extent;
                            fSqrDist = fS1*(fS1 - 2.0f*fTmpS1) +
                                fS0*(fS0 + 2.0f*fB0) + fC;
                        }
                    }
                    else  // region 2 (corner)
                    {
                        fS1 = m_pSegment1->Extent;
                        fTmpS0 = -(fA01*fS1 + fB0);
                        if( fTmpS0 < -m_pSegment0->Extent )
                        {
                            fS0 = -m_pSegment0->Extent;
                            fSqrDist = fS0*(fS0 - 2.0f*fTmpS0) +
                                fS1*(fS1 + 2.0f*fB1) + fC;
                        }
                        else if( fTmpS0 <= m_pSegment0->Extent )
                        {
                            fS0 = fTmpS0;
                            fSqrDist = -fS0*fS0 + fS1*(fS1 + 2.0f*fB1) + fC;
                        }
                        else
                        {
                            fS0 = m_pSegment0->Extent;
                            fTmpS1 = -(fA01*fS0 + fB1);
                            if( fTmpS1 < -m_pSegment1->Extent )
                            {
                                fS1 = -m_pSegment1->Extent;
                                fSqrDist = fS1*(fS1 - 2.0f*fTmpS1) +
                                    fS0*(fS0 + 2.0f*fB0) + fC;
                            }
                            else if( fTmpS1 <= m_pSegment1->Extent )
                            {
                                fS1 = fTmpS1;
                                fSqrDist = -fS1*fS1 + fS0*(fS0 + 2.0f*fB0)
                                    + fC;
                            }
                            else
                            {
                                fS1 = m_pSegment1->Extent;
                                fSqrDist = fS1*(fS1 - 2.0f*fTmpS1) +
                                    fS0*(fS0 + 2.0f*fB0) + fC;
                            }
                        }
                    }
                }
                else  // region 8 (corner)
                {
                    fS1 = -m_pSegment1->Extent;
                    fTmpS0 = -(fA01*fS1 + fB0);
                    if( fTmpS0 < -m_pSegment0->Extent )
                    {
                        fS0 = -m_pSegment0->Extent;
                        fSqrDist = fS0*(fS0 - 2.0f*fTmpS0) +
                            fS1*(fS1 + 2.0f*fB1) + fC;
                    }
                    else if( fTmpS0 <= m_pSegment0->Extent )
                    {
                        fS0 = fTmpS0;
                        fSqrDist = -fS0*fS0 + fS1*(fS1 + 2.0f*fB1) + fC;
                    }
                    else
                    {
                        fS0 = m_pSegment0->Extent;
                        fTmpS1 = -(fA01*fS0 + fB1);
                        if( fTmpS1 > m_pSegment1->Extent )
                        {
                            fS1 = m_pSegment1->Extent;
                            fSqrDist = fS1*(fS1 - 2.0f*fTmpS1) +
                                fS0*(fS0 + 2.0f*fB0) + fC;
                        }
                        else if( fTmpS1 >= -m_pSegment1->Extent )
                        {
                            fS1 = fTmpS1;
                            fSqrDist = -fS1*fS1 + fS0*(fS0 + 2.0f*fB0)
                                + fC;
                        }
                        else
                        {
                            fS1 = -m_pSegment1->Extent;
                            fSqrDist = fS1*(fS1 - 2.0f*fTmpS1) +
                                fS0*(fS0 + 2.0f*fB0) + fC;
                        }
                    }
                }
            }
        }
        else 
        {
            if( fS1 >= -fExtDet1 )
            {
                if( fS1 <= fExtDet1 )  // region 5 (side)
                {
                    fS0 = -m_pSegment0->Extent;
                    fTmpS1 = -(fA01*fS0 + fB1);
                    if( fTmpS1 < -m_pSegment1->Extent )
                    {
                        fS1 = -m_pSegment1->Extent;
                        fSqrDist = fS1*(fS1 - 2.0f*fTmpS1) +
                            fS0*(fS0 + 2.0f*fB0) + fC;
                    }
                    else if( fTmpS1 <= m_pSegment1->Extent )
                    {
                        fS1 = fTmpS1;
                        fSqrDist = -fS1*fS1 + fS0*(fS0 + 2.0f*fB0) + fC;
                    }
                    else
                    {
                        fS1 = m_pSegment1->Extent;
                        fSqrDist = fS1*(fS1 - 2.0f*fTmpS1) +
                            fS0*(fS0 + 2.0f*fB0) + fC;
                    }
                }
                else  // region 4 (corner)
                {
                    fS1 = m_pSegment1->Extent;
                    fTmpS0 = -(fA01*fS1 + fB0);
                    if( fTmpS0 > m_pSegment0->Extent )
                    {
                        fS0 = m_pSegment0->Extent;
                        fSqrDist = fS0*(fS0 - 2.0f*fTmpS0) +
                            fS1*(fS1 + 2.0f*fB1) + fC;
                    }
                    else if( fTmpS0 >= -m_pSegment0->Extent )
                    {
                        fS0 = fTmpS0;
                        fSqrDist = -fS0*fS0 + fS1*(fS1 + 2.0f*fB1) + fC;
                    }
                    else
                    {
                        fS0 = -m_pSegment0->Extent;
                        fTmpS1 = -(fA01*fS0 + fB1);
                        if( fTmpS1 < -m_pSegment1->Extent )
                        {
                            fS1 = -m_pSegment1->Extent;
                            fSqrDist = fS1*(fS1 - 2.0f*fTmpS1) +
                                fS0*(fS0 + 2.0f*fB0) + fC;
                        }
                        else if( fTmpS1 <= m_pSegment1->Extent )
                        {
                            fS1 = fTmpS1;
                            fSqrDist = -fS1*fS1 + fS0*(fS0 + 2.0f*fB0)
                                + fC;
                        }
                        else
                        {
                            fS1 = m_pSegment1->Extent;
                            fSqrDist = fS1*(fS1 - 2.0f*fTmpS1) +
                                fS0*(fS0 + 2.0f*fB0) + fC;
                        }
                    }
                }
            }
            else   // region 6 (corner)
            {
                fS1 = -m_pSegment1->Extent;
                fTmpS0 = -(fA01*fS1 + fB0);
                if( fTmpS0 > m_pSegment0->Extent )
                {
                    fS0 = m_pSegment0->Extent;
                    fSqrDist = fS0*(fS0 - 2.0f*fTmpS0) +
                        fS1*(fS1 + 2.0f*fB1) + fC;
                }
                else if( fTmpS0 >= -m_pSegment0->Extent )
                {
                    fS0 = fTmpS0;
                    fSqrDist = -fS0*fS0 + fS1*(fS1 + 2.0f*fB1) + fC;
                }
                else
                {
                    fS0 = -m_pSegment0->Extent;
                    fTmpS1 = -(fA01*fS0 + fB1);
                    if( fTmpS1 < -m_pSegment1->Extent )
                    {
                        fS1 = -m_pSegment1->Extent;
                        fSqrDist = fS1*(fS1 - 2.0f*fTmpS1) +
                            fS0*(fS0 + 2.0f*fB0) + fC;
                    }
                    else if( fTmpS1 <= m_pSegment1->Extent )
                    {
                        fS1 = fTmpS1;
                        fSqrDist = -fS1*fS1 + fS0*(fS0 + 2.0f*fB0)
                            + fC;
                    }
                    else
                    {
                        fS1 = m_pSegment1->Extent;
                        fSqrDist = fS1*(fS1 - 2.0f*fTmpS1) +
                            fS0*(fS0 + 2.0f*fB0) + fC;
                    }
                }
            }
        }
    }
    else
    {
        // ���߶�ƽ��.
        float fE0pE1 = m_pSegment0->Extent + m_pSegment1->Extent;
        float fSign = (fA01 > 0.0f ? -1.0f : 1.0f);
        float fLambda = -fB0;
        if( fLambda < -fE0pE1 )
        {
            fLambda = -fE0pE1;
        }
        else if( fLambda > fE0pE1 )
        {
            fLambda = fE0pE1;
        }

        fS1 = fSign*fB0*m_pSegment1->Extent/fE0pE1;
        fS0 = fLambda + fSign*fS1;
        fSqrDist = fLambda*(fLambda + 2.0f*fB0) + fC;
    }

    m_ClosestPoint0 = m_pSegment0->Origin + fS0*m_pSegment0->Direction;
    m_ClosestPoint1 = m_pSegment1->Origin + fS1*m_pSegment1->Direction;

    return SEMath<float>::FAbs(fSqrDist);
}
//----------------------------------------------------------------------------
float SEDistSegment2Segment2f::Get(float fS1, const SEVector2f& rVelocity0, 
    const SEVector2f& rVelocity1)
{
    SEVector2f vec2fMOrigin0 = m_pSegment0->Origin + fS1*rVelocity0;
    SEVector2f vec2fMOrigin1 = m_pSegment1->Origin + fS1*rVelocity1;
    SESegment2f tempMSegment0(vec2fMOrigin0, m_pSegment0->Direction,
        m_pSegment0->Extent);
    SESegment2f tempMSegment1(vec2fMOrigin1, m_pSegment1->Direction,
        m_pSegment1->Extent);

    return SEDistSegment2Segment2f(tempMSegment0, tempMSegment1).Get();
}
//----------------------------------------------------------------------------
float SEDistSegment2Segment2f::GetSquared(float fS1, const SEVector2f& 
    rVelocity0, const SEVector2f& rVelocity1)
{
    SEVector2f vec2fMOrigin0 = m_pSegment0->Origin + fS1*rVelocity0;
    SEVector2f vec2fMOrigin1 = m_pSegment1->Origin + fS1*rVelocity1;
    SESegment2f tempMSegment0(vec2fMOrigin0, m_pSegment0->Direction,
        m_pSegment0->Extent);
    SESegment2f tempMSegment1(vec2fMOrigin1, m_pSegment1->Direction,
        m_pSegment1->Extent);

    return SEDistSegment2Segment2f(tempMSegment0, tempMSegment1).GetSquared();
}
//----------------------------------------------------------------------------
