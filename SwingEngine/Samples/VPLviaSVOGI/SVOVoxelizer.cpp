#include "VPLviaSVOGI.pch.h"
#include "SVOVoxelizer.h"

using namespace Swing;

//----------------------------------------------------------------------------
BuildSVO::BuildSVO()
{
}
//----------------------------------------------------------------------------
BuildSVO::~BuildSVO()
{
}
//----------------------------------------------------------------------------
void BuildSVO::OnGetShaderConstants()
{
}
//----------------------------------------------------------------------------
void BuildSVO::OnPreDispatch(unsigned int)
{
}
//----------------------------------------------------------------------------
void BuildSVO::OnPostDispatch(unsigned int pass)
{
    switch( pass )
    {
    case BUILD_SVO_INIT_ROOT_PASS:
        mDevice->MemoryBarrier(MBT_Structured);
        break;

    case BUILD_SVO_FLAG_NODES_PASS:
        mDevice->MemoryBarrier(MBT_Structured);
        break;

    case BUILD_SVO_ALLOC_NODES_PASS:
        mDevice->MemoryBarrier(MBT_AtomicCounter);
        break;

    case BUILD_SVO_POST_ALLOC_NODES_PASS:
        mDevice->MemoryBarrier(MBT_Structured | MBT_Command);
        break;

    case BUILD_SVO_INIT_NODES_PASS:
        mDevice->MemoryBarrier(MBT_Structured);
        break;

    default:
        break;
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
GatherVoxelFragmentListInfo::GatherVoxelFragmentListInfo()
{
}
//----------------------------------------------------------------------------
GatherVoxelFragmentListInfo::~GatherVoxelFragmentListInfo()
{
}
//----------------------------------------------------------------------------
void GatherVoxelFragmentListInfo::OnGetShaderConstants()
{
}
//----------------------------------------------------------------------------
void GatherVoxelFragmentListInfo::OnPreDispatch(unsigned int)
{
    mDevice->MemoryBarrier(MBT_AtomicCounter);
}
//----------------------------------------------------------------------------
void GatherVoxelFragmentListInfo::OnPostDispatch(unsigned int)
{
    mDevice->MemoryBarrier(MBT_Command);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
SVOVoxelizer::SVOVoxelizer(SEGPUDevice* device, SERenderSet* renderSet)
    :
    SEVoxelizer(device, renderSet)
{
    mVoxelizerType = VT_SVO;
    mSceneBBMaxLength = 0.0f;
}
//----------------------------------------------------------------------------
SVOVoxelizer::~SVOVoxelizer()
{
    mAtomicCounterBuffer = 0;
    mVoxelFragmentListBuffer = 0;
    mVoxelFragmentListBufferIndirectView = 0;
    mSVOBuffer = 0;
    mSVOBufferIndirectView = 0;
    mSVOUniformBuffer = 0;
    mSceneAABBBuffer = 0;
    mGatherVoxelFragmentListInfoTask = 0;
    mBuildSVOTask = 0;
    mBeginBuildingSVOPSB = 0;
    mEndBuildingSVOPSB = 0;
}
//----------------------------------------------------------------------------
void SVOVoxelizer::Initialize(SEGPUDevice*, int voxelGridDim, 
    SEAxisAlignedBox3f* sceneBB)
{
    VoxelGridDim = voxelGridDim;
    mSceneBB = sceneBB;

    mVoxelizerViewport.X = 0;
    mVoxelizerViewport.Y = 0;
    mVoxelizerViewport.Width = voxelGridDim;
    mVoxelizerViewport.Height = voxelGridDim;

    // Create gather voxel fragment list info task.
    SEShaderProgramInfo gatherVoxelFragmentListInfoProgramInfo;
    gatherVoxelFragmentListInfoProgramInfo.CShaderFileName = 
        "VPLviaSVOGI/cGatherVoxelFragmentListInfo.glsl";
    gatherVoxelFragmentListInfoProgramInfo.ShaderStageFlag = 
        SEShaderStage::SS_Compute;

    SEComputePass* passGatherVoxelFragmentListInfo = new SEComputePass(
        gatherVoxelFragmentListInfoProgramInfo);
    mGatherVoxelFragmentListInfoTask = new GatherVoxelFragmentListInfo();
    mGatherVoxelFragmentListInfoTask->AddPass(passGatherVoxelFragmentListInfo);
    mGatherVoxelFragmentListInfoTask->CreateDeviceResource(mDevice);

    // Create build SVO task.
    SEShaderProgramInfo buildSVOInitRootProgramInfo;
    buildSVOInitRootProgramInfo.VShaderFileName = 
        "VPLviaSVOGI/vBuildSVOInitRoot.glsl";
    buildSVOInitRootProgramInfo.ShaderStageFlag = SEShaderStage::SS_Vertex;
    SEComputePass* passBuildSVOInitRoot = new SEComputePass(
        buildSVOInitRootProgramInfo);

    SEShaderProgramInfo buildSVOFlagNodesProgramInfo;
    buildSVOFlagNodesProgramInfo.VShaderFileName = 
        "VPLviaSVOGI/vBuildSVOFlagNodes.glsl";
    buildSVOFlagNodesProgramInfo.ShaderStageFlag = SEShaderStage::SS_Vertex;
    SEComputePass* passBuildSVOFlagNodes = new SEComputePass(
        buildSVOFlagNodesProgramInfo);

    SEShaderProgramInfo buildSVOAllocateNodesProgramInfo;
    buildSVOAllocateNodesProgramInfo.VShaderFileName = 
        "VPLviaSVOGI/vBuildSVOAllocateNodes.glsl";
    buildSVOAllocateNodesProgramInfo.ShaderStageFlag = SEShaderStage::SS_Vertex;
    SEComputePass* passBuildSVOAllocateNodes = new SEComputePass(
        buildSVOAllocateNodesProgramInfo);

    SEShaderProgramInfo buildSVOPostAllocateNodesProgramInfo;
    buildSVOPostAllocateNodesProgramInfo.VShaderFileName = 
        "VPLviaSVOGI/vBuildSVOPostAllocateNodes.glsl";
    buildSVOPostAllocateNodesProgramInfo.ShaderStageFlag = 
        SEShaderStage::SS_Vertex;
    SEComputePass* passBuildSVOPostAllocateNodes = new SEComputePass(
        buildSVOPostAllocateNodesProgramInfo);

    SEShaderProgramInfo buildSVOInitNodesProgramInfo;
    buildSVOInitNodesProgramInfo.VShaderFileName = 
        "VPLviaSVOGI/vBuildSVOInitNodes.glsl";
    buildSVOInitNodesProgramInfo.ShaderStageFlag = SEShaderStage::SS_Vertex;
    SEComputePass* passBuildSVOInitNodes = new SEComputePass(
        buildSVOInitNodesProgramInfo);

    SEShaderProgramInfo buildSVOSplatLeafNodesProgramInfo;
    buildSVOSplatLeafNodesProgramInfo.VShaderFileName = 
        "VPLviaSVOGI/vBuildSVOSplatLeafNodes.glsl";
    buildSVOSplatLeafNodesProgramInfo.ShaderStageFlag = SEShaderStage::SS_Vertex;
    SEComputePass* passBuildSVOSplatLeafNodes = new SEComputePass(
        buildSVOSplatLeafNodesProgramInfo);

    mBuildSVOTask = new BuildSVO();
    mBuildSVOTask->AddPass(passBuildSVOInitRoot);
    mBuildSVOTask->AddPass(passBuildSVOFlagNodes);
    mBuildSVOTask->AddPass(passBuildSVOAllocateNodes);
    mBuildSVOTask->AddPass(passBuildSVOPostAllocateNodes);
    mBuildSVOTask->AddPass(passBuildSVOInitNodes);
    mBuildSVOTask->AddPass(passBuildSVOSplatLeafNodes);
    mBuildSVOTask->CreateDeviceResource(mDevice);

    // Create voxel fragment list buffer.
    unsigned int voxelCount = VoxelGridDim * VoxelGridDim * VoxelGridDim;
    mVoxelFragmentListBuffer = new SEStructuredBuffer();
    unsigned int voxelFragmentCount = 
        unsigned int((float)voxelCount*1.0f/*0.2f*/); // voxel fragment ratio.
    size_t bufferSize = sizeof(VoxelFragmentBufferHead) + 
        voxelFragmentCount*sizeof(VoxelFragment);
    mVoxelFragmentListBuffer->ReserveMutableDeviceResource(mDevice, 
        bufferSize, BU_Dynamic_Copy);

    SEBufferViewDesc viewDesc;
    viewDesc.Type = BT_DrawIndirect;
    mVoxelFragmentListBufferIndirectView = new SEBufferView(viewDesc);
    mVoxelFragmentListBufferIndirectView->CreateDeviceResource(mDevice,
        mVoxelFragmentListBuffer);

    // Create SVO buffer.
    mSVOMaxLevel = (unsigned int)SEMathf::Log2((float)VoxelGridDim);
    mSVOBuffer = new SEStructuredBuffer();
    // interior node ratio (1/7, fixed) and leaf node ratio.
    mSVONodeMaxCount = unsigned int((float)voxelCount*2.0f/*(0.143f + 0.05f)*/);
    bufferSize = sizeof(SVONodeBufferHead) + mSVONodeMaxCount*sizeof(SVONode);
    mSVOBuffer->ReserveMutableDeviceResource(mDevice, bufferSize, 
        BU_Dynamic_Copy);

    mSVOBufferIndirectView = new SEBufferView(viewDesc);
    mSVOBufferIndirectView->CreateDeviceResource(mDevice, mSVOBuffer);

    // Create SVO uniform buffer.
    mSVOUniformBuffer = new SEUniformBuffer();
    bufferSize = sizeof(unsigned int) * 2;
    mSVOUniformBuffer->ReserveMutableDeviceResource(mDevice, bufferSize, 
        BU_Dynamic_Draw);
    unsigned int svoUniformBufferData[2] = { 0, VoxelGridDim };
    mSVOUniformBuffer->UpdateSubData(0, 0, sizeof(unsigned int) * 2, 
        (void*)svoUniformBufferData);

    // Create atomic counter buffer. We create 2 atomic counters here.
    mAtomicCounterBuffer = new SEAtomicCounterBuffer();
#ifdef DEBUG_VOXEL
    mAtomicCounterBuffer->ReserveMutableDeviceResource(mDevice, 
        sizeof(unsigned int) * 2, BU_Dynamic_Copy);
#else
    mAtomicCounterBuffer->ReserveImmutableDeviceResource(mDevice, 
        sizeof(unsigned int) * 2);
#endif

    // Initialize scene bounding box buffer.
    SEVector3f sceneBBMin(mSceneBB->Min[0], mSceneBB->Min[1], mSceneBB->Min[2]);
    SEVector3f center = mSceneBB->GetBoxCenter();
    SEVector3f extension = mSceneBB->GetExtension();
    SEVector3f inv2extension = SEVector3f(1.0f / (2.0f*extension.X), 
        1.0f / (2.0f*extension.Y), 1.0f / (2.0f*extension.Z));
    mVoxelFragmentListBuffer->Bind();
    VoxelFragmentBufferHead* bufferData = 
        (VoxelFragmentBufferHead*)mVoxelFragmentListBuffer->Map(BA_Write_Only);
    bufferData->SceneBBMin = SEVector4f(sceneBBMin.X, sceneBBMin.Y, sceneBBMin.Z, 1.0f);
    bufferData->SceneBBCenter = SEVector4f(center.X, center.Y, center.Z, 1.0f);
    bufferData->SceneBBExtension = SEVector4f(extension.X, extension.Y, extension.Z, 0.0f);
    bufferData->Inv2SceneBBExtension = SEVector4f(inv2extension.X, inv2extension.Y, inv2extension.Z, 0.0f);
    mVoxelFragmentListBuffer->Unmap();

    mSceneAABBBuffer = new SEStructuredBuffer();
    bufferSize = sizeof(SEVector4f)*2;
    mSceneAABBBuffer->ReserveMutableDeviceResource(mDevice, bufferSize, 
        BU_Dynamic_Copy);
    mSceneAABBBuffer->Bind();
    SEVector4f* sceneAABBBufferData = (SEVector4f*)mSceneAABBBuffer->Map(BA_Write_Only);
    sceneAABBBufferData[0] = SEVector4f(center.X, center.Y, center.Z, 1.0f);
    sceneAABBBufferData[1] = SEVector4f(extension.X, extension.Y, extension.Z, 0.0f);
    mSceneAABBBuffer->Unmap();

    // Create PSBs.

    mBeginBuildingSVOPSB = new SEPipelineStateBlock();
    mBeginBuildingSVOPSB->PipelineStageFlag |= PB_OutputMerger;
    mBeginBuildingSVOPSB->PipelineStageFlag |= PB_Rasterizer;

    // Disable depth test and color buffer outputs.
    mBeginBuildingSVOPSB->OutputMerger.OutputMergerOpFlag |= OMB_DepthStencil;
    mBeginBuildingSVOPSB->OutputMerger.DepthStencil.DepthEnable = false;
    mBeginBuildingSVOPSB->OutputMerger.OutputMergerOpFlag |= OMB_Blend;
    mBeginBuildingSVOPSB->OutputMerger.Blend.BlendStateOpFlag |= BSB_ColorMask;
    mBeginBuildingSVOPSB->OutputMerger.Blend.FrameBufferTarget[0].ColorMask = 
        CMB_Red | CMB_Green | CMB_Blue | CMB_Alpha;

    // Disable cull face and wireframe mode.
    mBeginBuildingSVOPSB->Rasterizer.RasterizerOpFlag |= RB_CullMode;
    mBeginBuildingSVOPSB->Rasterizer.CullMode = PCM_Cull_None;
    mBeginBuildingSVOPSB->Rasterizer.RasterizerOpFlag |= RB_FillMode;
    mBeginBuildingSVOPSB->Rasterizer.FillMode = PFM_Solid;
    mBeginBuildingSVOPSB->CreateDeviceResource(mDevice);

    mEndBuildingSVOPSB = new SEPipelineStateBlock();
    mEndBuildingSVOPSB->PipelineStageFlag |= PB_OutputMerger;
    mEndBuildingSVOPSB->PipelineStageFlag |= PB_Rasterizer;

    // Enable depth test and color buffer outputs.
    mEndBuildingSVOPSB->OutputMerger.OutputMergerOpFlag |= OMB_DepthStencil;
    mEndBuildingSVOPSB->OutputMerger.DepthStencil.DepthEnable = true;
    mEndBuildingSVOPSB->OutputMerger.OutputMergerOpFlag |= OMB_Blend;
    mEndBuildingSVOPSB->OutputMerger.Blend.BlendStateOpFlag |= BSB_ColorMask;
    mEndBuildingSVOPSB->OutputMerger.Blend.FrameBufferTarget[0].ColorMask = 0;

    // Enable cull face.
    mEndBuildingSVOPSB->Rasterizer.RasterizerOpFlag |= RB_CullMode;
    mEndBuildingSVOPSB->Rasterizer.CullMode = PCM_Cull_Back;
    mEndBuildingSVOPSB->CreateDeviceResource(mDevice);
}
//----------------------------------------------------------------------------
static SEViewportState oldViewport;
//----------------------------------------------------------------------------
void SVOVoxelizer::OnRender(int technique, int pass, SERTGICamera*)
{
    mSVOUniformBuffer->Bind(0);
    mAtomicCounterBuffer->Bind(0);
    mVoxelFragmentListBuffer->Bind(1);
    mSceneAABBBuffer->Bind(2);
    mSVOBuffer->Bind(3);

    // Cache old viewport values and set new values.
    mDevice->GetViewport(&oldViewport);
    mDevice->SetViewport(&mVoxelizerViewport);

    // Reset counters.
#ifdef DEBUG_VOXEL
    unsigned int* counterData = 
        (unsigned int*)mAtomicCounterBuffer->Map(BA_Write_Only);
    assert(counterData);
    counterData[0] = 0;
    counterData[1] = 0;
    mAtomicCounterBuffer->Unmap();
#else
    unsigned int counterData[2];
    counterData[0] = 0;
    counterData[1] = 0;
    mAtomicCounterBuffer->Clear(BIF_R32UI, BF_R32UI, BCT_Unsigned_Int, 
        counterData);
#endif

    //----------------------- Begin building SVO -----------------------//

    // Scene voxelization pass.
    mDevice->ApplyPipelineStateBlock(mBeginBuildingSVOPSB);
    VoxelizeScene(technique, pass);

    // Gather voxel fragment list info pass.
    mGatherVoxelFragmentListInfoTask->DispatchCompute(0, 1, 1, 1);
#ifdef DEBUG_VOXEL
    mVoxelFragmentListBuffer->Bind();
    unsigned int* dispatchIndirectCommandbufferData = 
        (unsigned int*)mVoxelFragmentListBuffer->Map(BA_Read_Only);
    //infoPanel->SetTimingLabelValue("GVF Count", 
    (double)dispatchIndirectCommandbufferData[0]);
    mVoxelFragmentListBuffer->Unmap();
#endif

    // Build SVO init root pass.
    mBuildSVOTask->DispatchVertex(BUILD_SVO_INIT_ROOT_PASS, 1);
#ifdef DEBUG_VOXEL
    mSVOBuffer->Bind();
    void* svoBufferData = mSVOBuffer->Map(BA_Read_Only);
    SVONodeBufferHead* svoBufferHeadData = (SVONodeBufferHead*)svoBufferData;
    SVONodeTile* svoNodeTileData = (SVONodeTile*)(svoBufferHeadData + 1);
    mSVOBuffer->Unmap();
#endif

    unsigned int curLevel = 1;
    for( ; curLevel < mSVOMaxLevel; ++curLevel )
    {
        // Update SVO uniform buffer.
        mSVOUniformBuffer->UpdateSubData(0, 0, sizeof(unsigned int), 
            (void*)&curLevel);
#ifdef DEBUG_VOXEL
        unsigned int* svoUniformBufferData = 
            (unsigned int*)mSVOUniformBuffer->Map(BA_Read_Only);
        mSVOUniformBuffer->Unmap();
#endif

        // Flag SVO nodes pass.
        mBuildSVOTask->DispatchVertexIndirect(BUILD_SVO_FLAG_NODES_PASS,
            mVoxelFragmentListBuffer, mVoxelFragmentListBufferIndirectView, 0);
#ifdef DEBUG_VOXEL
        mSVOBuffer->Bind();
        svoBufferData = mSVOBuffer->Map(BA_Read_Only);
        svoBufferHeadData = (SVONodeBufferHead*)svoBufferData;
        svoNodeTileData = (SVONodeTile*)(svoBufferHeadData + 1);
        mSVOBuffer->Unmap();
#endif

        // Allocate SVO nodes pass.
        mBuildSVOTask->DispatchVertexIndirect(BUILD_SVO_ALLOC_NODES_PASS,
            mSVOBuffer, mSVOBufferIndirectView, 0);
#ifdef DEBUG_VOXEL
        mSVOBuffer->Bind();
        svoBufferData = mSVOBuffer->Map(BA_Read_Only);
        svoBufferHeadData = (SVONodeBufferHead*)svoBufferData;
        svoNodeTileData = (SVONodeTile*)(svoBufferHeadData + 1);
        mSVOBuffer->Unmap();
#endif

        // Post allocate SVO nodes pass.
        mBuildSVOTask->DispatchVertex(BUILD_SVO_POST_ALLOC_NODES_PASS, 1);

        // Init SVO nodes pass.
        mBuildSVOTask->DispatchVertexIndirect(BUILD_SVO_INIT_NODES_PASS,
            mSVOBuffer, mSVOBufferIndirectView, 0);
#ifdef DEBUG_VOXEL
        mSVOBuffer->Bind();
        svoBufferData = mSVOBuffer->Map(BA_Read_Only);
        svoBufferHeadData = (SVONodeBufferHead*)svoBufferData;
        svoNodeTileData = (SVONodeTile*)(svoBufferHeadData + 1);
        mSVOBuffer->Unmap();
#endif
    }

    // Splat SVO leaf nodes pass.
    mBuildSVOTask->DispatchVertexIndirect(BUILD_SVO_SPLAT_LEAF_NODES_PASS,
        mVoxelFragmentListBuffer, mVoxelFragmentListBufferIndirectView, 0);
#ifdef DEBUG_VOXEL
    mSVOBuffer->Bind();
    svoBufferData = mSVOBuffer->Map(BA_Read_Only);
    svoBufferHeadData = (SVONodeBufferHead*)svoBufferData;
    svoNodeTileData = (SVONodeTile*)(svoBufferHeadData + 1);
    mSVOBuffer->Unmap();
#endif

    //------------------------ End building SVO ------------------------//

    // Restore device states.
    mDevice->ApplyPipelineStateBlock(mEndBuildingSVOPSB);
    mDevice->SetViewport(&oldViewport);
}
//----------------------------------------------------------------------------
SEStructuredBuffer* SVOVoxelizer::GetVoxelFragmentListBuffer() const
{
    return mVoxelFragmentListBuffer;
}
//----------------------------------------------------------------------------
SEStructuredBuffer* SVOVoxelizer::GetSVOBuffer() const
{
    return mSVOBuffer;
}
//----------------------------------------------------------------------------
SEBufferView* SVOVoxelizer::GetSVOBufferIndirectView() const
{
    return mSVOBufferIndirectView;
}
//----------------------------------------------------------------------------
SEUniformBuffer* SVOVoxelizer::GetSVOUniformBuffer() const
{
    return mSVOUniformBuffer;
}
//----------------------------------------------------------------------------