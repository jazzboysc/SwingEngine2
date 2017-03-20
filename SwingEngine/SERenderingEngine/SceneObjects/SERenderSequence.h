// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_RenderSequence_H
#define Swing_RenderSequence_H

#include "SERenderingEngineLIB.h"
#include "SERenderObject.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 03/30/2015
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SERenderSequence : public SERenderObject
{
public:
    SERenderSequence(SEMaterial* material);
    virtual ~SERenderSequence();

    // Implement base class interface.
    virtual void SetCamera(SERTGICamera* camera);
    virtual SERTGICamera* GetCamera() const;
    virtual int GetVoxelizerRasterDimension(SEVoxelizer* voxelizer);
    virtual SEPrimitive* GetPrimitive() const;
    virtual SEMaterial* GetMaterial() const;
    virtual void SetWorldTransform(const SEMatrix4f& worldTrans);
    virtual SEMatrix4f GetWorldTransform() const;
    virtual void SetWorldTranslation(const SEVector3f& translation);
    virtual SEVector3f GetWorldTranslation() const;
    virtual void SetWorldScale(const SEVector3f& scale);
    virtual SEVector3f GetWorldScale() const;
    virtual void SetRenderCache(SERenderCache* renderCache);
    virtual SERenderCache* GetRenderCache();
    virtual void UpdateRenderCache();
    virtual void Render(int technique, int pass,
        SESubRenderer* subRenderer = 0);
    virtual void OnUpdateShaderConstants(int technique, int pass);
    virtual void OnRender(SERenderPass* pass, SERenderPassInfo* renderPassInfo);
    virtual void OnEnableBuffers();
    virtual void OnDisableBuffers();

    void Update(unsigned int frame);
    void SetActiveRenderObject(unsigned int index);
    void SetFrequence(float value);
    void AddRenderObject(SERenderObject* renderObject);

protected:
    std::vector<SERenderObjectPtr> mObjects;
    unsigned int mActiveObjectIndex;
    float mFrequence;
};

typedef SESmartPointer<SERenderSequence> SERenderSequencePtr;

}

#endif