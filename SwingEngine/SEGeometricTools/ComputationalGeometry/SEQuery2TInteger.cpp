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
#include "SEQuery2TInteger.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEQuery2TIntegerf::SEQuery2TIntegerf(int iVCount, const SEVector2f* aVertex)
    :
    SEQuery2f(iVCount, aVertex)
{
}
//----------------------------------------------------------------------------
SEQuery::Type SEQuery2TIntegerf::GetType() const
{
    return SEQuery::QT_INTEGER;
}
//----------------------------------------------------------------------------
int SEQuery2TIntegerf::ToLine(const SEVector2f& rP, int iV0, int iV1) const
{
    const SEVector2f& rV0 = m_aVertex[iV0];
    const SEVector2f& rV1 = m_aVertex[iV1];

    SETInteger<2> tempX0((int)rP[0] - (int)rV0[0]);
    SETInteger<2> tempY0((int)rP[1] - (int)rV0[1]);
    SETInteger<2> tempX1((int)rV1[0] - (int)rV0[0]);
    SETInteger<2> tempY1((int)rV1[1] - (int)rV0[1]);

    SETInteger<2> tempDet2 = Det2(tempX0,tempY0,tempX1, tempY1);

    return (tempDet2 > 0 ? +1 : (tempDet2 < 0 ? -1 : 0));
}
//----------------------------------------------------------------------------
int SEQuery2TIntegerf::ToCircumcircle(const SEVector2f& rP, int iV0,
    int iV1, int iV2) const
{
    const SEVector2f& rV0 = m_aVertex[iV0];
    const SEVector2f& rV1 = m_aVertex[iV1];
    const SEVector2f& rV2 = m_aVertex[iV2];

    int aiP[2] = { (int)rP[0], (int)rP[1] };
    int aiV0[2] = { (int)rV0[0], (int)rV0[1] };
    int aiV1[2] = { (int)rV1[0], (int)rV1[1] };
    int aiV2[2] = { (int)rV2[0], (int)rV2[1] };

    SETInteger<4> tempS0x(aiV0[0] + aiP[0]);
    SETInteger<4> tempD0x(aiV0[0] - aiP[0]);
    SETInteger<4> tempS0y(aiV0[1] + aiP[1]);
    SETInteger<4> tempD0y(aiV0[1] - aiP[1]);
    SETInteger<4> tempS1x(aiV1[0] + aiP[0]);
    SETInteger<4> tempD1x(aiV1[0] - aiP[0]);
    SETInteger<4> tempS1y(aiV1[1] + aiP[1]);
    SETInteger<4> tempD1y(aiV1[1] - aiP[1]);
    SETInteger<4> tempS2x(aiV2[0] + aiP[0]);
    SETInteger<4> tempD2x(aiV2[0] - aiP[0]);
    SETInteger<4> tempS2y(aiV2[1] + aiP[1]);
    SETInteger<4> tempD2y(aiV2[1] - aiP[1]);
    SETInteger<4> tempZ0 = tempS0x*tempD0x + tempS0y*tempD0y;
    SETInteger<4> tempZ1 = tempS1x*tempD1x + tempS1y*tempD1y;
    SETInteger<4> tempZ2 = tempS2x*tempD2x + tempS2y*tempD2y;
    SETInteger<4> tempDet3 = Det3(tempD0x, tempD0y, tempZ0, 
        tempD1x, tempD1y, tempZ1, tempD2x, tempD2y, tempZ2);

    return (tempDet3 < 0 ? 1 : (tempDet3 > 0 ? -1 : 0));
}
//----------------------------------------------------------------------------
SETInteger<2> SEQuery2TIntegerf::Dot(SETInteger<2>& rX0, SETInteger<2>& rY0,
    SETInteger<2>& rX1, SETInteger<2>& rY1)
{
    return rX0*rX1 + rY0*rY1;
}
//----------------------------------------------------------------------------
SETInteger<2> SEQuery2TIntegerf::Det2(SETInteger<2>& rX0, SETInteger<2>& rY0,
    SETInteger<2>& rX1, SETInteger<2>& rY1)
{
    return rX0*rY1 - rX1*rY0;
}
//----------------------------------------------------------------------------
SETInteger<4> SEQuery2TIntegerf::Det3(SETInteger<4>& rX0, SETInteger<4>& rY0,
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
