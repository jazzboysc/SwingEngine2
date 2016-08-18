#ifndef RTGI_Visualizer_H
#define RTGI_Visualizer_H

#include "SwingEngineRenderingEngine.h"
#include "GridVoxelizer.h"
#include "SVOVoxelizer.h"
#include "ShadowMapsGenerator.h"
#include "RSMRenderer.h"
#include "DirectLightingRenderer.h"
#include "IndirectLightingRenderer.h"
#include "VPLGenerator.h"
#include "GBufferSplitter.h"
#include "SplittedBufferMerger.h"
#include "GAwareFilter.h"
#include "SELightManager.h"
#include "SEPointSet.h"

namespace Swing
{

#define RTGI_Visualizer_IndirectCommandBuffer_Name "IndirectCommandBuffer"

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/06/2014
//----------------------------------------------------------------------------
class GatherVoxelBuffer : public SEComputeTask
{
public:
    GatherVoxelBuffer();
    ~GatherVoxelBuffer();

    // Implement base class interface.
    virtual void OnGetShaderConstants();
    virtual void OnPreDispatch(unsigned int pass);
    virtual void OnPostDispatch(unsigned int pass);

    SEAxisAlignedBox3f* SceneBB;

private:
    SEShaderUniform mSceneBBMinLoc;
    SEShaderUniform mSceneBBExtensionLoc;
};

typedef SESmartPointer<GatherVoxelBuffer> GatherVoxelBufferPtr;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/29/2014
//----------------------------------------------------------------------------
class VisualizerScreenQuad : public SEScreenQuad
{
public:
    VisualizerScreenQuad(SEMaterial* material);
    virtual ~VisualizerScreenQuad();

    // Implement base class interface.
    virtual void OnUpdateShaderConstants(int technique, int pass);
    virtual void OnGetShaderConstants();

    SETexture2DPtr TempTexture;
    SETexture2DPtr TempTexture2;
    SETexture2DPtr GBufferPositionTexture;
    SETexture2DPtr GBufferNormalTexture;
	SETexture2DPtr GBufferAlbedoTexture;
    SETexture2DArrayPtr TempTextureArray;

    int ShowMode;
    int TextureArrayIndex;
	int CurMipmapLevel;
	int MaxMipmapLevel;
    bool IsShadowMap;

    SEAxisAlignedBox3f* SceneBB;
    SEStructuredBufferPtr VoxelBuffer;
    int VoxelGridDim;
    bool UseToneMapper;
	bool ShowImportance;
    float MaxRadiance;

    float HDRExposure;
    float HDRMinExposure;
    float HDRMaxExposure;

private:
    // SM0
    SEShaderUniform mUseToneMapperSM0Loc;
    SEShaderUniform mTempSamplerSM0Loc;

    // SM1
    SEShaderUniform mTempSamplerArraySM1Loc;
    SEShaderUniform mTextureArrayIndexSM1Loc;
	SEShaderUniform mCurMipmapLevelSM1Loc;
    SEShaderUniform mIsShadowMapLoc;
    SEShaderUniform mUseToneMapperSM1Loc;
	SEShaderUniform mShowImportanceSM1Loc;
    SEShaderUniform mMaxRadianceSM1Loc;

    // SM2
    SEShaderUniform mTempSamplerSM2Loc;
    SEShaderUniform mTempSampler2SM2Loc;
	SEShaderUniform mAlbedoSamplerSM2Loc;
    SEShaderUniform mUseToneMapperSM2Loc;
    SEShaderUniform mExposureSM2Loc;

    // SM3
    SEShaderUniform mTempSamplerSM3Loc;
    SEShaderUniform mSceneBBMinSM3Loc;
    SEShaderUniform mSceneBBExtensionSM3Loc;

    // SM4
    SEShaderUniform mTempSamplerSM4Loc;
    SEShaderUniform mSceneBBMinSM4Loc;
    SEShaderUniform mSceneBBExtensionSM4Loc;
    SEShaderUniform mDimSM4Loc;
};

typedef SESmartPointer<VisualizerScreenQuad> VisualizerScreenQuadPtr;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/07/2014
//----------------------------------------------------------------------------
class VoxelCubeTriMesh : public SETriangleMesh
{
public:
    VoxelCubeTriMesh(SEMaterial* material, SERTGICamera* camera);
    virtual ~VoxelCubeTriMesh();

    // Implement base class interface.
    virtual void OnGetShaderConstants();
    virtual void OnUpdateShaderConstants(int technique, int pass);

    SEVector3f MaterialColor;
};

typedef SESmartPointer<VoxelCubeTriMesh> VoxelCubeTriMeshPtr;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 02/02/2015
//----------------------------------------------------------------------------
class SVOCubeMesh : public SEQuadMesh
{
public:
    SVOCubeMesh(SEMaterial* material, SERTGICamera* camera);
    virtual ~SVOCubeMesh();

    // Implement base class interface.
    virtual void OnGetShaderConstants();
    virtual void OnUpdateShaderConstants(int technique, int pass);

    SEAxisAlignedBox3f* SceneBB;
    bool IsLeafPass;

private:
    SEShaderUniform mSceneBBMinLoc;
    SEShaderUniform mVoxelExtensionLoc;
    SEShaderUniform mIsLeafPassLoc;
};

typedef SESmartPointer<SVOCubeMesh> SVOCubeMeshPtr;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 03/15/2015
//----------------------------------------------------------------------------
class VPLPointSet : public SEPointSet
{
public:
    VPLPointSet(SEMaterial* material, SERTGICamera* camera);
    ~VPLPointSet();

