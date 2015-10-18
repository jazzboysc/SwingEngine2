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
#include "SEDistRectangle3Rectangle3.h"
#include "SEDistSegment3Rectangle3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEDistRectangle3Rectangle3f::SEDistRectangle3Rectangle3f(
    const SERectangle3f& rRectangle0, const SERectangle3f& rRectangle1)
    :
    m_pRectangle0(&rRectangle0),
    m_pRectangle1(&rRectangle1)
{
}
//----------------------------------------------------------------------------
const SERectangle3f& SEDistRectangle3Rectangle3f::GetRectangle0() const
{
    return *m_pRectangle0;
}
//----------------------------------------------------------------------------
const SERectangle3f& SEDistRectangle3Rectangle3f::GetRectangle1() const
{
    return *m_pRectangle1;
}
//----------------------------------------------------------------------------
float SEDistRectangle3Rectangle3f::Get()
{
    float fSqrDist = GetSquared();

    return SEMath<float>::Sqrt(fSqrDist);
}
//----------------------------------------------------------------------------
float SEDistRectangle3Rectangle3f::GetSquared()
{
    // 把rectangle0各边和rectangle1比较.
    float fSqrDist = SEMath<float>::MAX_REAL, fSqrDistTmp;
    SESegment3f tempEdge;
    int i0, i1;
    for( i1 = 0; i1 < 2; i1++ )
    {
        for( i0 = -1; i0 <= 1; i0 += 2 )
        {
            tempEdge.Origin = m_pRectangle0->Center +
                (i0*m_pRectangle0->Extent[1 - i1]) *
                m_pRectangle0->Axis[1 - i1];
            tempEdge.Direction = m_pRectangle0->Axis[i1];
            tempEdge.Extent = m_pRectangle0->Extent[i1];
            SEDistSegment3Rectangle3f tempSRDist(tempEdge, *m_pRectangle1);
            fSqrDistTmp = tempSRDist.GetSquared();
            if( fSqrDistTmp < fSqrDist )
            {
                m_ClosestPoint0 = tempSRDist.GetClosestPoint0();
                m_ClosestPoint1 = tempSRDist.GetClosestPoint1();
                fSqrDist = fSqrDistTmp;
            }
        }
    }

    // 把rectangle1各边和rectangle0比较.
    for( i1 = 0; i1 < 2; i1++ )
    {
        for( i0 = -1; i0 <= 1; i0 += 2 )
        {
            tempEdge.Origin = m_pRectangle1->Center +
                (i0*m_pRectangle1->Extent[1 - i1]) *
                m_pRectangle1->Axis[1 - i1];
            tempEdge.Direction = m_pRectangle1->Axis[i1];
            tempEdge.Extent = m_pRectangle1->Extent[i1];
            SEDistSegment3Rectangle3f tempSRDist(tempEdge, *m_pRectangle0);
            fSqrDistTmp = tempSRDist.GetSquared();
            if( fSqrDistTmp < fSqrDist )
            {
                m_ClosestPoint0 = tempSRDist.GetClosestPoint0();
                m_ClosestPoint1 = tempSRDist.GetClosestPoint1();
                fSqrDist = fSqrDistTmp;
            }
        }
    }

    return fSqrDist;
}
//----------------------------------------------------------------------------
float SEDistRectangle3Rectangle3f::Get(float fT, const SEVector3f& rVelocity0, 
    const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMCenter0 = m_pRectangle0->Center + fT*rVelocity0;
    SEVector3f vec3fMCenter1 = m_pRectangle1->Center + fT*rVelocity1;
    SERectangle3f tempMRectangle0(vec3fMCenter0, m_pRectangle0->Axis,
        m_pRectangle0->Extent);
    SERectangle3f tempMRectangle1(vec3fMCenter1, m_pRectangle1->Axis,
        m_pRectangle1->Extent);

    return SEDistRectangle3Rectangle3f(tempMRectangle0, tempMRectangle1).Get();
}
//----------------------------------------------------------------------------
float SEDistRectangle3Rectangle3f::GetSquared(float fT,
    const SEVector3f& rVelocity0, const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMCenter0 = m_pRectangle0->Center + fT*rVelocity0;
    SEVector3f vec3fMCenter1 = m_pRectangle1->Center + fT*rVelocity1;
    SERectangle3f tempMRectangle0(vec3fMCenter0, m_pRectangle0->Axis,
        m_pRectangle0->Extent);
    SERectangle3f tempMRectangle1(vec3fMCenter1, m_pRectangle1->Axis,
        m_pRectangle1->Extent);

    return SEDistRectangle3Rectangle3f(tempMRectangle0,
        tempMRectangle1).GetSquared();
}
//----------------------------------------------------------------------------
