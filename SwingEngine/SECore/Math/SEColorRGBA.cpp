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
#include "SEColorRGBA.h"

using namespace Swing;

const SEColorRGBA SEColorRGBA::SE_RGBA_BLACK(0.0f, 0.0f, 0.0f, 1.0f);
const SEColorRGBA SEColorRGBA::SE_RGBA_WHITE(1.0f, 1.0f, 1.0f, 1.0f);
const SEColorRGBA SEColorRGBA::SE_RGBA_RED(1.0f, 0.0f, 0.0f, 1.0f);
const SEColorRGBA SEColorRGBA::SE_RGBA_GREEN(0.0f, 1.0f, 0.0f, 1.0f);
const SEColorRGBA SEColorRGBA::SE_RGBA_BLUE(0.0f, 0.0f, 1.0f, 1.0f);
const SEColorRGBA SEColorRGBA::SE_RGBA_INVALID(-1.0f, -1.0f, -1.0f, -1.0f);

//----------------------------------------------------------------------------
SEColorRGBA::SEColorRGBA()
{
    m_fData[0] = 0.0f;
    m_fData[1] = 0.0f;
    m_fData[2] = 0.0f;
    m_fData[3] = 0.0f;
}
//----------------------------------------------------------------------------
SEColorRGBA::SEColorRGBA(float fR, float fG, float fB, float fA)
{
    m_fData[0] = fR;
    m_fData[1] = fG;
    m_fData[2] = fB;
    m_fData[3] = fA;
}
//----------------------------------------------------------------------------
SEColorRGBA::SEColorRGBA(const float* pData)
{
    m_fData[0] = pData[0];
    m_fData[1] = pData[1];
    m_fData[2] = pData[2];
    m_fData[3] = pData[3];
}
//----------------------------------------------------------------------------
SEColorRGBA::SEColorRGBA(const SEColorRGBA& rColor)
{
    m_fData[0] = rColor.m_fData[0];
    m_fData[1] = rColor.m_fData[1];
    m_fData[2] = rColor.m_fData[2];
    m_fData[3] = rColor.m_fData[3];
}
//----------------------------------------------------------------------------
SEColorRGBA& SEColorRGBA::operator=(const SEColorRGBA& rColor)
{
    m_fData[0] = rColor.m_fData[0];
    m_fData[1] = rColor.m_fData[1];
    m_fData[2] = rColor.m_fData[2];
    m_fData[3] = rColor.m_fData[3];

    return *this;
}
//----------------------------------------------------------------------------
bool SEColorRGBA::operator==(const SEColorRGBA& rColor) const
{
    return
        m_fData[0] == rColor.m_fData[0] &&
        m_fData[1] == rColor.m_fData[1] &&
        m_fData[2] == rColor.m_fData[2] &&
        m_fData[3] == rColor.m_fData[3];
}
//----------------------------------------------------------------------------
bool SEColorRGBA::operator!=(const SEColorRGBA& rColor) const
{
    return
        m_fData[0] != rColor.m_fData[0] ||
        m_fData[1] != rColor.m_fData[1] ||
        m_fData[2] != rColor.m_fData[2] ||
        m_fData[3] != rColor.m_fData[3];
}
//----------------------------------------------------------------------------
bool SEColorRGBA::operator<(const SEColorRGBA& rColor) const
{
    return CompareData(rColor) < 0;
}
//----------------------------------------------------------------------------
bool SEColorRGBA::operator<=(const SEColorRGBA& rColor) const
{
    return CompareData(rColor) <= 0;
}
//----------------------------------------------------------------------------
bool SEColorRGBA::operator>(const SEColorRGBA& rColor) const
{
    return CompareData(rColor) > 0;
}
//----------------------------------------------------------------------------
bool SEColorRGBA::operator>=(const SEColorRGBA& rColor) const
{
    return CompareData(rColor) >= 0;
}
//----------------------------------------------------------------------------
SEColorRGBA SEColorRGBA::operator+(const SEColorRGBA& rColor) const
{
    return SEColorRGBA(
        m_fData[0] + rColor.m_fData[0],
        m_fData[1] + rColor.m_fData[1],
        m_fData[2] + rColor.m_fData[2],
        m_fData[3] + rColor.m_fData[3]);
}
//----------------------------------------------------------------------------
SEColorRGBA SEColorRGBA::operator-(const SEColorRGBA& rColor) const
{
    return SEColorRGBA(
        m_fData[0] - rColor.m_fData[0],
        m_fData[1] - rColor.m_fData[1],
        m_fData[2] - rColor.m_fData[2],
        m_fData[3] - rColor.m_fData[3]);
}
//----------------------------------------------------------------------------
SEColorRGBA SEColorRGBA::operator*(const SEColorRGBA& rColor) const
{
    return SEColorRGBA(
        m_fData[0] * rColor.m_fData[0],
        m_fData[1] * rColor.m_fData[1],
        m_fData[2] * rColor.m_fData[2],
        m_fData[3] * rColor.m_fData[3]);
}
//----------------------------------------------------------------------------
SEColorRGBA SEColorRGBA::operator*(float fScalar) const
{
    return SEColorRGBA(
        fScalar * m_fData[0],
        fScalar * m_fData[1],
        fScalar * m_fData[2],
        fScalar * m_fData[3]);
}
//----------------------------------------------------------------------------
SEColorRGBA Swing::operator*(float fScalar, const SEColorRGBA& rColor)
{
    return SEColorRGBA(
        fScalar * rColor.m_fData[0],
        fScalar * rColor.m_fData[1],
        fScalar * rColor.m_fData[2],
        fScalar * rColor.m_fData[3]);
}
//----------------------------------------------------------------------------
SEColorRGBA& SEColorRGBA::operator+=(const SEColorRGBA& rColor)
{
    m_fData[0] += rColor.m_fData[0];
    m_fData[1] += rColor.m_fData[1];
    m_fData[2] += rColor.m_fData[2];
    m_fData[3] += rColor.m_fData[3];

    return *this;
}
//----------------------------------------------------------------------------
SEColorRGBA& SEColorRGBA::operator-=(const SEColorRGBA& rColor)
{
    m_fData[0] -= rColor.m_fData[0];
    m_fData[1] -= rColor.m_fData[1];
    m_fData[2] -= rColor.m_fData[2];
    m_fData[3] -= rColor.m_fData[3];

    return *this;
}
//----------------------------------------------------------------------------
SEColorRGBA& SEColorRGBA::operator*=(const SEColorRGBA& rColor)
{
    m_fData[0] *= rColor.m_fData[0];
    m_fData[1] *= rColor.m_fData[1];
    m_fData[2] *= rColor.m_fData[2];
    m_fData[3] *= rColor.m_fData[3];

    return *this;
}
//----------------------------------------------------------------------------
SEColorRGBA& SEColorRGBA::operator*=(float fScalar)
{
    m_fData[0] *= fScalar;
    m_fData[1] *= fScalar;
    m_fData[2] *= fScalar;
    m_fData[3] *= fScalar;

    return *this;
}
//----------------------------------------------------------------------------
void SEColorRGBA::Clamp()
{
    for( int i = 0; i < 4; i++ )
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
void SEColorRGBA::ScaleByMax()
{
    // 找到最大RGB值
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

    // 裁剪A值到[0,1]区间
    if( m_fData[3] > 1.0f )
    {
        m_fData[3] = 1.0f;
    }
    else if( m_fData[3] < 0.0f )
    {
        m_fData[3] = 0.0f;
    }
}
//----------------------------------------------------------------------------
