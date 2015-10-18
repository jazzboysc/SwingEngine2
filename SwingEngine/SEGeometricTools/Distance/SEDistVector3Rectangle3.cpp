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
#include "SEDistVector3Rectangle3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEDistVector3Rectangle3f::SEDistVector3Rectangle3f(const SEVector3f& rVector, 
    const SERectangle3f& rRectangle)
    :
    m_pVector(&rVector),
    m_pRectangle(&rRectangle)
{
}
//----------------------------------------------------------------------------
const SEVector3f& SEDistVector3Rectangle3f::GetVector() const
{
    return *m_pVector;
}
//----------------------------------------------------------------------------
const SERectangle3f& SEDistVector3Rectangle3f::GetRectangle()
    const
{
    return *m_pRectangle;
}
//----------------------------------------------------------------------------
float SEDistVector3Rectangle3f::Get()
{
    float fSqrDist = GetSquared();

    return SEMath<float>::Sqrt(fSqrDist);
}
//----------------------------------------------------------------------------
float SEDistVector3Rectangle3f::GetSquared()
{
    SEVector3f vec3fDiff = m_pRectangle->Center - *m_pVector;
    float fB0 = vec3fDiff.Dot(m_pRectangle->Axis[0]);
    float fB1 = vec3fDiff.Dot(m_pRectangle->Axis[1]);
    float fS0 = -fB0, fS1 = -fB1;
    float fSqrDistance = vec3fDiff.GetSquaredLength();

    if( fS0 < -m_pRectangle->Extent[0] )
    {
        fS0 = -m_pRectangle->Extent[0];
    }
    else if( fS0 > m_pRectangle->Extent[0] )
    {
        fS0 = m_pRectangle->Extent[0];
    }
    fSqrDistance += fS0*(fS0 + 2.0f*fB0);

    if( fS1 < -m_pRectangle->Extent[1] )
    {
        fS1 = -m_pRectangle->Extent[1];
    }
    else if( fS1 > m_pRectangle->Extent[1] )
    {
        fS1 = m_pRectangle->Extent[1];
    }
    fSqrDistance += fS1*(fS1 + 2.0f*fB1);

    // account for numerical round-off error
    if( fSqrDistance < 0.0f )
    {
        fSqrDistance = 0.0f;
    }

    m_ClosestPoint0 = *m_pVector;
    m_ClosestPoint1 = m_pRectangle->Center + fS0*m_pRectangle->Axis[0] +
        fS1*m_pRectangle->Axis[1];
    m_afRectCoord[0] = fS0;
    m_afRectCoord[1] = fS1;

    return fSqrDistance;
}
//----------------------------------------------------------------------------
float SEDistVector3Rectangle3f::Get(float fT, const SEVector3f& rVelocity0, 
    const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMVector = *m_pVector + fT*rVelocity0;
    SEVector3f vec3fMCenter = m_pRectangle->Center + fT*rVelocity1;
    SERectangle3f tempMRectangle(vec3fMCenter, m_pRectangle->Axis[0],
        m_pRectangle->Axis[1], m_pRectangle->Extent[0], 
        m_pRectangle->Extent[1]);

    return SEDistVector3Rectangle3f(vec3fMVector, tempMRectangle).Get();
}
//----------------------------------------------------------------------------
float SEDistVector3Rectangle3f::GetSquared(float fT, const SEVector3f& 
    rVelocity0, const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMVector = *m_pVector + fT*rVelocity0;
    SEVector3f vec3fMCenter = m_pRectangle->Center + fT*rVelocity1;
    SERectangle3f tempMRectangle(vec3fMCenter, m_pRectangle->Axis[0],
        m_pRectangle->Axis[1], m_pRectangle->Extent[0], 
        m_pRectangle->Extent[1]);

    return SEDistVector3Rectangle3f(vec3fMVector, tempMRectangle).GetSquared();
}
//----------------------------------------------------------------------------
float SEDistVector3Rectangle3f::GetRectangleCoordinate(int i) const
{
    SE_ASSERT( 0 <= i && i < 2 );

    return m_afRectCoord[i];
}
//----------------------------------------------------------------------------
