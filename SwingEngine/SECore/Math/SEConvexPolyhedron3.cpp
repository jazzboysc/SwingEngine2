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
#include "SEConvexPolyhedron3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEConvexPolyhedron3f::SEConvexPolyhedron3f(int iVCount, SEVector3f* aVertex, 
    int iTCount, int* aiIndex, SEPlane3f* aPlane, bool bOwner)
    :
    SEPolyhedron3f(iVCount, aVertex, iTCount, aiIndex, bOwner)
{
    if( aPlane )
    {
        m_aPlane = aPlane;
        m_bPlaneOwner = bOwner;
    }
    else
    {
        m_aPlane = SE_NEW SEPlane3f[m_iTCount];
        m_bPlaneOwner = true;
        UpdatePlanes();
    }
}
//----------------------------------------------------------------------------
SEConvexPolyhedron3f::SEConvexPolyhedron3f(const SEConvexPolyhedron3f& rPoly)
    :
    SEPolyhedron3f(rPoly)
{
    m_aPlane = 0;
    m_bPlaneOwner = false;
    *this = rPoly;
}
//----------------------------------------------------------------------------
SEConvexPolyhedron3f::~SEConvexPolyhedron3f()
{
    if( m_bPlaneOwner )
    {
        SE_DELETE[] m_aPlane;
    }
}
//----------------------------------------------------------------------------
SEConvexPolyhedron3f& SEConvexPolyhedron3f::operator=(
    const SEConvexPolyhedron3f& rPoly)
{
    SEPolyhedron3f::operator=(rPoly);

    if( m_bPlaneOwner )
    {
        SE_DELETE[] m_aPlane;
    }

    m_bPlaneOwner = rPoly.m_bPlaneOwner;

    if( m_bPlaneOwner )
    {
        m_aPlane = SE_NEW SEPlane3f[m_iTCount];
        size_t uiSize = m_iTCount * sizeof(SEPlane3f);
        SESystem::Memcpy(m_aPlane, uiSize, rPoly.m_aPlane, uiSize);
    }
    else
    {
        m_aPlane = rPoly.m_aPlane;
    }

    m_TModified = rPoly.m_TModified;

    return *this;
}
//----------------------------------------------------------------------------
const SEPlane3f* SEConvexPolyhedron3f::GetPlanes() const
{
    return m_aPlane;
}
//----------------------------------------------------------------------------
const SEPlane3f& SEConvexPolyhedron3f::GetPlane(int i) const
{
    SE_ASSERT( 0 <= i && i < m_iTCount );

    return m_aPlane[i];
}
//----------------------------------------------------------------------------
void SEConvexPolyhedron3f::SetVertex(int i, const SEVector3f& rV)
{
    SEPolyhedron3f::SetVertex(i, rV);

    const int* piIndex = m_aiIndex;
    for( int j = 0; j < m_iTCount; j++ )
    {
        int iV0 = *piIndex++;
        int iV1 = *piIndex++;
        int iV2 = *piIndex++;
        if( i == iV0 || i == iV1 || i == iV2 )
        {
            m_TModified.insert(j);
        }
    }
}
//----------------------------------------------------------------------------
void SEConvexPolyhedron3f::UpdatePlane(int i, const SEVector3f& rAverage)
{
    int iBase = 3 * i;
    int iV0 = m_aiIndex[iBase++];
    int iV1 = m_aiIndex[iBase++];
    int iV2 = m_aiIndex[iBase];

    SEVector3f& rV0 = m_aVertex[iV0];
    SEVector3f& rV1 = m_aVertex[iV1];
    SEVector3f& rV2 = m_aVertex[iV2];

    SEVector3f vec3fDiff = rAverage - rV0;
    SEVector3f vec3fE1 = rV1 - rV0;
    SEVector3f vec3fE2 = rV2 - rV0;
    SEVector3f vec3fNormal = vec3fE2.Cross(vec3fE1);
    float fLength = vec3fNormal.GetLength();
    if( fLength > SEMathf::ZERO_TOLERANCE )
    {
        vec3fNormal /= fLength;
        float fDot = vec3fNormal.Dot(vec3fDiff);
        SE_ASSERT( fDot >= 0.0f );

        if( fDot < 0.0f )
        {
            vec3fNormal = -vec3fNormal;
        }
    }
    else
    {
        // 三角面是退化的.  
        // 用一个指向平均顶点中心的向量代替法线.
        vec3fNormal = vec3fDiff;
        vec3fNormal.Normalize();
    }

    // 法线指向多面体内部.
    m_aPlane[i] = SEPlane3f(vec3fNormal, vec3fNormal.Dot(rV0));
}
//----------------------------------------------------------------------------
void SEConvexPolyhedron3f::UpdatePlanes()
{
    SEVector3f vec3fAverage = this->ComputeVertexAverage();
    int i;

    if( m_TModified.empty() )
    {
        for( i = 0; i < m_iTCount; i++ )
        {
            UpdatePlane(i, vec3fAverage);
        }
    }
    else
    {
        std::set<int>::iterator tempIter = m_TModified.begin();
        for( /**/; tempIter != m_TModified.end(); tempIter++ )
        {
            i = *tempIter;
            UpdatePlane(i, vec3fAverage);
        }
    }
}
//----------------------------------------------------------------------------
bool SEConvexPolyhedron3f::IsConvex(float fThreshold) const
{
    float fMax = -SEMathf::MAX_REAL, fMin = SEMathf::MAX_REAL;
    for( int j = 0; j < m_iTCount; j++ )
    {
        const SEPlane3f& rPlane = m_aPlane[j];
        for( int i = 0; i < m_iVCount; i++ )
        {
            float fDistance = rPlane.GetDistance(m_aVertex[i]);
            if( fDistance < fMin )
            {
                fMin = fDistance;
            }
            if( fDistance > fMax )
            {
                fMax = fDistance;
            }
            if( fDistance < fThreshold )
            {
                return false;
            }
        }
    }

    return true;
}
//----------------------------------------------------------------------------
bool SEConvexPolyhedron3f::ContainsPoint(const SEVector3f& rP, 
    float fThreshold) const
{
    for( int i = 0; i < m_iTCount; i++ )
    {
        float fDistance = m_aPlane[i].GetDistance(rP);
        if( fDistance < fThreshold )
        {
            return false;
        }
    }

    return true;
}
//----------------------------------------------------------------------------
