// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERenderingEnginePCH.h"
#include "SERTGICamera.h"
#include "SEMath.h"

using namespace Swing;

//----------------------------------------------------------------------------
SERTGICamera::SERTGICamera(bool IsPerspective)
	:
	mLocation(0.0f, 0.0f, 0.0f), 
	mRight(1.0f, 0.0f, 0.0f),
	mUp(0.0f, 1.0f, 0.0f), 
	mDirection(0.0f, 0.0f, 1.0f),
	mIsPerspective(IsPerspective),
	mRot()
{
	if( mIsPerspective )
	{
		SetPerspectiveFrustum(45.0f, 1.0f, 0.01f, 100.0f);
	}
	else
	{
		SetOrthogonalFrustum(1.0f, 1.0f, 0.01f, 100.0f);
	}

    SERenderCache::SECacheFlag flag = SERenderCache::SECacheFlag(
        SERenderCache::SECacheFlag::CF_ViewTransform |
        SERenderCache::SECacheFlag::CF_ProjTransform);
    mRenderCache = SE_NEW SERenderCache();
    mRenderCache->SetCacheFlag(flag);
}
//----------------------------------------------------------------------------
SERTGICamera::~SERTGICamera()
{
    mRenderCache = 0;
}
//----------------------------------------------------------------------------
void SERTGICamera::SetLookAt(const SEVector3f& location, const SEVector3f& lookAt, 
	const SEVector3f& up)
{
	mLocation = location;
    mDirection = lookAt - location;
    mDirection.Normalize();
    mRight = up.Cross(mDirection);
    mRight.Normalize();
    mUp = mDirection.Cross(mRight);

    SEMatrix3f rotation;
    rotation[0][0] = mRight.X;
    rotation[1][0] = mRight.Y;
    rotation[2][0] = mRight.Z;
    rotation[0][1] = mUp.X;
    rotation[1][1] = mUp.Y;
    rotation[2][1] = mUp.Z;
    rotation[0][2] = mDirection.X;
    rotation[1][2] = mDirection.Y;
    rotation[2][2] = mDirection.Z;

    mRot.FromRotationMatrix(rotation);
}
//----------------------------------------------------------------------------
void SERTGICamera::SetLocation(const SEVector3f& location)
{
	mLocation = location;
}
//----------------------------------------------------------------------------
SEVector3f SERTGICamera::GetLocation() const
{
	return mLocation;
}
//----------------------------------------------------------------------------
void SERTGICamera::SetAxes(const SEVector3f& right, const SEVector3f& up, 
    const SEVector3f& direction)
{
	mRight = right;
	mUp = up;
	mDirection = direction;
}
//----------------------------------------------------------------------------
void SERTGICamera::SetPerspectiveFrustum(float upFovDegrees, float aspectRatio, 
	float nearPlane, float farPlane)
{
	SE_ASSERT( mIsPerspective );

    float halfAngleRadians = 0.5f * upFovDegrees * SEMathf::DEG_TO_RAD;
    mFrustum[VF_UMAX] = nearPlane * SEMathf::Tan(halfAngleRadians);
    mFrustum[VF_RMAX] = aspectRatio * mFrustum[VF_UMAX];
    mFrustum[VF_UMIN] = -mFrustum[VF_UMAX];
    mFrustum[VF_RMIN] = -mFrustum[VF_RMAX];
    mFrustum[VF_DMIN] = nearPlane;
    mFrustum[VF_DMAX] = farPlane;

	mUpFovDegrees = upFovDegrees;
	mAspectRatio = aspectRatio;
}
//----------------------------------------------------------------------------
void SERTGICamera::SetOrthogonalFrustum(float upMax, float aspectRatio, 
	float nearPlane, float farPlane)
{
	SE_ASSERT( !mIsPerspective );

    mFrustum[VF_UMAX] = upMax;
    mFrustum[VF_RMAX] = aspectRatio * mFrustum[VF_UMAX];
    mFrustum[VF_UMIN] = -mFrustum[VF_UMAX];
    mFrustum[VF_RMIN] = -mFrustum[VF_RMAX];
    mFrustum[VF_DMIN] = nearPlane;
    mFrustum[VF_DMAX] = farPlane;

	mAspectRatio = aspectRatio;
}
//----------------------------------------------------------------------------
void SERTGICamera::GetNearFarPlane(float* nearFarPlane) const
{
    SE_ASSERT( nearFarPlane );
    nearFarPlane[0] = mFrustum[VF_DMIN];
    nearFarPlane[1] = mFrustum[VF_DMAX];
}
//----------------------------------------------------------------------------
SEMatrix4f SERTGICamera::GetViewTransform()
{
	SEMatrix4f res;

	float EdR = mLocation.Dot(mRight);
    float EdU = mLocation.Dot(mUp);
    float EdD = mLocation.Dot(mDirection);

	res[0][0] = mRight.X;
	res[1][0] = mRight.Y;
	res[2][0] = mRight.Z;
	res[3][0] = -EdR;

	res[0][1] = mUp.X;
	res[1][1] = mUp.Y;
	res[2][1] = mUp.Z;
	res[3][1] = -EdU;

	res[0][2] = mDirection.X;
	res[1][2] = mDirection.Y;
	res[2][2] = mDirection.Z;
	res[3][2] = -EdD;

	res[0][3] = 0.0f;
	res[1][3] = 0.0f;
	res[2][3] = 0.0f;
	res[3][3] = 1.0f;

	return res;
}
//----------------------------------------------------------------------------
SEMatrix4f SERTGICamera::GetProjectionTransform()
{
	SEMatrix4f res;

    // Assume the viewing volume is symmetric.

	if( mIsPerspective )
	{
        float halfAngleRadians = 0.5f * mUpFovDegrees * SEMathf::DEG_TO_RAD;
        float tanHalfFovy = SEMathf::Tan(halfAngleRadians);
        float invDDiff = 1.0f / (mFrustum[VF_DMAX] - mFrustum[VF_DMIN]);

        res[0][0] = 1.0f / (mAspectRatio * tanHalfFovy);
        res[1][1] = 1.0f / (tanHalfFovy);
        res[2][2] = (mFrustum[VF_DMAX] + mFrustum[VF_DMIN]) * invDDiff;
        res[2][3] = 1.0f;
        res[3][2] = -2.0f * mFrustum[VF_DMAX] * mFrustum[VF_DMIN] * invDDiff;
	}
	else
	{
        float invRDiff = 1.0f / (mFrustum[VF_RMAX] - mFrustum[VF_RMIN]);
        float invUDiff = 1.0f / (mFrustum[VF_UMAX] - mFrustum[VF_UMIN]);
        float invDDiff = 1.0f / (mFrustum[VF_DMAX] - mFrustum[VF_DMIN]);

        res[0][0] = 2.0f * invRDiff;
        res[1][1] = 2.0f * invUDiff;
        res[2][2] = invDDiff;
        res[3][0] = -(mFrustum[VF_RMIN] + mFrustum[VF_RMAX])*invRDiff;
        res[3][1] = -(mFrustum[VF_UMIN] + mFrustum[VF_UMAX])*invUDiff;
        res[3][2] = -mFrustum[VF_DMIN] * invDDiff;
        res[3][3] = 1.0f;
	}

	return res;
}
//----------------------------------------------------------------------------
bool SERTGICamera::IsPerspective() const
{
	return mIsPerspective;
}
//----------------------------------------------------------------------------
SEVector3f SERTGICamera::GetDirection() const
{
	return mDirection;
}
//----------------------------------------------------------------------------
SEVector3f SERTGICamera::GetRight() const
{
	return mRight;
}
//----------------------------------------------------------------------------
void SERTGICamera::SetAngle(const float& horizontalAngle, const float& verticalAngle)
{
    mRot.FromEulerAngles(SEVector3f(verticalAngle*SEMathf::DEG_TO_RAD,
        horizontalAngle*SEMathf::DEG_TO_RAD, 0.0f));
    mRot.Normalize();

    SEMatrix3f rotM;
    mRot.ToRotationMatrix(rotM);

    mRight.X = rotM[0][0];
    mRight.Y = rotM[1][0];
    mRight.Z = rotM[2][0];
    mUp.X = rotM[0][1];
    mUp.Y = rotM[1][1];
    mUp.Z = rotM[2][1];
    mDirection.X = rotM[0][2];
    mDirection.Y = rotM[1][2];
    mDirection.Z = rotM[2][2];
}
//----------------------------------------------------------------------------
void SERTGICamera::GetAngle(float& horizontalAngle, float& verticalAngle)
{
    SEMatrix3f rotation;
    rotation[0][0] = mRight.X;
    rotation[1][0] = mRight.Y;
    rotation[2][0] = mRight.Z;
    rotation[0][1] = mUp.X;
    rotation[1][1] = mUp.Y;
    rotation[2][1] = mUp.Z;
    rotation[0][2] = mDirection.X;
    rotation[1][2] = mDirection.Y;
    rotation[2][2] = mDirection.Z;

    mRot.FromRotationMatrix(rotation);
    mRot.Normalize();
    SEVector3f euler = mRot.GetEulerAngles();
	horizontalAngle = euler.Y*SEMathf::RAD_TO_DEG;
    verticalAngle = euler.X*SEMathf::RAD_TO_DEG;
}
//----------------------------------------------------------------------------
float SERTGICamera::GetFoV() const
{
	return mUpFovDegrees;
}
//----------------------------------------------------------------------------
void SERTGICamera::Rotate(const SEVector3f& eulerAngles)
{
    SEQuaternionf yaw, pitch;

    yaw.FromEulerAngles(
        SEVector3f(0.0f, eulerAngles.X*SEMathf::DEG_TO_RAD, 0.0f));
    pitch.FromEulerAngles(
        SEVector3f(eulerAngles.Y*SEMathf::DEG_TO_RAD, 0.0f, 0.0f));

	mRot = pitch * mRot * yaw;
    mRot.Normalize();

    SEMatrix3f rotM;
    mRot.ToRotationMatrix(rotM);

    mRight.X = rotM[0][0];
    mRight.Y = rotM[1][0];
    mRight.Z = rotM[2][0];
    mUp.X = rotM[0][1];
    mUp.Y = rotM[1][1];
    mUp.Z = rotM[2][1];
    mDirection.X = rotM[0][2];
    mDirection.Y = rotM[1][2];
    mDirection.Z = rotM[2][2];
}
//----------------------------------------------------------------------------
SEMatrix4f SERTGICamera::GetViewCacheTransform()
{
    SE_ASSERT(mRenderCache);
    return *mRenderCache->GetViewCache();
}
//----------------------------------------------------------------------------
SEMatrix4f SERTGICamera::GetProjectionCacheTransform()
{
    SE_ASSERT(mRenderCache);
    return *mRenderCache->GetProjCache();
}
//----------------------------------------------------------------------------
void SERTGICamera::UpdateRenderCache()
{
    SE_ASSERT(mRenderCache);
    mRenderCache->Update(this);
}
//----------------------------------------------------------------------------