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
#include "SEDistVector2Box2.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEDistVector2Box2f::SEDistVector2Box2f(const SEVector2f& rVector, const 
    SEBox2f& rBox)
    :
    m_pVector(&rVector),
    m_pBox(&rBox)
{
}
//----------------------------------------------------------------------------
const SEVector2f& SEDistVector2Box2f::GetVector() const
{
    return *m_pVector;
}
//----------------------------------------------------------------------------
const SEBox2f& SEDistVector2Box2f::GetBox() const
{
    return *m_pBox;
}
//----------------------------------------------------------------------------
float SEDistVector2Box2f::Get()
{
    return SEMath<float>::Sqrt(GetSquared());
}
//----------------------------------------------------------------------------
float SEDistVector2Box2f::GetSquared()
{
    // 在box的坐标体系下计算.
    SEVector2f vec2fDiff = *m_pVector - m_pBox->Center;

    // 计算squared distance和box上的最近点.
    float fSqrDistance = 0.0f, fDelta;
    SEVector2f vec2fClosest;
    int i;
    for( i = 0; i < 2; i++ )
    {
        vec2fClosest[i] = vec2fDiff.Dot(m_pBox->Axis[i]);
        if( vec2fClosest[i] < -m_pBox->Extent[i] )
        {
            fDelta = vec2fClosest[i] + m_pBox->Extent[i];
            fSqrDistance += fDelta*fDelta;
            vec2fClosest[i] = -m_pBox->Extent[i];
        }
        else if( vec2fClosest[i] > m_pBox->Extent[i] )
        {
            fDelta = vec2fClosest[i] - m_pBox->Extent[i];
            fSqrDistance += fDelta*fDelta;
            vec2fClosest[i] = m_pBox->Extent[i];
        }
    }

    m_ClosestPoint0 = *m_pVector;
    m_ClosestPoint1 = m_pBox->Center;
    for( i = 0; i < 2; i++ )
    {
        m_ClosestPoint1 += vec2fClosest[i]*m_pBox->Axis[i];
    }

    return fSqrDistance;
}
//----------------------------------------------------------------------------
float SEDistVector2Box2f::Get(float fT, const SEVector2f& rVelocity0,
    const SEVector2f& rVelocity1)
{
    SEVector2f vec2fMVector = *m_pVector + fT*rVelocity0;
    SEVector2f vec2fMCenter = m_pBox->Center + fT*rVelocity1;
    SEBox2f tempMBox(vec2fMCenter, m_pBox->Axis, m_pBox->Extent);

    return SEDistVector2Box2f(vec2fMVector, tempMBox).Get();
}
//----------------------------------------------------------------------------
float SEDistVector2Box2f::GetSquared(float fT, const SEVector2f& rVelocity0, 
    const SEVector2f& rVelocity1)
{
    SEVector2f vec2fMVector = *m_pVector + fT*rVelocity0;
    SEVector2f vec2fMCenter = m_pBox->Center + fT*rVelocity1;
    SEBox2f tempMBox(vec2fMCenter, m_pBox->Axis, m_pBox->Extent);

    return SEDistVector2Box2f(vec2fMVector, tempMBox).GetSquared();
}
//----------------------------------------------------------------------------
