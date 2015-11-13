#include "VPLviaSVOGI.pch.h"
#include "DirectLightingRenderer.h"

using namespace Swing;

//----------------------------------------------------------------------------
DirectLightingScreenQuad::DirectLightingScreenQuad(SEMaterial* material)
    :
    SEScreenQuad(material, 0)
{
    ShowShadow = true;
}
//----------------------------------------------------------------------------
DirectLightingScreenQuad::~DirectLightingScreenQuad()
{
}
//----------------------------------------------------------------------------
void DirectLightingScreenQuad::OnUpdateShaderConstants(int, int)
{
    mGBufferPositionSamplerLoc.SetValue(0);
    mGBufferNormalSamplerLoc.SetValue(1);
    mGBufferAlbedoSamplerLoc.SetValue(2);
    mShadowMapSamplerLoc.SetValue(3);
    mShowShadow.SetValue(ShowShadow);
}
//----------------------------------------------------------------------------
void DirectLightingScreenQuad::OnGetShaderConstants()
{
    SEShaderProgram* program = mMaterial->GetProgram(0, 0);
    assert(program);

    if( program )
    {
        program->GetUniformLocation(&mGBufferPositionSamplerLoc, "GBufferPositionSampler");
        program->GetUniformLocation(&mGBufferNormalSamplerLoc, "GBufferNormalSampler");
        program->GetUniformLocation(&mGBufferAlbedoSamplerLoc, "GBufferAlbedoSampler");
        program->GetUniformLocation(&mShadowMapSamplerLoc, "ShadowMapSampler");
        program->GetUniformLocation(&mShowShadow, "ShowShadow");
    }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
DirectLightingRenderer::DirectLightingRenderer(SEGPUDevice* device, 
    SERenderSet* renderSet)
    :
    SESubRenderer(device, renderSet)
{
    mPSB = new SEPipelineStateBlock();
    mPSB->PipelineStageFlag |= PB_OutputMerger;
    mPSB->OutputMerger.OutputMergerOpFlag |= OMB_Clear;
    mPSB->OutputMerger.ClearMask = OMCB_Color_Buffer | OMCB_Depth_Buffer;
}
//----------------------------------------------------------------------------
DirectLightingRenderer::~DirectLightingRenderer()
{
    mPSB = 0;
}
//----------------------------------------------------------------------------
void DirectLightingRenderer::Initialize(SEGPUDevice* device, int width, 
    int height, SEBufferFormat format, SEGBufferRenderer* gbufferRenderer, 
    ShadowMapsGenerator* shadowMapsGenerator)
{
    SEShaderProgramInfo directLightingProgramInfo;
    directLightingProgramInfo.VShaderFileName = "VPLviaSVOGI/vDirectLighting.glsl";
    directLightingProgramInfo.FShaderFileName = "VPLviaSVOGI/fDirectLighting.glsl";
    directLightingProgramInfo.ShaderStageFlag = SEShaderStage::SS_Vertex |
        SEShaderStage::SS_Fragment;
    SERenderPassTargetsInfo directLightingTargetsInfo;
    directLightingTargetsInfo.ColorTargetCount = 1;
    directLightingTargetsInfo.ColorTargetFormats[0] = format;
    directLightingTargetsInfo.DepthFormat = BF_Depth;
    directLightingTargetsInfo.StencilFormat = BF_Unknown;
    SERenderPass* passDirectLighting = new SERenderPass(
        directLightingProgramInfo, directLightingTargetsInfo);

    SETechnique* techDirectLighting = new SETechnique();
    techDirectLighting->AddPass(passDirectLighting);
    SEMaterialTemplate* mtDirectLighting = new SEMaterialTemplate();
    mtDirectLighting->AddTechnique(techDirectLighting);

    SEMaterial* material = new SEMaterial(mtDirectLighting);
    mDirectLightingScreenQuad = new DirectLightingScreenQuad(material);
    mDirectLightingScreenQuad->LoadFromPLYFile("screenquad.ply");
    mDirectLightingScreenQuad->SetTCoord(0, SEVector2f(0.0f, 0.0f));
    mDirectLightingScreenQuad->SetTCoord(1, SEVector2f(1.0f, 0.0f));
    mDirectLightingScreenQuad->SetTCoord(2, SEVector2f(1.0f, 1.0f));
    mDirectLightingScreenQuad->SetTCoord(3, SEVector2f(0.0f, 1.0f));
    mDirectLightingScreenQuad->CreateDeviceResource(device);

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
    AddInputDependency(gbufferRenderer, RTGI_GBuffer_Position_Name, &view);

    view.BindingSlot = 1;
    AddInputDependency(gbufferRenderer, RTGI_GBuffer_Normal_Name, &view);

    view.BindingSlot = 2;
    AddInputDependency(gbufferRenderer, RTGI_GBuffer_Albedo_Name, &view);

    view.BindingSlot = 3;
    AddInputDependency(shadowMapsGenerator, 
        RTGI_ShadowMapRenderer_ShadowMaps_Name, &view);

    // Create output.
    AddFrameBufferTarget(RTGI_DirectLightingRenderer_DirectLighting_Name, 
        width, height, 0, TT_Texture2D, format);
    CreateFrameBuffer(width, height, 0, TT_Texture2D);
}
//----------------------------------------------------------------------------
void DirectLightingRenderer::Render()
{
    SESubRenderer::RenderSingle(mDirectLightingScreenQuad, 0, 0, 
        SRO_FrameBuffer, mPSB, 0);
}
//----------------------------------------------------------------------------
void DirectLightingRenderer::ShowShadow(bool value)
{
    mDirectLightingScreenQuad->ShowShadow = value;
}
//----------------------------------------------------------------------------