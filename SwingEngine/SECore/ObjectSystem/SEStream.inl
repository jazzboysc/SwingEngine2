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
inline int SEStream::GetOrderedCount() const
{
    return (int)m_Ordered.size();
}
//----------------------------------------------------------------------------
inline SEObject* SEStream::GetOrderedObject(int i) const
{
    SE_ASSERT( 0 <= i && i < (int)m_Ordered.size() );

    if( i < 0 || i >= (int)m_Ordered.size() )
    {
        return 0;
    }

    return m_Ordered[i];
}
//----------------------------------------------------------------------------
inline int SEStream::GetBufferSize() const
{
    return m_iBufferSize;
}
//----------------------------------------------------------------------------
inline int SEStream::GetBufferNext() const
{
    return m_iBufferNext;
}
//----------------------------------------------------------------------------
inline SEStreamVersion SEStream::GetVersion() const
{
    return m_Version;
}
//----------------------------------------------------------------------------
