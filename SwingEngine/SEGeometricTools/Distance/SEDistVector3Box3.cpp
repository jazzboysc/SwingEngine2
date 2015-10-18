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
#include "SEDistVector3Box3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEDistVector3Box3f::SEDistVector3Box3f(const SEVector3f& rVector, const 
    SEBox3f& rBox)
    :
    m_pVector(&rVector),
    m_pBox(&rBox)
{
}
//----------------------------------------------------------------------------
const SEVector3f& SEDistVector3Box3f::GetVector() const
{
    return *m_pVector;
}
//----------------------------------------------------------------------------
const SEBox3f& SEDistVector3Box3f::GetBox() const
{
    return *m_pBox;
}
//----------------------------------------------------------------------------
float SEDistVector3Box3f::Get()
{
    return SEMath<float>::Sqrt(GetSquared());
}
//----------------------------------------------------------------------------
float SEDistVector3Box3f::GetSquared()
{
    // 在box的坐标体系下计算.
    SEVector3f vec3fDiff = *m_pVector - m_pBox->Center;

    // 计算squared distance和box上的最近点.
    float fSqrDistance = 0.0f, fDelta;
    SEVector3f vec3fClosest;
    int i;
    for( i = 0; i < 3; i++ )
    {
        vec3fClosest[i] = vec3fDiff.Dot(m_pBox->Axis[i]);
        if( vec3fClosest[i] < -m_pBox->Extent[i] )
        {
            fDelta = vec3fClosest[i] + m_pBox->Extent[i];
            fSqrDistance += fDelta*fDelta;
            vec3fClosest[i] = -m_pBox->Extent[i];
        }
        else if( vec3fClosest[i] > m_pBox->Extent[i] )
        {
            fDelta = vec3fClosest[i] - m_pBox->Extent[i];
            fSqrDistance += fDelta*fDelta;
            vec3fClosest[i] = m_pBox->Extent[i];
        }
    }

    m_ClosestPoint0 = *m_pVector;
    m_ClosestPoint1 = m_pBox->Center;
    for( i = 0; i < 3; i++ )
    {
        m_ClosestPoint1 += vec3fClosest[i]*m_pBox->Axis[i];
    }

    return fSqrDistance;
}
//----------------------------------------------------------------------------
float SEDistVector3Box3f::Get(float fT, const SEVector3f& rVelocity0,
    const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMVector = *m_pVector + fT*rVelocity0;
    SEVector3f vec3fMCenter = m_pBox->Center + fT*rVelocity1;
    SEBox3f tempMBox(vec3fMCenter, m_pBox->Axis,m_pBox->Extent);

    return SEDistVector3Box3f(vec3fMVector, tempMBox).Get();
}
//----------------------------------------------------------------------------
float SEDistVector3Box3f::GetSquared(float fT, const SEVector3f& rVelocity0, 
    const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMVector = *m_pVector + fT*rVelocity0;
    SEVector3f vec3fMCenter = m_pBox->Center + fT*rVelocity1;
    SEBox3f tempMBox(vec3fMCenter, m_pBox->Axis,m_pBox->Extent);

    return SEDistVector3Box3f(vec3fMVector, tempMBox).GetSquared();
}
//----------------------------------------------------------------------------
