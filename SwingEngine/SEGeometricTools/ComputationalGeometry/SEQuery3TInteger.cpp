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
#include "SEQuery3TInteger.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEQuery3TIntegerf::SEQuery3TIntegerf(int iVCount, const SEVector3f* aVertex)
    :
    SEQuery3f(iVCount, aVertex)
{
}
//----------------------------------------------------------------------------
SEQuery::Type SEQuery3TIntegerf::GetType() const
{
    return SEQuery::QT_INTEGER;
}
//----------------------------------------------------------------------------
int SEQuery3TIntegerf::ToPlane(const SEVector3f& rP, int iV0, int iV1, int 
    iV2) const
{
    const SEVector3f& rV0 = m_aVertex[iV0];
    const SEVector3f& rV1 = m_aVertex[iV1];
    const SEVector3f& rV2 = m_aVertex[iV2];

    SETInteger<4> tempX0((int)rP[0] - (int)rV0[0]);
    SETInteger<4> tempY0((int)rP[1] - (int)rV0[1]);
    SETInteger<4> tempZ0((int)rP[2] - (int)rV0[2]);
    SETInteger<4> tempX1((int)rV1[0] - (int)rV0[0]);
    SETInteger<4> tempY1((int)rV1[1] - (int)rV0[1]);
    SETInteger<4> tempZ1((int)rV1[2] - (int)rV0[2]);
    SETInteger<4> tempX2((int)rV2[0] - (int)rV0[0]);
    SETInteger<4> tempY2((int)rV2[1] - (int)rV0[1]);
    SETInteger<4> tempZ2((int)rV2[2] - (int)rV0[2]);

    SETInteger<4> tempDet3 = Det3(tempX0, tempY0, tempZ0, 
        tempX1, tempY1, tempZ1, tempX2, tempY2, tempZ2);

    return (tempDet3 > 0 ? +1 : (tempDet3 < 0 ? -1 : 0));
}
//----------------------------------------------------------------------------
int SEQuery3TIntegerf::ToCircumsphere(const SEVector3f& rP, int iV0, 
    int iV1, int iV2, int iV3) const
{
    const SEVector3f& rV0 = m_aVertex[iV0];
    const SEVector3f& rV1 = m_aVertex[iV1];
    const SEVector3f& rV2 = m_aVertex[iV2];
    const SEVector3f& rV3 = m_aVertex[iV3];

    int aiP[3] = { (int)rP[0], (int)rP[1], (int)rP[2] };
    int aiV0[3] = { (int)rV0[0], (int)rV0[1], (int)rV0[2] };
    int aiV1[3] = { (int)rV1[0], (int)rV1[1], (int)rV1[2] };
    int aiV2[3] = { (int)rV2[0], (int)rV2[1], (int)rV2[2] };
    int aiV3[3] = { (int)rV3[0], (int)rV3[1], (int)rV3[2] };

    SETInteger<6> tempS0x(aiV0[0] + aiP[0]);
    SETInteger<6> tempD0x(aiV0[0] - aiP[0]);
    SETInteger<6> tempS0y(aiV0[1] + aiP[1]);
    SETInteger<6> tempD0y(aiV0[1] - aiP[1]);
    SETInteger<6> tempS0z(aiV0[2] + aiP[2]);
    SETInteger<6> tempD0z(aiV0[2] - aiP[2]);
    SETInteger<6> tempS1x(aiV1[0] + aiP[0]);
    SETInteger<6> tempD1x(aiV1[0] - aiP[0]);
    SETInteger<6> tempS1y(aiV1[1] + aiP[1]);
    SETInteger<6> tempD1y(aiV1[1] - aiP[1]);
    SETInteger<6> tempS1z(aiV1[2] + aiP[2]);
    SETInteger<6> tempD1z(aiV1[2] - aiP[2]);
    SETInteger<6> tempS2x(aiV2[0] + aiP[0]);
    SETInteger<6> tempD2x(aiV2[0] - aiP[0]);
    SETInteger<6> tempS2y(aiV2[1] + aiP[1]);
    SETInteger<6> tempD2y(aiV2[1] - aiP[1]);
    SETInteger<6> tempS2z(aiV2[2] + aiP[2]);
    SETInteger<6> tempD2z(aiV2[2] - aiP[2]);
    SETInteger<6> tempS3x(aiV3[0] + aiP[0]);
    SETInteger<6> tempD3x(aiV3[0] - aiP[0]);
    SETInteger<6> tempS3y(aiV3[1] + aiP[1]);
    SETInteger<6> tempD3y(aiV3[1] - aiP[1]);
    SETInteger<6> tempS3z(aiV3[2] + aiP[2]);
    SETInteger<6> tempD3z(aiV3[2] - aiP[2]);
    SETInteger<6> tempW0 = tempS0x*tempD0x + tempS0y*tempD0y + tempS0z*tempD0z;
    SETInteger<6> tempW1 = tempS1x*tempD1x + tempS1y*tempD1y + tempS1z*tempD1z;
    SETInteger<6> tempW2 = tempS2x*tempD2x + tempS2y*tempD2y + tempS2z*tempD2z;
    SETInteger<6> tempW3 = tempS3x*tempD3x + tempS3y*tempD3y + tempS3z*tempD3z;
    SETInteger<6> tempDet4 = Det4(tempD0x, tempD0y, tempD0z, tempW0, 
        tempD1x, tempD1y, tempD1z, tempW1, tempD2x, tempD2y, tempD2z, tempW2, 
        tempD3x, tempD3y, tempD3z, tempW3);

    return (tempDet4 > 0 ? 1 : (tempDet4 < 0 ? -1 : 0));
}
//----------------------------------------------------------------------------
SETInteger<3> SEQuery3TIntegerf::Dot(SETInteger<3>& rX0, SETInteger<3>& rY0, 
    SETInteger<3>& rZ0, SETInteger<3>& rX1, SETInteger<3>& rY1, 
    SETInteger<3>& rZ1)
{
    return rX0*rX1 + rY0*rY1 + rZ0*rZ1;
}
//----------------------------------------------------------------------------
SETInteger<4> SEQuery3TIntegerf::Det3(SETInteger<4>& rX0, SETInteger<4>& rY0, 
    SETInteger<4>& rZ0, SETInteger<4>& rX1, SETInteger<4>& rY1, 
    SETInteger<4>& rZ1, SETInteger<4>& rX2, SETInteger<4>& rY2, 
    SETInteger<4>& rZ2)
{
    SETInteger<4> tempC00 = rY1*rZ2 - rY2*rZ1;
    SETInteger<4> tempC01 = rY2*rZ0 - rY0*rZ2;
    SETInteger<4> tempC02 = rY0*rZ1 - rY1*rZ0;

    return rX0*tempC00 + rX1*tempC01 + rX2*tempC02;
}
//----------------------------------------------------------------------------
SETInteger<6> SEQuery3TIntegerf::Det4(SETInteger<6>& rX0, SETInteger<6>& rY0, 
    SETInteger<6>& rZ0, SETInteger<6>& rW0, SETInteger<6>& rX1, 
    SETInteger<6>& rY1, SETInteger<6>& rZ1, SETInteger<6>& rW1, 
    SETInteger<6>& rX2, SETInteger<6>& rY2, SETInteger<6>& rZ2, 
    SETInteger<6>& rW2, SETInteger<6>& rX3, SETInteger<6>& rY3, 
    SETInteger<6>& rZ3, SETInteger<6>& rW3)
{
    SETInteger<6> tempA0 = rX0*rY1 - rX1*rY0;
    SETInteger<6> tempA1 = rX0*rY2 - rX2*rY0;
    SETInteger<6> tempA2 = rX0*rY3 - rX3*rY0;
    SETInteger<6> tempA3 = rX1*rY2 - rX2*rY1;
    SETInteger<6> tempA4 = rX1*rY3 - rX3*rY1;
    SETInteger<6> tempA5 = rX2*rY3 - rX3*rY2;
    SETInteger<6> tempB0 = rZ0*rW1 - rZ1*rW0;
    SETInteger<6> tempB1 = rZ0*rW2 - rZ2*rW0;
    SETInteger<6> tempB2 = rZ0*rW3 - rZ3*rW0;
    SETInteger<6> tempB3 = rZ1*rW2 - rZ2*rW1;
    SETInteger<6> tempB4 = rZ1*rW3 - rZ3*rW1;
    SETInteger<6> tempB5 = rZ2*rW3 - rZ3*rW2;

    return tempA0*tempB5 - tempA1*tempB4 + tempA2*tempB3 + tempA3*tempB2 - 
        tempA4*tempB1 + tempA5*tempB0;
}
//----------------------------------------------------------------------------
