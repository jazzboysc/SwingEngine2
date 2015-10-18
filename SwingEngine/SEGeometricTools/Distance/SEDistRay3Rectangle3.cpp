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
#include "SEDistRay3Rectangle3.h"
#include "SEDistLine3Rectangle3.h"
#include "SEDistVector3Rectangle3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEDistRay3Rectangle3f::SEDistRay3Rectangle3f(const SERay3f& rRay,
    const SERectangle3f& rRectangle)
    :
    m_pRay(&rRay),
    m_pRectangle(&rRectangle)
{
}
//----------------------------------------------------------------------------
const SERay3f& SEDistRay3Rectangle3f::GetRay() const
{
    return *m_pRay;
}
//----------------------------------------------------------------------------
const SERectangle3f& SEDistRay3Rectangle3f::GetRectangle() const
{
    return *m_pRectangle;
}
//----------------------------------------------------------------------------
float SEDistRay3Rectangle3f::Get()
{
    float fSqrDist = GetSquared();

    return SEMath<float>::Sqrt(fSqrDist);
}
//----------------------------------------------------------------------------
float SEDistRay3Rectangle3f::GetSquared()
{
    SEDistLine3Rectangle3f tempLRDist(SELine3f(m_pRay->Origin, 
        m_pRay->Direction), *m_pRectangle);

    float fSqrDist = tempLRDist.GetSquared();

    m_fRayParameter = tempLRDist.GetLineParameter();
    if( m_fRayParameter >= 0.0f )
    {
        m_ClosestPoint0 = tempLRDist.GetClosestPoint0();
        m_ClosestPoint1 = tempLRDist.GetClosestPoint1();
        m_afRectCoord[0] = tempLRDist.GetRectangleCoordinate(0);
        m_afRectCoord[1] = tempLRDist.GetRectangleCoordinate(1);
    }
    else
    {
        m_ClosestPoint0 = m_pRay->Origin;
        SEDistVector3Rectangle3f tempVRDist(m_ClosestPoint0, *m_pRectangle);
        fSqrDist = tempVRDist.GetSquared();
        m_ClosestPoint1 = tempVRDist.GetClosestPoint1();
        m_fRayParameter = 0.0f;
        m_afRectCoord[0] = tempVRDist.GetRectangleCoordinate(0);
        m_afRectCoord[1] = tempVRDist.GetRectangleCoordinate(1);
    }

    return fSqrDist;
}
//----------------------------------------------------------------------------
float SEDistRay3Rectangle3f::Get(float fT, const SEVector3f& rVelocity0, 
    const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMOrigin = m_pRay->Origin + fT*rVelocity0;
    SEVector3f vec3fMCenter = m_pRectangle->Center + fT*rVelocity1;
    SERay3f tempMRay(vec3fMOrigin, m_pRay->Direction);
    SERectangle3f tempMRectangle(vec3fMCenter, m_pRectangle->Axis,
        m_pRectangle->Extent);

    return SEDistRay3Rectangle3f(tempMRay, tempMRectangle).Get();
}
//----------------------------------------------------------------------------
float SEDistRay3Rectangle3f::GetSquared(float fT, const SEVector3f& rVelocity0, 
    const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMOrigin = m_pRay->Origin + fT*rVelocity0;
    SEVector3f vec3fMCenter = m_pRectangle->Center + fT*rVelocity1;
    SERay3f tempMRay(vec3fMOrigin, m_pRay->Direction);
    SERectangle3f tempMRectangle(vec3fMCenter, m_pRectangle->Axis,
        m_pRectangle->Extent);

    return SEDistRay3Rectangle3f(tempMRay, tempMRectangle).GetSquared();
}
//----------------------------------------------------------------------------
float SEDistRay3Rectangle3f::GetRayParameter() const
{
    return m_fRayParameter;
}
//----------------------------------------------------------------------------
float SEDistRay3Rectangle3f::GetRectangleCoordinate(int i) const
{
    SE_ASSERT( 0 <= i && i < 2 );

    return m_afRectCoord[i];
}
//----------------------------------------------------------------------------