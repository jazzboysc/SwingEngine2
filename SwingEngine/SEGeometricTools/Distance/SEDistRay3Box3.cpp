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
#include "SEDistRay3Box3.h"
#include "SEDistVector3Box3.h"
#include "SEDistLine3Box3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEDistRay3Box3f::SEDistRay3Box3f(const SERay3f& rRay, const SEBox3f& rBox)
    :
    m_pRay(&rRay),
    m_pBox(&rBox)
{
}
//----------------------------------------------------------------------------
const SERay3f& SEDistRay3Box3f::GetRay() const
{
    return *m_pRay;
}
//----------------------------------------------------------------------------
const SEBox3f& SEDistRay3Box3f::GetBox() const
{
    return *m_pBox;
}
//----------------------------------------------------------------------------
float SEDistRay3Box3f::Get()
{
    float fSqrDist = GetSquared();

    return SEMath<float>::Sqrt(fSqrDist);
}
//----------------------------------------------------------------------------
float SEDistRay3Box3f::GetSquared()
{
    SELine3f tempLine(m_pRay->Origin, m_pRay->Direction);
    SEDistLine3Box3f tempLBDist(tempLine, *m_pBox);

    float fSqrDistance = tempLBDist.GetSquared();

    if( tempLBDist.GetLineParameter() >= 0.0f )
    {
        m_ClosestPoint0 = tempLBDist.GetClosestPoint0();
        m_ClosestPoint1 = tempLBDist.GetClosestPoint1();
    }
    else
    {
        SEDistVector3Box3f tempVBDist(m_pRay->Origin, *m_pBox);
        fSqrDistance = tempVBDist.GetSquared();
        m_ClosestPoint0 = tempVBDist.GetClosestPoint0();
        m_ClosestPoint1 = tempVBDist.GetClosestPoint1();
    }

    return fSqrDistance;
}
//----------------------------------------------------------------------------
float SEDistRay3Box3f::Get(float fT, const SEVector3f& rVelocity0,
    const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMOrigin = m_pRay->Origin + fT*rVelocity0;
    SEVector3f vec3fMCenter = m_pBox->Center + fT*rVelocity1;
    SERay3f tempMRay(vec3fMOrigin, m_pRay->Direction);
    SEBox3f tempMBox(vec3fMCenter, m_pBox->Axis,m_pBox->Extent);

    return SEDistRay3Box3f(tempMRay, tempMBox).Get();
}
//----------------------------------------------------------------------------
float SEDistRay3Box3f::GetSquared(float fT, const SEVector3f& rVelocity0, 
    const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMOrigin = m_pRay->Origin + fT*rVelocity0;
    SEVector3f vec3fMCenter = m_pBox->Center + fT*rVelocity1;
    SERay3f tempMRay(vec3fMOrigin, m_pRay->Direction);
    SEBox3f tempMBox(vec3fMCenter, m_pBox->Axis,m_pBox->Extent);

    return SEDistRay3Box3f(tempMRay, tempMBox).GetSquared();
}
//----------------------------------------------------------------------------