    // Implement base class interface.
    virtual void OnGetShaderConstants();
    virtual void OnUpdateShaderConstants(int technique, int pass);

    int CurVPLSubsetID;
    int VPLCount;
    int PatternSize;
    bool ShowVPLSubset;
    bool ShowVPLFluxContrast;

private:
    SEShaderUniform mViewLoc;
    SEShaderUniform mProjLoc;
    SEShaderUniform mCurVPLSubsetIDLoc;
    SEShaderUniform mVPLCountLoc;
    SEShaderUniform mPatternSizeLoc;
    SEShaderUniform mShowVPLSubsetLoc;
    SEShaderUniform mShowVPLFluxContrastLoc;
};

typedef SESmartPointer<VPLPointSet> VPLPointSetPtr;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 12/05/2014
//----------------------------------------------------------------------------
class Visualizer : public SESubRenderer
{
public:
    enum ShowMode
    {
        SM_VoxelBuffer,
        SM_VoxelGrid,
        SM_SVOGrid,
        SM_Shadow,
        SM_VPLShadow,
        SM_GBufferPosition,
        SM_GBufferNormal,
        SM_GBufferAlbedo,
        SM_GBufferRPC,
		SM_GBufferSplitterPosition,
		SM_GBufferSplitterNormal,
        SM_RSMPosition,
        SM_RSMNormal,
        SM_RSMFlux,
        SM_DirectLighting,
        SM_IndirectLighting,
        SM_MergedBuffer,
        SM_FilteredIndirectLighting,
        SM_Final
    };

    Visualizer(SEGPUDevice* device, SERenderSet* renderSet = 0);
    virtual ~Visualizer();

    void Initialize(SEGPUDevice* device, SEVoxelizer* voxelizer, 
        VPLGenerator* vplGenerator, ShadowMapsGenerator* shadowMapsGenerator, 
        SEGBufferRenderer* gbufferRenderer, GBufferSplitter* gbufferSplitter, 
		RSMRenderer* rsmRenderer, DirectLightingRenderer* directLightingRenderer, 
        IndirectLightingRenderer* indirectLightingRenderer, 
        SplittedBufferMerger* bufferMerger, GAwareFilter* filter, SEAxisAlignedBox3f* sceneBB, 
		int voxelGridDim, int voxelGridLocalGroupDim, SERTGICamera* mainCamera, 
		int patternSize, int vplCount, float toneMapperMaxRadiance, 
		SELightManager* lightManager);

    void Render(int technique, int pass);

    // Implement base class interface.
    void OnRender(int technique, int pass, SERTGICamera* camera);

    void SetShowMode(ShowMode mode);

    int GetCurVPLSubsetIndex() const;
    void SetCurVPLSubsetIndex(int value);

    int GetCurTextureMapIndex() const;
    void SetCurTextureMapIndex(int value);

    void SetShowVPL(bool value);
    void SetShowVPLSubset(bool value);
    void SetShowVPLFluxContrast(bool value);

	int GetCurMipmapLevel() const;
	void SetCurMipmapLevel(int value);

	void SetShowRSMFluxImportance(bool value);

    void SetUseHDRToneMapping(bool value);
    void SetHDRExposure(float value);

private:
    SEVoxelizer::SEVoxelizerType mVoxelizerType;
    int mVoxelGridDim;
    int mVoxelGridLocalGroupDim;
    int mGlobalDim;

    ShowMode mShowMode;
    bool mShowVPL;
    int mVPLSubsetCount;
    int mCurVPLSubsetIndex;
    int mShadowMapCount;
    int mCurShadowMapIndex;

    VisualizerScreenQuadPtr mScreenQuad;

    SETexture2DArrayPtr mShadowMapsTexture;
    SETexture2DPtr mGBufferPositionTexture;
    SETexture2DPtr mGBufferNormalTexture;
    SETexture2DPtr mGBufferAlbedoTexture;
    SETexture2DPtr mGBufferRPCTexture;
	SETexture2DPtr mGBufferSplitterPositionTexture;
	SETexture2DPtr mGBufferSplitterNormalTexture;
    SETexture2DArrayPtr mRSMPositionTextureArray;
    SETexture2DArrayPtr mRSMNormalTextureArray;
    SETexture2DArrayPtr mRSMFluxTextureArray;
    SETexture2DPtr mDirectLightingTexture;
    SETexture2DPtr mIndirectLightingTexture;
    SETexture2DPtr mMergedBufferTexture;
	SETexture2DPtr mFilteredIndirectLightingTexture;

    // VPL visualization.
    SEStructuredBufferPtr mVPLBuffer;
    VPLPointSetPtr mVPLPointSet;
	int mVPLViewportWidth, mVPLViewportHeight;

    // Grid voxelizer visualization.
    SEStructuredBufferPtr mVoxelBuffer;
    VoxelCubeTriMeshPtr mVoxelCubeModel;
    GatherVoxelBufferPtr mGatherVoxelBufferTask;

    // SVO voxelizer visualization.
    SEStructuredBufferPtr mVoxelFragmentListBuffer;
    SEStructuredBufferPtr mSVOBuffer;
    SEUniformBufferPtr mSVOUniformBuffer;
    SVOCubeMeshPtr mSVONodeCubeModel;

	SELightManagerPtr mLightManager;
	SERTGICamera* mMainCamera;

    SEPipelineStateBlockPtr mCommonPSB;
    SEPipelineStateBlockPtr mShowSVOGridPSB;
    SEPipelineStateBlockPtr mShowVPLPSB;
};

typedef SESmartPointer<Visualizer> VisualizerPtr;

}

#endif