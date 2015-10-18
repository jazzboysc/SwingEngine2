// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SESpatialInfo.h"

using namespace Swing;

//----------------------------------------------------------------------------
SESpatialInfo::SESpatialInfo()
    :
    mWorldScale(1.0f, 1.0f, 1.0f)
{
    mWorldTransform.Identity();
}
//----------------------------------------------------------------------------
SESpatialInfo::~SESpatialInfo()
{
}
//----------------------------------------------------------------------------
void SESpatialInfo::SetWorldTransform(const SEMatrix4f& worldTrans)
{
    mWorldTransform = worldTrans;
    mWorldTransform[0][0] *= mWorldScale[0];
    mWorldTransform[0][1] *= mWorldScale[1];
    mWorldTransform[0][2] *= mWorldScale[2];
    mWorldTransform[1][0] *= mWorldScale[0];
    mWorldTransform[1][1] *= mWorldScale[1];
    mWorldTransform[1][2] *= mWorldScale[2];
    mWorldTransform[2][0] *= mWorldScale[0];
    mWorldTransform[2][1] *= mWorldScale[1];
    mWorldTransform[2][2] *= mWorldScale[2];
}
//----------------------------------------------------------------------------
void SESpatialInfo::SetWorldTranslation(const SEVector3f& translation)
{
    mWorldTransform[3][0] = translation.X;
    mWorldTransform[3][1] = translation.Y;
    mWorldTransform[3][2] = translation.Z;
}
//----------------------------------------------------------------------------
void SESpatialInfo::SetWorldScale(const SEVector3f& scale)
{
    mWorldTransform[0][0] /= mWorldScale[0];
    mWorldTransform[0][1] /= mWorldScale[1];
    mWorldTransform[0][2] /= mWorldScale[2];
    mWorldTransform[1][0] /= mWorldScale[0];
    mWorldTransform[1][1] /= mWorldScale[1];
    mWorldTransform[1][2] /= mWorldScale[2];
    mWorldTransform[2][0] /= mWorldScale[0];
    mWorldTransform[2][1] /= mWorldScale[1];
    mWorldTransform[2][2] /= mWorldScale[2];
    mWorldTransform[0][0] *= scale[0];
    mWorldTransform[0][1] *= scale[1];
    mWorldTransform[0][2] *= scale[2];
    mWorldTransform[1][0] *= scale[0];
    mWorldTransform[1][1] *= scale[1];
    mWorldTransform[1][2] *= scale[2];
    mWorldTransform[2][0] *= scale[0];
    mWorldTransform[2][1] *= scale[1];
    mWorldTransform[2][2] *= scale[2];
    mWorldScale = scale;
}
//----------------------------------------------------------------------------