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
#include "SEDistSegment3Rectangle3.h"
#include "SEDistLine3Rectangle3.h"
#include "SEDistVector3Rectangle3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEDistSegment3Rectangle3f::SEDistSegment3Rectangle3f(const SESegment3f& 
    rSegment, const SERectangle3f& rRectangle)
    :
    m_pSegment(&rSegment),
    m_pRectangle(&rRectangle)
{
}
//----------------------------------------------------------------------------
const SESegment3f& SEDistSegment3Rectangle3f::GetSegment() const
{
    return *m_pSegment;
}
//----------------------------------------------------------------------------
const SERectangle3f& SEDistSegment3Rectangle3f::GetRectangle() const
{
    return *m_pRectangle;
}
//----------------------------------------------------------------------------
float SEDistSegment3Rectangle3f::Get()
{
    float fSqrDist = GetSquared();

    return SEMath<float>::Sqrt(fSqrDist);
}
//----------------------------------------------------------------------------
float SEDistSegment3Rectangle3f::GetSquared()
{
    SEDistLine3Rectangle3f tempLRDist(SELine3f(m_pSegment->Origin, 
        m_pSegment->Direction), *m_pRectangle);

    float fSqrDist = tempLRDist.GetSquared();

    m_fSegmentParameter = tempLRDist.GetLineParameter();
    if( m_fSegmentParameter >= -m_pSegment->Extent)
    {
        if( m_fSegmentParameter <= m_pSegment->Extent)
        {
            m_ClosestPoint0 = tempLRDist.GetClosestPoint0();
            m_ClosestPoint1 = tempLRDist.GetClosestPoint1();
            m_afRectCoord[0] = tempLRDist.GetRectangleCoordinate(0);
            m_afRectCoord[1] = tempLRDist.GetRectangleCoordinate(1);
        }
        else
        {
            m_ClosestPoint0 = m_pSegment->GetPosEnd();
            SEDistVector3Rectangle3f tempVRDist(m_ClosestPoint0,
                *m_pRectangle);
            fSqrDist = tempVRDist.GetSquared();
            m_ClosestPoint1 = tempVRDist.GetClosestPoint1();
            m_fSegmentParameter = m_pSegment->Extent;
            m_afRectCoord[0] = tempVRDist.GetRectangleCoordinate(0);
            m_afRectCoord[1] = tempVRDist.GetRectangleCoordinate(1);
        }
    }
    else
    {
        m_ClosestPoint0 = m_pSegment->GetNegEnd();
        SEDistVector3Rectangle3f tempVRDist(m_ClosestPoint0, *m_pRectangle);
        fSqrDist = tempVRDist.GetSquared();
        m_ClosestPoint1 = tempVRDist.GetClosestPoint1();
        m_fSegmentParameter = -m_pSegment->Extent;
        m_afRectCoord[0] = tempVRDist.GetRectangleCoordinate(0);
        m_afRectCoord[1] = tempVRDist.GetRectangleCoordinate(1);
    }

    return fSqrDist;
}
//----------------------------------------------------------------------------
float SEDistSegment3Rectangle3f::Get(float fT, const SEVector3f& rVelocity0, 
    const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMOrigin = m_pSegment->Origin + fT*rVelocity0;
    SEVector3f vec3fMCenter = m_pRectangle->Center + fT*rVelocity1;
    SESegment3f tempMSegment(vec3fMOrigin, m_pSegment->Direction,
        m_pSegment->Extent);
    SERectangle3f tempMRectangle(vec3fMCenter, m_pRectangle->Axis,
        m_pRectangle->Extent);

    return SEDistSegment3Rectangle3f(tempMSegment, tempMRectangle).Get();
}
//----------------------------------------------------------------------------
float SEDistSegment3Rectangle3f::GetSquared(float fT, 
    const SEVector3f& rVelocity0, const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMOrigin = m_pSegment->Origin + fT*rVelocity0;
    SEVector3f vec3fMCenter = m_pRectangle->Center + fT*rVelocity1;
    SESegment3f tempMSegment(vec3fMOrigin, m_pSegment->Direction,
        m_pSegment->Extent);
    SERectangle3f tempMRectangle(vec3fMCenter, m_pRectangle->Axis,
        m_pRectangle->Extent);

    return SEDistSegment3Rectangle3f(tempMSegment, 
        tempMRectangle).GetSquared();
}
//----------------------------------------------------------------------------
float SEDistSegment3Rectangle3f::GetSegmentParameter() const
{
    return m_fSegmentParameter;
}
//----------------------------------------------------------------------------
float SEDistSegment3Rectangle3f::GetRectangleCoordinate(int i) const
{
    SE_ASSERT( 0 <= i && i < 2 );

    return m_afRectCoord[i];
}
//----------------------------------------------------------------------------
