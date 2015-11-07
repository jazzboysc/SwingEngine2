#ifndef RTGI_VPLviaSVOGIApp_H
#define RTGI_VPLviaSVOGIApp_H

#include "SEWglApplication.h"
#include "SELightManager.h"
#include "SceneMesh.h"
#include "SEGPUTimer.h"
#include "ShadowMapsGenerator.h"
#include "RSMRenderer.h"
#include "VPLGenerator.h"
#include "DirectLightingRenderer.h"
#include "IndirectLightingRenderer.h"
#include "GridVoxelizer.h"
#include "SVOVoxelizer.h"
#include "Visualizer.h"
#include "SEGBufferRenderer.h"
#include "GBufferSplitter.h"
#include "SplittedBufferMerger.h"
#include "GAwareFilter.h"
#include "SERenderSequence.h"
#include "SEGPUDeviceInspector.h"
#include "FormEventListener.h"

//#define SHOW_TIMING
//#define DEMO_SPONZA_SCENE
#define DEMO_CORNELL_SCENE

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/29/2014
//----------------------------------------------------------------------------
class VPLviaSVOGI : public SEWglApplication, GUIFramework::FormEventListener
{
public:
	VPLviaSVOGI(int width = 768, int height = 768);
	~VPLviaSVOGI();

    // Override SEWglApplication interface.
	void Initialize(SEGPUDevice* device) override;
	void ProcessInput() override;
	void FrameFunc() override;
	void Terminate() override;

    // Override FormEventListener interface.
    void OnRadioButtonClick(System::Object^  sender, System::EventArgs^  e) override;
    void OnCheckBoxClick(System::Object^  sender, System::EventArgs^  e) override;
    void OnButtonClick(System::Object^  sender, System::EventArgs^  e) override;

private:
	void CreateSceneLights(SEGPUDevice* device);
	SEMaterialTemplate* CreateSceneModelMaterialTemplate();
	void CreateScene(SEMaterialTemplate* sceneMT, SEGPUDevice* device, SEAxisAlignedBox3f* sceneBB,
		SERenderSet* voxelizerSet, SERenderSet* gbufferSet,
		SERenderSet* shadowMapSet, SERenderSet* rsmSet);
	void CreateGUI();

    friend class SceneMesh;
    friend class SVOCubeMesh;

    bool mIsRotatingModel;
    bool mIsWireframe;

    SEGPUDeviceInspectorPtr mDeviceInspector;

    // Scene lights manager.
    SELightManagerPtr mLightManager;

    // GI temporal coherence.
    bool mUseTC;

    // Scene voxelization.
    enum { VOXEL_DIMENSION = 128 };
    enum { VOXEL_LOCAL_GROUP_DIM = 8 };
    SEVoxelizer::SEVoxelizerType mVoxelizerType;
    SEVoxelizerPtr mVoxelizer;

    // G-buffer.
    SEGBufferRendererPtr mGBufferRenderer;

	// G-buffer splitter.
	GBufferSplitterPtr mGBufferSplitter;

    // Scene shadow map.
    enum { SHADOWMAP_DIMENSION = 1024 };
    ShadowMapsGeneratorPtr mShadowMapRenderer;

    // Direct lighting.
    DirectLightingRendererPtr mDirectLightingRenderer;

    // Indirect lighting.
    IndirectLightingRendererPtr mIndirectLightingRenderer;

	// Splitted buffer merger.
	SplittedBufferMergerPtr mSplittedBufferMerger;

	// G-aware filter.
	GAwareFilterPtr mGAwareFilter;

    // Scene light RSM.
    enum { RSM_POINT_LIGHT_FACE_COUNT = 6 };
    enum { RSM_DIMENSION = 128 };
    RSMRendererPtr mRSMRenderer;

    // Sample RSM.
    VPLGeneratorPtr mVPLGenerator;

    // VPL stuff.
    enum { INTERLEAVED_PATTERN_SIZE = 4 };
    enum { VPL_SAMPLE_COUNT = 512 };

    // Visualizer.
    VisualizerPtr mVisualizer;

    friend class SceneMesh;
    enum SceneModelPass
    {
        SMP_Voxelization = 0,
        SMP_PointLightShadowMap,
        SMP_SpotLightShadowMap = SMP_PointLightShadowMap + 1,
        SMP_GBuffer,
        SMP_PointLightRSM,
        SMP_SpotLightRSM = SMP_PointLightRSM + 1
    };

    // Scene models.
	SceneMeshPtr mGround;
	SceneMeshPtr mCeiling;
    SceneMeshPtr mBackWall;
    SceneMeshPtr mLeftWall;
    SceneMeshPtr mRightWall;
    SceneMeshPtr mModel;
    SERenderSequencePtr mModel2Sequence;
	std::vector<SceneMeshPtr> mSponza;
    int mSceneTriangleCount;

	// Render sets.
    SERenderSetPtr mGBufferObjects;
	SERenderSetPtr mRSMObjects;
    SERenderSetPtr mShadowCasters;
    SERenderSetPtr mVoxelizedObjects;

    SEAxisAlignedBox3f mSceneBB;
    SEGPUTimerPtr mTimer;
    SEPipelineStateBlockPtr mInitPSB;
};

}

#endif