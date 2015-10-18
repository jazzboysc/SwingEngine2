// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

//----------------------------------------------------------------------------
inline void SEReferencable::IncrementReferenceCount()
{
    SE_AtomicAdd(&mReferenceCount, 1);
}
//----------------------------------------------------------------------------
inline int SEReferencable::GetReferenceCount() const
{
    return mReferenceCount;
}
//----------------------------------------------------------------------------