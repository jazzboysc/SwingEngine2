// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SERenderSequence.h"

using namespace Swing;

//----------------------------------------------------------------------------
SERenderSequence::SERenderSequence(SEMaterial* material)
    :
    SERenderObject(material)
{
    mActiveObjectIndex = 0;
    mFrequence = 1.0f;
}
//----------------------------------------------------------------------------
SERenderSequence::~SERenderSequence()
{
    for( int i = 0; i < (int)mObjects.size(); ++i )
    {
        mObjects[i] = 0;
    }
}
//----------------------------------------------------------------------------
void SERenderSequence::SetCamera(SERTGICamera* camera)
{
    mObjects[mActiveObjectIndex]->SetCamera(camera);
}
//----------------------------------------------------------------------------
SERTGICamera* SERenderSequence::GetCamera() const
{
    return mObjects[mActiveObjectIndex]->GetCamera();
}
//----------------------------------------------------------------------------
int SERenderSequence::GetVoxelizerRasterDimension(SEVoxelizer* voxelizer)
{
    return mObjects[mActiveObjectIndex]->GetVoxelizerRasterDimension(
        voxelizer);
}
//----------------------------------------------------------------------------
SEPrimitive* SERenderSequence::GetPrimitive() const
{
    return mObjects[mActiveObjectIndex]->GetPrimitive();
}
//----------------------------------------------------------------------------
SEMaterial* SERenderSequence::GetMaterial() const
{
    return mObjects[mActiveObjectIndex]->GetMaterial();
}
//----------------------------------------------------------------------------
void SERenderSequence::SetWorldTransform(const SEMatrix4f& worldTrans)
{
    for( int i = 0; i < (int)mObjects.size(); ++i )
    {
        mObjects[i]->SetWorldTransform(worldTrans);
    }
}
//----------------------------------------------------------------------------
SEMatrix4f SERenderSequence::GetWorldTransform() const
{
    return mObjects[mActiveObjectIndex]->GetWorldTransform();
}
//----------------------------------------------------------------------------
void SERenderSequence::SetWorldTranslation(const SEVector3f& translation)
{
    for( int i = 0; i < (int)mObjects.size(); ++i )
    {
        mObjects[i]->SetWorldTranslation(translation);
    }
}
//----------------------------------------------------------------------------
SEVector3f SERenderSequence::GetWorldTranslation() const
{
    return mObjects[mActiveObjectIndex]->GetWorldTranslation();
}
//----------------------------------------------------------------------------
void SERenderSequence::SetWorldScale(const SEVector3f& scale)
{
    for( int i = 0; i < (int)mObjects.size(); ++i )
    {
        mObjects[i]->SetWorldScale(scale);
    }
}
//----------------------------------------------------------------------------
SEVector3f SERenderSequence::GetWorldScale() const
{
    return mObjects[mActiveObjectIndex]->GetWorldScale();
}
//----------------------------------------------------------------------------
void SERenderSequence::SetRenderCache(SERenderCache* renderCache)
{
    mObjects[mActiveObjectIndex]->SetRenderCache(renderCache);
}
//----------------------------------------------------------------------------
SERenderCache* SERenderSequence::GetRenderCache()
{
    return mObjects[mActiveObjectIndex]->GetRenderCache();
}
//----------------------------------------------------------------------------
void SERenderSequence::UpdateRenderCache()
{
    mObjects[mActiveObjectIndex]->UpdateRenderCache();
}
//----------------------------------------------------------------------------
void SERenderSequence::Render(int technique, int pass, SESubRenderer* subRenderer)
{
    mObjects[mActiveObjectIndex]->Render(technique, pass, subRenderer);
}
//----------------------------------------------------------------------------
void SERenderSequence::OnUpdateShaderConstants(int technique, int pass)
{
    mObjects[mActiveObjectIndex]->OnUpdateShaderConstants(technique, pass);
}
//----------------------------------------------------------------------------
void SERenderSequence::OnRender(SERenderPass* pass, SERenderPassInfo* renderPassInfo)
{
    mObjects[mActiveObjectIndex]->OnRender(pass, renderPassInfo);
}
//----------------------------------------------------------------------------
void SERenderSequence::OnEnableBuffers()
{
    mObjects[mActiveObjectIndex]->OnEnableBuffers();
}
//----------------------------------------------------------------------------
void SERenderSequence::OnDisableBuffers()
{
    mObjects[mActiveObjectIndex]->OnDisableBuffers();
}
//----------------------------------------------------------------------------
void SERenderSequence::Update(unsigned int frame)
{
    int sequenceSize = (int)mObjects.size();
    SetActiveRenderObject(unsigned int((float)frame*mFrequence) % 
        sequenceSize);
}
//----------------------------------------------------------------------------
void SERenderSequence::SetActiveRenderObject(unsigned int index)
{
    SE_ASSERT(index >= 0 && index < mObjects.size());
    mActiveObjectIndex = index;
}
//----------------------------------------------------------------------------
void SERenderSequence::SetFrequence(float value)
{
    SE_ASSERT(value > 0.0f);
    mFrequence = value;
}
//----------------------------------------------------------------------------
void SERenderSequence::AddRenderObject(SERenderObject* renderObject)
{
    mObjects.push_back(renderObject);
}
//----------------------------------------------------------------------------