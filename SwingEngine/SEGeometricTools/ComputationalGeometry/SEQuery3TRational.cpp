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
#include "SEQuery3TRational.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEQuery3TRationalf::SEQuery3TRationalf(int iVCount, const SEVector3f* aVertex)
    :
    SEQuery3f(iVCount, aVertex)
{
    m_aRVertex = SE_NEW RVector[m_iVCount];
    m_abEvaluated = SE_NEW bool[m_iVCount];
    memset(m_abEvaluated, 0, m_iVCount*sizeof(bool));
}
//----------------------------------------------------------------------------
SEQuery3TRationalf::~SEQuery3TRationalf()
{
    SE_DELETE[] m_aRVertex;
    SE_DELETE[] m_abEvaluated;
}
//----------------------------------------------------------------------------
SEQuery::Type SEQuery3TRationalf::GetType() const
{
    return SEQuery::QT_RATIONAL;
}
//----------------------------------------------------------------------------
int SEQuery3TRationalf::ToPlane(int i, int iV0, int iV1, int iV2) const
{
    int aiIndex[4] = { i, iV0, iV1, iV2 };
    Convert(4, aiIndex);

    return ToPlane(m_aRVertex[i], iV0, iV1, iV2);
}
//----------------------------------------------------------------------------
int SEQuery3TRationalf::ToPlane(const SEVector3f& rP, int iV0, 
    int iV1, int iV2) const
{
    RVector tempRP;
    tempRP[0] = Rational(rP[0]);
    tempRP[1] = Rational(rP[1]);
    tempRP[2] = Rational(rP[2]);
    int aiIndex[3] = { iV0, iV1, iV2 };
    Convert(3, aiIndex);

    return ToPlane(tempRP, iV0, iV1, iV2);
}
//----------------------------------------------------------------------------
int SEQuery3TRationalf::ToTetrahedron(int i, int iV0, int iV1, int iV2, 
    int iV3) const
{
    int aiIndex[5] = { i, iV0, iV1, iV2, iV3 };
    Convert(5, aiIndex);

    return ToTetrahedron(m_aRVertex[i], iV0, iV1, iV2, iV3);
}
//----------------------------------------------------------------------------
int SEQuery3TRationalf::ToTetrahedron(const SEVector3f& rP, int iV0, 
    int iV1, int iV2, int iV3) const
{
    RVector tempRP;
    tempRP[0] = Rational(rP[0]);
    tempRP[1] = Rational(rP[1]);
    tempRP[2] = Rational(rP[2]);
    int aiIndex[4] = { iV0, iV1, iV2, iV3 };
    Convert(4, aiIndex);

    return ToTetrahedron(tempRP, iV0, iV1, iV2, iV3);
}
//----------------------------------------------------------------------------
int SEQuery3TRationalf::ToCircumsphere(int i, int iV0, int iV1, int iV2, 
    int iV3) const
{
    int aiIndex[5] = { i, iV0, iV1, iV2, iV3 };
    Convert(5, aiIndex);

    return ToCircumsphere(m_aRVertex[i], iV0, iV1, iV2, iV3);
}
//----------------------------------------------------------------------------
int SEQuery3TRationalf::ToCircumsphere(const SEVector3f& rP, int iV0, 
    int iV1, int iV2, int iV3) const
{
    RVector tempRP;
    tempRP[0] = Rational(rP[0]);
    tempRP[1] = Rational(rP[1]);
    tempRP[2] = Rational(rP[2]);
    int aiIndex[4] = { iV0, iV1, iV2, iV3 };
    Convert(4, aiIndex);

    return ToCircumsphere(tempRP, iV0, iV1, iV2, iV3);
}
//----------------------------------------------------------------------------
void SEQuery3TRationalf::Convert(int iCount, int* aiIndex) const
{
    for( int i = 0; i < iCount; i++)
    {
        int j = aiIndex[i];
        if( !m_abEvaluated[j] )
        {
            m_abEvaluated[j] = true;
            m_aRVertex[j][0] = Rational(m_aVertex[j][0]);
            m_aRVertex[j][1] = Rational(m_aVertex[j][1]);
            m_aRVertex[j][2] = Rational(m_aVertex[j][2]);
        }
    }
}
//----------------------------------------------------------------------------
int SEQuery3TRationalf::ToPlane(const RVector& rRP, int iV0, int iV1, 
    int iV2) const
{
    Rational tempX0 = rRP[0] - m_aRVertex[iV0][0];
    Rational tempY0 = rRP[1] - m_aRVertex[iV0][1];
    Rational tempZ0 = rRP[2] - m_aRVertex[iV0][2];
    Rational tempX1 = m_aRVertex[iV1][0] - m_aRVertex[iV0][0];
    Rational tempY1 = m_aRVertex[iV1][1] - m_aRVertex[iV0][1];
    Rational tempZ1 = m_aRVertex[iV1][2] - m_aRVertex[iV0][2];
    Rational tempX2 = m_aRVertex[iV2][0] - m_aRVertex[iV0][0];
    Rational tempY2 = m_aRVertex[iV2][1] - m_aRVertex[iV0][1];
    Rational tempZ2 = m_aRVertex[iV2][2] - m_aRVertex[iV0][2];

    Rational tempDet3 = Det3(tempX0, tempY0, tempZ0, 
        tempX1, tempY1, tempZ1, tempX2, tempY2, tempZ2);

    return (tempDet3 > 0 ? +1 : (tempDet3 < 0 ? -1 : 0));
}
//----------------------------------------------------------------------------
int SEQuery3TRationalf::ToTetrahedron (const RVector& rRP, int iV0, 
    int iV1, int iV2, int iV3) const
{
    int iSign0 = ToPlane(rRP, iV1, iV2, iV3);
    if( iSign0 > 0 )
    {
        return +1;
    }

    int iSign1 = ToPlane(rRP, iV0, iV2, iV3);
    if( iSign1 < 0 )
    {
        return +1;
    }

    int iSign2 = ToPlane(rRP, iV0, iV1, iV3);
    if( iSign2 > 0 )
    {
        return +1;
    }

    int iSign3 = ToPlane(rRP, iV0, iV1, iV2);
    if( iSign3 < 0 )
    {
        return +1;
    }

    return ((iSign0 && iSign1 && iSign2 && iSign3) ? -1 : 0);
}
//----------------------------------------------------------------------------
int SEQuery3TRationalf::ToCircumsphere(const RVector& rRP, int iV0, 
    int iV1, int iV2, int iV3) const
{
    RVector& rRV0 = m_aRVertex[iV0];
    RVector& rRV1 = m_aRVertex[iV1];
    RVector& rRV2 = m_aRVertex[iV2];
    RVector& rRV3 = m_aRVertex[iV3];

    Rational tempS0x = rRV0[0] + rRP[0];
    Rational tempD0x = rRV0[0] - rRP[0];
    Rational tempS0y = rRV0[1] + rRP[1];
    Rational tempD0y = rRV0[1] - rRP[1];
    Rational tempS0z = rRV0[2] + rRP[2];
    Rational tempD0z = rRV0[2] - rRP[2];
    Rational tempS1x = rRV1[0] + rRP[0];
    Rational tempD1x = rRV1[0] - rRP[0];
    Rational tempS1y = rRV1[1] + rRP[1];
    Rational tempD1y = rRV1[1] - rRP[1];
    Rational tempS1z = rRV1[2] + rRP[2];
    Rational tempD1z = rRV1[2] - rRP[2];
    Rational tempS2x = rRV2[0] + rRP[0];
    Rational tempD2x = rRV2[0] - rRP[0];
    Rational tempS2y = rRV2[1] + rRP[1];
    Rational tempD2y = rRV2[1] - rRP[1];
    Rational tempS2z = rRV2[2] + rRP[2];
    Rational tempD2z = rRV2[2] - rRP[2];
    Rational tempS3x = rRV3[0] + rRP[0];
    Rational tempD3x = rRV3[0] - rRP[0];
    Rational tempS3y = rRV3[1] + rRP[1];
    Rational tempD3y = rRV3[1] - rRP[1];
    Rational tempS3z = rRV3[2] + rRP[2];
    Rational tempD3z = rRV3[2] - rRP[2];
    Rational tempW0 = tempS0x*tempD0x + tempS0y*tempD0y + tempS0z*tempD0z;
    Rational tempW1 = tempS1x*tempD1x + tempS1y*tempD1y + tempS1z*tempD1z;
    Rational tempW2 = tempS2x*tempD2x + tempS2y*tempD2y + tempS2z*tempD2z;
    Rational tempW3 = tempS3x*tempD3x + tempS3y*tempD3y + tempS3z*tempD3z;
    Rational tempDet4 = Det4(tempD0x, tempD0y, tempD0z, tempW0, 
        tempD1x, tempD1y, tempD1z, tempW1, tempD2x, tempD2y, tempD2z, 
        tempW2, tempD3x, tempD3y, tempD3z, tempW3);

    return (tempDet4 > 0 ? 1 : (tempDet4 < 0 ? -1 : 0));
}
//----------------------------------------------------------------------------
SEQuery3TRationalf::Rational SEQuery3TRationalf::Dot(
    Rational& rX0, Rational& rY0, Rational& rZ0, Rational& rX1, 
    Rational& rY1, Rational& rZ1)
{
    return rX0*rX1 + rY0*rY1 + rZ0*rZ1;
}
//----------------------------------------------------------------------------
SEQuery3TRationalf::Rational SEQuery3TRationalf::Det3(
    Rational& rX0, Rational& rY0, Rational& rZ0, Rational& rX1, 
    Rational& rY1, Rational& rZ1, Rational& rX2, Rational& rY2, 
    Rational& rZ2)
{
    Rational tempC00 = rY1*rZ2 - rY2*rZ1;
    Rational tempC01 = rY2*rZ0 - rY0*rZ2;
    Rational tempC02 = rY0*rZ1 - rY1*rZ0;

    return rX0*tempC00 + rX1*tempC01 + rX2*tempC02;
}
//----------------------------------------------------------------------------
SEQuery3TRationalf::Rational SEQuery3TRationalf::Det4(
    Rational& rX0, Rational& rY0, Rational& rZ0, Rational& rW0, 
    Rational& rX1, Rational& rY1, Rational& rZ1, Rational& rW1, 
    Rational& rX2, Rational& rY2, Rational& rZ2, Rational& rW2, 
    Rational& rX3, Rational& rY3, Rational& rZ3, Rational& rW3)
{
    Rational tempA0 = rX0*rY1 - rX1*rY0;
    Rational tempA1 = rX0*rY2 - rX2*rY0;
    Rational tempA2 = rX0*rY3 - rX3*rY0;
    Rational tempA3 = rX1*rY2 - rX2*rY1;
    Rational tempA4 = rX1*rY3 - rX3*rY1;
    Rational tempA5 = rX2*rY3 - rX3*rY2;
    Rational tempB0 = rZ0*rW1 - rZ1*rW0;
    Rational tempB1 = rZ0*rW2 - rZ2*rW0;
    Rational tempB2 = rZ0*rW3 - rZ3*rW0;
    Rational tempB3 = rZ1*rW2 - rZ2*rW1;
    Rational tempB4 = rZ1*rW3 - rZ3*rW1;
    Rational tempB5 = rZ2*rW3 - rZ3*rW2;

    return tempA0*tempB5 - tempA1*tempB4 + tempA2*tempB3 + tempA3*tempB2 - 
        tempA4*tempB1 + tempA5*tempB0;
}
//----------------------------------------------------------------------------
