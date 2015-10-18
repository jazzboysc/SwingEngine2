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
#include "SEColorRGB.h"

using namespace Swing;

const SEColorRGB SEColorRGB::SE_RGB_BLACK(0.0f, 0.0f, 0.0f);
const SEColorRGB SEColorRGB::SE_RGB_WHITE(1.0f, 1.0f, 1.0f);
const SEColorRGB SEColorRGB::SE_RGB_RED(1.0f, 0.0f, 0.0f);
const SEColorRGB SEColorRGB::SE_RGB_GREEN(0.0f, 1.0f, 0.0f);
const SEColorRGB SEColorRGB::SE_RGB_BLUE(0.0f, 0.0f, 1.0f);
const SEColorRGB SEColorRGB::SE_RGB_INVALID(-1.0f, -1.0f, -1.0f);

//----------------------------------------------------------------------------
SEColorRGB::SEColorRGB()
{
    m_fData[0] = 0.0f;
    m_fData[1] = 0.0f;
    m_fData[2] = 0.0f;
}
//----------------------------------------------------------------------------
SEColorRGB::SEColorRGB(float fR, float fG, float fB)
{
    m_fData[0] = fR;
    m_fData[1] = fG;
    m_fData[2] = fB;
}
//----------------------------------------------------------------------------
SEColorRGB::SEColorRGB(const float* pData)
{
    m_fData[0] = pData[0];
    m_fData[1] = pData[1];
    m_fData[2] = pData[2];
}
//----------------------------------------------------------------------------
SEColorRGB::SEColorRGB(const SEColorRGB& rColor)
{
    m_fData[0] = rColor.m_fData[0];
    m_fData[1] = rColor.m_fData[1];
    m_fData[2] = rColor.m_fData[2];
}
//----------------------------------------------------------------------------
SEColorRGB& SEColorRGB::operator=(const SEColorRGB& rColor)
{
    m_fData[0] = rColor.m_fData[0];
    m_fData[1] = rColor.m_fData[1];
    m_fData[2] = rColor.m_fData[2];

    return *this;
}
//----------------------------------------------------------------------------
bool SEColorRGB::operator==(const SEColorRGB& rColor) const
{
    return
        m_fData[0] == rColor.m_fData[0] &&
        m_fData[1] == rColor.m_fData[1] &&
        m_fData[2] == rColor.m_fData[2];
}
//----------------------------------------------------------------------------
bool SEColorRGB::operator!=(const SEColorRGB& rColor) const
{
    return
        m_fData[0] != rColor.m_fData[0] ||
        m_fData[1] != rColor.m_fData[1] ||
        m_fData[2] != rColor.m_fData[2];
}
//----------------------------------------------------------------------------
bool SEColorRGB::operator<(const SEColorRGB& rColor) const
{
    return CompareData(rColor) < 0;
}
//----------------------------------------------------------------------------
bool SEColorRGB::operator<=(const SEColorRGB& rColor) const
{
    return CompareData(rColor) <= 0;
}
//----------------------------------------------------------------------------
bool SEColorRGB::operator>(const SEColorRGB& rColor) const
{
    return CompareData(rColor) > 0;
}
//----------------------------------------------------------------------------
bool SEColorRGB::operator>=(const SEColorRGB& rColor) const
{
    return CompareData(rColor) >= 0;
}
//----------------------------------------------------------------------------
SEColorRGB SEColorRGB::operator+(const SEColorRGB& rColor) const
{
    return SEColorRGB(
        m_fData[0] + rColor.m_fData[0],
        m_fData[1] + rColor.m_fData[1],
        m_fData[2] + rColor.m_fData[2]);
}
//----------------------------------------------------------------------------
SEColorRGB SEColorRGB::operator-(const SEColorRGB& rColor) const
{
    return SEColorRGB(
        m_fData[0] - rColor.m_fData[0],
        m_fData[1] - rColor.m_fData[1],
        m_fData[2] - rColor.m_fData[2]);
}
//----------------------------------------------------------------------------
SEColorRGB SEColorRGB::operator*(const SEColorRGB& rColor) const
{
    return SEColorRGB(
        m_fData[0] * rColor.m_fData[0],
        m_fData[1] * rColor.m_fData[1],
        m_fData[2] * rColor.m_fData[2]);
}
//----------------------------------------------------------------------------
SEColorRGB SEColorRGB::operator*(float fScalar) const
{
    return SEColorRGB(
        fScalar * m_fData[0],
        fScalar * m_fData[1],
        fScalar * m_fData[2]);
}
//----------------------------------------------------------------------------
SEColorRGB Swing::operator*(float fScalar, const SEColorRGB& rColor)
{
    return SEColorRGB(
        fScalar * rColor.m_fData[0],
        fScalar * rColor.m_fData[1],
        fScalar * rColor.m_fData[2]);
}
//----------------------------------------------------------------------------
SEColorRGB& SEColorRGB::operator+=(const SEColorRGB& rColor)
{
    m_fData[0] += rColor.m_fData[0];
    m_fData[1] += rColor.m_fData[1];
    m_fData[2] += rColor.m_fData[2];

    return *this;
}
//----------------------------------------------------------------------------
SEColorRGB& SEColorRGB::operator-=(const SEColorRGB& rColor)
{
    m_fData[0] -= rColor.m_fData[0];
    m_fData[1] -= rColor.m_fData[1];
    m_fData[2] -= rColor.m_fData[2];

    return *this;
}
//----------------------------------------------------------------------------
SEColorRGB& SEColorRGB::operator*=(const SEColorRGB& rColor)
{
    m_fData[0] *= rColor.m_fData[0];
    m_fData[1] *= rColor.m_fData[1];
    m_fData[2] *= rColor.m_fData[2];

    return *this;
}
//----------------------------------------------------------------------------
SEColorRGB& SEColorRGB::operator*=(float fScalar)
{
    m_fData[0] *= fScalar;
    m_fData[1] *= fScalar;
    m_fData[2] *= fScalar;

    return *this;
}
//----------------------------------------------------------------------------
void SEColorRGB::Clamp()
{
    for( int i = 0; i < 3; i++ )
    {
        if( m_fData[i] > 1.0f )
        {
            m_fData[i] = 1.0f;
        }
        else if( m_fData[i] < 0.0f )
        {
            m_fData[i] = 0.0f;
        }
    }
}
//----------------------------------------------------------------------------
void SEColorRGB::ScaleByMax()
{
    float fMax = m_fData[0];

    if( m_fData[1] > fMax )
    {
        fMax = m_fData[1];
    }
    if( m_fData[2] > fMax )
    {
        fMax = m_fData[2];
    }

    if( fMax > 1.0f )
    {
        float fInvMax = 1.0f/fMax;
        for( int i = 0; i < 3; i++ )
        {
            m_fData[i] *= fInvMax;
        }
    }
}
//----------------------------------------------------------------------------
