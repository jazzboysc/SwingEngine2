// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SERenderObject.h"

using namespace Swing;

//----------------------------------------------------------------------------
SERenderObject::SERenderObject(SEMaterial* material)
{
    mPrimitive = SE_NEW SEPrimitive();
    mMaterial = material;

    // SEMaterial object cannot be shared between render objects.
    if( mMaterial )
    {
        SE_ASSERT(!material->mRenderObject);
        mMaterial->mRenderObject = this;
    }
}
//----------------------------------------------------------------------------
SERenderObject::~SERenderObject()
{
    mPrimitive = 0;
	mMaterial = 0;
    mSpatialInfo = 0;
    mRenderCache = 0;
}
//----------------------------------------------------------------------------
void SERenderObject::SetCamera(SERTGICamera* camera)
{
    mCamera = camera;
}
//----------------------------------------------------------------------------
SERTGICamera* SERenderObject::GetCamera() const
{
    return mCamera;
}
//----------------------------------------------------------------------------
int SERenderObject::GetVoxelizerRasterDimension(SEVoxelizer*)
{
    SE_ASSERT(false);
    return 0;
}
//----------------------------------------------------------------------------
SEPrimitive* SERenderObject::GetPrimitive() const
{
    return mPrimitive;
}
//----------------------------------------------------------------------------
SEMaterial* SERenderObject::GetMaterial() const
{
    return mMaterial;
}
//----------------------------------------------------------------------------
void SERenderObject::SetWorldTransform(const SEMatrix4f& worldTrans)
{
    SE_ASSERT(mSpatialInfo);
    mSpatialInfo->SetWorldTransform(worldTrans);
}
//----------------------------------------------------------------------------
SEMatrix4f SERenderObject::GetWorldTransform() const
{
    SE_ASSERT(mSpatialInfo);
    return mSpatialInfo->GetWorldTransform();
}
//----------------------------------------------------------------------------
void SERenderObject::SetWorldTranslation(const SEVector3f& translation)
{
    SE_ASSERT(mSpatialInfo);
    mSpatialInfo->SetWorldTranslation(translation);
}
//----------------------------------------------------------------------------
SEVector3f SERenderObject::GetWorldTranslation() const
{
    SE_ASSERT(mSpatialInfo);
    return mSpatialInfo->GetWorldTranslation();
}
//----------------------------------------------------------------------------
void SERenderObject::SetWorldScale(const SEVector3f& scale)
{
    SE_ASSERT(mSpatialInfo);
    mSpatialInfo->SetWorldScale(scale);
}
//----------------------------------------------------------------------------
SEVector3f SERenderObject::GetWorldScale() const
{
    SE_ASSERT(mSpatialInfo);
    return mSpatialInfo->GetWorldScale();
}
//----------------------------------------------------------------------------
void SERenderObject::SetRenderCache(SERenderCache* renderCache)
{
    mRenderCache = renderCache;
}
//----------------------------------------------------------------------------
SERenderCache* SERenderObject::GetRenderCache()
{
    return mRenderCache;
}
//----------------------------------------------------------------------------
void SERenderObject::UpdateRenderCache()
{
    SE_ASSERT(mRenderCache && mSpatialInfo);
    mRenderCache->Update(mSpatialInfo);
}
//----------------------------------------------------------------------------