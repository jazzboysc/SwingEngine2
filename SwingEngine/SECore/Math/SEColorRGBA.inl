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

//----------------------------------------------------------------------------
inline SEColorRGBA::operator const float*() const
{
    return m_fData;
}
//----------------------------------------------------------------------------
inline SEColorRGBA::operator float*()
{
    return m_fData;
}
//----------------------------------------------------------------------------
inline float SEColorRGBA::operator[](int i) const
{
    SE_ASSERT( 0 <= i && i <= 3 );

    if( i < 0 )
    {
        i = 0;
    }
    else if( i > 3 )
    {
        i = 3;
    }

    return m_fData[i];
}
//----------------------------------------------------------------------------
inline float& SEColorRGBA::operator[](int i)
{
    SE_ASSERT( 0 <= i && i <= 3 );

    if( i < 0 )
    {
        i = 0;
    }
    else if( i > 3 )
    {
        i = 3;
    }

    return m_fData[i];
}
//----------------------------------------------------------------------------
inline int SEColorRGBA::CompareData(const SEColorRGBA& rColor) const
{
    return memcmp(m_fData, rColor.m_fData, 4*sizeof(float));
}
//----------------------------------------------------------------------------
