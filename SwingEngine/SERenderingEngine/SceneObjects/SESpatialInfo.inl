// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

//----------------------------------------------------------------------------
SEMatrix4f SESpatialInfo::GetWorldTransform() const
{
    return mWorldTransform;
}
//----------------------------------------------------------------------------
SEVector3f SESpatialInfo::GetWorldTranslation() const
{
    SEVector3f res;
    res.X = mWorldTransform[3][0];
    res.Y = mWorldTransform[3][1];
    res.Z = mWorldTransform[3][2];
    return res;
}
//----------------------------------------------------------------------------
SEVector3f SESpatialInfo::GetWorldScale() const
{
    return mWorldScale;
}
//----------------------------------------------------------------------------