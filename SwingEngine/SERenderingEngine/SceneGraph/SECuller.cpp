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
#include "SECuller.h"

using namespace Swing;

//----------------------------------------------------------------------------
SECuller::SECuller(int iMaxCount, int iGrowBy, const SECamera* pCamera)
    :
    m_VisibleSet(iMaxCount, iGrowBy),
    m_AudibleSet(iMaxCount, iGrowBy)
{
    m_pCamera = pCamera;
    m_iPlaneCount = 6;

    // m_Frustum, m_aPlane, m_uiPlaneState 都在ComputeUnculledSet里初始化.
}
//----------------------------------------------------------------------------
SECuller::~SECuller()
{
}
//----------------------------------------------------------------------------
void SECuller::SetFrustum(const float* afFrustum)
{
    // 我们要根据截投体参数计算新的截投体平面方程,
    // 注意所有公式的叉积符号都使用左手原则推导.

    SE_ASSERT( m_pCamera );

    if( !m_pCamera )
    {
        return;
    }

    // 复制截投体参数
    size_t uiSize = SECamera::VF_COUNT * sizeof(float);
    SESystem::Memcpy(m_Frustum, uiSize, afFrustum, uiSize);
    float fDMin2 = m_Frustum[SECamera::VF_DMIN] * m_Frustum[SECamera::VF_DMIN];
    float fRMin2 = m_Frustum[SECamera::VF_RMIN] * m_Frustum[SECamera::VF_RMIN];
    float fRMax2 = m_Frustum[SECamera::VF_RMAX] * m_Frustum[SECamera::VF_RMAX];
    float fUMin2 = m_Frustum[SECamera::VF_UMIN] * m_Frustum[SECamera::VF_UMIN];
    float fUMax2 = m_Frustum[SECamera::VF_UMAX] * m_Frustum[SECamera::VF_UMAX];

    // 获取摄像机坐标系,E:RUD
    SEVector3f vec3fLoc = m_pCamera->GetLocation();
    SEVector3f vec3fRVec = m_pCamera->GetRVector();
    SEVector3f vec3fUVec = m_pCamera->GetUVector();
    SEVector3f vec3fDVec = m_pCamera->GetDVector();
    float fDdE = vec3fDVec.Dot(vec3fLoc);

    // 更新near plane
    m_aPlane[SECamera::VF_DMIN].Normal = vec3fDVec;
    m_aPlane[SECamera::VF_DMIN].Constant = fDdE + m_Frustum[SECamera::VF_DMIN];

    // 更新far plane
    m_aPlane[SECamera::VF_DMAX].Normal = -vec3fDVec;
    m_aPlane[SECamera::VF_DMAX].Constant = -(fDdE + m_Frustum[
        SECamera::VF_DMAX]);

    // 更新bottom plane
    float fInvLength = 1.0f / SEMathf::Sqrt(fDMin2 + fUMin2);
    float fC0 = -m_Frustum[SECamera::VF_UMIN]*fInvLength;  // D component
    float fC1 = +m_Frustum[SECamera::VF_DMIN]*fInvLength;  // U component
    m_aPlane[SECamera::VF_UMIN].Normal = fC0*vec3fDVec + fC1*vec3fUVec;
    m_aPlane[SECamera::VF_UMIN].Constant = vec3fLoc.Dot(
        m_aPlane[SECamera::VF_UMIN].Normal);

    // 更新top plane
    fInvLength = 1.0f / SEMathf::Sqrt(fDMin2 + fUMax2);
    fC0 = +m_Frustum[SECamera::VF_UMAX]*fInvLength;  // D component
    fC1 = -m_Frustum[SECamera::VF_DMIN]*fInvLength;  // U component
    m_aPlane[SECamera::VF_UMAX].Normal = fC0*vec3fDVec + fC1*vec3fUVec;
    m_aPlane[SECamera::VF_UMAX].Constant = vec3fLoc.Dot(
        m_aPlane[SECamera::VF_UMAX].Normal);

    // 更新left plane
    fInvLength = 1.0f / SEMathf::Sqrt(fDMin2 + fRMin2);
    fC0 = -m_Frustum[SECamera::VF_RMIN]*fInvLength;  // D component
    fC1 = +m_Frustum[SECamera::VF_DMIN]*fInvLength;  // R component
    m_aPlane[SECamera::VF_RMIN].Normal = fC0*vec3fDVec + fC1*vec3fRVec;
    m_aPlane[SECamera::VF_RMIN].Constant = vec3fLoc.Dot(
        m_aPlane[SECamera::VF_RMIN].Normal);

    // 更新right plane
    fInvLength = 1.0f / SEMathf::Sqrt(fDMin2 + fRMax2);
    fC0 = +m_Frustum[SECamera::VF_RMAX]*fInvLength;  // D component
    fC1 = -m_Frustum[SECamera::VF_DMIN]*fInvLength;  // R component
    m_aPlane[SECamera::VF_RMAX].Normal = fC0*vec3fDVec + fC1*vec3fRVec;
    m_aPlane[SECamera::VF_RMAX].Constant = vec3fLoc.Dot(
        m_aPlane[SECamera::VF_RMAX].Normal);

    // 所有平面初始化为激活状态
    m_uiPlaneState = (unsigned int)~0;
}
//----------------------------------------------------------------------------
bool SECuller::IsInFrustum(const SEBoundingVolume* pBound)
{
    // 开始于尾部,很有可能是最容易剔除对象的用户自定义平面.
    int iP = m_iPlaneCount - 1;
    unsigned int uiMask = 1 << iP;

    for( int i = 0; i < m_iPlaneCount; ++i, --iP, uiMask >>= 1 )
    {
        if( m_uiPlaneState & uiMask )
        {
            int iSide = pBound->OnWhichSide(m_aPlane[iP]);

            if( iSide < 0 )
            {
                // 对象在某平面负半空间,则剔除
                return false;
            }

            if( iSide > 0 )
            {
                // 对象在该平面正半空间.在之后的剔除过程中,
                // 没有必要进一步比较该对象的所有子对象与该平面的关系,
                // 因此设置该平面状态为非激活状态.
                m_uiPlaneState &= ~uiMask;
            }
        }
    }

    // 对象或在全部平面正半空间,或相交.
    return true;
}
//----------------------------------------------------------------------------
bool SECuller::IsInFrustum(int iVertexCount, const SEVector3f* aVertex,
    bool bIgnoreNearPlane)
{
    // The Boolean variable bIgnoreNearPlane should be set to 'true' when
    // the test polygon is a portal.  This avoids the situation when the
    // portal is in the view pyramid (eye+left/right/top/bottom), but is
    // between the eye and near plane.  In such a situation you do not want
    // the portal system to cull the portal.  This situation typically occurs
    // when the camera moves through the portal from current region to
    // adjacent region.

    // Start with last pushed plane, which is potentially the most
    // restrictive plane.
    int iP = m_iPlaneCount - 1;
    for( int i = 0; i < m_iPlaneCount; ++i, --iP )
    {
        SEPlane3f& rPlane = m_aPlane[iP];
        if( bIgnoreNearPlane && iP == SECamera::VF_DMIN )
        {
            continue;
        }

        int iV;
        for( iV = 0; iV < iVertexCount; ++iV )
        {
            int iSide = rPlane.OnWhichSide(aVertex[iV]);
            if( iSide >= 0 )
            {
                // polygon is not totally outside this plane
                break;
            }
        }

        if( iV == iVertexCount )
        {
            // polygon is totally outside this plane
            return false;
        }
    }

    return true;
}
//----------------------------------------------------------------------------
bool SECuller::IsSingleInFrustum(const SEBoundingVolume* pBound) const
{
    // 开始于尾部,很有可能是最容易剔除对象的用户自定义平面.
    int iP = m_iPlaneCount - 1;

    for( int i = 0; i < m_iPlaneCount; ++i, --iP )
    {
        int iSide = pBound->OnWhichSide(m_aPlane[iP]);

        if( iSide < 0 )
        {
            // 对象在某平面负半空间,则剔除
            return false;
        }
    }

    // 对象或在全部平面正半空间,或相交.
    return true;
}
//----------------------------------------------------------------------------
int SECuller::OnWhichSide(const SEPlane3f& rPlane) const
{
    // The plane is N*(X-C) = 0 where the * indicates dot product. The signed
    // distance from the camera location E to the plane is N*(E-C).
    float fNdEmC = rPlane.GetDistance(m_pCamera->GetLocation());

    float fNdD = rPlane.Normal.Dot(m_pCamera->GetDVector());
    float fNdU = rPlane.Normal.Dot(m_pCamera->GetUVector());
    float fNdR = rPlane.Normal.Dot(m_pCamera->GetRVector());
    float fFdN = m_Frustum[SECamera::VF_DMAX]/m_Frustum[SECamera::VF_DMIN];

    int iPositive = 0, iNegative = 0;
    float fSgnDist;

    // check near-plane vertices
    float fPDMin = m_Frustum[SECamera::VF_DMIN]*fNdD;
    float fNUMin = m_Frustum[SECamera::VF_UMIN]*fNdU;
    float fNUMax = m_Frustum[SECamera::VF_UMAX]*fNdU;
    float fNRMin = m_Frustum[SECamera::VF_RMIN]*fNdR;
    float fNRMax = m_Frustum[SECamera::VF_RMAX]*fNdR;

    // V = E + dmin*D + umin*U + rmin*R
    // N*(V-C) = N*(E-C) + dmin*(N*D) + umin*(N*U) + rmin*(N*R)
    fSgnDist = fNdEmC + fPDMin + fNUMin + fNRMin;
    if( fSgnDist > 0.0f )
    {
        iPositive++;
    }
    else if( fSgnDist < 0.0f )
    {
        iNegative++;
    }

    // V = E + dmin*D + umin*U + rmax*R
    // N*(V-C) = N*(E-C) + dmin*(N*D) + umin*(N*U) + rmax*(N*R)
    fSgnDist = fNdEmC + fPDMin + fNUMin + fNRMax;
    if( fSgnDist > 0.0f )
    {
        iPositive++;
    }
    else if( fSgnDist < 0.0f )
    {
        iNegative++;
    }

    // V = E + dmin*D + umax*U + rmin*R
    // N*(V-C) = N*(E-C) + dmin*(N*D) + umax*(N*U) + rmin*(N*R)
    fSgnDist = fNdEmC + fPDMin + fNUMax + fNRMin;
    if( fSgnDist > 0.0f )
    {
        iPositive++;
    }
    else if( fSgnDist < 0.0f )
    {
        iNegative++;
    }

    // V = E + dmin*D + umax*U + rmax*R
    // N*(V-C) = N*(E-C) + dmin*(N*D) + umax*(N*U) + rmax*(N*R)
    fSgnDist = fNdEmC + fPDMin + fNUMax + fNRMax;
    if( fSgnDist > 0.0f )
    {
        iPositive++;
    }
    else if( fSgnDist < 0.0f )
    {
        iNegative++;
    }

    // check far-plane vertices (s = dmax/dmin)
    float fPDMax = m_Frustum[SECamera::VF_DMAX]*fNdD;
    float fFUMin = fFdN*fNUMin;
    float fFUMax = fFdN*fNUMax;
    float fFRMin = fFdN*fNRMin;
    float fFRMax = fFdN*fNRMax;

    // V = E + dmax*D + umin*U + rmin*R
    // N*(V-C) = N*(E-C) + dmax*(N*D) + s*umin*(N*U) + s*rmin*(N*R)
    fSgnDist = fNdEmC + fPDMax + fFUMin + fFRMin;
    if( fSgnDist > 0.0f )
    {
        iPositive++;
    }
    else if( fSgnDist < 0.0f )
    {
        iNegative++;
    }

    // V = E + dmax*D + umin*U + rmax*R
    // N*(V-C) = N*(E-C) + dmax*(N*D) + s*umin*(N*U) + s*rmax*(N*R)
    fSgnDist = fNdEmC + fPDMax + fFUMin + fFRMax;
    if( fSgnDist > 0.0f )
    {
        iPositive++;
    }
    else if( fSgnDist < 0.0f )
    {
        iNegative++;
    }

    // V = E + dmax*D + umax*U + rmin*R
    // N*(V-C) = N*(E-C) + dmax*(N*D) + s*umax*(N*U) + s*rmin*(N*R)
    fSgnDist = fNdEmC + fPDMax + fFUMax + fFRMin;
    if( fSgnDist > 0.0f )
    {
        iPositive++;
    }
    else if( fSgnDist < 0.0f )
    {
        iNegative++;
    }

    // V = E + dmax*D + umax*U + rmax*R
    // N*(V-C) = N*(E-C) + dmax*(N*D) + s*umax*(N*U) + s*rmax*(N*R)
    fSgnDist = fNdEmC + fPDMax + fFUMax + fFRMax;
    if( fSgnDist > 0.0f )
    {
        iPositive++;
    }
    else if( fSgnDist < 0.0f )
    {
        iNegative++;
    }

    if( iPositive > 0 )
    {
        if( iNegative > 0 )
        {
            // frustum straddles the plane
            return 0;
        }

        // frustum is fully on the positive side
        return +1;
    }

    // frustum is fully on the negative side
    return -1;
}
//----------------------------------------------------------------------------
void SECuller::ComputeUnculledSet(SESpatial* pScene)
{
    SE_ASSERT( m_pCamera && pScene );

    if( m_pCamera && pScene )
    {
        SetFrustum(m_pCamera->GetFrustum());
        m_VisibleSet.Clear();
        m_AudibleSet.Clear();
        pScene->OnGetUnculledSet(*this, false);
    }
}
//----------------------------------------------------------------------------
