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
#include "SEQuery3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEQuery3f::SEQuery3f (int iVCount, const SEVector3f* aVertex)
{
    SE_ASSERT( iVCount > 0 && aVertex );

    m_iVCount = iVCount;
    m_aVertex = aVertex;
}
//----------------------------------------------------------------------------
SEQuery3f::~SEQuery3f()
{
}
//----------------------------------------------------------------------------
SEQuery::Type SEQuery3f::GetType() const
{
    return SEQuery::QT_REAL;
}
//----------------------------------------------------------------------------
int SEQuery3f::GetCount() const
{
    return m_iVCount;
}
//----------------------------------------------------------------------------
const SEVector3f* SEQuery3f::GetVertices() const
{
    return m_aVertex;
}
//----------------------------------------------------------------------------
int SEQuery3f::ToPlane(int i, int iV0, int iV1, int iV2) const
{
    return ToPlane(m_aVertex[i], iV0, iV1, iV2);
}
//----------------------------------------------------------------------------
int SEQuery3f::ToPlane(const SEVector3f& rP, int iV0, int iV1, int iV2) const
{
    const SEVector3f& rV0 = m_aVertex[iV0];
    const SEVector3f& rV1 = m_aVertex[iV1];
    const SEVector3f& rV2 = m_aVertex[iV2];

    float fX0 = rP[0] - rV0[0];
    float fY0 = rP[1] - rV0[1];
    float fZ0 = rP[2] - rV0[2];
    float fX1 = rV1[0] - rV0[0];
    float fY1 = rV1[1] - rV0[1];
    float fZ1 = rV1[2] - rV0[2];
    float fX2 = rV2[0] - rV0[0];
    float fY2 = rV2[1] - rV0[1];
    float fZ2 = rV2[2] - rV0[2];

    float fDet3 = Det3(fX0, fY0, fZ0, fX1, fY1, fZ1, fX2, fY2, fZ2);

    return (fDet3 > 0.0f ? +1 : (fDet3 < 0.0f ? -1 : 0));
}
//----------------------------------------------------------------------------
int SEQuery3f::ToTetrahedron(int i, int iV0, int iV1, int iV2, 
    int iV3) const
{
    return ToTetrahedron(m_aVertex[i], iV0, iV1, iV2, iV3);
}
//----------------------------------------------------------------------------
int SEQuery3f::ToTetrahedron(const SEVector3f& rP, int iV0, 
    int iV1, int iV2, int iV3) const
{
    int iSign0 = ToPlane(rP, iV1, iV2, iV3);
    if( iSign0 > 0 )
    {
        return +1;
    }

    int iSign1 = ToPlane(rP, iV0, iV2, iV3);
    if( iSign1 < 0 )
    {
        return +1;
    }

    int iSign2 = ToPlane(rP, iV0, iV1, iV3);
    if( iSign2 > 0 )
    {
        return +1;
    }

    int iSign3 = ToPlane(rP, iV0, iV1, iV2);
    if( iSign3 < 0 )
    {
        return +1;
    }

    return ((iSign0 && iSign1 && iSign2 && iSign3) ? -1 : 0);
}
//----------------------------------------------------------------------------
int SEQuery3f::ToCircumsphere(int i, int iV0, int iV1, int iV2, int iV3) const
{
    return ToCircumsphere(m_aVertex[i], iV0, iV1, iV2, iV3);
}
//----------------------------------------------------------------------------
int SEQuery3f::ToCircumsphere(const SEVector3f& rP, int iV0, int iV1, int iV2, 
    int iV3) const
{
    const SEVector3f& rV0 = m_aVertex[iV0];
    const SEVector3f& rV1 = m_aVertex[iV1];
    const SEVector3f& rV2 = m_aVertex[iV2];
    const SEVector3f& rV3 = m_aVertex[iV3];

    float fS0x = rV0[0] + rP[0];
    float fD0x = rV0[0] - rP[0];
    float fS0y = rV0[1] + rP[1];
    float fD0y = rV0[1] - rP[1];
    float fS0z = rV0[2] + rP[2];
    float fD0z = rV0[2] - rP[2];
    float fS1x = rV1[0] + rP[0];
    float fD1x = rV1[0] - rP[0];
    float fS1y = rV1[1] + rP[1];
    float fD1y = rV1[1] - rP[1];
    float fS1z = rV1[2] + rP[2];
    float fD1z = rV1[2] - rP[2];
    float fS2x = rV2[0] + rP[0];
    float fD2x = rV2[0] - rP[0];
    float fS2y = rV2[1] + rP[1];
    float fD2y = rV2[1] - rP[1];
    float fS2z = rV2[2] + rP[2];
    float fD2z = rV2[2] - rP[2];
    float fS3x = rV3[0] + rP[0];
    float fD3x = rV3[0] - rP[0];
    float fS3y = rV3[1] + rP[1];
    float fD3y = rV3[1] - rP[1];
    float fS3z = rV3[2] + rP[2];
    float fD3z = rV3[2] - rP[2];
    float fW0 = fS0x*fD0x + fS0y*fD0y + fS0z*fD0z;
    float fW1 = fS1x*fD1x + fS1y*fD1y + fS1z*fD1z;
    float fW2 = fS2x*fD2x + fS2y*fD2y + fS2z*fD2z;
    float fW3 = fS3x*fD3x + fS3y*fD3y + fS3z*fD3z;
    float fDet4 = Det4(fD0x, fD0y, fD0z, fW0, fD1x, fD1y, fD1z, fW1, fD2x, 
        fD2y, fD2z, fW2, fD3x, fD3y, fD3z, fW3);

    return (fDet4 > 0.0f ? 1 : (fDet4 < 0.0f ? -1 : 0));
}
//----------------------------------------------------------------------------
float SEQuery3f::Dot(float fX0, float fY0, float fZ0, float fX1, float fY1, 
    float fZ1)
{
    return fX0*fX1 + fY0*fY1 + fZ0*fZ1;
}
//----------------------------------------------------------------------------
float SEQuery3f::Det3(float fX0, float fY0, float fZ0, float fX1, float fY1, 
    float fZ1, float fX2, float fY2, float fZ2)
{
    float fC00 = fY1*fZ2 - fY2*fZ1;
    float fC01 = fY2*fZ0 - fY0*fZ2;
    float fC02 = fY0*fZ1 - fY1*fZ0;

    return fX0*fC00 + fX1*fC01 + fX2*fC02;
}
//----------------------------------------------------------------------------
float SEQuery3f::Det4(float fX0, float fY0, float fZ0, float fW0, float fX1, 
    float fY1, float fZ1, float fW1, float fX2, float fY2, float fZ2, 
    float fW2, float fX3, float fY3, float fZ3, float fW3)
{
    float fA0 = fX0*fY1 - fX1*fY0;
    float fA1 = fX0*fY2 - fX2*fY0;
    float fA2 = fX0*fY3 - fX3*fY0;
    float fA3 = fX1*fY2 - fX2*fY1;
    float fA4 = fX1*fY3 - fX3*fY1;
    float fA5 = fX2*fY3 - fX3*fY2;
    float fB0 = fZ0*fW1 - fZ1*fW0;
    float fB1 = fZ0*fW2 - fZ2*fW0;
    float fB2 = fZ0*fW3 - fZ3*fW0;
    float fB3 = fZ1*fW2 - fZ2*fW1;
    float fB4 = fZ1*fW3 - fZ3*fW1;
    float fB5 = fZ2*fW3 - fZ3*fW2;

    return fA0*fB5 - fA1*fB4 + fA2*fB3 + fA3*fB2 - fA4*fB1 + fA5*fB0;
}
//----------------------------------------------------------------------------
