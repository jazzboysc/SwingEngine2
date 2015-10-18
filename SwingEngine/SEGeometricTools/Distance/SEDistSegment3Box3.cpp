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
#include "SEDistSegment3Box3.h"
#include "SEDistVector3Box3.h"
#include "SEDistLine3Box3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEDistSegment3Box3f::SEDistSegment3Box3f(const SESegment3f& rSegment,
    const SEBox3f& rBox)
    :
    m_pSegment(&rSegment),
    m_pBox(&rBox)
{
}
//----------------------------------------------------------------------------
const SESegment3f& SEDistSegment3Box3f::GetSegment() const
{
    return *m_pSegment;
}
//----------------------------------------------------------------------------
const SEBox3f& SEDistSegment3Box3f::GetBox() const
{
    return *m_pBox;
}
//----------------------------------------------------------------------------
float SEDistSegment3Box3f::Get()
{
    float fSqrDist = GetSquared();

    return SEMath<float>::Sqrt(fSqrDist);
}
//----------------------------------------------------------------------------
float SEDistSegment3Box3f::GetSquared()
{
    SELine3f tempLine(m_pSegment->Origin, m_pSegment->Direction);
    SEDistLine3Box3f tempLBDist(tempLine, *m_pBox);

    float fSqrDistance = tempLBDist.GetSquared();

    float fLParam = tempLBDist.GetLineParameter();
    if( fLParam >= -m_pSegment->Extent )
    {
        if( fLParam <= m_pSegment->Extent )
        {
            m_ClosestPoint0 = tempLBDist.GetClosestPoint0();
            m_ClosestPoint1 = tempLBDist.GetClosestPoint1();
        }
        else
        {
            SEDistVector3Box3f tempVBDist(m_pSegment->GetPosEnd(), *m_pBox);
            fSqrDistance = tempVBDist.GetSquared();
            m_ClosestPoint0 = tempVBDist.GetClosestPoint0();
            m_ClosestPoint1 = tempVBDist.GetClosestPoint1();
        }
    }
    else
    {
        SEDistVector3Box3f tempVBDist(m_pSegment->GetNegEnd(), *m_pBox);
        fSqrDistance = tempVBDist.GetSquared();
        m_ClosestPoint0 = tempVBDist.GetClosestPoint0();
        m_ClosestPoint1 = tempVBDist.GetClosestPoint1();
    }

    return fSqrDistance;
}
//----------------------------------------------------------------------------
float SEDistSegment3Box3f::Get(float fT, const SEVector3f& rVelocity0,
    const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMOrigin = m_pSegment->Origin + fT*rVelocity0;
    SEVector3f vec3fMCenter = m_pBox->Center + fT*rVelocity1;
    SESegment3f tempMSegment(vec3fMOrigin, m_pSegment->Direction,
        m_pSegment->Extent);
    SEBox3f tempMBox(vec3fMCenter, m_pBox->Axis, m_pBox->Extent);

    return SEDistSegment3Box3f(tempMSegment, tempMBox).Get();
}
//----------------------------------------------------------------------------
float SEDistSegment3Box3f::GetSquared(float fT, const SEVector3f& rVelocity0, 
    const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMOrigin = m_pSegment->Origin + fT*rVelocity0;
    SEVector3f vec3fMCenter = m_pBox->Center + fT*rVelocity1;
    SESegment3f tempMSegment(vec3fMOrigin, m_pSegment->Direction,
        m_pSegment->Extent);
    SEBox3f tempMBox(vec3fMCenter, m_pBox->Axis, m_pBox->Extent);

    return SEDistSegment3Box3f(tempMSegment, tempMBox).GetSquared();
}
//----------------------------------------------------------------------------
