#ifndef RTGI_GBufferSplitter_H
#define RTGI_GBufferSplitter_H

#include "SwingEngineRenderingEngine.h"

namespace Swing
{

#define RTGI_GBufferSplitter_Position_Name "SplitterPosition"
#define RTGI_GBufferSplitter_Normal_Name   "SplitterNormal"

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 07/06/2015
//----------------------------------------------------------------------------
class GBufferSplitterScreenQuad : public SEScreenQuad
{
public:
    GBufferSplitterScreenQuad(SEMaterial* material);
    virtual ~GBufferSplitterScreenQuad();

    // Implement base class interface.
    virtual void OnUpdateShaderConstants(int technique, int pass);
    virtual void OnGetShaderConstants();

	SEVector4f TileInfo;  // x:tile width y:tile height z:m w:n

private:
	SEShaderUniform mTileInfoLoc;
	SEShaderUniform mGBufferPositionSamplerLoc;
	SEShaderUniform mGBufferNormalSamplerLoc;
};

typedef SESmartPointer<GBufferSplitterScreenQuad> GBufferSplitterScreenQuadPtr;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 07/06/2015
//----------------------------------------------------------------------------
class GBufferSplitter : public SESubRenderer
{
public:
    GBufferSplitter(SEGPUDevice* device, SERenderSet* renderSet = 0);
    virtual ~GBufferSplitter();

    void Initialize(SEGPUDevice* device, SEGBufferRenderer* gbufferRenderer,
        int m, int n);

    void Render();

private:
    SEPipelineStateBlockPtr mPSB;
    GBufferSplitterScreenQuadPtr mGBufferSplitterScreenQuad;
	int mTileM, mTileN;
	int mTileWidth, mTileHeight;
};

typedef SESmartPointer<GBufferSplitter> GBufferSplitterPtr;

}

#endif