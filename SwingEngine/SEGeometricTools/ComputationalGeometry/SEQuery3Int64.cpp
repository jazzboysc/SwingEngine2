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
#include "SEQuery3Int64.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEQuery3Int64f::SEQuery3Int64f(int iVCount, const SEVector3f* aVertex)
    :
    SEQuery3f(iVCount, aVertex)
{
}
//----------------------------------------------------------------------------
SEQuery::Type SEQuery3Int64f::GetType() const
{
    return SEQuery::QT_INT64;
}
//----------------------------------------------------------------------------
int SEQuery3Int64f::ToPlane(const SEVector3f& rP, int iV0, int iV1, 
    int iV2) const
{
    const SEVector3f& rV0 = m_aVertex[iV0];
    const SEVector3f& rV1 = m_aVertex[iV1];
    const SEVector3f& rV2 = m_aVertex[iV2];

    SE_Int64 iX0 = (SE_Int64)rP[0] - (SE_Int64)rV0[0];
    SE_Int64 iY0 = (SE_Int64)rP[1] - (SE_Int64)rV0[1];
    SE_Int64 iZ0 = (SE_Int64)rP[2] - (SE_Int64)rV0[2];
    SE_Int64 iX1 = (SE_Int64)rV1[0] - (SE_Int64)rV0[0];
    SE_Int64 iY1 = (SE_Int64)rV1[1] - (SE_Int64)rV0[1];
    SE_Int64 iZ1 = (SE_Int64)rV1[2] - (SE_Int64)rV0[2];
    SE_Int64 iX2 = (SE_Int64)rV2[0] - (SE_Int64)rV0[0];
    SE_Int64 iY2 = (SE_Int64)rV2[1] - (SE_Int64)rV0[1];
    SE_Int64 iZ2 = (SE_Int64)rV2[2] - (SE_Int64)rV0[2];

    SE_Int64 iDet3 = Det3(iX0, iY0, iZ0, iX1, iY1, iZ1, iX2, iY2, iZ2);

    return (iDet3 > 0 ? +1 : (iDet3 < 0 ? -1 : 0));
}
//----------------------------------------------------------------------------
int SEQuery3Int64f::ToCircumsphere(const SEVector3f& rP, int iV0, 
    int iV1, int iV2, int iV3) const
{
    const SEVector3f& rV0 = m_aVertex[iV0];
    const SEVector3f& rV1 = m_aVertex[iV1];
    const SEVector3f& rV2 = m_aVertex[iV2];
    const SEVector3f& rV3 = m_aVertex[iV3];

    SE_Int64 aiP[3] = { (SE_Int64)rP[0], (SE_Int64)rP[1], 
        (SE_Int64)rP[2] };
    SE_Int64 aiV0[3] = { (SE_Int64)rV0[0], (SE_Int64)rV0[1], 
        (SE_Int64)rV0[2] };
    SE_Int64 aiV1[3] = { (SE_Int64)rV1[0], (SE_Int64)rV1[1], 
        (SE_Int64)rV1[2] };
    SE_Int64 aiV2[3] = { (SE_Int64)rV2[0], (SE_Int64)rV2[1], 
        (SE_Int64)rV2[2] };
    SE_Int64 aiV3[3] = { (SE_Int64)rV3[0], (SE_Int64)rV3[1], 
        (SE_Int64)rV3[2] };

    SE_Int64 iS0x = aiV0[0] + aiP[0];
    SE_Int64 iD0x = aiV0[0] - aiP[0];
    SE_Int64 iS0y = aiV0[1] + aiP[1];
    SE_Int64 iD0y = aiV0[1] - aiP[1];
    SE_Int64 iS0z = aiV0[2] + aiP[2];
    SE_Int64 iD0z = aiV0[2] - aiP[2];
    SE_Int64 iS1x = aiV1[0] + aiP[0];
    SE_Int64 iD1x = aiV1[0] - aiP[0];
    SE_Int64 iS1y = aiV1[1] + aiP[1];
    SE_Int64 iD1y = aiV1[1] - aiP[1];
    SE_Int64 iS1z = aiV1[2] + aiP[2];
    SE_Int64 iD1z = aiV1[2] - aiP[2];
    SE_Int64 iS2x = aiV2[0] + aiP[0];
    SE_Int64 iD2x = aiV2[0] - aiP[0];
    SE_Int64 iS2y = aiV2[1] + aiP[1];
    SE_Int64 iD2y = aiV2[1] - aiP[1];
    SE_Int64 iS2z = aiV2[2] + aiP[2];
    SE_Int64 iD2z = aiV2[2] - aiP[2];
    SE_Int64 iS3x = aiV3[0] + aiP[0];
    SE_Int64 iD3x = aiV3[0] - aiP[0];
    SE_Int64 iS3y = aiV3[1] + aiP[1];
    SE_Int64 iD3y = aiV3[1] - aiP[1];
    SE_Int64 iS3z = aiV3[2] + aiP[2];
    SE_Int64 iD3z = aiV3[2] - aiP[2];
    SE_Int64 iW0 = iS0x*iD0x + iS0y*iD0y + iS0z*iD0z;
    SE_Int64 iW1 = iS1x*iD1x + iS1y*iD1y + iS1z*iD1z;
    SE_Int64 iW2 = iS2x*iD2x + iS2y*iD2y + iS2z*iD2z;
    SE_Int64 iW3 = iS3x*iD3x + iS3y*iD3y + iS3z*iD3z;
    SE_Int64 iDet4 = Det4(iD0x, iD0y, iD0z, iW0, iD1x, iD1y, iD1z, iW1, iD2x, 
        iD2y, iD2z, iW2, iD3x, iD3y, iD3z, iW3);

    return (iDet4 > 0 ? 1 : (iDet4 < 0 ? -1 : 0));
}
//----------------------------------------------------------------------------
SE_Int64 SEQuery3Int64f::Dot(SE_Int64 iX0, SE_Int64 iY0, SE_Int64 iZ0, 
    SE_Int64 iX1, SE_Int64 iY1, SE_Int64 iZ1)
{
    return iX0*iX1 + iY0*iY1 + iZ0*iZ1;
}
//----------------------------------------------------------------------------
SE_Int64 SEQuery3Int64f::Det3(SE_Int64 iX0, SE_Int64 iY0, 
    SE_Int64 iZ0, SE_Int64 iX1, SE_Int64 iY1, SE_Int64 iZ1, 
    SE_Int64 iX2, SE_Int64 iY2, SE_Int64 iZ2)
{
    SE_Int64 iC00 = iY1*iZ2 - iY2*iZ1;
    SE_Int64 iC01 = iY2*iZ0 - iY0*iZ2;
    SE_Int64 iC02 = iY0*iZ1 - iY1*iZ0;

    return iX0*iC00 + iX1*iC01 + iX2*iC02;
}
//----------------------------------------------------------------------------
SE_Int64 SEQuery3Int64f::Det4(SE_Int64 iX0, SE_Int64 iY0, 
    SE_Int64 iZ0, SE_Int64 iW0, SE_Int64 iX1, SE_Int64 iY1, SE_Int64 iZ1, 
    SE_Int64 iW1, SE_Int64 iX2, SE_Int64 iY2, SE_Int64 iZ2, SE_Int64 iW2, 
    SE_Int64 iX3, SE_Int64 iY3, SE_Int64 iZ3, SE_Int64 iW3)
{
    SE_Int64 iA0 = iX0*iY1 - iX1*iY0;
    SE_Int64 iA1 = iX0*iY2 - iX2*iY0;
    SE_Int64 iA2 = iX0*iY3 - iX3*iY0;
    SE_Int64 iA3 = iX1*iY2 - iX2*iY1;
    SE_Int64 iA4 = iX1*iY3 - iX3*iY1;
    SE_Int64 iA5 = iX2*iY3 - iX3*iY2;
    SE_Int64 iB0 = iZ0*iW1 - iZ1*iW0;
    SE_Int64 iB1 = iZ0*iW2 - iZ2*iW0;
    SE_Int64 iB2 = iZ0*iW3 - iZ3*iW0;
    SE_Int64 iB3 = iZ1*iW2 - iZ2*iW1;
    SE_Int64 iB4 = iZ1*iW3 - iZ3*iW1;
    SE_Int64 iB5 = iZ2*iW3 - iZ3*iW2;

    return iA0*iB5 - iA1*iB4 + iA2*iB3 + iA3*iB2 - iA4*iB1 + iA5*iB0;
}
//----------------------------------------------------------------------------
