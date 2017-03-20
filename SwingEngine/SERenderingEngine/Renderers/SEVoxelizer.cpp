// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#include "SERenderingEnginePCH.h"
#include "SEVoxelizer.h"

using namespace Swing;

//----------------------------------------------------------------------------
SEVoxelizer::SEVoxelizer(SEGPUDevice* device, SERenderSet* renderSet)
    :
    SESubRenderer(device, renderSet),
    mVoxelizerType(VT_Unknown)
{
    RasterizerDimBias = 0;
    VoxelGridDim = 0;
}
//----------------------------------------------------------------------------
SEVoxelizer::~SEVoxelizer()
{
}
//----------------------------------------------------------------------------
void SEVoxelizer::Render(int technique, int pass)
{
    SESubRenderer::Render(technique, pass, SRO_GenericBuffer, 0);
}
//----------------------------------------------------------------------------
float SEVoxelizer::GetSceneBBMaxLength() const
{
    return mSceneBBMaxLength;
}
//----------------------------------------------------------------------------
void SEVoxelizer::VoxelizeScene(int technique, int pass)
{
    SEVector3f sceneBBLen = mSceneBB->GetExtension() * 2.0f;
    mSceneBBMaxLength = SE_MAX(sceneBBLen.X,
        SE_MAX(sceneBBLen.Y, sceneBBLen.Z));

    SE_ASSERT(mRenderSet);
    int renderObjectCount = mRenderSet->GetRenderObjectCount();
    for( int i = 0; i < renderObjectCount; ++i )
    {
        SERenderObject* object = mRenderSet->GetRenderObject(i);
        object->Render(technique, pass, this);
    }
}
//----------------------------------------------------------------------------