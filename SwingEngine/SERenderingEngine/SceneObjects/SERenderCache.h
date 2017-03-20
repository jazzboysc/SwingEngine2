// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_RenderCache_H
#define Swing_RenderCache_H

#include "SERenderingEngineLIB.h"
#include "SEReferencable.h"
#include "SESpatialInfo.h"

namespace Swing
{

class SERTGICamera;
//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 03/07/2015
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SERenderCache : public SEReferencable
{
public:
    enum SECacheFlag
    {
        CF_Nothing        = 0,
        CF_WorldTransform = 1,
        CF_ViewTransform  = 2,
        CF_ProjTransform  = 4
    };

    SERenderCache();
    virtual ~SERenderCache();

    void SetCacheFlag(SECacheFlag flag);

    inline SECacheFlag GetCacheFlag() const;
    inline unsigned int GetCacheSize() const;
    inline SEMatrix4f* GetWorldCache() const;
    inline SEMatrix4f* GetViewCache() const;
    inline SEMatrix4f* GetProjCache() const;

    void Update(SESpatialInfo* spatialInfo);
    void Update(SERTGICamera* camera);

protected:
    SECacheFlag mCacheFlag;
    unsigned int mCacheSize;
    SEMatrix4f* mWorldCache;
    SEMatrix4f* mViewCache;
    SEMatrix4f* mProjCache;
    unsigned char* mCache;
};

typedef SESmartPointer<SERenderCache> SERenderCachePtr;

#include "SERenderCache.inl"

}

#endif