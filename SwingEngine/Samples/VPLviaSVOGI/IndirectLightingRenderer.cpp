#include "VPLviaSVOGI.pch.h"
#include "IndirectLightingRenderer.h"
#include "GBufferSplitter.h"
#include "SEApplication.h"

using namespace Swing;

//----------------------------------------------------------------------------
IndirectLightingScreenQuad::IndirectLightingScreenQuad(SEMaterial* material)
    :
    SEScreenQuad(material, 0)
{
    BounceSingularity = 30.0f;
    VPLVisibilityTest = true;
    UseTC = true;
}
//----------------------------------------------------------------------------
IndirectLightingScreenQuad::~IndirectLightingScreenQuad()
{
}
//----------------------------------------------------------------------------
void IndirectLightingScreenQuad::OnUpdateShaderConstants(int, int)
{
    mVPLCountLoc.SetValue(VPLCount);
    mPatternSizeLoc.SetValue(PatternSize);
    mBounceSingularityLoc.SetValue(BounceSingularity);
    mGBufferPositionSamplerLoc.SetValue(0);
    mGBufferNormalSamplerLoc.SetValue(1);
    mDimLoc.SetValue(VoxelGridDim);
    if( SceneBB )
    {
        mSceneBBCenterLoc.SetValue(SceneBB->GetBoxCenter());
        mSceneBBExtensionLoc.SetValue(SceneBB->GetExtension());
    }
    mVPLVisibilityTestLoc.SetValue(VPLVisibilityTest);
    if( UseTC )
    {
        SEApplication* app = SEApplication::GetInstance();
        mFrameCounterLoc.SetValue((int)app->FrameCounter);
    }
}
//----------------------------------------------------------------------------
void IndirectLightingScreenQuad::OnGetShaderConstants()
{
    SEShaderProgram* program = mMaterial->GetProgram(0, 0);

    program->GetUniformLocation(&mVPLCountLoc, "VPLCount");
    program->GetUniformLocation(&mPatternSizeLoc, "PatternSize");
    program->GetUniformLocation(&mBounceSingularityLoc, "BounceSingularity");
    program->GetUniformLocation(&mGBufferPositionSamplerLoc, "GBufferPositionSampler");
    program->GetUniformLocation(&mGBufferNormalSamplerLoc, "GBufferNormalSampler");
    program->GetUniformLocation(&mSceneBBCenterLoc, "SceneBBCenter");
    program->GetUniformLocation(&mSceneBBExtensionLoc, "SceneBBExtension");
    program->GetUniformLocation(&mDimLoc, "dim");
    program->GetUniformLocation(&mVPLVisibilityTestLoc, "VPLVisibilityTest");
    if( UseTC )
    {
        program->GetUniformLocation(&mFrameCounterLoc, "FrameCounter");
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
IndirectLightingRenderer::IndirectLightingRenderer(SEGPUDevice* device, 
    SERenderSet* renderSet)
    :
    SESubRenderer(device, renderSet)
{
    mVoxelizerType = SEVoxelizer::VT_Unknown;
    mUseTC = true;

    mPSB = new SEPipelineStateBlock();
    mPSB->PipelineStageFlag |= PB_OutputMerger;
    mPSB->OutputMerger.OutputMergerOpFlag |= OMB_Clear;
    mPSB->OutputMerger.ClearMask = OMCB_Color_Buffer | OMCB_Depth_Buffer;
}
//----------------------------------------------------------------------------
IndirectLightingRenderer::~IndirectLightingRenderer()
{
    mPSB = 0;
    mIndirectLightingScreenQuad = 0;

    mVoxelFragmentListBuffer = 0;
    mSVOBuffer = 0;

    IndirectLightingTexture = 0;
}
//----------------------------------------------------------------------------
void IndirectLightingRenderer::Initialize(SEGPUDevice* device, int width, 
    int height, SEBufferFormat format, int vplCount, int patternSize, 
    SEAxisAlignedBox3f* sceneBB, int voxelGridDim, SESubRenderer* gbufferSource,
    VPLGenerator* vplGenerator, SEVoxelizer* voxelizer, bool useTC)
{
    mVoxelizerType = voxelizer->GetVoxelizerType();
    mUseTC = useTC;

    SEShaderProgramInfo indirectLightingProgramInfo;
    indirectLightingProgramInfo.VShaderFileName = 
        "VPLviaSVOGI/vIndirectLighting.glsl";
    if( mVoxelizerType == SEVoxelizer::VT_Grid )
    {
        indirectLightingProgramInfo.FShaderFileName =
            "VPLviaSVOGI/fGridIndirectLighting.glsl";
    }
    else if( mVoxelizerType == SEVoxelizer::VT_SVO )
    {
        if( mUseTC )
        {
            indirectLightingProgramInfo.FShaderFileName =
                "VPLviaSVOGI/fSVOIndirectLightingTC.glsl";
        }
        else
        {
            indirectLightingProgramInfo.FShaderFileName =
                "VPLviaSVOGI/fSVOIndirectLighting.glsl";
        }
    }
    else
    {
        assert(false);
    }
    indirectLightingProgramInfo.ShaderStageFlag = SEShaderStage::SS_Vertex |
                                                  SEShaderStage::SS_Fragment;
    SERenderPassTargetsInfo targetsInfo;
    targetsInfo.ColorTargetCount = 1;
    targetsInfo.ColorTargetFormats[0] = format;
    SERenderPass* passIndirectLighting = new SERenderPass(
        indirectLightingProgramInfo, targetsInfo);

    SETechnique* techIndirectLighting = new SETechnique();
    techIndirectLighting->AddPass(passIndirectLighting);
    SEMaterialTemplate* mtIndirectLighting = new SEMaterialTemplate();
    mtIndirectLighting->AddTechnique(techIndirectLighting);

    SEMaterial* material = new SEMaterial(mtIndirectLighting);
    mIndirectLightingScreenQuad = new IndirectLightingScreenQuad(material);
    mIndirectLightingScreenQuad->LoadFromPLYFile("screenquad.ply");
    mIndirectLightingScreenQuad->SetTCoord(0, SEVector2f(0.0f, 0.0f));
    mIndirectLightingScreenQuad->SetTCoord(1, SEVector2f(1.0f, 0.0f));
    mIndirectLightingScreenQuad->SetTCoord(2, SEVector2f(1.0f, 1.0f));
    mIndirectLightingScreenQuad->SetTCoord(3, SEVector2f(0.0f, 1.0f));
    mIndirectLightingScreenQuad->VPLCount = vplCount;
    mIndirectLightingScreenQuad->PatternSize = patternSize;
    mIndirectLightingScreenQuad->SceneBB = sceneBB;
    mIndirectLightingScreenQuad->VoxelGridDim = voxelGridDim;
    mIndirectLightingScreenQuad->UseTC = useTC;
    mIndirectLightingScreenQuad->CreateDeviceResource(device);

    // Setup inputs.

    SERendererInputDataView view;
    view.Type = RDT_Texture;
    view.BindingType = BF_BindIndex;
    view.Sampler.MinFilter = FT_Nearest;
    view.Sampler.MagFilter = FT_Nearest;
    view.Sampler.WrapS = WT_Clamp;
    view.Sampler.WrapT = WT_Clamp;

    ClearInputDependency();

    view.BindingSlot = 0;
    AddInputDependency(gbufferSource, RTGI_GBufferSplitter_Position_Name, &view);

    view.BindingSlot = 1;
    AddInputDependency(gbufferSource, RTGI_GBufferSplitter_Normal_Name, &view);

    view.Type = RDT_StructuredBuffer;
    view.BindingType = BF_BindIndex;
    view.BindingSlot = 0;
    AddInputDependency(vplGenerator, RTGI_VPLGenerator_VPLBuffer_Name, &view);

    if( mVoxelizerType == SEVoxelizer::VT_Grid )
    {
        view.Type = RDT_StructuredBuffer;
        view.BindingType = BF_BindIndex;
        view.BindingSlot = 1;
        AddInputDependency(voxelizer, RTGI_GridVoxelizer_VoxelBuffer_Name,
            &view);
    }
    else if( mVoxelizerType == SEVoxelizer::VT_SVO )
    {
        // Cache SVO buffers.
        mVoxelFragmentListBuffer = ((SVOVoxelizer*)(
            SEVoxelizer*)voxelizer)->GetVoxelFragmentListBuffer();
        mSVOBuffer = ((SVOVoxelizer*)(SEVoxelizer*)voxelizer)->GetSVOBuffer();
    }
    else
    {
        assert(false);
    }

    // Create output.
    if( mUseTC )
    {
        IndirectLightingTexture = new SETexture2D();
        IndirectLightingTexture->CreateRenderTarget(mGPUDevice, width, height,
            format, false);
    }
    else
    {
        int w = width;
        int h = height;
        AddFrameBufferTarget(
            RTGI_IndirectLightingRenderer_IndirectLighting_Name, w, h, 0, 
            TT_Texture2D, format);
        CreateFrameBuffer(w, h, 0, TT_Texture2D);
        IndirectLightingTexture = (SETexture2D*)GetFrameBufferTextureByName(
            RTGI_IndirectLightingRenderer_IndirectLighting_Name);
    }
}
//----------------------------------------------------------------------------
void IndirectLightingRenderer::Render()
{
    if( mVoxelizerType == SEVoxelizer::VT_SVO )
    {
        mVoxelFragmentListBuffer->Bind(1);
        mSVOBuffer->Bind(3);
    }

    if( mUseTC )
    {
        IndirectLightingTexture->BindToImageUnit(0, BA_Read_Write);
        SESubRenderer::RenderSingle(mIndirectLightingScreenQuad, 0, 0,
            SRO_GenericImage, mPSB, 0);
    }
    else
    {
        SESubRenderer::RenderSingle(mIndirectLightingScreenQuad, 0, 0,
            SRO_FrameBuffer, mPSB, 0);
    }
}
//----------------------------------------------------------------------------
void IndirectLightingRenderer::VPLVisibilityTest(bool value)
{
    mIndirectLightingScreenQuad->VPLVisibilityTest = value;
}
//----------------------------------------------------------------------------