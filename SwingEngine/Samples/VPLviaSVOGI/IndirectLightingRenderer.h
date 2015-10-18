#ifndef RTGI_IndirectLightingRenderer_H
#define RTGI_IndirectLightingRenderer_H

#include "SwingEngineRenderingEngine.h"
#include "SEScreenQuad.h"
#include "SETexture2D.h"
#include "SEStructuredBuffer.h"
#include "VPLGenerator.h"
#include "GridVoxelizer.h"
#include "SVOVoxelizer.h"

namespace Swing
{

#define RTGI_IndirectLightingRenderer_IndirectLighting_Name "IndirectLighting"

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/29/2014
//----------------------------------------------------------------------------
class IndirectLightingScreenQuad : public SEScreenQuad
{
public:
    IndirectLightingScreenQuad(SEMaterial* material);
    virtual ~IndirectLightingScreenQuad();

    // Implement base class interface.
    virtual void OnUpdateShaderConstants(int technique, int pass);
    virtual void OnGetShaderConstants();

    int VPLCount;
    int PatternSize;
    float BounceSingularity;
    SEAxisAlignedBox3f* SceneBB;
    int VoxelGridDim;
    bool VPLVisibilityTest;
    bool UseTC;

private:
    SEShaderUniform mVPLCountLoc;
    SEShaderUniform mPatternSizeLoc;
    SEShaderUniform mBounceSingularityLoc;
    SEShaderUniform mGBufferPositionSamplerLoc;
    SEShaderUniform mGBufferNormalSamplerLoc;
    SEShaderUniform mSceneBBCenterLoc;
    SEShaderUniform mSceneBBExtensionLoc;
    SEShaderUniform mDimLoc;
    SEShaderUniform mVPLVisibilityTestLoc;
    SEShaderUniform mFrameCounterLoc;
};

typedef SESmartPointer<IndirectLightingScreenQuad> IndirectLightingScreenQuadPtr;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 12/03/2014
//----------------------------------------------------------------------------
class IndirectLightingRenderer : public SESubRenderer
{
public:
    IndirectLightingRenderer(SEGPUDevice* device, SERenderSet* renderSet = 0);
    virtual ~IndirectLightingRenderer();

    void Initialize(SEGPUDevice* device, int width, int height, 
		SEBufferFormat format, int vplCount, int patternSize, SEAxisAlignedBox3f* sceneBB, 
		int voxelGridDim, SESubRenderer* gbufferSource, 
		VPLGenerator* vplGenerator, SEVoxelizer* voxelizer, bool useTC);

    void Render();

    void VPLVisibilityTest(bool value);

    SETexture2DPtr IndirectLightingTexture;

private:
    SEPipelineStateBlockPtr mPSB;
    IndirectLightingScreenQuadPtr mIndirectLightingScreenQuad;

    SEVoxelizer::SEVoxelizerType mVoxelizerType;

    // SVO voxelizer.
    SEStructuredBufferPtr mVoxelFragmentListBuffer;
    SEStructuredBufferPtr mSVOBuffer;

    bool mUseTC;
};

typedef SESmartPointer<IndirectLightingRenderer> IndirectLightingRendererPtr;

}

#endif