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
#include "SEConvexHull1.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEConvexHull1f::SEConvexHull1f(int iVertexCount, float* afVertex, 
    float fEpsilon, bool bOwner, SEQuery::Type eQueryType)
    :
    SEConvexHullf(iVertexCount, fEpsilon, bOwner, eQueryType)
{
    SE_ASSERT(afVertex);
    m_afVertex = afVertex;

    std::vector<SESortedVertex> tempArray(m_iVertexCount);
    int i;
    for( i = 0; i < m_iVertexCount; i++ )
    {
        tempArray[i].Value = m_afVertex[i];
        tempArray[i].Index = i;
    }
    std::sort(tempArray.begin(), tempArray.end());

    float fRange = tempArray[m_iVertexCount-1].Value - tempArray[0].Value;
    if( fRange >= m_fEpsilon )
    {
        m_iDimension = 1;
        m_iSimplexCount = 2;
        m_aiIndex = SE_NEW int[2];
        m_aiIndex[0] = tempArray[0].Index;
        m_aiIndex[1] = tempArray[m_iVertexCount - 1].Index;
    }
}
//----------------------------------------------------------------------------
SEConvexHull1f::~SEConvexHull1f()
{
    if( m_bOwner )
    {
        SE_DELETE[] m_afVertex;
    }
}
//----------------------------------------------------------------------------
const float* SEConvexHull1f::GetVertices() const
{
    return m_afVertex;
}
//----------------------------------------------------------------------------
SEConvexHull1f::SEConvexHull1f(const char* acFilename)
    :
    SEConvexHullf(0, 0.0f, false, SEQuery::QT_REAL)
{
    m_afVertex = 0;
    bool bLoaded = Load(acFilename);
    SE_ASSERT( bLoaded );
    (void)bLoaded;  // avoid warning in Release build
}
//----------------------------------------------------------------------------
bool SEConvexHull1f::Load(const char* acFilename)
{
    FILE* pIFile = SESystem::Fopen(acFilename, "rb");
    if( !pIFile )
    {
        return false;
    }

    SEConvexHullf::Load(pIFile);

    if( m_bOwner )
    {
        SE_DELETE[] m_afVertex;
    }

    m_bOwner = true;
    m_afVertex = SE_NEW float[m_iVertexCount];

    size_t uiSize = sizeof(float);
    if( uiSize == 4 )
    {
        SESystem::Read4le(pIFile, m_iVertexCount, m_afVertex);
    }
    else // uiSize == 8
    {
        SESystem::Read8le(pIFile, m_iVertexCount, m_afVertex);
    }

    SESystem::Fclose(pIFile);

    return true;
}
//----------------------------------------------------------------------------
bool SEConvexHull1f::Save(const char* acFilename) const
{
    FILE* pOFile = SESystem::Fopen(acFilename, "wb");
    if( !pOFile )
    {
        return false;
    }

    SEConvexHullf::Save(pOFile);

    size_t uiSize = sizeof(float);
    if( uiSize == 4 )
    {
        SESystem::Write4le(pOFile, m_iVertexCount, m_afVertex);
    }
    else // uiSize == 8
    {
        SESystem::Write8le(pOFile, m_iVertexCount, m_afVertex);
    }

    SESystem::Fclose(pOFile);

    return true;
}
//----------------------------------------------------------------------------