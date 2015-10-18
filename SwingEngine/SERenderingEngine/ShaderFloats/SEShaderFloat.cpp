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

#include "SERenderingEnginePCH.h"
#include "SEShaderFloat.h"

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, SEShaderFloat, SEObject);
SE_IMPLEMENT_STREAM(SEShaderFloat);
SE_IMPLEMENT_DEFAULT_NAME_ID(SEShaderFloat, SEObject);

//----------------------------------------------------------------------------
SEShaderFloat::SEShaderFloat()
    :
    m_iDataCount(0),
    m_afData(0),
    m_bAllowUpdater(false)
{
}
//----------------------------------------------------------------------------
SEShaderFloat::SEShaderFloat(int iDataCount)
    :
    m_bAllowUpdater(false)
{
    SetDataCount(iDataCount);
}
//----------------------------------------------------------------------------
SEShaderFloat::~SEShaderFloat()
{
    SE_DELETE[] m_afData;
}
//----------------------------------------------------------------------------
void SEShaderFloat::SetDataCount(int iDataCount)
{
    SE_ASSERT( iDataCount > 0 );

    m_iDataCount = iDataCount;
    if( m_afData )
    {
        SE_DELETE[] m_afData;
    }
    m_afData = SE_NEW float[m_iDataCount];
}
//----------------------------------------------------------------------------
void SEShaderFloat::SetData(const float* pSrcData)
{
    float* pDst = m_afData;
    for( int i = 0; i < m_iDataCount; ++i )
    {
        *pDst++ = *pSrcData++;
    }
}
//----------------------------------------------------------------------------
void SEShaderFloat::GetData(float* pDstData)
{
    float* pSrc = m_afData;
    for( int i = 0; i < m_iDataCount; ++i )
    {
        *pDstData++ = *pSrc++;
    }
}
//----------------------------------------------------------------------------
SEShaderFloat& SEShaderFloat::operator=(const float* pSrcData)
{
    float* pDst = m_afData;
    for( int i = 0; i < m_iDataCount; ++i )
    {
        *pDst++ = *pSrcData++;
    }
    return *this;
}
//----------------------------------------------------------------------------
void SEShaderFloat::Update(const SEGeometry*, const SECamera*)
{
    // Stub for derived classes.
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Streaming support.
//----------------------------------------------------------------------------
void SEShaderFloat::Load(SEStream& rStream, SEStream::SELink* pLink)
{
    SE_BEGIN_DEBUG_STREAM_LOAD;

    SEObject::Load(rStream, pLink);

    rStream.Read(m_iDataCount);
    m_afData = SE_NEW float[m_iDataCount];
    rStream.Read(m_iDataCount, m_afData);

    rStream.Read(m_bAllowUpdater);

    SE_END_DEBUG_STREAM_LOAD(SEShaderFloat);
}
//----------------------------------------------------------------------------
void SEShaderFloat::Link(SEStream& rStream, SEStream::SELink* pLink)
{
    SEObject::Link(rStream, pLink);
}
//----------------------------------------------------------------------------
bool SEShaderFloat::Register(SEStream& rStream) const
{
    return SEObject::Register(rStream);
}
//----------------------------------------------------------------------------
void SEShaderFloat::Save(SEStream& rStream) const
{
    SE_BEGIN_DEBUG_STREAM_SAVE;

    SEObject::Save(rStream);

    rStream.Write(m_iDataCount);
    rStream.Write(m_iDataCount, m_afData);
    rStream.Write(m_bAllowUpdater);

    SE_END_DEBUG_STREAM_SAVE(SEShaderFloat);
}
//----------------------------------------------------------------------------
int SEShaderFloat::GetDiskUsed(const SEStreamVersion& rVersion) const
{
    int iSize = SEObject::GetDiskUsed(rVersion);
    iSize += sizeof(m_iDataCount);
    iSize += m_iDataCount*sizeof(m_afData[0]);
    iSize += sizeof(char);  // m_bAllowUpdater
    return iSize;
}
//----------------------------------------------------------------------------