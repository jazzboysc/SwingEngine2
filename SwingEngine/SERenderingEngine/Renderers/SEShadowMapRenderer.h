// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_ShadowMapRenderer_H
#define Swing_ShadowMapRenderer_H

#include "SERenderingEngineLIB.h"
#include "SESubRenderer.h"

namespace Swing
{

#define RTGI_ShadowMapRenderer_ShadowMap_Name "ShadowMap"

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 12/05/2014
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SEShadowMapRenderer : public SESubRenderer
{
public:
    SEShadowMapRenderer(SEGPUDevice* device, SERenderSet* renderSet = 0);
    virtual ~SEShadowMapRenderer();

    void CreateShadowMap(int width, int height, SEBufferFormat format);
    void Render(int technique, int pass, SERTGICamera* camera);

protected:
    SEPipelineStateBlockPtr mPSB;
};

typedef SESmartPointer<SEShadowMapRenderer> SEShadowMapRendererPtr;

}

#endif