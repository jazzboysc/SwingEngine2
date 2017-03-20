// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERenderingEnginePCH.h"
#include "SERenderCache.h"
#include "SERTGICamera.h"

#include <cstring>

using namespace Swing;

//----------------------------------------------------------------------------
SERenderCache::SERenderCache()
    :
    mCacheFlag(CF_Nothing),
    mCacheSize(0),
    mWorldCache(0),
    mViewCache(0),
    mProjCache(0),
    mCache(0)
{
}
//----------------------------------------------------------------------------
SERenderCache::~SERenderCache()
{
    SE_DELETE[] mCache;
}
//----------------------------------------------------------------------------
void SERenderCache::SetCacheFlag(SERenderCache::SECacheFlag flag)
{
    if( mCache )
    {
        SE_DELETE[] mCache;
        mCache = 0;
        mCacheSize = 0;
    }

    mCacheFlag = flag;
    if( mCacheFlag & SERenderCache::SECacheFlag::CF_WorldTransform )
    {
        mCacheSize += sizeof(SEMatrix4f);
    }

    if( mCacheFlag & SERenderCache::SECacheFlag::CF_ViewTransform )
    {
        mCacheSize += sizeof(SEMatrix4f);
    }

    if( mCacheFlag & SERenderCache::SECacheFlag::CF_ProjTransform )
    {
        mCacheSize += sizeof(SEMatrix4f);
    }

    if( mCacheSize > 0 )
    {
        mCache = SE_NEW unsigned char[mCacheSize];
        memset(mCache, 0, mCacheSize);

        SEMatrix4f* data = (SEMatrix4f*)mCache;
        if( mCacheFlag & SERenderCache::SECacheFlag::CF_WorldTransform )
        {
            mWorldCache = data++;
        }

        if( mCacheFlag & SERenderCache::SECacheFlag::CF_ViewTransform )
        {
            mViewCache = data++;
        }

        if( mCacheFlag & SERenderCache::SECacheFlag::CF_ProjTransform )
        {
            mProjCache = data++;
        }
    }
}
//----------------------------------------------------------------------------
void SERenderCache::Update(SESpatialInfo* spatialInfo)
{
    *mWorldCache = spatialInfo->mWorldTransform;
}
//----------------------------------------------------------------------------
void SERenderCache::Update(SERTGICamera* camera)
{
    SE_ASSERT(camera);
    *mViewCache = camera->GetViewTransform();
    *mProjCache = camera->GetProjectionTransform();
}
//----------------------------------------------------------------------------