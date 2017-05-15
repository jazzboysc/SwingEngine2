// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERenderingEnginePCH.h"
#include "SERTGILight.h"

using namespace Swing;

//----------------------------------------------------------------------------
SERTGILight::SERTGILight(SERTGILightType type)
	:
    Intensity(50.0f, 50.0f, 50.0f),
    mLightType(type),
    mProjector(0),
    mParams1(0.0f, 0.0f, 0.0f, 0.0f),
    mParams2(0.0f, 0.0f, 0.0f, 0.0f),
    Color(1.0f, 1.0f, 1.0f),
    ColorIntensity(10.0f),
    SampleCount(8),
    CanCastShadow(true),
    Visible(true),
    Width(1.0f),
    Height(1.0f),
    Spherical(false),
    HDRFile(false)
{
}
//----------------------------------------------------------------------------
SERTGILight::~SERTGILight()
{
    SE_DELETE mProjector;
    mLightMesh = 0;
}
//----------------------------------------------------------------------------
void SERTGILight::SetProjector(SERTGICamera* projector)
{
    mProjector = projector;
    mLocation = mProjector->GetLocation();
}
//----------------------------------------------------------------------------
SERTGICamera* SERTGILight::GetProjector() const
{
    return mProjector;
}
//----------------------------------------------------------------------------
void SERTGILight::SetLocation(const SEVector3f& location)
{
    if( mProjector )
    {
        mProjector->SetLocation(location);
    }
    mLocation = location;
}
//----------------------------------------------------------------------------
void SERTGILight::SetLightMesh(SETriangleMesh* lightMesh)
{
    mLightMesh = lightMesh;
}
//----------------------------------------------------------------------------
SETriangleMesh* SERTGILight::GetLightMesh() const
{
    return mLightMesh;
}
//----------------------------------------------------------------------------
void SERTGILight::RenderLightMesh(int technique, int pass, SESubRenderer* subRenderer)
{
    if( mLightMesh )
    {
        mLightMesh->Render(technique, pass, subRenderer);
    }
}
//----------------------------------------------------------------------------
void SERTGILight::OnUpdateLightBufferCache(SESceneLight* cache)
{
    SE_ASSERT(cache);
    if( cache )
    {
        cache->Intensity.X = Intensity.X;
        cache->Intensity.Y = Intensity.Y;
        cache->Intensity.Z = Intensity.Z;

        cache->WorldPositionAndType.X = mLocation.X;
        cache->WorldPositionAndType.Y = mLocation.Y;
        cache->WorldPositionAndType.Z = mLocation.Z;
        cache->WorldPositionAndType.W = float((int)mLightType);

        cache->Params1 = mParams1;
        cache->Params2 = mParams2;

        SE_ASSERT(mProjector);
        if( mProjector )
        {
            SEMatrix4f lightView = mProjector->GetViewTransform();
            cache->LightProjectorView = lightView;

            SEMatrix4f lightProj = mProjector->GetProjectionTransform();
            cache->LightProjectorProj = lightProj;

            mProjector->GetNearFarPlane(
                (float*)&cache->LightProjectorNearFar);
        }
    }
}
//----------------------------------------------------------------------------
SEMatrix3f SERTGILight::GetRotation() const
{
    if( mProjector )
    {
        return mProjector->GetRotation();
    }
    else
    {
        return SEMatrix3f::IDENTITY;
    }
}
//----------------------------------------------------------------------------
SEVector3f SERTGILight::GetLocation() const
{
    return mLocation;
}
//----------------------------------------------------------------------------
SEColorRGB SERTGILight::GetColor() const
{
    return Color;
}
//----------------------------------------------------------------------------
float SERTGILight::GetIntensity() const
{
    return ColorIntensity;
}
//----------------------------------------------------------------------------
SE_UInt32 SERTGILight::GetSampleCount() const
{
    return SampleCount;
}
//----------------------------------------------------------------------------
bool SERTGILight::CastShadow() const
{
    return CanCastShadow;
}
//----------------------------------------------------------------------------
bool SERTGILight::IsVisible() const
{
    return Visible;
}
//----------------------------------------------------------------------------
float SERTGILight::GetWidth() const
{
    return Width;
}
//----------------------------------------------------------------------------
float SERTGILight::GetHeight() const
{
    return Height;
}
//----------------------------------------------------------------------------
bool SERTGILight::IsSpherical() const
{
    return Spherical;
}
//----------------------------------------------------------------------------
bool SERTGILight::UseHDRFile() const
{
    return HDRFile;
}
//----------------------------------------------------------------------------
const char* SERTGILight::GetHDRFilePath() const
{
    return HDRFilePath.c_str();
}
//----------------------------------------------------------------------------