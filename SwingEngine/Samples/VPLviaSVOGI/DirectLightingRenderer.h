#ifndef RTGI_DirectLightingRenderer_H
#define RTGI_DirectLightingRenderer_H

#include "SwingEngineRenderingEngine.h"
#include "ShadowMapsGenerator.h"
#include "SELightManager.h"

namespace Swing
{

#define RTGI_DirectLightingRenderer_DirectLighting_Name "DirectLighting"

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/29/2014
//----------------------------------------------------------------------------
class DirectLightingScreenQuad : public SEScreenQuad
{
public:
    DirectLightingScreenQuad(SEMaterial* material);
    virtual ~DirectLightingScreenQuad();

    // Implement base class interface.
    virtual void OnUpdateShaderConstants(int technique, int pass);
    virtual void OnGetShaderConstants();

    bool ShowShadow;

private:
    SEShaderUniform mGBufferPositionSamplerLoc;
    SEShaderUniform mGBufferNormalSamplerLoc;
    SEShaderUniform mGBufferAlbedoSamplerLoc;
    SEShaderUniform mShadowMapSamplerLoc;
    SEShaderUniform mShowShadow;
};

typedef SESmartPointer<DirectLightingScreenQuad> DirectLightingScreenQuadPtr;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 12/05/2014
//----------------------------------------------------------------------------
class DirectLightingRenderer : public SESubRenderer
{
public:
    DirectLightingRenderer(SEGPUDevice* device, SERenderSet* renderSet = 0);
    virtual ~DirectLightingRenderer();

    void Initialize(SEGPUDevice* device, int width, int height, 
        SEBufferFormat format, SEGBufferRenderer* gbufferRenderer, 
        ShadowMapsGenerator* shadowMapsGenerator);

    void Render();

    void ShowShadow(bool value);

private:
    SEPipelineStateBlockPtr mPSB;
    DirectLightingScreenQuadPtr mDirectLightingScreenQuad;
};

typedef SESmartPointer<DirectLightingRenderer> DirectLightingRendererPtr;

}

#endif