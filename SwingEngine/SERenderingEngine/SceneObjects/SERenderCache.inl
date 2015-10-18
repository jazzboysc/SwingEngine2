// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

//----------------------------------------------------------------------------
SERenderCache::SECacheFlag SERenderCache::GetCacheFlag() const
{
    return mCacheFlag;
}
//----------------------------------------------------------------------------
unsigned int SERenderCache::GetCacheSize() const
{
    return mCacheSize;
}
//----------------------------------------------------------------------------
SEMatrix4f* SERenderCache::GetWorldCache() const
{
    return mWorldCache;
}
//----------------------------------------------------------------------------
SEMatrix4f* SERenderCache::GetViewCache() const
{
    return mViewCache;
}
//----------------------------------------------------------------------------
SEMatrix4f* SERenderCache::GetProjCache() const
{
    return mProjCache;
}
//----------------------------------------------------------------------------