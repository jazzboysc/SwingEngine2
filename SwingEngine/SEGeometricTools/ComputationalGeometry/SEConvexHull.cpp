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
#include "SEConvexHull.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEConvexHullf::SEConvexHullf(int iVertexCount, float fEpsilon, bool bOwner, 
    SEQuery::Type eQueryType)
{
    SE_ASSERT( iVertexCount > 0 && fEpsilon >= 0.0f );

    m_eQueryType = eQueryType;
    m_iVertexCount = iVertexCount;
    m_iDimension = 0;
    m_iSimplexCount = 0;
    m_aiIndex = 0;
    m_fEpsilon = fEpsilon;
    m_bOwner = bOwner;
}
//----------------------------------------------------------------------------
SEConvexHullf::~SEConvexHullf()
{
    SE_DELETE[] m_aiIndex;
}
//----------------------------------------------------------------------------
int SEConvexHullf::GetQueryType() const
{
    return m_eQueryType;
}
//----------------------------------------------------------------------------
int SEConvexHullf::GetVertexCount() const
{
    return m_iVertexCount;
}
//----------------------------------------------------------------------------
float SEConvexHullf::GetEpsilon() const
{
    return m_fEpsilon;
}
//----------------------------------------------------------------------------
bool SEConvexHullf::GetOwner() const
{
    return m_bOwner;
}
//----------------------------------------------------------------------------
int SEConvexHullf::GetDimension() const
{
    return m_iDimension;
}
//----------------------------------------------------------------------------
int SEConvexHullf::GetSimplexCount() const
{
    return m_iSimplexCount;
}
//----------------------------------------------------------------------------
const int* SEConvexHullf::GetIndices() const
{
    return m_aiIndex;
}
//----------------------------------------------------------------------------
bool SEConvexHullf::Load(FILE* pIFile)
{
    SE_DELETE[] m_aiIndex;

    // fixed-size members
    int iQueryType;
    SESystem::Read4le(pIFile, 1, &iQueryType);
    m_eQueryType = (SEQuery::Type)iQueryType;
    SESystem::Read4le(pIFile, 1, &m_iVertexCount);
    SESystem::Read4le(pIFile, 1, &m_iDimension);
    SESystem::Read4le(pIFile, 1, &m_iSimplexCount);
    SESystem::Read4le(pIFile, 1, &m_fEpsilon);

    // variable-size members
    int iICount;
    SESystem::Read4le(pIFile, 1, &iICount);
    if( 1 <= m_iDimension && m_iDimension <= 3 )
    {
        SE_ASSERT( iICount == (m_iDimension+1)*m_iSimplexCount );

        m_aiIndex = SE_NEW int[iICount];
        SESystem::Read4le(pIFile, iICount, m_aiIndex);

        return true;
    }

    m_aiIndex = 0;

    return m_iDimension == 0;
}
//----------------------------------------------------------------------------
bool SEConvexHullf::Save(FILE* pOFile) const
{
    // fixed-size members
    int iQueryType = (int)m_eQueryType;
    SESystem::Write4le(pOFile, 1, &iQueryType);
    SESystem::Write4le(pOFile, 1, &m_iVertexCount);
    SESystem::Write4le(pOFile, 1, &m_iDimension);
    SESystem::Write4le(pOFile, 1, &m_iSimplexCount);
    SESystem::Write4le(pOFile, 1, &m_fEpsilon);

    // The member m_bOwner is not streamed because on a Load call, this
    // object will allocate the vertices and own this memory.

    // variable-size members
    int iICount;
    if( 1 <= m_iDimension && m_iDimension <= 3 )
    {
        iICount = (m_iDimension + 1)*m_iSimplexCount;
        SESystem::Write4le(pOFile, 1, &iICount);
        SESystem::Write4le(pOFile, iICount, m_aiIndex);

        return true;
    }

    iICount = 0;
    SESystem::Write4le(pOFile, 1, &iICount);

    return m_iDimension == 0;
}
//----------------------------------------------------------------------------
