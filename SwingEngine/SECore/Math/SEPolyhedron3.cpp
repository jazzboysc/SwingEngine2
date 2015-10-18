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
#include "SEPolyhedron3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEPolyhedron3f::SEPolyhedron3f(int iVCount, SEVector3f* aVertex, int iTCount, 
    int* aiIndex, bool bOwner)
{
    // polyhedron必须至少是个四面体.
    SE_ASSERT( iVCount >= 4 && aVertex );
    SE_ASSERT( iTCount >= 4 && aiIndex );

    m_iVCount = iVCount;
    m_aVertex = aVertex;
    m_iTCount = iTCount;
    m_aiIndex = aiIndex;
    m_bOwner = bOwner;
}
//----------------------------------------------------------------------------
SEPolyhedron3f::SEPolyhedron3f(const SEPolyhedron3f& rPoly)
{
    m_aVertex = 0;
    m_aiIndex = 0;
    m_bOwner = false;
    *this = rPoly;
}
//----------------------------------------------------------------------------
SEPolyhedron3f::~SEPolyhedron3f()
{
    if( m_bOwner )
    {
        SE_DELETE[] m_aVertex;
        SE_DELETE[] m_aiIndex;
    }
}
//----------------------------------------------------------------------------
SEPolyhedron3f& SEPolyhedron3f::operator=(const SEPolyhedron3f& rPoly)
{
    if( m_bOwner )
    {
        SE_DELETE[] m_aVertex;
        SE_DELETE[] m_aiIndex;
    }

    m_iVCount = rPoly.m_iVCount;
    m_iTCount = rPoly.m_iTCount;
    m_bOwner = rPoly.m_bOwner;

    if( m_bOwner )
    {
        m_aVertex = SE_NEW SEVector3f[m_iVCount];
        size_t uiSize = m_iVCount * sizeof(SEVector3f);
        SESystem::Memcpy(m_aVertex, uiSize, rPoly.m_aVertex, uiSize);
        m_aiIndex = SE_NEW int[3 * m_iTCount];
        uiSize = 3 * m_iTCount * sizeof(int);
        SESystem::Memcpy(m_aiIndex, uiSize, rPoly.m_aiIndex, uiSize);
    }
    else
    {
        m_aVertex = rPoly.m_aVertex;
        m_aiIndex = rPoly.m_aiIndex;
    }

    return *this;
}
//----------------------------------------------------------------------------
int SEPolyhedron3f::GetVCount() const
{
    return m_iVCount;
}
//----------------------------------------------------------------------------
const SEVector3f* SEPolyhedron3f::GetVertices() const
{
    return m_aVertex;
}
//----------------------------------------------------------------------------
const SEVector3f& SEPolyhedron3f::GetVertex(int i) const
{
    SE_ASSERT( 0 <= i && i < m_iVCount );

    return m_aVertex[i];
}
//----------------------------------------------------------------------------
int SEPolyhedron3f::GetTCount() const
{
    return m_iTCount;
}
//----------------------------------------------------------------------------
const int* SEPolyhedron3f::GetIndices() const
{
    return m_aiIndex;
}
//----------------------------------------------------------------------------
const int* SEPolyhedron3f::GetTriangle(int i) const
{
    SE_ASSERT( 0 <= i && i < m_iTCount );

    return &m_aiIndex[3 * i];
}
//----------------------------------------------------------------------------
void SEPolyhedron3f::SetVertex(int i, const SEVector3f& rV)
{
    SE_ASSERT( 0 <= i && i < m_iVCount );

    m_aVertex[i] = rV;
}
//----------------------------------------------------------------------------
SEVector3f* SEPolyhedron3f::GetVertices()
{
    return m_aVertex;
}
//----------------------------------------------------------------------------
SEVector3f SEPolyhedron3f::ComputeVertexAverage() const
{
    SEVector3f vec3fAverage = m_aVertex[0];
    for( int i = 1; i < m_iVCount; i++ )
    {
        vec3fAverage += m_aVertex[i];
    }

    vec3fAverage /= (float)m_iVCount;

    return vec3fAverage;
}
//----------------------------------------------------------------------------
float SEPolyhedron3f::ComputeSurfaceArea() const
{
    float fSurfaceArea = 0.0f;

    const int* piIndex = m_aiIndex;
    for( int i = 0; i < m_iTCount; i++ )
    {
        int iV0 = *piIndex++;
        int iV1 = *piIndex++;
        int iV2 = *piIndex++;
        SEVector3f vec3fEdge0 = m_aVertex[iV1] - m_aVertex[iV0];
        SEVector3f vec3fEdge1 = m_aVertex[iV2] - m_aVertex[iV0];
        SEVector3f vec3fCross = vec3fEdge0.Cross(vec3fEdge1);
        fSurfaceArea += vec3fCross.GetLength();
    }

    fSurfaceArea *= 0.5f;

    return fSurfaceArea;
}
//----------------------------------------------------------------------------
float SEPolyhedron3f::ComputeVolume() const
{
    float fVolume = 0.0f;

    const int* piIndex = m_aiIndex;
    for( int i = 0; i < m_iTCount; i++ )
    {
        int iV0 = *piIndex++;
        int iV1 = *piIndex++;
        int iV2 = *piIndex++;
        fVolume +=
            m_aVertex[iV0].Dot(m_aVertex[iV1].Cross(m_aVertex[iV2]));
    }

    fVolume /= 6.0f;

    return fVolume;
}
//----------------------------------------------------------------------------
