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
#include "SEDistVector3Frustum3.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEDistVector3Frustum3f::SEDistVector3Frustum3f(const SEVector3f& rVector,
    const SEFrustum3f& rFrustum)
    :
    m_pVector(&rVector),
    m_pFrustum(&rFrustum)
{
}
//----------------------------------------------------------------------------
const SEVector3f& SEDistVector3Frustum3f::GetVector() const
{
    return *m_pVector;
}
//----------------------------------------------------------------------------
const SEFrustum3f& SEDistVector3Frustum3f::GetFrustum() const
{
    return *m_pFrustum;
}
//----------------------------------------------------------------------------
float SEDistVector3Frustum3f::Get()
{
    return SEMath<float>::Sqrt(GetSquared());
}
//----------------------------------------------------------------------------
float SEDistVector3Frustum3f::GetSquared()
{
    // 计算顶点在截投体坐标体系下的坐标.
    SEVector3f vec3fDiff = *m_pVector - m_pFrustum->Origin;
    SEVector3f vec3fTest = SEVector3f(
        vec3fDiff.Dot(m_pFrustum->RVector),
        vec3fDiff.Dot(m_pFrustum->UVector),
        vec3fDiff.Dot(m_pFrustum->DVector));

    // perform calculations in octant with nonnegative R and U coordinates
    bool bRSignChange;
    if( vec3fTest.X < 0.0f )
    {
        bRSignChange = true;
        vec3fTest.X = -vec3fTest.X;
    }
    else
    {
        bRSignChange = false;
    }

    bool bUSignChange;
    if( vec3fTest.Y < 0.0f )
    {
        bUSignChange = true;
        vec3fTest.Y = -vec3fTest.Y;
    }
    else
    {
        bUSignChange = false;
    }

    // frustum derived parameters
    float fRMin = m_pFrustum->RBound;
    float fRMax = m_pFrustum->GetDRatio()*fRMin;
    float fUMin = m_pFrustum->UBound;
    float fUMax = m_pFrustum->GetDRatio()*fUMin;
    float fDMin = m_pFrustum->DMin;
    float fDMax = m_pFrustum->DMax;
    float fRMinSqr = fRMin*fRMin;
    float fUMinSqr = fUMin*fUMin;
    float fDMinSqr = fDMin*fDMin;
    float fMinRDDot = fRMinSqr + fDMinSqr;
    float fMinUDDot = fUMinSqr + fDMinSqr;
    float fMinRUDDot = fRMinSqr + fMinUDDot;
    float fMaxRDDot = m_pFrustum->GetDRatio()*fMinRDDot;
    float fMaxUDDot = m_pFrustum->GetDRatio()*fMinUDDot;
    float fMaxRUDDot = m_pFrustum->GetDRatio()*fMinRUDDot;

    // Algorithm computes closest point in all cases by determining in which
    // Voronoi region of the vertices, edges, and faces of the frustum that
    // the test point lives.
    SEVector3f vec3fClosest;
    float fRDot, fUDot, fRDDot, fUDDot, fRUDDot, fREdgeDot, fUEdgeDot, fT;
    if( vec3fTest.Z >= fDMax )
    {
        if( vec3fTest.X <= fRMax )
        {
            if( vec3fTest.Y <= fUMax )
            {
                // F-face
                vec3fClosest.X = vec3fTest.X;
                vec3fClosest.Y = vec3fTest.Y;
                vec3fClosest.Z = fDMax;
            }
            else
            {
                // UF-edge
                vec3fClosest.X = vec3fTest.X;
                vec3fClosest.Y = fUMax;
                vec3fClosest.Z = fDMax;
            }
        }
        else
        {
            if( vec3fTest.Y <= fUMax )
            {
                // LF-edge
                vec3fClosest.X = fRMax;
                vec3fClosest.Y = vec3fTest.Y;
                vec3fClosest.Z = fDMax;
            }
            else
            {
                // LUF-vertex
                vec3fClosest.X = fRMax;
                vec3fClosest.Y = fUMax;
                vec3fClosest.Z = fDMax;
            }
        }
    }
    else if( vec3fTest.Z <= fDMin )
    {
        if( vec3fTest.X <= fRMin )
        {
            if( vec3fTest.Y <= fUMin )
            {
                // N-face
                vec3fClosest.X = vec3fTest.X;
                vec3fClosest.Y = vec3fTest.Y;
                vec3fClosest.Z = fDMin;
            }
            else
            {
                fUDDot = fUMin*vec3fTest.Y + fDMin*vec3fTest.Z;
                if( fUDDot >= fMaxUDDot )
                {
                    // UF-edge
                    vec3fClosest.X = vec3fTest.X;
                    vec3fClosest.Y = fUMax;
                    vec3fClosest.Z = fDMax;
                }
                else if( fUDDot >= fMinUDDot )
                {
                    // U-face
                    fUDot = fDMin*vec3fTest.Y - fUMin*vec3fTest.Z;
                    fT = fUDot/fMinUDDot;
                    vec3fClosest.X = vec3fTest.X;
                    vec3fClosest.Y = vec3fTest.Y - fT*fDMin;
                    vec3fClosest.Z = vec3fTest.Z + fT*fUMin;
                }
                else
                {
                    // UN-edge
                    vec3fClosest.X = vec3fTest.X;
                    vec3fClosest.Y = fUMin;
                    vec3fClosest.Z = fDMin;
                }
            }
        }
        else
        {
            if( vec3fTest.Y <= fUMin )
            {
                fRDDot = fRMin*vec3fTest.X + fDMin*vec3fTest.Z;
                if( fRDDot >= fMaxRDDot )
                {
                    // LF-edge
                    vec3fClosest.X = fRMax;
                    vec3fClosest.Y = vec3fTest.Y;
                    vec3fClosest.Z = fDMax;
                }
                else if( fRDDot >= fMinRDDot )
                {
                    // L-face
                    fRDot = fDMin*vec3fTest.X - fRMin*vec3fTest.Z;
                    fT = fRDot/fMinRDDot;
                    vec3fClosest.X = vec3fTest.X - fT*fDMin;
                    vec3fClosest.Y = vec3fTest.Y;
                    vec3fClosest.Z = vec3fTest.Z + fT*fRMin;
                }
                else
                {
                    // LN-edge
                    vec3fClosest.X = fRMin;
                    vec3fClosest.Y = vec3fTest.Y;
                    vec3fClosest.Z = fDMin;
                }
            }
            else
            {
                fRUDDot = fRMin*vec3fTest.X + fUMin*vec3fTest.Y + 
                    fDMin*vec3fTest.Z;
                fREdgeDot = fUMin*fRUDDot - fMinRUDDot*vec3fTest.Y;
                if( fREdgeDot >= 0.0f )
                {
                    fRDDot = fRMin*vec3fTest.X + fDMin*vec3fTest.Z;
                    if( fRDDot >= fMaxRDDot )
                    {
                        // LF-edge
                        vec3fClosest.X = fRMax;
                        vec3fClosest.Y = vec3fTest.Y;
                        vec3fClosest.Z = fDMax;
                    }
                    else if( fRDDot >= fMinRDDot )
                    {
                        // L-face
                        fRDot = fDMin*vec3fTest.X - fRMin*vec3fTest.Z;
                        fT = fRDot/fMinRDDot;
                        vec3fClosest.X = vec3fTest.X - fT*fDMin;
                        vec3fClosest.Y = vec3fTest.Y;
                        vec3fClosest.Z = vec3fTest.Z + fT*fRMin;
                    }
                    else
                    {
                        // LN-edge
                        vec3fClosest.X = fRMin;
                        vec3fClosest.Y = vec3fTest.Y;
                        vec3fClosest.Z = fDMin;
                    }
                }
                else
                {
                    fUEdgeDot = fRMin*fRUDDot - fMinRUDDot*vec3fTest.X;
                    if( fUEdgeDot >= 0.0f )
                    {
                        fUDDot = fUMin*vec3fTest.Y + fDMin*vec3fTest.Z;
                        if( fUDDot >= fMaxUDDot )
                        {
                            // UF-edge
                            vec3fClosest.X = vec3fTest.X;
                            vec3fClosest.Y = fUMax;
                            vec3fClosest.Z = fDMax;
                        }
                        else if( fUDDot >= fMinUDDot )
                        {
                            // U-face
                            fUDot = fDMin*vec3fTest.Y - fUMin*vec3fTest.Z;
                            fT = fUDot/fMinUDDot;
                            vec3fClosest.X = vec3fTest.X;
                            vec3fClosest.Y = vec3fTest.Y - fT*fDMin;
                            vec3fClosest.Z = vec3fTest.Z + fT*fUMin;
                        }
                        else
                        {
                            // UN-edge
                            vec3fClosest.X = vec3fTest.X;
                            vec3fClosest.Y = fUMin;
                            vec3fClosest.Z = fDMin;
                        }
                    }
                    else
                    {
                        if( fRUDDot >= fMaxRUDDot )
                        {
                            // LUF-vertex
                            vec3fClosest.X = fRMax;
                            vec3fClosest.Y = fUMax;
                            vec3fClosest.Z = fDMax;
                        }
                        else if( fRUDDot >= fMinRUDDot )
                        {
                            // LU-edge
                            fT = fRUDDot/fMinRUDDot;
                            vec3fClosest.X = fT*fRMin;
                            vec3fClosest.Y = fT*fUMin;
                            vec3fClosest.Z = fT*fDMin;
                        }
                        else
                        {
                            // LUN-vertex
                            vec3fClosest.X = fRMin;
                            vec3fClosest.Y = fUMin;
                            vec3fClosest.Z = fDMin;
                        }
                    }
                }
            }
        }
    }
    else
    {
        fRDot = fDMin*vec3fTest.X - fRMin*vec3fTest.Z;
        fUDot = fDMin*vec3fTest.Y - fUMin*vec3fTest.Z;
        if( fRDot <= 0.0f )
        {
            if( fUDot <= 0.0f )
            {
                // point inside frustum
                vec3fClosest = vec3fTest;
            }
            else
            {
                fUDDot = fUMin*vec3fTest.Y + fDMin*vec3fTest.Z;
                if( fUDDot >= fMaxUDDot )
                {
                    // UF-edge
                    vec3fClosest.X = vec3fTest.X;
                    vec3fClosest.Y = fUMax;
                    vec3fClosest.Z = fDMax;
                }
                else
                {
                    // U-face
                    fUDot = fDMin*vec3fTest.Y - fUMin*vec3fTest.Z;
                    fT = fUDot/fMinUDDot;
                    vec3fClosest.X = vec3fTest.X;
                    vec3fClosest.Y = vec3fTest.Y - fT*fDMin;
                    vec3fClosest.Z = vec3fTest.Z + fT*fUMin;
                }
            }
        }
        else
        {
            if( fUDot <= 0.0f )
            {
                fRDDot = fRMin*vec3fTest.X + fDMin*vec3fTest.Z;
                if( fRDDot >= fMaxRDDot )
                {
                    // LF-edge
                    vec3fClosest.X = fRMax;
                    vec3fClosest.Y = vec3fTest.Y;
                    vec3fClosest.Z = fDMax;
                }
                else
                {
                    // L-face
                    fRDot = fDMin*vec3fTest.X - fRMin*vec3fTest.Z;
                    fT = fRDot/fMinRDDot;
                    vec3fClosest.X = vec3fTest.X - fT*fDMin;
                    vec3fClosest.Y = vec3fTest.Y;
                    vec3fClosest.Z = vec3fTest.Z + fT*fRMin;
                }
            }
            else
            {
                fRUDDot = fRMin*vec3fTest.X + fUMin*vec3fTest.Y + 
                    fDMin*vec3fTest.Z;
                fREdgeDot = fUMin*fRUDDot - fMinRUDDot*vec3fTest.Y;
                if( fREdgeDot >= 0.0f )
                {
                    fRDDot = fRMin*vec3fTest.X + fDMin*vec3fTest.Z;
                    if( fRDDot >= fMaxRDDot )
                    {
                        // LF-edge
                        vec3fClosest.X = fRMax;
                        vec3fClosest.Y = vec3fTest.Y;
                        vec3fClosest.Z = fDMax;
                    }
                    else // assert( fRDDot >= fMinRDDot ) from geometry
                    {
                        // L-face
                        fRDot = fDMin*vec3fTest.X - fRMin*vec3fTest.Z;
                        fT = fRDot/fMinRDDot;
                        vec3fClosest.X = vec3fTest.X - fT*fDMin;
                        vec3fClosest.Y = vec3fTest.Y;
                        vec3fClosest.Z = vec3fTest.Z + fT*fRMin;
                    }
                }
                else
                {
                    fUEdgeDot = fRMin*fRUDDot - fMinRUDDot*vec3fTest.X;
                    if( fUEdgeDot >= 0.0f )
                    {
                        fUDDot = fUMin*vec3fTest.Y + fDMin*vec3fTest.Z;
                        if( fUDDot >= fMaxUDDot )
                        {
                            // UF-edge
                            vec3fClosest.X = vec3fTest.X;
                            vec3fClosest.Y = fUMax;
                            vec3fClosest.Z = fDMax;
                        }
                        else // assert( fUDDot >= fMinUDDot ) from geometry
                        {
                            // U-face
                            fUDot = fDMin*vec3fTest.Y - fUMin*vec3fTest.Z;
                            fT = fUDot/fMinUDDot;
                            vec3fClosest.X = vec3fTest.X;
                            vec3fClosest.Y = vec3fTest.Y - fT*fDMin;
                            vec3fClosest.Z = vec3fTest.Z + fT*fUMin;
                        }
                    }
                    else
                    {
                        if( fRUDDot >= fMaxRUDDot )
                        {
                            // LUF-vertex
                            vec3fClosest.X = fRMax;
                            vec3fClosest.Y = fUMax;
                            vec3fClosest.Z = fDMax;
                        }
                        else // assert( fRUDDot >= fMinRUDDot ) from geometry
                        {
                            // LU-edge
                            fT = fRUDDot/fMinRUDDot;
                            vec3fClosest.X = fT*fRMin;
                            vec3fClosest.Y = fT*fUMin;
                            vec3fClosest.Z = fT*fDMin;
                        }
                    }
                }
            }
        }
    }

    vec3fDiff = vec3fTest - vec3fClosest;

    // convert back to original quadrant
    if( bRSignChange )
    {
        vec3fClosest.X = -vec3fClosest.X;
    }

    if( bUSignChange )
    {
        vec3fClosest.Y = -vec3fClosest.Y;
    }

    m_ClosestPoint0 = *m_pVector;

    // convert back to original coordinates
    m_ClosestPoint1 = m_pFrustum->Origin +
        vec3fClosest.X*m_pFrustum->RVector +
        vec3fClosest.Y*m_pFrustum->UVector +
        vec3fClosest.Z*m_pFrustum->DVector;

    // compute and return squared distance
    return vec3fDiff.GetSquaredLength();
}
//----------------------------------------------------------------------------
float SEDistVector3Frustum3f::Get(float fT, const SEVector3f& rVelocity0, 
    const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMVector = *m_pVector + fT*rVelocity0;
    SEVector3f vec3fMOrigin = m_pFrustum->Origin + fT*rVelocity1;
    SEFrustum3f tempMFrustum(vec3fMOrigin, m_pFrustum->RVector,
        m_pFrustum->UVector, m_pFrustum->DVector, m_pFrustum->RBound, 
        m_pFrustum->UBound, m_pFrustum->DMin, m_pFrustum->DMax);

    return SEDistVector3Frustum3f(vec3fMVector, tempMFrustum).Get();
}
//----------------------------------------------------------------------------
float SEDistVector3Frustum3f::GetSquared(float fT,
    const SEVector3f& rVelocity0, const SEVector3f& rVelocity1)
{
    SEVector3f vec3fMVector = *m_pVector + fT*rVelocity0;
    SEVector3f vec3fMOrigin = m_pFrustum->Origin + fT*rVelocity1;
    SEFrustum3f tempMFrustum(vec3fMOrigin, m_pFrustum->RVector,
        m_pFrustum->UVector, m_pFrustum->DVector, m_pFrustum->RBound, 
        m_pFrustum->UBound, m_pFrustum->DMin, m_pFrustum->DMax);

    return SEDistVector3Frustum3f(vec3fMVector, tempMFrustum).GetSquared();
}
//----------------------------------------------------------------------------
