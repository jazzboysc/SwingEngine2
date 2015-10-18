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

#include "SECorePCH.h"
#include "SEFrustum3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEFrustum3f::SEFrustum3f()
    :
    Origin(SEVector3f::ZERO),
    RVector(SEVector3f::UNIT_X),
    UVector(SEVector3f::UNIT_Y),
    DVector(SEVector3f::UNIT_Z)
{
    RBound = 1.0f;
    UBound = 1.0f;
    DMin = 1.0f;
    DMax = 2.0f;

    Update();
}
//----------------------------------------------------------------------------
SEFrustum3f::SEFrustum3f(const SEVector3f& rOrigin, const SEVector3f& rRVector, 
    const SEVector3f& rUVector, const SEVector3f& rDVector, 
    float fRBound, float fUBound, float fDMin, float fDMax)
    :
    Origin(rOrigin),
    RVector(rRVector),
    UVector(rUVector),
    DVector(rDVector)
{
    RBound = fRBound;
    UBound = fUBound;
    DMin = fDMin;
    DMax = fDMax;

    Update();
}
//----------------------------------------------------------------------------
void SEFrustum3f::Update()
{
    m_fDRatio = DMax / DMin;
    m_fMTwoUF = -2.0f * UBound * DMax;
    m_fMTwoRF = -2.0f * RBound * DMax;
}
//----------------------------------------------------------------------------
float SEFrustum3f::GetDRatio() const
{
    return m_fDRatio;
}
//----------------------------------------------------------------------------
float SEFrustum3f::GetMTwoUF() const
{
    return m_fMTwoUF;
}
//----------------------------------------------------------------------------
float SEFrustum3f::GetMTwoRF() const
{
    return m_fMTwoRF;
}
//----------------------------------------------------------------------------
void SEFrustum3f::ComputeVertices(SEVector3f aVertex[8]) const
{
    // 待检查.
    // 目前顶点为逆时针索引顺序.

    SEVector3f vec3fRScaled = RBound*RVector;
    SEVector3f vec3fUScaled = UBound*UVector;
    SEVector3f vec3fDScaled = DMin*DVector;

    aVertex[0] = vec3fDScaled - vec3fUScaled - vec3fRScaled;
    aVertex[1] = vec3fDScaled - vec3fUScaled + vec3fRScaled;
    aVertex[2] = vec3fDScaled + vec3fUScaled + vec3fRScaled;
    aVertex[3] = vec3fDScaled + vec3fUScaled - vec3fRScaled;

    for( int i = 0, ip = 4; i < 4; i++, ip++ )
    {
        aVertex[ip] = Origin + m_fDRatio*aVertex[i];
        aVertex[i] += Origin;
    }
}
//----------------------------------------------------------------------------
