// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_RTGICamera_H
#define Swing_RTGICamera_H

#include "SERenderingEngineLIB.h"
#include "SERenderCache.h"
#include "SEVector3.h"
#include "SEMatrix4.h"
#include "SEQuaternion.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 09/29/2013
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SERTGICamera
{
public:
	SERTGICamera(bool IsPerspective = true);
	~SERTGICamera();

	// Camera frame
	//
	//       U
	//       |    D 
	//       |  / 
	//       | / 
	//       |/_________R
	//       E

    // Default location  E = (0,0,0)
    // Default direction R = (1,0,0)
    // Default up        U = (0,1,0)
    // Default right     D = (0,0,1)

	void SetLookAt(const SEVector3f& location, const SEVector3f& lookAt, const SEVector3f& up);
    void SetLocation(const SEVector3f& location);
    void SetAxes(const SEVector3f& right, const SEVector3f& up, const SEVector3f& direction);
	void SetAngle(const float& horizontalAngle, const float& verticalAngle);
	void SetPerspectiveFrustum(float upFovDegrees, float aspectRatio, float nearPlane, 
		float farPlane);
	void SetOrthogonalFrustum(float upMax, float aspectRatio, float nearPlane, 
		float farPlane);

    void Rotate(const SEVector3f& eulerAngles);

	SEVector3f GetLocation() const;
	SEVector3f GetDirection() const;
	SEVector3f GetRight() const;

	void GetAngle(float& horizontalAngle, float& verticalAngle);
	float GetFoV() const;
    void GetNearFarPlane(float* nearFarPlane) const;

	SEMatrix4f GetViewTransform();
	SEMatrix4f GetProjectionTransform();
    SEMatrix4f GetViewCacheTransform();
    SEMatrix4f GetProjectionCacheTransform();

	bool IsPerspective() const;

    // View frustum.
    enum
    {
        VF_DMIN     = 0,  // near
        VF_DMAX     = 1,  // far
        VF_UMIN     = 2,  // bottom
        VF_UMAX     = 3,  // top
        VF_RMIN     = 4,  // left
        VF_RMAX     = 5,  // right
        VF_COUNT    = 6
    };

    // Render cache delegate functions.
    inline void SetRenderCache(SERenderCache* renderCache);
    inline SERenderCache* GetRenderCache();
    void UpdateRenderCache();

protected:
    // Camera space stuff.
    SEQuaternionf mRot;
	SEVector3f mLocation, mRight, mUp, mDirection;

    // Projection space stuff.
    float mFrustum[VF_COUNT];  // near, far, bottom, top, left, right
	float mUpFovDegrees, mAspectRatio;
    bool mIsPerspective;

    // Camera render cache.
    SERenderCachePtr mRenderCache;
};

#include "SERTGICamera.inl"

}

#endif