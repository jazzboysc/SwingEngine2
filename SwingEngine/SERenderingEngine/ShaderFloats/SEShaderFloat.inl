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
inline int SEShaderFloat::GetDataCount() const
{
    return m_iDataCount;
}
//----------------------------------------------------------------------------
inline const float* SEShaderFloat::GetData() const
{
    return m_afData;
}
//----------------------------------------------------------------------------
inline float* SEShaderFloat::GetData()
{
    return m_afData;
}
//----------------------------------------------------------------------------
inline float SEShaderFloat::operator[](int i) const
{
    SE_ASSERT( 0 <= i && i < m_iDataCount );
    return m_afData[i];
}
//----------------------------------------------------------------------------
inline float& SEShaderFloat::operator[](int i)
{
    SE_ASSERT( 0 <= i && i < m_iDataCount );
    return m_afData[i];
}
//----------------------------------------------------------------------------
inline void SEShaderFloat::EnableUpdater()
{
    m_bAllowUpdater = true;
}
//----------------------------------------------------------------------------
inline void SEShaderFloat::DisableUpdater()
{
    m_bAllowUpdater = false;
}
//----------------------------------------------------------------------------
inline bool SEShaderFloat::AllowUpdater() const
{
    return m_bAllowUpdater;
}
//----------------------------------------------------------------------------
