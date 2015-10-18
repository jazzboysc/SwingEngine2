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
#include "SEDistTriangle3Rectangle3.h"
#include "SEDistSegment3Triangle3.h"
#include "SEDistSegment3Rectangle3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEDistTriangle3Rectangle3f::SEDistTriangle3Rectangle3f(
    const SETriangle3f& rTriangle, const SERectangle3f& rRectangle)
    :
    m_pTriangle(&rTriangle),
    m_pRectangle(&rRectangle)
{
}
//----------------------------------------------------------------------------
const SETriangle3f& SEDistTriangle3Rectangle3f::GetTriangle() const
{
    return *m_pTriangle;
}
//----------------------------------------------------------------------------
const SERectangle3f& SEDistTriangle3Rectangle3f::GetRectangle() const
{
    return *m_pRectangle;
}
//----------------------------------------------------------------------------
float SEDistTriangle3Rectangle3f::Get()
{
    float fSqrDist = GetSquared();

    return SEMath<float>::Sqrt(fSqrDist);
}
//----------------------------------------------------------------------------
float SEDistTriangle3Rectangle3f::GetSquared()
{
    // 把triangle各边和rectangle比较.
    float fSqrDist = SEMath<float>::MAX_REAL, fSqrDistTmp;
    SESegment3f tempEdge;
    int i0, i1;
    for( i0 = 2, i1 = 0; i1 < 3; i0 = i1++ )
    {
        tempEdge.Origin = 0.5f*(m_pTriangle->V[i0] + m_pTriangle->V[i1]);
        tempEdge.Direction = m_pTriangle->V[i1] - m_pTriangle->V[i0];
        tempEdge.Extent = 0.5f*tempEdge.Direction.Normalize();
        SEDistSegment3Rectangle3f tempSRDist(tempEdge, *m_pRectangle);
        fSqrDistTmp = tempSRDist.GetSquared();
        if( fSqrDistTmp < fSqrDist )
        {
            m_ClosestPoint0 = tempSRDist.GetClosestPoint0();
            m_ClosestPoint1 = tempSRDist.GetClosestPoint1();
            fSqrDist = fSqrDistTmp;
        }
    }

    // 把rectangle各边和triangle比较.
    for( i1 = 0; i1 < 2; i1++ )
    {
        for( i0 = -1; i0 <= 1; i0 += 2 )
        {
            tempEdge.Origin = m_pRectangle->Center +
                (i0*m_pRectangle->Extent[1 - i1]) *
                m_pRectangle->Axis[1 - i1];
            tempEdge.Direction = m_pRectangle->Axis[i1];
            tempEdge.Extent = m_pRectangle->Extent[i1];
            SEDistSegment3Triangle3f tempSTDist(tempEdge, *m_pTriangle);
            fSqrDistTmp = tempSTDist.GetSquared();
            if( fSqrDistTmp < fSqrDist )
            {
                m_ClosestPoint0 = tempSTDist.GetClosestPoint0();
                m_ClosestPoint1 = tempSTDist.GetClosestPoint1();
                fSqrDist = fSqrDistTmp;
            }
        }
    }

    return fSqrDist;
}
//----------------------------------------------------------------------------
float SEDistTriangle3Rectangle3f::Get(float fT, const SEVector3f& rVelocity0, 
    const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMV0 = m_pTriangle->V[0] + fT*rVelocity0;
    SEVector3f vec3fMV1 = m_pTriangle->V[1] + fT*rVelocity0;
    SEVector3f vec3fMV2 = m_pTriangle->V[2] + fT*rVelocity0;
    SEVector3f vec3fMCenter = m_pRectangle->Center + fT*rVelocity1;
    SETriangle3f tempMTriangle(vec3fMV0, vec3fMV1, vec3fMV2);
    SERectangle3f tempMRectangle(vec3fMCenter, m_pRectangle->Axis,
        m_pRectangle->Extent);

    return SEDistTriangle3Rectangle3f(tempMTriangle, tempMRectangle).Get();
}
//----------------------------------------------------------------------------
float SEDistTriangle3Rectangle3f::GetSquared(float fT, 
    const SEVector3f& rVelocity0, const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMV0 = m_pTriangle->V[0] + fT*rVelocity0;
    SEVector3f vec3fMV1 = m_pTriangle->V[1] + fT*rVelocity0;
    SEVector3f vec3fMV2 = m_pTriangle->V[2] + fT*rVelocity0;
    SEVector3f vec3fMCenter = m_pRectangle->Center + fT*rVelocity1;
    SETriangle3f tempMTriangle(vec3fMV0, vec3fMV1, vec3fMV2);
    SERectangle3f tempMRectangle(vec3fMCenter, m_pRectangle->Axis,
        m_pRectangle->Extent);

    return SEDistTriangle3Rectangle3f(tempMTriangle, 
        tempMRectangle).GetSquared();
}
//----------------------------------------------------------------------------
