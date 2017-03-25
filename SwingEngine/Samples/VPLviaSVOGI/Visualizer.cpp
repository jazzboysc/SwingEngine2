#include "VPLviaSVOGI.pch.h"
#include "Visualizer.h"
#include "VPLviaSVOGIApp.h"

using namespace Swing;

//----------------------------------------------------------------------------
GatherVoxelBuffer::GatherVoxelBuffer()
{
}
//----------------------------------------------------------------------------
GatherVoxelBuffer::~GatherVoxelBuffer()
{
}
//----------------------------------------------------------------------------
void GatherVoxelBuffer::OnGetShaderConstants()
{
    SEComputePass* p = (SEComputePass*)GetPass(0);
    SEShaderProgram* program = p->GetShaderProgram();

    program->GetUniformLocation(&mSceneBBMinLoc, "SceneBBMin");
    program->GetUniformLocation(&mSceneBBExtensionLoc, "SceneBBExtension");
}
//----------------------------------------------------------------------------
void GatherVoxelBuffer::OnPreDispatch(unsigned int)
{
    SEVector3f bbMin(SceneBB->Min[0], SceneBB->Min[1], SceneBB->Min[2]);
    SEVector3f extension = SceneBB->GetExtension();
    mSceneBBMinLoc.SetValue(bbMin);
    mSceneBBExtensionLoc.SetValue(extension);
}
//----------------------------------------------------------------------------
void GatherVoxelBuffer::OnPostDispatch(unsigned int)
{
    mGPUDevice->GPUMemoryBarrier(MBT_Command);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
VisualizerScreenQuad::VisualizerScreenQuad(SEMaterial* material)
    :
    SEScreenQuad(material, 0)
{
    ShowMode = 0;
    TextureArrayIndex = 0;
	CurMipmapLevel = 0;
	MaxMipmapLevel = 0;
    IsShadowMap = false;
    SceneBB = 0;
    UseToneMapper = true;
	ShowImportance = false;
    MaxRadiance = 500.0f;

    HDRMinExposure = 0.0f;
    HDRMaxExposure = 2.0f;
    HDRExposure = 2.0f;
}
//----------------------------------------------------------------------------
VisualizerScreenQuad::~VisualizerScreenQuad()
{
    TempTexture = 0;
    TempTexture2 = 0;
    GBufferPositionTexture = 0;
    GBufferNormalTexture = 0;
    TempTextureArray = 0;
    VoxelBuffer = 0;
}
//----------------------------------------------------------------------------
void VisualizerScreenQuad::OnUpdateShaderConstants(int, int pass)
{
    SESamplerDesc sampler;
    sampler.MinFilter = FT_Nearest;
    sampler.MagFilter = FT_Nearest;
    sampler.WrapS = WT_Clamp;
    sampler.WrapT = WT_Clamp;

    if( pass == 0 )
    {
        mUseToneMapperSM0Loc.SetValue(UseToneMapper);
        mTempSamplerSM0Loc.SetValue(0);
        if( TempTexture )
        {
            TempTexture->BindToSampler(0, &sampler);
        }
    }
    else if( pass == 1 )
    {
        mTempSamplerArraySM1Loc.SetValue(0);
        if( TempTextureArray )
        {
            TempTextureArray->BindToSampler(0, &sampler);
        }
        mTextureArrayIndexSM1Loc.SetValue(TextureArrayIndex);
		mCurMipmapLevelSM1Loc.SetValue(CurMipmapLevel);
        mIsShadowMapLoc.SetValue(IsShadowMap);
        mUseToneMapperSM1Loc.SetValue(UseToneMapper);
		mShowImportanceSM1Loc.SetValue(ShowImportance);
        mMaxRadianceSM1Loc.SetValue(MaxRadiance);
    }
    else if( pass == 2 )
    {
        mTempSamplerSM2Loc.SetValue(0);
        if( TempTexture )
        {
            TempTexture->BindToSampler(0, &sampler);
        }

        mTempSampler2SM2Loc.SetValue(1);
        if( TempTexture2 )
        {
            TempTexture2->BindToSampler(1, &sampler);
        }

        mAlbedoSamplerSM2Loc.SetValue(2);
        if( GBufferAlbedoTexture )
        {
			GBufferAlbedoTexture->BindToSampler(2, &sampler);
        }

        mUseToneMapperSM2Loc.SetValue(UseToneMapper);
        mExposureSM2Loc.SetValue(HDRExposure);
    }
    else if( pass == 3 )
    {
        mTempSamplerSM3Loc.SetValue(0);
        if( TempTexture )
        {
            TempTexture->BindToSampler(0, &sampler);
        }

        if( SceneBB )
        {
            SEVector3f sceneBBMin(SceneBB->Min[0], SceneBB->Min[1], SceneBB->Min[2]);
            mSceneBBMinSM3Loc.SetValue(sceneBBMin);
            mSceneBBExtensionSM3Loc.SetValue(SceneBB->GetExtension());
        }
    }
    else if( pass == 4 )
    {
        mTempSamplerSM4Loc.SetValue(0);
        if( TempTexture )
        {
            TempTexture->BindToSampler(0, &sampler);
        }

        if( SceneBB )
        {
            SEVector3f sceneBBMin(SceneBB->Min[0], SceneBB->Min[1], SceneBB->Min[2]);
            mSceneBBMinSM4Loc.SetValue(sceneBBMin);
            mSceneBBExtensionSM4Loc.SetValue(SceneBB->GetExtension());
        }

        mDimSM4Loc.SetValue(VoxelGridDim);
    }

    if( VoxelBuffer )
    {
        VoxelBuffer->Bind(0);
    }
}
//----------------------------------------------------------------------------
void VisualizerScreenQuad::OnGetShaderConstants()
{
    // SM0
    SEShaderProgram* program = mMaterial->GetProgram(0, 0);
    program->GetUniformLocation(&mUseToneMapperSM0Loc, "UseToneMapper");
    program->GetUniformLocation(&mTempSamplerSM0Loc, "tempSampler");

    // SM1
    program = mMaterial->GetProgram(0, 1);
    program->GetUniformLocation(&mTempSamplerArraySM1Loc, "tempSamplerArray");
    program->GetUniformLocation(&mTextureArrayIndexSM1Loc, "TextureArrayIndex");
	program->GetUniformLocation(&mCurMipmapLevelSM1Loc, "CurMipmapLevel");
    program->GetUniformLocation(&mIsShadowMapLoc, "IsShadowMap");
    program->GetUniformLocation(&mUseToneMapperSM1Loc, "UseToneMapper");
	program->GetUniformLocation(&mShowImportanceSM1Loc, "ShowImportance");
    program->GetUniformLocation(&mMaxRadianceSM1Loc, "MaxRadiance");

    // SM2
    program = mMaterial->GetProgram(0, 2);
    program->GetUniformLocation(&mTempSamplerSM2Loc, "tempSampler");
    program->GetUniformLocation(&mTempSampler2SM2Loc, "tempSampler2");
	program->GetUniformLocation(&mAlbedoSamplerSM2Loc, "GBufferAlbedoSampler");
    program->GetUniformLocation(&mUseToneMapperSM2Loc, "UseToneMapper");
    program->GetUniformLocation(&mExposureSM2Loc, "Exposure");

    // SM3
    program = mMaterial->GetProgram(0, 3);
    program->GetUniformLocation(&mTempSamplerSM3Loc, "tempSampler");
    program->GetUniformLocation(&mSceneBBMinSM3Loc, "SceneBBMin");
    program->GetUniformLocation(&mSceneBBExtensionSM3Loc, "SceneBBExtension");

    // SM4
    program = mMaterial->GetProgram(0, 4);
    program->GetUniformLocation(&mTempSamplerSM4Loc, "tempSampler");
    program->GetUniformLocation(&mSceneBBMinSM4Loc, "SceneBBMin");
    program->GetUniformLocation(&mSceneBBExtensionSM4Loc, "SceneBBExtension");
    program->GetUniformLocation(&mDimSM4Loc, "dim");
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
VoxelCubeTriMesh::VoxelCubeTriMesh(SEMaterial* material, SERTGICamera* camera)
    :
    SETriangleMesh(material, camera),
    MaterialColor(0.75f, 0.75f, 0.75f)
{
}
//----------------------------------------------------------------------------
VoxelCubeTriMesh::~VoxelCubeTriMesh()
{
}
//----------------------------------------------------------------------------
void VoxelCubeTriMesh::OnGetShaderConstants()
{
    SETriangleMesh::OnGetShaderConstants();
}
//----------------------------------------------------------------------------
void VoxelCubeTriMesh::OnUpdateShaderConstants(int technique, int pass)
{
    SETriangleMesh::OnUpdateShaderConstants(technique, pass);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
SVOCubeMesh::SVOCubeMesh(SEMaterial* material, SERTGICamera* camera)
    :
    SEQuadMesh(material, camera)
{
    SceneBB = 0;
    IsLeafPass = true;
}
//----------------------------------------------------------------------------
SVOCubeMesh::~SVOCubeMesh()
{
}
//----------------------------------------------------------------------------
void SVOCubeMesh::OnGetShaderConstants()
{
    SEQuadMesh::OnGetShaderConstants();

    SEShaderProgram* program = mMaterial->GetProgram(0, 0);
    program->GetUniformLocation(&mSceneBBMinLoc, "SceneBBMin");
    program->GetUniformLocation(&mVoxelExtensionLoc, "VoxelExtension");
    program->GetUniformLocation(&mIsLeafPassLoc, "IsLeafPass");
}
//----------------------------------------------------------------------------
void SVOCubeMesh::OnUpdateShaderConstants(int technique, int pass)
{
    SEQuadMesh::OnUpdateShaderConstants(technique, pass);

    SEVector3f sceneBBMin(SceneBB->Min[0], SceneBB->Min[1], SceneBB->Min[2]);
    SEVector3f voxelExtension = SceneBB->GetDelta();
    voxelExtension /= VPLviaSVOGI::VOXEL_DIMENSION;

    mSceneBBMinLoc.SetValue(sceneBBMin);
    mVoxelExtensionLoc.SetValue(voxelExtension);
    mIsLeafPassLoc.SetValue(IsLeafPass);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
VPLPointSet::VPLPointSet(SEMaterial* material, SERTGICamera* camera)
    :
    SEPointSet(material)
{
    SetCamera(camera);
    CurVPLSubsetID = 0;
    VPLCount = 0;
    PatternSize = 0;
    ShowVPLSubset = false;
    ShowVPLFluxContrast = false;
}
//----------------------------------------------------------------------------
VPLPointSet::~VPLPointSet()
{

}
//----------------------------------------------------------------------------
void VPLPointSet::OnGetShaderConstants()
{
    SEShaderProgram* program = mMaterial->GetProgram(0, 0);
    program->GetUniformLocation(&mViewLoc, "View");
    program->GetUniformLocation(&mProjLoc, "Proj");
    program->GetUniformLocation(&mCurVPLSubsetIDLoc, "CurVPLSubsetID");
    program->GetUniformLocation(&mVPLCountLoc, "VPLCount");
    program->GetUniformLocation(&mPatternSizeLoc, "PatternSize");
    program->GetUniformLocation(&mShowVPLSubsetLoc, "ShowVPLSubset");
    program->GetUniformLocation(&mShowVPLSubsetLoc, "ShowVPLSubset");
    program->GetUniformLocation(&mShowVPLFluxContrastLoc, 
        "ShowVPLFluxContrast");
}
//----------------------------------------------------------------------------
void VPLPointSet::OnUpdateShaderConstants(int, int)
{
    if( mCamera )
    {
        SEMatrix4f viewTrans = mCamera->GetViewTransform();
        mViewLoc.SetValue(viewTrans);

        SEMatrix4f projTrans = mCamera->GetProjectionTransform();
        mProjLoc.SetValue(projTrans);
    }
    mCurVPLSubsetIDLoc.SetValue(CurVPLSubsetID);
    mVPLCountLoc.SetValue(VPLCount);
    mPatternSizeLoc.SetValue(PatternSize);
    mShowVPLSubsetLoc.SetValue(ShowVPLSubset);
    mShowVPLFluxContrastLoc.SetValue(ShowVPLFluxContrast);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
Visualizer::Visualizer(SEGPUDevice* device, SERenderSet* renderSet)
    :
    SESubRenderer(device, renderSet)
{
    mShowVPL = false;
    mVPLSubsetCount = 0;
    mCurVPLSubsetIndex = 0;
    mShadowMapCount = 0;
    mCurShadowMapIndex = 0;
}
//----------------------------------------------------------------------------
Visualizer::~Visualizer()
{
    mVoxelBuffer = 0;
    mVPLBuffer = 0;
    mVPLPointSet = 0;
    mVoxelFragmentListBuffer = 0;
    mSVOBuffer = 0;
    mSVOUniformBuffer = 0;
    mSVONodeCubeModel = 0;
    mShadowMapsTexture = 0;
    mGBufferPositionTexture = 0;
    mGBufferNormalTexture = 0;
    mGBufferAlbedoTexture = 0;
	mGBufferSplitterPositionTexture = 0;
	mGBufferSplitterNormalTexture = 0;
    mGBufferRPCTexture = 0;
    mRSMPositionTextureArray = 0;
    mRSMNormalTextureArray = 0;
    mRSMFluxTextureArray = 0;
    mDirectLightingTexture = 0;
    mIndirectLightingTexture = 0;
    mMergedBufferTexture = 0;
    mScreenQuad = 0;
    mVoxelCubeModel = 0;
	mLightManager = 0;
    mCommonPSB = 0;
    mShowSVOGridPSB = 0;
    mShowVPLPSB = 0;
}
//----------------------------------------------------------------------------
void Visualizer::Initialize(SEGPUDevice* device, SEVoxelizer* voxelizer, 
    VPLGenerator* vplGenerator, ShadowMapsGenerator* shadowMapsGenerator, 
	SEGBufferRenderer* gbufferRenderer, GBufferSplitter* gbufferSplitter, 
	RSMRenderer* rsmRenderer, DirectLightingRenderer* directLightingRenderer,
    IndirectLightingRenderer* indirectLightingRenderer, 
	SplittedBufferMerger* bufferMerger, GAwareFilter* filter, SEAxisAlignedBox3f* sceneBB, 
	int voxelGridDim, int voxelGridLocalGroupDim, SERTGICamera* mainCamera, 
	int patternSize, int vplCount, float toneMapperMaxRadiance, 
	SELightManager* lightManager)
{
	mLightManager = lightManager;
	mMainCamera = mainCamera;

    mVoxelizerType = voxelizer->GetVoxelizerType();
    mVoxelGridDim = voxelGridDim;
    mVoxelGridLocalGroupDim = voxelGridLocalGroupDim;
    mGlobalDim = mVoxelGridDim / mVoxelGridLocalGroupDim;

    // Output to backbuffer.
    SERenderPassTargetsInfo targetsInfo;
    targetsInfo.ColorTargetCount = 1;
    targetsInfo.ColorTargetFormats[0] = BF_RGBA;

    SEShaderProgramInfo tempResultSM0ProgramInfo;
    tempResultSM0ProgramInfo.VShaderFileName = "VPLviaSVOGI/vTempResult.glsl";
    tempResultSM0ProgramInfo.FShaderFileName = "VPLviaSVOGI/fTempResultSM0.glsl";
    tempResultSM0ProgramInfo.ShaderStageFlag = SEShaderStage::SS_Vertex |
                                               SEShaderStage::SS_Fragment;
    SERenderPass* passTempResultSM0 = new SERenderPass(tempResultSM0ProgramInfo, targetsInfo);

    SEShaderProgramInfo tempResultSM1ProgramInfo;
    tempResultSM1ProgramInfo.VShaderFileName = "VPLviaSVOGI/vTempResult.glsl";
    tempResultSM1ProgramInfo.FShaderFileName = "VPLviaSVOGI/fTempResultSM1.glsl";
    tempResultSM1ProgramInfo.ShaderStageFlag = SEShaderStage::SS_Vertex |
                                               SEShaderStage::SS_Fragment;
    SERenderPass* passTempResultSM1 = new SERenderPass(tempResultSM1ProgramInfo, targetsInfo);

    SEShaderProgramInfo tempResultSM2ProgramInfo;
    tempResultSM2ProgramInfo.VShaderFileName = "VPLviaSVOGI/vTempResult.glsl";
    tempResultSM2ProgramInfo.FShaderFileName = "VPLviaSVOGI/fTempResultSM2.glsl";
    tempResultSM2ProgramInfo.ShaderStageFlag = SEShaderStage::SS_Vertex |
                                               SEShaderStage::SS_Fragment;
    SERenderPass* passTempResultSM2 = new SERenderPass(tempResultSM2ProgramInfo, targetsInfo);

    SEShaderProgramInfo tempResultSM3ProgramInfo;
    tempResultSM3ProgramInfo.VShaderFileName = "VPLviaSVOGI/vTempResult.glsl";
    tempResultSM3ProgramInfo.FShaderFileName = "VPLviaSVOGI/fTempResultSM3.glsl";
    tempResultSM3ProgramInfo.ShaderStageFlag = SEShaderStage::SS_Vertex |
                                               SEShaderStage::SS_Fragment;
    SERenderPass* passTempResultSM3 = new SERenderPass(tempResultSM3ProgramInfo, targetsInfo);

    SEShaderProgramInfo tempResultSM4ProgramInfo;
    tempResultSM4ProgramInfo.VShaderFileName = "VPLviaSVOGI/vTempResult.glsl";
    tempResultSM4ProgramInfo.FShaderFileName = "VPLviaSVOGI/fTempResultSM4.glsl";
    tempResultSM4ProgramInfo.ShaderStageFlag = SEShaderStage::SS_Vertex |
                                               SEShaderStage::SS_Fragment;
    SERenderPass* passTempResultSM4 = new SERenderPass(tempResultSM4ProgramInfo, targetsInfo);

    SETechnique* techScreenQuad = new SETechnique();
    techScreenQuad->AddPass(passTempResultSM0);
    techScreenQuad->AddPass(passTempResultSM1);
    techScreenQuad->AddPass(passTempResultSM2);
    techScreenQuad->AddPass(passTempResultSM3);
    techScreenQuad->AddPass(passTempResultSM4);
    SEMaterialTemplate* mtScreenQuad = new SEMaterialTemplate();
    mtScreenQuad->AddTechnique(techScreenQuad);

    mShadowMapCount = shadowMapsGenerator->GetShadowMapCount();

    // Cache temp buffer and textures needed for visualization.
    mShadowMapsTexture = 
        (SETexture2DArray*)shadowMapsGenerator->GetFrameBufferTextureByName(
        RTGI_ShadowMapRenderer_ShadowMaps_Name);
    mGBufferPositionTexture = 
        (SETexture2D*)gbufferRenderer->GetFrameBufferTextureByName(
        RTGI_GBuffer_Position_Name);
    mGBufferNormalTexture = 
        (SETexture2D*)gbufferRenderer->GetFrameBufferTextureByName(
        RTGI_GBuffer_Normal_Name);
    mGBufferAlbedoTexture = 
        (SETexture2D*)gbufferRenderer->GetFrameBufferTextureByName(
        RTGI_GBuffer_Albedo_Name);
    mGBufferRPCTexture =
        (SETexture2D*)gbufferRenderer->GetFrameBufferTextureByName(
        RTGI_GBuffer_RPC_Name);
	mGBufferSplitterPositionTexture =
		(SETexture2D*)gbufferSplitter->GetFrameBufferTextureByName(
		RTGI_GBufferSplitter_Position_Name);
	mGBufferSplitterNormalTexture =
		(SETexture2D*)gbufferSplitter->GetFrameBufferTextureByName(
		RTGI_GBufferSplitter_Normal_Name);
    mRSMPositionTextureArray = 
        (SETexture2DArray*)rsmRenderer->GetFrameBufferTextureByName(
        RTGI_RSMRenderer_RSMPosition_Name);
    mRSMNormalTextureArray = 
        (SETexture2DArray*)rsmRenderer->GetFrameBufferTextureByName(
        RTGI_RSMRenderer_RSMNormal_Name);
    mRSMFluxTextureArray = 
        (SETexture2DArray*)rsmRenderer->GetFrameBufferTextureByName(
        RTGI_RSMRenderer_RSMFlux_Name);
    mDirectLightingTexture = 
        (SETexture2D*)directLightingRenderer->GetFrameBufferTextureByName(
        RTGI_DirectLightingRenderer_DirectLighting_Name);
    mIndirectLightingTexture = indirectLightingRenderer->IndirectLightingTexture;
    mMergedBufferTexture = (SETexture2D*)bufferMerger->GetFrameBufferTextureByName(
        RTGI_SplittedBufferMerger_MergedBuffer_Name);
	mFilteredIndirectLightingTexture = (SETexture2D*)filter->GetFrameBufferTextureByName(
		RTGI_GAwareFilter_FilteredBuffer_Name);
    mVPLBuffer = (SEStructuredBuffer*)vplGenerator->GetGenericBufferByName(
        RTGI_VPLGenerator_VPLBuffer_Name);

    // Create VPL point set for VPL visualization.

    mVPLSubsetCount = patternSize*patternSize;

    SEShaderProgramInfo showVPLProgramInfo;
    showVPLProgramInfo.VShaderFileName = "VPLviaSVOGI/vShowVPL.glsl";
    showVPLProgramInfo.FShaderFileName = "VPLviaSVOGI/fShowVPL.glsl";
    showVPLProgramInfo.ShaderStageFlag = SEShaderStage::SS_Vertex |
                                         SEShaderStage::SS_Fragment;
    SERenderPass* passShowVPL = new SERenderPass(showVPLProgramInfo, targetsInfo);

    SETechnique* techShowVPL = new SETechnique();
    techShowVPL->AddPass(passShowVPL);
    SEMaterialTemplate* mtShowVPL = new SEMaterialTemplate();
    mtShowVPL->AddTechnique(techShowVPL);

    float* vplPointSetVB = new float[4];
    memset(vplPointSetVB, 0, sizeof(float) * 4);
    SEMaterial* showVPLMaterial = new SEMaterial(mtShowVPL);
    mVPLPointSet = new VPLPointSet(showVPLMaterial, mainCamera);
    mVPLPointSet->LoadFromSystemMemory(1, vplPointSetVB, 4);
    mVPLPointSet->CreateDeviceResource(mGPUDevice);
    mVPLPointSet->PointSize = 5.0f;
    mVPLPointSet->VPLCount = vplCount;
    mVPLPointSet->CurVPLSubsetID = mCurVPLSubsetIndex;
    mVPLPointSet->PatternSize = patternSize;
    SetShowVPLSubset(false);
    delete[] vplPointSetVB;

	const SEGPUDeviceDescription& deviceDesc =
		mGPUDevice->GetDeviceDescription();
	mVPLViewportWidth = deviceDesc.FramebufferWidth / mVPLPointSet->PatternSize;
	mVPLViewportHeight = deviceDesc.FramebufferHeight / mVPLPointSet->PatternSize;

    if( mVoxelizerType == SEVoxelizer::VT_Grid )
    {
        SEShaderProgramInfo showVoxelGridProgramInfo;
        showVoxelGridProgramInfo.VShaderFileName = 
            "VPLviaSVOGI/vShowVoxelGrid.glsl";
        showVoxelGridProgramInfo.FShaderFileName = 
            "VPLviaSVOGI/fShowVoxelGrid.glsl";
        showVoxelGridProgramInfo.ShaderStageFlag = SEShaderStage::SS_Vertex |
                                                   SEShaderStage::SS_Fragment;
        SERenderPass* passShowVoxelGrid = new SERenderPass(showVoxelGridProgramInfo, targetsInfo);

        SETechnique* techShowVoxelGrid = new SETechnique();
        techShowVoxelGrid->AddPass(passShowVoxelGrid);
        SEMaterialTemplate* mtShowVoxelGrid = new SEMaterialTemplate();
        mtShowVoxelGrid->AddTechnique(techShowVoxelGrid);

        // Cache grid voxelizer's voxel buffer.
        mVoxelBuffer = (SEStructuredBuffer*)voxelizer->GetGenericBufferByName(
            RTGI_GridVoxelizer_VoxelBuffer_Name);

        // Create gather voxel buffer task.
        SEShaderProgramInfo gatherVoxelBufferProgramInfo;
        gatherVoxelBufferProgramInfo.CShaderFileName =
            "VPLviaSVOGI/cGatherVoxelBuffer.glsl";
        gatherVoxelBufferProgramInfo.ShaderStageFlag = SEShaderStage::SS_Compute;
        SEComputePass* passGatherVoxelBuffer =
            new SEComputePass(gatherVoxelBufferProgramInfo);
        mGatherVoxelBufferTask = new GatherVoxelBuffer();
        mGatherVoxelBufferTask->AddPass(passGatherVoxelBuffer);
        mGatherVoxelBufferTask->CreateDeviceResource(device);
        mGatherVoxelBufferTask->SceneBB = sceneBB;

        // Create indirect command buffer.
        int voxelCount = voxelGridDim * voxelGridDim * voxelGridDim;
        int bufferSize = sizeof(unsigned int) * 5 + sizeof(float) * 35
            + voxelCount*sizeof(float) * 4;
        AddGenericBufferTarget(RTGI_Visualizer_IndirectCommandBuffer_Name,
            RDT_StructuredBuffer, bufferSize, BU_Dynamic_Copy,
            BF_BindIndexTo, 1);

        // Create gathered voxel GPU memory allocator counter.
        bufferSize = sizeof(unsigned int);
        AddGenericBufferTarget("GPUMemoryAllocatorCounter",
            RDT_AtomicCounterBuffer, bufferSize, BU_Dynamic_Copy, BF_BindIndex,
            0, true, 0);

        // Create voxel cube model.
        SEMaterial* material = new SEMaterial(mtShowVoxelGrid);
        mVoxelCubeModel = new VoxelCubeTriMesh(material, mainCamera);
        mVoxelCubeModel->LoadFromPLYFile("box.ply");
        mVoxelCubeModel->GenerateNormals();
        mVoxelCubeModel->SetIndirectCommandBuffer(
            (SEStructuredBuffer*)GetGenericBufferByName(
            RTGI_Visualizer_IndirectCommandBuffer_Name), 0);
        mVoxelCubeModel->CreateDeviceResource(device);
    }
    else if( mVoxelizerType == SEVoxelizer::VT_SVO )
    {
        // Cache SVO buffers.
        mVoxelFragmentListBuffer = ((SVOVoxelizer*)voxelizer)->GetVoxelFragmentListBuffer();
        mSVOBuffer = ((SVOVoxelizer*)voxelizer)->GetSVOBuffer();
        mSVOUniformBuffer = ((SVOVoxelizer*)voxelizer)->GetSVOUniformBuffer();

        SEShaderProgramInfo showSVOProgramInfo;
        showSVOProgramInfo.VShaderFileName = "VPLviaSVOGI/vShowSVO.glsl";
        showSVOProgramInfo.FShaderFileName = "VPLviaSVOGI/fShowSVO.glsl";
        showSVOProgramInfo.ShaderStageFlag = SEShaderStage::SS_Vertex |
                                             SEShaderStage::SS_Fragment;
        SERenderPass* passShowSVO = new SERenderPass(showSVOProgramInfo, targetsInfo);

        SETechnique* techShowSVO = new SETechnique();
        techShowSVO->AddPass(passShowSVO);
        SEMaterialTemplate* mtShowSVO = new SEMaterialTemplate();
        mtShowSVO->AddTechnique(techShowSVO);

        // Create SVO node cube model.
        std::vector<SEVector3f> svoCubeVertices;
        svoCubeVertices.reserve(8);
        svoCubeVertices.push_back(SEVector3f(-1.0f, 1.0f, 1.0f));
        svoCubeVertices.push_back(SEVector3f(1.0f, 1.0f, 1.0f));
        svoCubeVertices.push_back(SEVector3f(1.0f, 1.0f, -1.0f));
        svoCubeVertices.push_back(SEVector3f(-1.0f, 1.0f, -1.0f));
        svoCubeVertices.push_back(SEVector3f(-1.0f, -1.0f, 1.0f));
        svoCubeVertices.push_back(SEVector3f(1.0f, -1.0f, 1.0f));
        svoCubeVertices.push_back(SEVector3f(1.0f, -1.0f, -1.0f));
        svoCubeVertices.push_back(SEVector3f(-1.0f, -1.0f, -1.0f));
        std::vector<unsigned int> svoCubeIndices;
        svoCubeIndices.reserve(24);
        static unsigned int indices[] = { 0, 1, 2, 3,
            0, 4, 5, 1,
            0, 3, 7, 4,
            2, 6, 7, 3,
            1, 5, 6, 2,
            4, 7, 6, 5 };
        for( int i = 0; i < 24; ++i )
        {
            svoCubeIndices.push_back(indices[i]);
        }

        SEMaterial* material = new SEMaterial(mtShowSVO);
        mSVONodeCubeModel = new SVOCubeMesh(material, mainCamera);
        mSVONodeCubeModel->LoadFromSystemMemory(svoCubeVertices, svoCubeIndices);
        mSVONodeCubeModel->SetIndirectCommandBuffer(mSVOBuffer, 16);
        mSVONodeCubeModel->CreateDeviceResource(device);
        mSVONodeCubeModel->SceneBB = sceneBB;
    }
    else
    {
        assert(false);
    }

    // Create screen quad.
    SEMaterial* material = new SEMaterial(mtScreenQuad);
    mScreenQuad = new VisualizerScreenQuad(material);
    mScreenQuad->LoadFromPLYFile("screenquad.ply");
    mScreenQuad->SetTCoord(0, SEVector2f(0.0f, 0.0f));
    mScreenQuad->SetTCoord(1, SEVector2f(1.0f, 0.0f));
    mScreenQuad->SetTCoord(2, SEVector2f(1.0f, 1.0f));
    mScreenQuad->SetTCoord(3, SEVector2f(0.0f, 1.0f));
    mScreenQuad->CreateDeviceResource(device);
    mScreenQuad->SceneBB = sceneBB;
    mScreenQuad->VoxelBuffer = mVoxelBuffer;
    mScreenQuad->VoxelGridDim = voxelGridDim;
    mScreenQuad->GBufferPositionTexture = mGBufferPositionTexture;
    mScreenQuad->GBufferNormalTexture = mGBufferNormalTexture;
	mScreenQuad->GBufferAlbedoTexture = mGBufferAlbedoTexture;
    mScreenQuad->MaxRadiance = toneMapperMaxRadiance;
	mScreenQuad->MaxMipmapLevel = rsmRenderer->GetRSMFluxMipmapMaxLevel();

    SetShowMode(SM_Final);

    // Create PSBs.

    mCommonPSB = new SEPipelineStateBlock();
    mCommonPSB->PipelineStageFlag |= PB_OutputMerger;
    mCommonPSB->OutputMerger.OutputMergerOpFlag |= OMB_DepthStencil;
    mCommonPSB->OutputMerger.DepthStencil.DepthEnable = true;
    mCommonPSB->OutputMerger.OutputMergerOpFlag |= OMB_Clear;
    mCommonPSB->OutputMerger.ClearMask = OMCB_Color_Buffer | OMCB_Depth_Buffer;
    mCommonPSB->PipelineStageFlag |= PB_Rasterizer;
    mCommonPSB->Rasterizer.RasterizerOpFlag |= RB_CullMode;
    mCommonPSB->Rasterizer.CullMode = PCM_Cull_Back;
    mCommonPSB->Rasterizer.RasterizerOpFlag |= RB_FillMode;
    mCommonPSB->Rasterizer.FillMode = PFM_Solid;
    mCommonPSB->CreateDeviceResource(mGPUDevice);

    mShowSVOGridPSB = new SEPipelineStateBlock();
    mShowSVOGridPSB->PipelineStageFlag |= PB_OutputMerger;
    mShowSVOGridPSB->OutputMerger.OutputMergerOpFlag |= OMB_DepthStencil;
    mShowSVOGridPSB->OutputMerger.DepthStencil.DepthEnable = false;
    mShowSVOGridPSB->PipelineStageFlag |= PB_Rasterizer;
    mShowSVOGridPSB->Rasterizer.RasterizerOpFlag |= RB_CullMode;
    mShowSVOGridPSB->Rasterizer.CullMode = PCM_Cull_None;
    mShowSVOGridPSB->Rasterizer.RasterizerOpFlag |= RB_FillMode;
    mShowSVOGridPSB->Rasterizer.FillMode = PFM_Wireframe;
    mShowSVOGridPSB->CreateDeviceResource(mGPUDevice);

    mShowVPLPSB = new SEPipelineStateBlock();
    mShowVPLPSB->PipelineStageFlag |= PB_OutputMerger;
    mShowVPLPSB->OutputMerger.OutputMergerOpFlag |= OMB_DepthStencil;
    mShowVPLPSB->OutputMerger.DepthStencil.DepthEnable = false;
    mShowVPLPSB->CreateDeviceResource(mGPUDevice);
}
//----------------------------------------------------------------------------
void Visualizer::Render(int technique, int pass)
{
    SESubRenderer::Render(technique, pass, SRO_GenericBuffer | SRO_BackBuffer, 
        0, 0);
}
//----------------------------------------------------------------------------
void Visualizer::OnRender(int, int, SERTGICamera*)
{
    mGPUDevice->ApplyPipelineStateBlock(mCommonPSB);

    if( mVoxelizerType == SEVoxelizer::VT_Grid && mShowMode == SM_VoxelGrid )
    {
        mVoxelBuffer->Bind(0);

        // Gather voxel buffer pass.
        mGatherVoxelBufferTask->DispatchCompute(0, mGlobalDim, mGlobalDim, 
            mGlobalDim);

        // Show voxel grid.
        mVoxelCubeModel->Render(0, 0);
    }
    else if( mVoxelizerType == SEVoxelizer::VT_SVO && mShowMode == SM_SVOGrid )
    {
        mGPUDevice->ApplyPipelineStateBlock(mShowSVOGridPSB);

		// Show SVO grid.
        mSVOUniformBuffer->Bind(0);
        mVoxelFragmentListBuffer->Bind(1);
        mSVOBuffer->Bind(3);
        mSVONodeCubeModel->IsLeafPass = false;
        mSVONodeCubeModel->Render(0, 0);
        mSVONodeCubeModel->IsLeafPass = true;
        mSVONodeCubeModel->Render(0, 0);
    }
    else
    {
		// Show image results.
        mScreenQuad->Render(0, mScreenQuad->ShowMode);
    }

    if( mShowVPL )
    {
        mGPUDevice->ApplyPipelineStateBlock(mShowVPLPSB);

        mVPLBuffer->Bind(0);

		// Update VPL instance count.
		if( mVPLPointSet->ShowVPLSubset || 
			mShowMode == SM_IndirectLighting ||
			mShowMode == SM_GBufferSplitterPosition ||
			mShowMode == SM_GBufferSplitterNormal )
		{
			mVPLPointSet->InstanceCount = mVPLPointSet->VPLCount / mVPLSubsetCount;
		}
		else
		{
			mVPLPointSet->InstanceCount = mVPLPointSet->VPLCount;
		}

		// Setup camera for VPLs.
		if( mShowMode == SM_RSMPosition || mShowMode == SM_RSMNormal || 
			mShowMode == SM_RSMFlux || mShowMode == SM_Shadow )
		{
			// Hack spot light VPL visualization on RSM flux texture map.
			SERTGILight* spotLight = mLightManager->GetSpotLight(
				mScreenQuad->TextureArrayIndex);
			if( spotLight )
			{
				SERTGICamera* camera = spotLight->GetProjector();
				mVPLPointSet->SetCamera(camera);
			}
		}
		else
		{
			mVPLPointSet->SetCamera(mMainCamera);
		}

		// Show VPLs.
		if( mShowMode == SM_IndirectLighting ||
			mShowMode == SM_GBufferSplitterPosition ||
			mShowMode == SM_GBufferSplitterNormal )
		{
			// Cache old values.
            static SEViewportState oldViewport;
            mGPUDevice->GetViewport(&oldViewport);
			bool oldShowVPLSubset = mVPLPointSet->ShowVPLSubset;

			mVPLPointSet->ShowVPLSubset = true;
            SEViewportState curViewport;
            curViewport.Width = mVPLViewportWidth;
            curViewport.Height = mVPLViewportHeight;
			for( int y = 0; y < mVPLPointSet->PatternSize; ++y )
			{
				for( int x = 0; x < mVPLPointSet->PatternSize; ++x )
				{
                    curViewport.X = x*mVPLViewportWidth;
                    curViewport.Y = y*mVPLViewportHeight;
                    mGPUDevice->SetViewport(&curViewport);

					mVPLPointSet->CurVPLSubsetID = mVPLPointSet->PatternSize*y + x;
					mVPLPointSet->Render(0, 0);
				}
			}

			// Restore old values.
            mGPUDevice->SetViewport(&oldViewport);
			mVPLPointSet->ShowVPLSubset = oldShowVPLSubset;
		}
		else
		{
			mVPLPointSet->CurVPLSubsetID = mCurVPLSubsetIndex;
			mVPLPointSet->Render(0, 0);
		}
    }
}
//----------------------------------------------------------------------------
void Visualizer::SetShowMode(ShowMode mode)
{
    mShowMode = mode;
    switch( mShowMode )
    {
    case SM_VoxelBuffer:
        mScreenQuad->ShowMode = 4;
        mScreenQuad->TempTexture = mGBufferPositionTexture;
        break;

    case SM_VoxelGrid:
        break;

    case SM_SVOGrid:
        break;

    case SM_Shadow:
        mScreenQuad->IsShadowMap = true;
        mScreenQuad->ShowMode = 1;
        mScreenQuad->UseToneMapper = false;
        mScreenQuad->TextureArrayIndex = 0;
        mScreenQuad->TempTextureArray = mShadowMapsTexture;
        break;

    case SM_VPLShadow:
        break;

    case SM_GBufferPosition:
        mScreenQuad->ShowMode = 3;
        mScreenQuad->TempTexture = mGBufferPositionTexture;
        break;

    case SM_GBufferNormal:
        mScreenQuad->ShowMode = 0;
        mScreenQuad->UseToneMapper = false;
        mScreenQuad->TempTexture = mGBufferNormalTexture;
        break;

    case SM_GBufferAlbedo:
        mScreenQuad->ShowMode = 0;
        mScreenQuad->UseToneMapper = false;
        mScreenQuad->TempTexture = mGBufferAlbedoTexture;
        break;

    case SM_GBufferRPC:
        mScreenQuad->ShowMode = 0;
        mScreenQuad->UseToneMapper = false;
        mScreenQuad->TempTexture = mGBufferRPCTexture;
        break;

	case SM_GBufferSplitterPosition:
		mScreenQuad->ShowMode = 3;
		mScreenQuad->TempTexture = mGBufferSplitterPositionTexture;
		break;

	case SM_GBufferSplitterNormal:
		mScreenQuad->ShowMode = 0;
		mScreenQuad->TempTexture = mGBufferSplitterNormalTexture;
		break;

    case SM_RSMPosition:
        mScreenQuad->IsShadowMap = false;
        mScreenQuad->ShowMode = 1;
        mScreenQuad->UseToneMapper = false;
        mScreenQuad->TextureArrayIndex = 0;
        mScreenQuad->TempTextureArray = mRSMPositionTextureArray;
        break;

    case SM_RSMNormal:
        mScreenQuad->IsShadowMap = false;
        mScreenQuad->ShowMode = 1;
        mScreenQuad->UseToneMapper = false;
        mScreenQuad->TextureArrayIndex = 0;
        mScreenQuad->TempTextureArray = mRSMNormalTextureArray;
        break;

    case SM_RSMFlux:
        mScreenQuad->IsShadowMap = false;
        mScreenQuad->ShowMode = 1;
        mScreenQuad->UseToneMapper = true;
        mScreenQuad->TextureArrayIndex = 0;
        mScreenQuad->TempTextureArray = mRSMFluxTextureArray;
        break;

    case SM_DirectLighting:
        mScreenQuad->ShowMode = 0;
        mScreenQuad->UseToneMapper = true;
        mScreenQuad->TempTexture = mDirectLightingTexture;
        break;

    case SM_IndirectLighting:
        mScreenQuad->ShowMode = 0;
        mScreenQuad->UseToneMapper = true;
        mScreenQuad->TempTexture = mIndirectLightingTexture;
        break;

    case SM_MergedBuffer:
        mScreenQuad->ShowMode = 0;
        mScreenQuad->UseToneMapper = true;
        mScreenQuad->TempTexture = mMergedBufferTexture;
        break;

    case SM_FilteredIndirectLighting:
        mScreenQuad->ShowMode = 0;
        mScreenQuad->UseToneMapper = true;
        mScreenQuad->TempTexture = mFilteredIndirectLightingTexture;
        break;

    case SM_Final:
        mScreenQuad->ShowMode = 2;
		mScreenQuad->TempTexture = mFilteredIndirectLightingTexture;
        mScreenQuad->TempTexture2 = mDirectLightingTexture;
        break;

    default:
        assert(false);
        break;
    }
}
//----------------------------------------------------------------------------
int Visualizer::GetCurVPLSubsetIndex() const
{
    return mCurVPLSubsetIndex;
}
//----------------------------------------------------------------------------
void Visualizer::SetCurVPLSubsetIndex(int value)
{
    mCurVPLSubsetIndex = SE_MIN(SE_MAX(0, value), mVPLSubsetCount - 1);
}
//----------------------------------------------------------------------------
int Visualizer::GetCurTextureMapIndex() const
{
    return mCurShadowMapIndex;
}
//----------------------------------------------------------------------------
void Visualizer::SetCurTextureMapIndex(int value)
{
    mCurShadowMapIndex = SE_MIN(SE_MAX(0, value), mShadowMapCount - 1);
    mScreenQuad->TextureArrayIndex = mCurShadowMapIndex;
}
//----------------------------------------------------------------------------
int Visualizer::GetCurMipmapLevel() const
{
	return mScreenQuad->CurMipmapLevel;
}
//----------------------------------------------------------------------------
void Visualizer::SetCurMipmapLevel(int value)
{
	mScreenQuad->CurMipmapLevel = SE_MIN(SE_MAX(0, value), 
		mScreenQuad->MaxMipmapLevel);
}
//----------------------------------------------------------------------------
void Visualizer::SetShowVPL(bool value)
{
    mShowVPL = value;
}
//----------------------------------------------------------------------------
void Visualizer::SetShowVPLSubset(bool value)
{
	mVPLPointSet->ShowVPLSubset = value;
}
//----------------------------------------------------------------------------
void Visualizer::SetShowVPLFluxContrast(bool value)
{
    mVPLPointSet->ShowVPLFluxContrast = value;
}
//----------------------------------------------------------------------------
void Visualizer::SetShowRSMFluxImportance(bool value)
{
	mScreenQuad->ShowImportance = value;
}
//----------------------------------------------------------------------------
void Visualizer::SetUseHDRToneMapping(bool value)
{
    mScreenQuad->UseToneMapper = value;
}
//----------------------------------------------------------------------------
void Visualizer::SetHDRExposure(float value)
{
    mScreenQuad->HDRExposure = value * mScreenQuad->HDRMaxExposure;
}
//----------------------------------------------------------------------------