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

#ifndef Swing_Camera_H
#define Swing_Camera_H

#include "SERenderingEngineLIB.h"
#include "SEPlatforms.h"
#include "SEObject.h"
#include "SERay3.h"

// ���ǵ���������ϵΪ��������ϵ{Origin;X,Y,Z}:
// Ĭ��Origin = (0,0,0)
// Ĭ��X��    = (1,0,0)
// Ĭ��Y��    = (0,1,0)
// Ĭ��Z��    = (0,0,1)

// ����ʹ��:
// (1) ��������ϵ
// (2) ����������תtheta��,theta > 0ʱΪ˳ʱ����ת
// (3) 3ά�����������ʹ������ԭ��

namespace Swing
{

//----------------------------------------------------------------------------
// Description:
// Date:20080707
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SECamera : public SEObject
{
    SE_DECLARE_RTTI;
    SE_DECLARE_NAME_ID;
    SE_DECLARE_STREAM;

public:
    SECamera(void);
    virtual ~SECamera(void);

    // �����{E;R,U,D}����������ϵ�±�ʾ:
    // Ĭ��location  E = (0,0,0)
    // Ĭ��right     R = (1,0,0)
    // Ĭ��up        U = (0,1,0)
    // Ĭ��direction D = (0,0,1)

    void SetFrame(const SEVector3f& rLocation, const SEVector3f& rRVector,
        const SEVector3f& rUVector, const SEVector3f& rDVector);
    void SetLocation(const SEVector3f& rLocation);
    void SetAxes(const SEVector3f& rRVector, const SEVector3f& rUVector, 
        const SEVector3f& rDVector);

    inline SEVector3f GetLocation(void) const;
    inline SEVector3f GetRVector(void) const;
    inline SEVector3f GetUVector(void) const;
    inline SEVector3f GetDVector(void) const;

    // view frustum����ö��.
    enum
    {
        VF_DMIN     = 0,  // near
        VF_DMAX     = 1,  // far
        VF_UMIN     = 2,  // bottom
        VF_UMAX     = 3,  // top
        VF_RMIN     = 4,  // left
        VF_RMAX     = 5,  // right
        VF_COUNT = 6
    };

    // ����view frustum����.
    void SetFrustum(float fRMin, float fRMax, float fUMin, float fUMax,
        float fDMin, float fDMax);

    void SetFrustum(const float* pFrustum);

    // �Գ�view frustum(umin = -umax, rmin = -rmax),
    // ����ʹ��U�᷽��FOV(field of view),aspect ratio,Dmin,Dmax��ʾ.
    void SetFrustum(float fUpFovDegrees, float fAspectRatio, float fDMin,
        float fDMax);

    // ��ȡview frustum����.
    void GetFrustum(float& rRMin, float& rRMax, float& rUMin,
        float& rUMax, float& rDMin, float& rDMax) const;

    // ��ȡview frustum����.
    inline const float* GetFrustum(void) const;

    // ��ȡview frustum����,
    // ����Ƿ�Ϊ�Գ�view frustum,
    // ����ֵΪtrueʱ,��ȡֵ�źϷ�.
    bool GetFrustum(float& rUpFovDegrees, float& rAspectRatio,
        float& rDMin, float& rDMax) const;

    // ��ȡview frustum����.
    inline float GetDMin(void) const;
    inline float GetDMax(void) const;
    inline float GetUMin(void) const;
    inline float GetUMax(void) const;
    inline float GetRMin(void) const;
    inline float GetRMax(void) const;

    // Perspective/orthogonal projection access.
    void SetPerspective(bool bPerspective);
    inline bool IsPerspective(void) const;

    // Viewport access(����Ϊ[0,1]^2).
    void SetViewport(float fLeft, float fRight, float fTop, float fBottom);
    void GetViewport(float& rLeft, float& rRight, float& rTop,
        float& rBottom);

    // Depth range access(����Ϊ[0,1]).
    void SetDepthRange(float fNear, float fFar);
    void GetDepthRange(float& rNear, float& rFar);

    // Mouse picking support. The (x,y) input point is in left-handed screen
    // coordinates (what you usually read from the windowing system). The
    // function returns 'true' if and only if the input point is located in
    // the current viewport. When 'true', the origin and direction values are
    // valid and are in world coordinates. The direction vector is unit length.
    bool GetPickRay(int iX, int iY, int iWidth, int iHeight, SERay3f& rRay)
        const;

    // Virtual track ball rotation support. (x0,y0) and (x1,y1) input points
    // are in a right-handed coordinates(the origin is at the center of window
    // screen, which is (width/2,height/2). Let r = width/height, then any 
    // point (x,y) on the screen is in the range x:[-r,+r] y:[-1,+1]). The
    // function returns 'true' if and only if the two points are different.
    // When 'true', the rotation matrix is valid and is in world coordinates.
    bool GetTrackBallRotate(float fX0, float fY0, float fX1, float fY1, 
        SEMatrix3f& rMat) const;

protected:
    // �����E;R,U,D����������ϵ��ʾ.
    SEVector3f m_Location, m_RVector, m_UVector, m_DVector;

    // ��Ͷ�����(near, far, bottom, top, left, right).
    float m_Frustum[VF_COUNT];

    // Ĭ��Ϊtrue��ʾperspective camera.
    // TO DO. Stream this member
    bool m_bPerspective;

    // �ӿڲ���,
    // ��ʱʹ��OpenGL���,�Ժ��ΪDirectX���.
    float m_fPortL, m_fPortR, m_fPortT, m_fPortB;

    // ��ȷ�Χ����.
    float m_fPortN, m_fPortF;

    // ��ǰ�������������Ⱦ��.
    // �ǿ�ʱ��ʾ��ǰ��������ڼ���״̬,
    // ���������ͬʱ��Ӹ������Ⱦ��,�������ڶ����Ⱦ��֮�乲��,
    // ǰ����ȷ��ֻ��һ����Ⱦ������ʹ�ø������,
    // ��Ⱦ�����������������Ա����.
    friend class SERenderer;
    SERenderer* m_pRenderer;
};

typedef SESmartPointer<SECamera> SECameraPtr;

#include "SECamera.inl"

}

#endif
