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
#include "SEConvexHull3.h"
#include "SEMapper3.h"
#include "SEQuery3Filtered.h"
#include "SEQuery3Int64.h"
#include "SEQuery3TInteger.h"
#include "SEQuery3TRational.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEConvexHull3f::SEConvexHull3f(int iVertexCount,  SEVector3f* aVertex, 
    float fEpsilon,  bool bOwner,  SEQuery::Type eQueryType)
    :
    SEConvexHullf(iVertexCount,  fEpsilon,  bOwner,  eQueryType), 
    m_LineOrigin(SEVector3f::ZERO), 
    m_LineDirection(SEVector3f::ZERO), 
    m_PlaneOrigin(SEVector3f::ZERO)
{
    SE_ASSERT( aVertex );

    m_aVertex = aVertex;
    m_aPlaneDirection[0] = SEVector3f::ZERO;
    m_aPlaneDirection[1] = SEVector3f::ZERO;
    m_aSVertex = 0;
    m_pQuery = 0;

    SEMapper3f tempMapper(m_iVertexCount,  m_aVertex,  m_fEpsilon);
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

    if( tempMapper.GetDimension() == 2 )
    {
        // The set is (nearly) coplanar.  The caller is responsible for
        // creating a Delaunay2 object.
        m_iDimension = 2;
        m_PlaneOrigin = tempMapper.GetOrigin();
        m_aPlaneDirection[0] = tempMapper.GetDirection(0);
        m_aPlaneDirection[1] = tempMapper.GetDirection(1);

        return;
    }

    m_iDimension = 3;

    int i0 = tempMapper.GetExtremeIndex(0);
    int i1 = tempMapper.GetExtremeIndex(1);
    int i2 = tempMapper.GetExtremeIndex(2);
    int i3 = tempMapper.GetExtremeIndex(3);

    m_aSVertex = SE_NEW SEVector3f[m_iVertexCount];
    int i;

    if( eQueryType != SEQuery::QT_RATIONAL && eQueryType != 
        SEQuery::QT_FILTERED )
    {
        // Transform the vertices to the cube [0, 1]^3.
        SEVector3f vec3fMin = tempMapper.GetMin();
        float fScale = 1.0f / tempMapper.GetMaxRange();
        for( i = 0; i < m_iVertexCount; i++ )
        {
            m_aSVertex[i] = (m_aVertex[i] - vec3fMin)*fScale;
        }

        float fExpand;
        if( eQueryType == SEQuery::QT_INT64 )
        {
            // Scale the vertices to the square [0, 2^{20}]^2 to allow use of
            // 64-bit integers.
            fExpand = (float)(1 << 20);
            m_pQuery = SE_NEW SEQuery3Int64f(m_iVertexCount,  m_aSVertex);
        }
        else if( eQueryType == SEQuery::QT_INTEGER )
        {
            // Scale the vertices to the square [0, 2^{24}]^2 to allow use of
            // SETInteger.
            fExpand = (float)(1 << 24);
            m_pQuery = SE_NEW SEQuery3TIntegerf(m_iVertexCount,  m_aSVertex);
        }
        else  // eQueryType == SEQuery::QT_REAL
        {
            // No scaling for floating point.
            fExpand = 1.0f;
            m_pQuery = SE_NEW SEQuery3f(m_iVertexCount,  m_aSVertex);
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
        size_t uiSize = m_iVertexCount * sizeof(SEVector3f);
        SESystem::Memcpy(m_aSVertex,  uiSize,  m_aVertex,  uiSize);

        if( eQueryType == SEQuery::QT_RATIONAL )
        {
            m_pQuery = SE_NEW SEQuery3TRationalf(m_iVertexCount, m_aSVertex);
        }
        else // eQueryType == SEQuery::QT_FILTERED
        {
            m_pQuery = SE_NEW SEQuery3Filteredf(m_iVertexCount,  m_aSVertex,  
                m_fEpsilon);
        }
    }

    SEHullTriangle3f* pT0;
    SEHullTriangle3f* pT1;
    SEHullTriangle3f* pT2;
    SEHullTriangle3f* pT3;

    if( tempMapper.GetExtremeCCW() )
    {
        pT0 = SE_NEW SEHullTriangle3f(i0,  i1,  i3);
        pT1 = SE_NEW SEHullTriangle3f(i0,  i2,  i1);
        pT2 = SE_NEW SEHullTriangle3f(i0,  i3,  i2);
        pT3 = SE_NEW SEHullTriangle3f(i1,  i2,  i3);
        pT0->AttachTo(pT1,  pT3,  pT2);
        pT1->AttachTo(pT2,  pT3,  pT0);
        pT2->AttachTo(pT0,  pT3,  pT1);
        pT3->AttachTo(pT1,  pT2,  pT0);
    }
    else
    {
        pT0 = SE_NEW SEHullTriangle3f(i0,  i3,  i1);
        pT1 = SE_NEW SEHullTriangle3f(i0,  i1,  i2);
        pT2 = SE_NEW SEHullTriangle3f(i0,  i2,  i3);
        pT3 = SE_NEW SEHullTriangle3f(i1,  i3,  i2);
        pT0->AttachTo(pT2,  pT3,  pT1);
        pT1->AttachTo(pT0,  pT3,  pT2);
        pT2->AttachTo(pT1,  pT3,  pT0);
        pT3->AttachTo(pT0,  pT2,  pT1);
    }

    m_Hull.clear();
    m_Hull.insert(pT0);
    m_Hull.insert(pT1);
    m_Hull.insert(pT2);
    m_Hull.insert(pT3);

    for( i = 0; i < m_iVertexCount; i++ )
    {
        if( !Update(i) )
        {
            DeleteHull();
            return;
        }
    }

    ExtractIndices();
}
//----------------------------------------------------------------------------
SEConvexHull3f::~SEConvexHull3f()
{
    if( m_bOwner )
    {
        SE_DELETE[] m_aVertex;
    }
    SE_DELETE[] m_aSVertex;
    SE_DELETE m_pQuery;
}
//----------------------------------------------------------------------------
const SEVector3f& SEConvexHull3f::GetLineOrigin() const
{
    return m_LineOrigin;
}
//----------------------------------------------------------------------------
const SEVector3f& SEConvexHull3f::GetLineDirection() const
{
    return m_LineDirection;
}
//----------------------------------------------------------------------------
const SEVector3f& SEConvexHull3f::GetPlaneOrigin() const
{
    return m_PlaneOrigin;
}
//----------------------------------------------------------------------------
const SEVector3f& SEConvexHull3f::GetPlaneDirection(int i) const
{
    SE_ASSERT( 0 <= i && i < 2 );

    return m_aPlaneDirection[i];
}
//----------------------------------------------------------------------------
SEConvexHull1f* SEConvexHull3f::GetConvexHull1() const
{
    SE_ASSERT( m_iDimension == 1 );

    if( m_iDimension != 1 )
    {
        return 0;
    }

    float* afProjection = SE_NEW float[m_iVertexCount];
    for( int i = 0; i < m_iVertexCount; i++ )
    {
        SEVector3f vec3fDiff = m_aVertex[i] - m_LineOrigin;
        afProjection[i] = m_LineDirection.Dot(vec3fDiff);
    }

    return SE_NEW SEConvexHull1f(m_iVertexCount,  afProjection, 
        m_fEpsilon,  true,  m_eQueryType);
}
//----------------------------------------------------------------------------
SEConvexHull2f* SEConvexHull3f::GetConvexHull2() const
{
    SE_ASSERT( m_iDimension == 2 );

    if( m_iDimension != 2 )
    {
        return 0;
    }

    SEVector2f* aProjection = SE_NEW SEVector2f[m_iVertexCount];
    for( int i = 0; i < m_iVertexCount; i++ )
    {
        SEVector3f vec3fDiff = m_aVertex[i] - m_PlaneOrigin;
        aProjection[i][0] = m_aPlaneDirection[0].Dot(vec3fDiff);
        aProjection[i][1] = m_aPlaneDirection[1].Dot(vec3fDiff);
    }

    return SE_NEW SEConvexHull2f(m_iVertexCount,  aProjection, 
        m_fEpsilon,  true,  m_eQueryType);
}
//----------------------------------------------------------------------------
bool SEConvexHull3f::Update(int i)
{
    // Locate a triangle visible to the input point (if possible).
    SEHullTriangle3f* pVisible = 0;
    SEHullTriangle3f* pTri;
    std::set<SEHullTriangle3f*>::iterator tempIter;
    for( tempIter = m_Hull.begin(); tempIter != m_Hull.end(); tempIter++ )
    {
        pTri = *tempIter;
        if( pTri->GetSign(i,  m_pQuery) > 0 )
        {
            pVisible = pTri;
            break;
        }
    }

    if( !pVisible )
    {
        // The point is inside the current hull; nothing to do.
        return true;
    }

    // Locate and remove the visible triangles.
    std::stack<SEHullTriangle3f*> tempVisible;
    std::map<int,  SETerminatorData> tempTerminator;
    tempVisible.push(pVisible);
    pVisible->OnStack = true;
    int j,  iV0,  iV1;
    while( !tempVisible.empty() )
    {
        pTri = tempVisible.top();
        tempVisible.pop();
        pTri->OnStack = false;
        for( j = 0; j < 3; j++ )
        {
            SEHullTriangle3f* pAdj = pTri->A[j];
            if( pAdj )
            {
                // Detach triangle and adjacent triangle from each other.
                int iNullIndex = pTri->DetachFrom(j,  pAdj);

                if( pAdj->GetSign(i,  m_pQuery) > 0 )
                {
                    if( !pAdj->OnStack )
                    {
                        // Adjacent triangle is visible.
                        tempVisible.push(pAdj);
                        pAdj->OnStack = true;
                    }
                }
                else
                {
                    // Adjacent triangle is invisible.
                    iV0 = pTri->V[j];
                    iV1 = pTri->V[(j + 1) % 3];
                    tempTerminator[iV0] = SETerminatorData(iV0, iV1, 
                        iNullIndex, pAdj);
                }
            }
        }
        m_Hull.erase(pTri);
        SE_DELETE pTri;
    }

    // Insert the new edges formed by the input point and the terminator
    // between visible and invisible triangles.
    int iSize = (int)tempTerminator.size();
    SE_ASSERT( iSize >= 3 );

    std::map<int,  SETerminatorData>::iterator pEdge =
        tempTerminator.begin();
    iV0 = pEdge->second.V[0];
    iV1 = pEdge->second.V[1];
    pTri = SE_NEW SEHullTriangle3f(i,  iV0,  iV1);
    m_Hull.insert(pTri);

    // save information for linking first/last inserted new triangles
    int iSaveV0 = pEdge->second.V[0];
    SEHullTriangle3f* pSaveTri = pTri;

    // establish adjacency links across terminator edge
    pTri->A[1] = pEdge->second.Tri;
    pEdge->second.Tri->A[pEdge->second.NullIndex] = pTri;
    for( j = 1; j < iSize; j++ )
    {
        pEdge = tempTerminator.find(iV1);
        SE_ASSERT( pEdge != tempTerminator.end() );

        iV0 = iV1;
        iV1 = pEdge->second.V[1];
        SEHullTriangle3f* pNext = SE_NEW SEHullTriangle3f(i,  iV0,  iV1);
        m_Hull.insert(pNext);

        // establish adjacency links across terminator edge
        pNext->A[1] = pEdge->second.Tri;
        pEdge->second.Tri->A[pEdge->second.NullIndex] = pNext;

        // establish adjacency links with previously inserted triangle
        pNext->A[0] = pTri;
        pTri->A[2] = pNext;

        pTri = pNext;
    }
    SE_ASSERT( iV1 == iSaveV0 );
    (void)iSaveV0;  // avoid warning in Release build

    // establish adjacency links between first/last triangles
    pSaveTri->A[0] = pTri;
    pTri->A[2] = pSaveTri;

    return true;
}
//----------------------------------------------------------------------------
void SEConvexHull3f::ExtractIndices()
{
    int iTCount = (int)m_Hull.size();
    m_iSimplexCount = iTCount;
    m_aiIndex = SE_NEW int[3 * m_iSimplexCount];

    std::set<SEHullTriangle3f*>::iterator tempIter;
    int i = 0;
    for( tempIter = m_Hull.begin(); tempIter != m_Hull.end(); tempIter++ )
    {
        SEHullTriangle3f* pTri = *tempIter;
        for( int j = 0; j < 3; j++ )
        {
            m_aiIndex[i++] = pTri->V[j];
        }
        SE_DELETE pTri;
    }
    m_Hull.clear();
}
//----------------------------------------------------------------------------
void SEConvexHull3f::DeleteHull()
{
    std::set<SEHullTriangle3f*>::iterator tempIter;
    for( tempIter = m_Hull.begin(); tempIter != m_Hull.end(); tempIter++ )
    {
        SEHullTriangle3f* pTri = *tempIter;
        SE_DELETE pTri;
    }
    m_Hull.clear();
}
//----------------------------------------------------------------------------
SEConvexHull3f::SEConvexHull3f(const char* acFilename)
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
bool SEConvexHull3f::Load(const char* acFilename)
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
    m_aVertex = SE_NEW SEVector3f[m_iVertexCount];
    m_aSVertex = SE_NEW SEVector3f[m_iVertexCount + 4];

    size_t uiSize = sizeof(float);
    int iVC = 3 * m_iVertexCount;
    if( uiSize == 4 )
    {
        SESystem::Read4le(pIFile,  iVC,  m_aVertex);
        SESystem::Read4le(pIFile,  iVC,  m_aSVertex);
        SESystem::Read4le(pIFile,  3,  (float*)m_LineOrigin);
        SESystem::Read4le(pIFile,  3,  (float*)m_LineDirection);
        SESystem::Read4le(pIFile,  3,  (float*)m_PlaneOrigin);
        SESystem::Read4le(pIFile,  3,  (float*)m_aPlaneDirection[0]);
        SESystem::Read4le(pIFile,  3,  (float*)m_aPlaneDirection[1]);
    }
    else // iSize == 8
    {
        SESystem::Read8le(pIFile,  iVC,  m_aVertex);
        SESystem::Read8le(pIFile,  iVC,  m_aSVertex);
        SESystem::Read8le(pIFile,  3,  (float*)m_LineOrigin);
        SESystem::Read8le(pIFile,  3,  (float*)m_LineDirection);
        SESystem::Read8le(pIFile,  3,  (float*)m_PlaneOrigin);
        SESystem::Read8le(pIFile,  3,  (float*)m_aPlaneDirection[0]);
        SESystem::Read8le(pIFile,  3,  (float*)m_aPlaneDirection[1]);
    }

    SESystem::Fclose(pIFile);

    switch( m_eQueryType )
    {
    case SEQuery::QT_INT64:
        m_pQuery = SE_NEW SEQuery3Int64f(m_iVertexCount,  m_aSVertex);

        break;
    case SEQuery::QT_INTEGER:
        m_pQuery = SE_NEW SEQuery3TIntegerf(m_iVertexCount,  m_aSVertex);

        break;
    case SEQuery::QT_RATIONAL:
        m_pQuery = SE_NEW SEQuery3TRationalf(m_iVertexCount,  m_aSVertex);

        break;
    case SEQuery::QT_REAL:
        m_pQuery = SE_NEW SEQuery3f(m_iVertexCount,  m_aSVertex);

        break;
    case SEQuery::QT_FILTERED:
        m_pQuery = SE_NEW SEQuery3Filteredf(m_iVertexCount,  m_aSVertex,  
            m_fEpsilon);

        break;
    }

    return true;
}
//----------------------------------------------------------------------------
bool SEConvexHull3f::Save(const char* acFilename) const
{
    FILE* pOFile = SESystem::Fopen(acFilename,  "wb");
    if( !pOFile )
    {
        return false;
    }

    SEConvexHullf::Save(pOFile);

    size_t uiSize = sizeof(float);
    int iVC = 3 * m_iVertexCount;
    if( uiSize == 4 )
    {
        SESystem::Write4le(pOFile, iVC, m_aVertex);
        SESystem::Write4le(pOFile, iVC, m_aSVertex);
        SESystem::Write4le(pOFile, 3, (const float*)m_LineOrigin);
        SESystem::Write4le(pOFile, 3, (const float*)m_LineDirection);
        SESystem::Write4le(pOFile, 3, (const float*)m_PlaneOrigin);
        SESystem::Write4le(pOFile, 3, (const float*)m_aPlaneDirection[0]);
        SESystem::Write4le(pOFile, 3, (const float*)m_aPlaneDirection[1]);
    }
    else // iSize == 8
    {
        SESystem::Write8le(pOFile, iVC, m_aVertex);
        SESystem::Write8le(pOFile, iVC, m_aSVertex);
        SESystem::Write8le(pOFile, 3, (const float*)m_LineOrigin);
        SESystem::Write8le(pOFile, 3, (const float*)m_LineDirection);
        SESystem::Write8le(pOFile, 3, (const float*)m_PlaneOrigin);
        SESystem::Write8le(pOFile, 3, (const float*)m_aPlaneDirection[0]);
        SESystem::Write8le(pOFile, 3, (const float*)m_aPlaneDirection[1]);
    }

    SESystem::Fclose(pOFile);

    return true;
}
//----------------------------------------------------------------------------