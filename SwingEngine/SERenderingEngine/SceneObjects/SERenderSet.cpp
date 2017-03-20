// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERenderingEnginePCH.h"
#include "SERenderSet.h"

using namespace Swing;

//----------------------------------------------------------------------------
SERenderSet::SERenderSet()
{
}
//----------------------------------------------------------------------------
SERenderSet::~SERenderSet()
{
}
//----------------------------------------------------------------------------
void SERenderSet::AddRenderObject(SERenderObject* renderObject)
{
    mRenderObjects.push_back(renderObject);
}
//----------------------------------------------------------------------------
int SERenderSet::GetRenderObjectCount() const
{
    return (int)mRenderObjects.size();
}
//----------------------------------------------------------------------------
SERenderObject* SERenderSet::GetRenderObject(int i) const
{
    SE_ASSERT( i >= 0 && i < (int)mRenderObjects.size() );
    return mRenderObjects[i];
}
//----------------------------------------------------------------------------
void SERenderSet::UpdateRenderCache()
{
    for( int i = 0; i < (int)mRenderObjects.size(); ++i )
    {
        mRenderObjects[i]->UpdateRenderCache();
    }
}
//----------------------------------------------------------------------------