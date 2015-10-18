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
#include "SECamera.h"
//#include "SERenderer.h"

using namespace Swing;

SE_IMPLEMENT_RTTI(Swing, SECamera, SEObject);
SE_IMPLEMENT_STREAM(SECamera);
SE_IMPLEMENT_DEFAULT_NAME_ID(SECamera, SEObject);

//----------------------------------------------------------------------------
SECamera::SECamera()
{
    m_pRenderer = 0;
    m_bPerspective = true;

    SetFrustum(-0.5f, 0.5f, -0.5f, 0.5f, 1.0f, 2.0f);
    SetViewport(0.0f, 1.0f, 1.0f, 0.0f);
    SetDepthRange(0.0f, 1.0f);
    SetFrame(SEVector3f::ZERO, SEVector3f::UNIT_X, SEVector3f::UNIT_Y, 
        SEVector3f::UNIT_Z); // 与世界空间坐标系重合
}
//----------------------------------------------------------------------------
SECamera::~SECamera()
{
}
//----------------------------------------------------------------------------
void SECamera::SetFrame(const SEVector3f& rLocation, const SEVector3f& 
    rRVector, const SEVector3f& rUVector, const SEVector3f& rDVector)
{
    m_Location = rLocation;
    SetAxes(rRVector, rUVector, rDVector);
}
//----------------------------------------------------------------------------
void SECamera::SetLocation(const SEVector3f& rLocation)
{
    m_Location = rLocation;

    //if( m_pRenderer )
    //{
    //    m_pRenderer->OnFrameChange();
    //}
}
//----------------------------------------------------------------------------
void SECamera::SetAxes(const SEVector3f& rRVector, const SEVector3f& rUVector,
    const SEVector3f& rDVector)
{
    m_RVector = rRVector;
    m_UVector = rUVector;
    m_DVector = rDVector;

    float fADet = SEMathf::FAbs(m_DVector.Dot(m_RVector.Cross(m_UVector)));
    if( SEMathf::FAbs(1.0f-fADet) > 0.01f )
    {
        // R,U,D不构成规范正交基,因此重新正交规范化这组基向量.
        SEVector3f::Orthonormalize(m_RVector, m_UVector, m_DVector);
    }

    //if( m_pRenderer )
    //{
    //    m_pRenderer->OnFrameChange();
    //}
}
//----------------------------------------------------------------------------
void SECamera::SetFrustum(float fRMin, float fRMax, float fUMin, float fUMax,
    float fDMin, float fDMax)
{
    m_Frustum[VF_DMIN] = fDMin;
    m_Frustum[VF_DMAX] = fDMax;
    m_Frustum[VF_UMIN] = fUMin;
    m_Frustum[VF_UMAX] = fUMax;
    m_Frustum[VF_RMIN] = fRMin;
    m_Frustum[VF_RMAX] = fRMax;

    //if( m_pRenderer )
    //{
    //    m_pRenderer->OnFrustumChange();
    //}
}
//----------------------------------------------------------------------------
void SECamera::SetFrustum(const float* pFrustum)
{
    size_t uiSize = VF_COUNT * sizeof(float);
    SESystem::Memcpy(m_Frustum, uiSize, pFrustum, uiSize);

    //if( m_pRenderer )
    //{
    //    m_pRenderer->OnFrustumChange();
    //}
}
//----------------------------------------------------------------------------
void SECamera::SetFrustum(float fUpFovDegrees, float fAspectRatio, float fDMin,
    float fDMax)
{
    float fHalfAngleRadians = 0.5f * fUpFovDegrees * SEMathf::DEG_TO_RAD;
    m_Frustum[VF_UMAX] = fDMin * SEMathf::Tan(fHalfAngleRadians);
    m_Frustum[VF_RMAX] = fAspectRatio * m_Frustum[VF_UMAX];
    m_Frustum[VF_UMIN] = -m_Frustum[VF_UMAX];
    m_Frustum[VF_RMIN] = -m_Frustum[VF_RMAX];
    m_Frustum[VF_DMIN] = fDMin;
    m_Frustum[VF_DMAX] = fDMax;

    //if( m_pRenderer )
    //{
    //    m_pRenderer->OnFrustumChange();
    //}
}
//----------------------------------------------------------------------------
void SECamera::GetFrustum(float& rRMin, float& rRMax, float& rUMin,
    float& rUMax, float& rDMin, float& rDMax) const
{
    rDMin = m_Frustum[VF_DMIN];
    rDMax = m_Frustum[VF_DMAX];
    rUMin = m_Frustum[VF_UMIN];
    rUMax = m_Frustum[VF_UMAX];
    rRMin = m_Frustum[VF_RMIN];
    rRMax = m_Frustum[VF_RMAX];
}
//----------------------------------------------------------------------------
bool SECamera::GetFrustum(float& rUpFovDegrees, float& rAspectRatio,
    float& rDMin, float& rDMax) const
{
    // 必须是对称的frustum
    if( m_Frustum[VF_RMIN] == -m_Frustum[VF_RMAX]
    &&  m_Frustum[VF_UMIN] == -m_Frustum[VF_UMAX] )
    {
        float fTemp = m_Frustum[VF_UMAX] / m_Frustum[VF_DMIN];
        rUpFovDegrees = 2.0f * SEMathf::ATan(fTemp) * SEMathf::RAD_TO_DEG;
        rAspectRatio = m_Frustum[VF_RMAX] / m_Frustum[VF_UMAX];
        rDMin = m_Frustum[VF_DMIN];
        rDMax = m_Frustum[VF_DMAX];

        return true;
    }

    return false;
}
//----------------------------------------------------------------------------
void SECamera::SetPerspective(bool bPerspective)
{
    m_bPerspective = bPerspective;

    //if( m_pRenderer )
    //{
    //    m_pRenderer->OnFrustumChange();
    //}
}
//----------------------------------------------------------------------------
void SECamera::SetViewport(float fLeft, float fRight, float fTop,
    float fBottom)
{
    m_fPortL = fLeft;
    m_fPortR = fRight;
    m_fPortT = fTop;
    m_fPortB = fBottom;

    //if( m_pRenderer )
    //{
    //    m_pRenderer->OnViewportChange();
    //}
}
//----------------------------------------------------------------------------
void SECamera::GetViewport(float& rLeft, float& rRight, float& rTop,
    float& rBottom)
{
    rLeft = m_fPortL;
    rRight = m_fPortR;
    rTop = m_fPortT;
    rBottom = m_fPortB;
}
//----------------------------------------------------------------------------
void SECamera::SetDepthRange(float fNear, float fFar)
{
    m_fPortN = fNear;
    m_fPortF = fFar;

    //if( m_pRenderer )
    //{
    //    m_pRenderer->OnDepthRangeChange();
    //}
}
//----------------------------------------------------------------------------
void SECamera::GetDepthRange(float& rNear, float& rFar)
{
    rNear = m_fPortN;
    rFar = m_fPortF;
}
//----------------------------------------------------------------------------
bool SECamera::GetPickRay(int iX, int iY, int iWidth, int iHeight,
    SERay3f& rRay) const
{
    float fPortX = ((float)iX) / (float)(iWidth - 1);
    if( fPortX < m_fPortL || fPortX > m_fPortR )
    {
        return false;
    }

    float fPortY = ((float)(iHeight - 1 - iY)) / (float)(iHeight - 1);
    if( fPortY < m_fPortB || fPortY > m_fPortT )
    {
        return false;
    }

    float fXWeight = (fPortX - m_fPortL) / (m_fPortR - m_fPortL);
    float fViewX = (1.0f - fXWeight)*m_Frustum[VF_RMIN] + 
        fXWeight*m_Frustum[VF_RMAX];
    float fYWeight = (fPortY - m_fPortB) / (m_fPortT - m_fPortB);
    float fViewY = (1.0f - fYWeight)*m_Frustum[VF_UMIN] + 
        fYWeight*m_Frustum[VF_UMAX];

    if( m_bPerspective )
    {
        // The pick ray is E + tW, t >= 0.
        // W = (Q - E)/|Q - E|, Q = E + n*D + Xv*R + Yv*U.
        rRay.Origin = m_Location;
        rRay.Direction = m_Frustum[VF_DMIN]*m_DVector + fViewX*m_RVector + 
            fViewY*m_UVector;
        rRay.Direction.Normalize();
    }
    else
    {
        // The pick ray is E' + tD, t >= 0.
        // E' = E + Xv*R + Yv*U.
        rRay.Origin = m_Location + fViewX*m_RVector + fViewY*m_UVector;
        rRay.Direction = m_DVector;
    }

    return true;
}
//----------------------------------------------------------------------------
bool SECamera::GetTrackBallRotate(float fX0, float fY0, float fX1, float fY1, 
    SEMatrix3f& rMat) const
{
    if( fX0 == fX1 && fY0 == fY1 )
    {
        return false;
    }

    // 获取球上的第一向量.
    float fLength = SEMathf::Sqrt(fX0*fX0 + fY0*fY0), fInvLength, fZ0, fZ1;
    if( fLength > 1.0f )
    {
        // 如果在单位圆之外,则投影到单位圆上.
        fInvLength = 1.0f / fLength;
        fX0 *= fInvLength;
        fY0 *= fInvLength;
        fZ0 = 0.0f;
    }
    else
    {
        // 如果在单位圆内,则直接计算出所对应的负单位半球上的点(x0,y0,z0).
        fZ0 = 1.0f - fX0*fX0 - fY0*fY0;
        fZ0 = (fZ0 <= 0.0f ? 0.0f : SEMathf::Sqrt(fZ0));
    }
    fZ0 *= -1.0f;

    // 摄像机世界坐标轴顺序为(R,U,D), 对应向量表示为(x,y,z).
    SEVector3f tempVec0(fX0, fY0, fZ0);

    // 获取球上的第二向量.
    fLength = SEMathf::Sqrt(fX1*fX1 + fY1*fY1);
    if( fLength > 1.0f )
    {
        // 如果在单位圆之外,则投影到单位圆上.
        fInvLength = 1.0f / fLength;
        fX1 *= fInvLength;
        fY1 *= fInvLength;
        fZ1 = 0.0f;
    }
    else
    {
        // 如果在单位圆内,则直接计算出所对应的负单位半球上的点(x1,y1,z1).
        fZ1 = 1.0f - fX1*fX1 - fY1*fY1;
        fZ1 = (fZ1 <= 0.0f ? 0.0f : SEMathf::Sqrt(fZ1));
    }
    fZ1 *= -1.0f;

    // 摄像机世界坐标轴顺序为(R,U,D), 对应向量表示为(x,y,z).
    SEVector3f tempVec1(fX1, fY1, fZ1);

    // 创建旋转矩阵所需的轴,角.
    SEVector3f vec3fAxis = tempVec1.Cross(tempVec0);
    float fDot = tempVec0.Dot(tempVec1);
    float fAngle;
    if( vec3fAxis.Normalize() > SEMathf::ZERO_TOLERANCE )
    {
        fAngle = SEMathf::ACos(fDot); 
    }
    else  // 两向量平行
    {
        if( fDot < 0.0f )
        {
            // 旋转pi弧度.
            fInvLength = SEMathf::InvSqrt(fX0*fX0 + fY0*fY0);
            vec3fAxis.X = fY0 * fInvLength;
            vec3fAxis.Y = -fX0 * fInvLength;
            vec3fAxis.Z = 0.0f;
            fAngle = SEMathf::PI;
        }
        else
        {
            // 旋转0弧度.
            vec3fAxis = SEVector3f::UNIT_X;
            fAngle = 0.0f;
        }
    }

    // 计算trackball运动所对应的世界旋转矩阵.
    // 之前算出的旋转轴处在摄像机坐标系下.需要转换到世界坐标系下.
    // 其世界坐标系下的向量表示为(xR + yU + zD).
    SEVector3f vec3fWorldAxis = vec3fAxis.X*m_RVector + vec3fAxis.Y*m_UVector 
        + vec3fAxis.Z*m_DVector;

    rMat.FromAxisAngle(vec3fWorldAxis, -fAngle);
    return true;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// streaming
//----------------------------------------------------------------------------
void SECamera::Load(SEStream& rStream, SEStream::SELink* pLink)
{
    SE_BEGIN_DEBUG_STREAM_LOAD;

    SEObject::Load(rStream, pLink);

    // native data
    rStream.Read(m_Location);
    rStream.Read(m_RVector);
    rStream.Read(m_UVector);
    rStream.Read(m_DVector);
    rStream.Read(m_Frustum[VF_DMIN]);
    rStream.Read(m_Frustum[VF_DMAX]);
    rStream.Read(m_Frustum[VF_UMIN]);
    rStream.Read(m_Frustum[VF_UMAX]);
    rStream.Read(m_Frustum[VF_RMIN]);
    rStream.Read(m_Frustum[VF_RMAX]);
    rStream.Read(m_fPortL);
    rStream.Read(m_fPortR);
    rStream.Read(m_fPortT);
    rStream.Read(m_fPortB);
    rStream.Read(m_fPortN);
    rStream.Read(m_fPortF);

    SE_END_DEBUG_STREAM_LOAD(SECamera);
}
//----------------------------------------------------------------------------
void SECamera::Link(SEStream& rStream, SEStream::SELink* pLink)
{
    SEObject::Link(rStream, pLink);
}
//----------------------------------------------------------------------------
bool SECamera::Register(SEStream& rStream) const
{
    return SEObject::Register(rStream);
}
//----------------------------------------------------------------------------
void SECamera::Save(SEStream& rStream) const
{
    SE_BEGIN_DEBUG_STREAM_SAVE;
    
    SEObject::Save(rStream);

    // native data
    rStream.Write(m_Location);
    rStream.Write(m_RVector);
    rStream.Write(m_UVector);
    rStream.Write(m_DVector);
    rStream.Write(m_Frustum[VF_DMIN]);
    rStream.Write(m_Frustum[VF_DMAX]);
    rStream.Write(m_Frustum[VF_UMIN]);
    rStream.Write(m_Frustum[VF_UMAX]);
    rStream.Write(m_Frustum[VF_RMIN]);
    rStream.Write(m_Frustum[VF_RMAX]);
    rStream.Write(m_fPortL);
    rStream.Write(m_fPortR);
    rStream.Write(m_fPortT);
    rStream.Write(m_fPortB);
    rStream.Write(m_fPortN);
    rStream.Write(m_fPortF);

    SE_END_DEBUG_STREAM_SAVE(SECamera);
}
//----------------------------------------------------------------------------
int SECamera::GetDiskUsed(const SEStreamVersion& rVersion) const
{
    int iSize = SEObject::GetDiskUsed(rVersion) +
        sizeof(m_Location) +
        sizeof(m_RVector) +
        sizeof(m_UVector) +
        sizeof(m_DVector) +
        sizeof(m_Frustum[VF_DMIN]) +
        sizeof(m_Frustum[VF_DMAX]) +
        sizeof(m_Frustum[VF_UMIN]) +
        sizeof(m_Frustum[VF_UMAX]) +
        sizeof(m_Frustum[VF_RMIN]) +
        sizeof(m_Frustum[VF_RMAX]) +
        sizeof(m_fPortL) +
        sizeof(m_fPortR) +
        sizeof(m_fPortT) +
        sizeof(m_fPortB) +
        sizeof(m_fPortN) +
        sizeof(m_fPortF);

    return iSize;
}
//----------------------------------------------------------------------------