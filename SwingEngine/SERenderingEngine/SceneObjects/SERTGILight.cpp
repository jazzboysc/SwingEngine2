// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

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
    mParams2(0.0f, 0.0f, 0.0f, 0.0f)
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
SEVector3f SERTGILight::GetLocation() const
{
    return mLocation;
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