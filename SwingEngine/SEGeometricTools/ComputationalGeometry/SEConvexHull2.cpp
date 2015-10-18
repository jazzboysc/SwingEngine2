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
#include "SEConvexHull2.h"
#include "SEMapper2.h"
#include "SEQuery2Filtered.h"
#include "SEQuery2Int64.h"
#include "SEQuery2TInteger.h"
#include "SEQuery2TRational.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEConvexHull2f::SEConvexHull2f(int iVertexCount, SEVector2f* aVertex, float 
    fEpsilon, bool bOwner, SEQuery::Type eQueryType)
    :
    SEConvexHullf(iVertexCount, fEpsilon, bOwner, eQueryType), 
    m_LineOrigin(SEVector2f::ZERO), 
    m_LineDirection(SEVector2f::ZERO)
{
    SE_ASSERT(aVertex);

    m_aVertex = aVertex;
    m_aSVertex = 0;
    m_pQuery = 0;

    SEMapper2f tempMapper(m_iVertexCount, m_aVertex, m_fEpsilon);
    if( tempMapper.GetDimension() == 0 )
    {
        // The values of m_iDimension,  m_aiIndex,  and m_aiAdjacent were
        // already initialized by the SEConvexHullf base class.
        return;
    }

    if( tempMapper.GetDimension() == 1 )
    {
        // The set is (nearly) collinear.  The caller is responsible for
        // creating a SEConvexHull1f object.
        m_iDimension = 1;
        m_LineOrigin = tempMapper.GetOrigin();
        m_LineDirection = tempMapper.GetDirection(0);
        return;
    }

    m_iDimension = 2;

    int i0 = tempMapper.GetExtremeIndex(0);
    int i1 = tempMapper.GetExtremeIndex(1);
    int i2 = tempMapper.GetExtremeIndex(2);

    m_aSVertex = SE_NEW SEVector2f[m_iVertexCount];
    int i;

    if( eQueryType != SEQuery::QT_RATIONAL && eQueryType != 
        SEQuery::QT_FILTERED )
    {
        // Transform the vertices to the square [0, 1]^2.
        SEVector2f vec2fMin = tempMapper.GetMin();
        float fScale = 1.0f / tempMapper.GetMaxRange();
        for( i = 0; i < m_iVertexCount; i++ )
        {
            m_aSVertex[i] = (m_aVertex[i] - vec2fMin)*fScale;
        }

        float fExpand;
        if( eQueryType == SEQuery::QT_INT64 )
        {
            // Scale the vertices to the square [0, 2^{20}]^2 to allow use of
            // 64-bit integers.
            fExpand = (float)(1 << 20);
            m_pQuery = SE_NEW SEQuery2Int64f(m_iVertexCount,  m_aSVertex);
        }
        else if( eQueryType == SEQuery::QT_INTEGER )
        {
            // Scale the vertices to the square [0, 2^{24}]^2 to allow use of
            // SETInteger.
            fExpand = (float)(1 << 24);
            m_pQuery = SE_NEW SEQuery2TIntegerf(m_iVertexCount,  m_aSVertex);
        }
        else  // eQueryType == SEQuery::QT_REAL
        {
            // No scaling for floating point.
            fExpand = 1.0f;
            m_pQuery = SE_NEW SEQuery2f(m_iVertexCount,  m_aSVertex);
        }

        for( i = 0; i < m_iVertexCount; i++ )
        {
            m_aSVertex[i] *= fExpand;
        }
    }
    else
    {
        // No transformation needed for exact rational arithmetic or filtered
        // predicates.
        size_t uiSize = m_iVertexCount * sizeof(SEVector2f);
        SESystem::Memcpy(m_aSVertex,  uiSize,  m_aVertex,  uiSize);

        if( eQueryType == SEQuery::QT_RATIONAL )
        {
            m_pQuery = SE_NEW SEQuery2TRationalf(m_iVertexCount,  m_aSVertex);
        }
        else // eQueryType == SEQuery::QT_FILTERED
        {
            m_pQuery = SE_NEW SEQuery2Filteredf(m_iVertexCount,  m_aSVertex,  
                m_fEpsilon);
        }
    }

    SEHullEdge2f* pE0;
    SEHullEdge2f* pE1;
    SEHullEdge2f* pE2;

    if( tempMapper.GetExtremeCCW() )
    {
        pE0 = SE_NEW SEHullEdge2f(i0,  i1);
        pE1 = SE_NEW SEHullEdge2f(i1,  i2);
        pE2 = SE_NEW SEHullEdge2f(i2,  i0);
    }
    else
    {
        pE0 = SE_NEW SEHullEdge2f(i0,  i2);
        pE1 = SE_NEW SEHullEdge2f(i2,  i1);
        pE2 = SE_NEW SEHullEdge2f(i1,  i0);
    }

    pE0->Insert(pE2,  pE1);
    pE1->Insert(pE0,  pE2);
    pE2->Insert(pE1,  pE0);

    SEHullEdge2f* pHull = pE0;
    for( i = 0; i < m_iVertexCount; i++ )
    {
        if( !Update(pHull,  i) )
        {
            pHull->DeleteAll();

            return;
        }
    }

    pHull->GetIndices(m_iSimplexCount,  m_aiIndex);
    pHull->DeleteAll();
}
//----------------------------------------------------------------------------
SEConvexHull2f::~SEConvexHull2f()
{
    if( m_bOwner )
    {
        SE_DELETE[] m_aVertex;
    }
    SE_DELETE[] m_aSVertex;
    SE_DELETE m_pQuery;
}
//----------------------------------------------------------------------------
const SEVector2f& SEConvexHull2f::GetLineOrigin() const
{
    return m_LineOrigin;
}
//----------------------------------------------------------------------------
const SEVector2f& SEConvexHull2f::GetLineDirection() const
{
    return m_LineDirection;
}
//----------------------------------------------------------------------------
SEConvexHull1f* SEConvexHull2f::GetConvexHull1() const
{
    SE_ASSERT( m_iDimension == 1 );

    if( m_iDimension != 1 )
    {
        return 0;
    }

    float* afProjection = SE_NEW float[m_iVertexCount];
    for( int i = 0; i < m_iVertexCount; i++ )
    {
        SEVector2f vec2fDiff = m_aVertex[i] - m_LineOrigin;
        afProjection[i] = m_LineDirection.Dot(vec2fDiff);
    }

    return SE_NEW SEConvexHull1f(m_iVertexCount,  afProjection, 
        m_fEpsilon,  true,  m_eQueryType);
}
//----------------------------------------------------------------------------
bool SEConvexHull2f::Update(SEHullEdge2f*& rpHull,  int i)
{
    // Locate an edge visible to the input point (if possible).
    SEHullEdge2f* pVisible = 0;
    SEHullEdge2f* pCurrent = rpHull;
    do
    {
        if( pCurrent->GetSign(i,  m_pQuery) > 0 )
        {
            pVisible = pCurrent;
            break;
        }

        pCurrent = pCurrent->A[1];
    }
    while( pCurrent != rpHull );

    if( !pVisible )
    {
        // The point is inside the current hull; nothing to do.
        return true;
    }

    // Remove the visible edges.
    SEHullEdge2f* pAdj0 = pVisible->A[0];
    SE_ASSERT( pAdj0 );

    if( !pAdj0 )
    {
        return false;
    }

    SEHullEdge2f* pAdj1 = pVisible->A[1];
    SE_ASSERT( pAdj1 );

    if( !pAdj1 )
    {
        return false;
    }

    pVisible->DeleteSelf();

    while( pAdj0->GetSign(i,  m_pQuery) > 0 )
    {
        rpHull = pAdj0;
        pAdj0 = pAdj0->A[0];
        SE_ASSERT( pAdj0 );

        if( !pAdj0 )
        {
            return false;
        }

        pAdj0->A[1]->DeleteSelf();
    }

    while( pAdj1->GetSign(i,  m_pQuery) > 0 )
    {
        rpHull = pAdj1;
        pAdj1 = pAdj1->A[1];
        SE_ASSERT( pAdj1 );

        if( !pAdj1 )
        {
            return false;
        }

        pAdj1->A[0]->DeleteSelf();
    }

    // Insert the new edges formed by the input point and the end points of
    // the polyline of invisible edges.
    SEHullEdge2f* pEdge0 = SE_NEW SEHullEdge2f(pAdj0->V[1],  i);
    SEHullEdge2f* pEdge1 = SE_NEW SEHullEdge2f(i,  pAdj1->V[0]);
    pEdge0->Insert(pAdj0,  pEdge1);
    pEdge1->Insert(pEdge0,  pAdj1);
    rpHull = pEdge0;

    return true;
}
//----------------------------------------------------------------------------
SEConvexHull2f::SEConvexHull2f(const char* acFilename)
    :
    SEConvexHullf(0,  0.0f,  false,  SEQuery::QT_REAL)
{
    m_aVertex = 0;
    m_aSVertex = 0;
    m_pQuery = 0;
    bool bLoaded = Load(acFilename);
    SE_ASSERT( bLoaded );
    (void)bLoaded;  // avoid warning in Release build
}
//----------------------------------------------------------------------------
bool SEConvexHull2f::Load(const char* acFilename)
{
    FILE* pIFile = SESystem::Fopen(acFilename,  "rb");
    if( !pIFile )
    {
        return false;
    }

    SEConvexHullf::Load(pIFile);

    SE_DELETE m_pQuery;
    SE_DELETE[] m_aSVertex;
    if( m_bOwner )
    {
        SE_DELETE[] m_aVertex;
    }

    m_bOwner = true;
    m_aVertex = SE_NEW SEVector2f[m_iVertexCount];
    m_aSVertex = SE_NEW SEVector2f[m_iVertexCount];

    size_t uiSize = sizeof(float);
    int iVC = 2*m_iVertexCount;
    if( uiSize == 4 )
    {
        SESystem::Read4le(pIFile,  iVC,  m_aVertex);
        SESystem::Read4le(pIFile,  iVC,  m_aSVertex);
        SESystem::Read4le(pIFile,  2,  (float*)m_LineOrigin);
        SESystem::Read4le(pIFile,  2,  (float*)m_LineDirection);
    }
    else // iSize == 8
    {
        SESystem::Read8le(pIFile,  iVC,  m_aVertex);
        SESystem::Read8le(pIFile,  iVC,  m_aSVertex);
        SESystem::Read8le(pIFile,  2,  (float*)m_LineOrigin);
        SESystem::Read8le(pIFile,  2,  (float*)m_LineDirection);
    }

    SESystem::Fclose(pIFile);

    switch( m_eQueryType )
    {
    case SEQuery::QT_INT64:
        m_pQuery = SE_NEW SEQuery2Int64f(m_iVertexCount,  m_aSVertex);

        break;
    case SEQuery::QT_INTEGER:
        m_pQuery = SE_NEW SEQuery2TIntegerf(m_iVertexCount,  m_aSVertex);

        break;
    case SEQuery::QT_RATIONAL:
        m_pQuery = SE_NEW SEQuery2TRationalf(m_iVertexCount,  m_aSVertex);

        break;
    case SEQuery::QT_REAL:
        m_pQuery = SE_NEW SEQuery2f(m_iVertexCount,  m_aSVertex);

        break;
    case SEQuery::QT_FILTERED:
        m_pQuery = SE_NEW SEQuery2Filteredf(m_iVertexCount,  m_aSVertex,  
            m_fEpsilon);

        break;
    }

    return true;
}
//----------------------------------------------------------------------------
bool SEConvexHull2f::Save(const char* acFilename) const
{
    FILE* pOFile = SESystem::Fopen(acFilename,  "wb");
    if( !pOFile )
    {
        return false;
    }

    SEConvexHullf::Save(pOFile);

    size_t uiSize = sizeof(float);
    int iVC = 2 * m_iVertexCount;
    if( uiSize == 4 )
    {
        SESystem::Write4le(pOFile,  iVC,  m_aVertex);
        SESystem::Write4le(pOFile,  iVC,  m_aSVertex);
        SESystem::Write4le(pOFile,  2,  (const float*)m_LineOrigin);
        SESystem::Write4le(pOFile,  2,  (const float*)m_LineDirection);
    }
    else // iSize == 8
    {
        SESystem::Write8le(pOFile,  iVC,  m_aVertex);
        SESystem::Write8le(pOFile,  iVC,  m_aSVertex);
        SESystem::Write8le(pOFile,  2,  (const float*)m_LineOrigin);
        SESystem::Write8le(pOFile,  2,  (const float*)m_LineDirection);
    }

    SESystem::Fclose(pOFile);

    return true;
}
//----------------------------------------------------------------------------