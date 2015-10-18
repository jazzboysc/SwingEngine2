// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_SpatialInfo_H
#define Swing_SpatialInfo_H

#include "SERenderingEngineLIB.h"
#include "SEReferencable.h"
#include "SEVector3.h"
#include "SEMatrix4.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 03/08/2015
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SESpatialInfo : public SEReferencable
{
public:
    SESpatialInfo();
    ~SESpatialInfo();

    void SetWorldTransform(const SEMatrix4f& worldTrans);
    inline SEMatrix4f GetWorldTransform() const;
    void SetWorldTranslation(const SEVector3f& translation);
    inline SEVector3f GetWorldTranslation() const;
    void SetWorldScale(const SEVector3f& scale);
    inline SEVector3f GetWorldScale() const;

    SEMatrix4f Offset;

private:
    friend class SERenderCache;
    SEMatrix4f mWorldTransform;
    SEVector3f mWorldScale;
};

typedef SESmartPointer<SESpatialInfo> SESpatialInfoPtr;

#include "SESpatialInfo.inl"

}

#endif