#include "VPLviaSVOGI.pch.h"
#include "GBufferSplitter.h"

using namespace Swing;

//----------------------------------------------------------------------------
GBufferSplitterScreenQuad::GBufferSplitterScreenQuad(SEMaterial* material)
    :
    SEScreenQuad(material, 0)
{
}
//----------------------------------------------------------------------------
GBufferSplitterScreenQuad::~GBufferSplitterScreenQuad()
{
}
//----------------------------------------------------------------------------
void GBufferSplitterScreenQuad::OnUpdateShaderConstants(int, int)
{
	mGBufferPositionSamplerLoc.SetValue(0);
	mGBufferNormalSamplerLoc.SetValue(1);
	mTileInfoLoc.SetValue(TileInfo);
}
//----------------------------------------------------------------------------
void GBufferSplitterScreenQuad::OnGetShaderConstants()
{
	SEShaderProgram* program = mMaterial->GetProgram(0, 0);

	program->GetUniformLocation(&mGBufferPositionSamplerLoc, "GBufferPositionSampler");
	program->GetUniformLocation(&mGBufferNormalSamplerLoc, "GBufferNormalSampler");
	program->GetUniformLocation(&mTileInfoLoc, "TileInfo");
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
GBufferSplitter::GBufferSplitter(SEGPUDevice* device, SERenderSet* renderSet)
    :
    SESubRenderer(device, renderSet),
	mTileM(0),
	mTileN(0)
{
    mPSB = new SEPipelineStateBlock();
    mPSB->PipelineStageFlag |= PB_OutputMerger;
    mPSB->OutputMerger.OutputMergerOpFlag |= OMB_Clear;
    mPSB->OutputMerger.ClearMask = OMCB_Color_Buffer | OMCB_Depth_Buffer;
}
//----------------------------------------------------------------------------
GBufferSplitter::~GBufferSplitter()
{
    mPSB = 0;
}
//----------------------------------------------------------------------------
void GBufferSplitter::Initialize(SEGPUDevice* device, 
    SEGBufferRenderer* gbufferRenderer, int m, int n)
{
	assert(device && gbufferRenderer && m > 0 && n > 0);

	const SEGBufferDesc& desc = gbufferRenderer->GetGBufferDesc();
	mTileM = m;
	mTileN = n;
	mTileWidth = desc.Width / m;
	mTileHeight = desc.Height / n;

    SEShaderProgramInfo programInfo;
    programInfo.VShaderFileName = "VPLviaSVOGI/vGBufferSplitter.glsl";
    programInfo.FShaderFileName = "VPLviaSVOGI/fGBufferSplitter.glsl";
    programInfo.ShaderStageFlag = SEShaderStage::SS_Vertex |
                                  SEShaderStage::SS_Fragment;
    SERenderPassTargetsInfo targetsInfo;
    targetsInfo.ColorTargetCount = 2;
    targetsInfo.ColorTargetFormats[0] = desc.PositionFormat;
    targetsInfo.ColorTargetFormats[1] = desc.NormalFormat;
    SERenderPass* passGBufferSplitter = new SERenderPass(programInfo, targetsInfo);

    SETechnique* techGBufferSplitter = new SETechnique();
    techGBufferSplitter->AddPass(passGBufferSplitter);
    SEMaterialTemplate* mtGBufferSplitter = new SEMaterialTemplate();
    mtGBufferSplitter->AddTechnique(techGBufferSplitter);

    SEMaterial* material = new SEMaterial(mtGBufferSplitter);
    mGBufferSplitterScreenQuad = new GBufferSplitterScreenQuad(material);
    mGBufferSplitterScreenQuad->LoadFromPLYFile("screenquad.ply");
    mGBufferSplitterScreenQuad->SetTCoord(0, SEVector2f(0.0f, 0.0f));
    mGBufferSplitterScreenQuad->SetTCoord(1, SEVector2f(1.0f, 0.0f));
    mGBufferSplitterScreenQuad->SetTCoord(2, SEVector2f(1.0f, 1.0f));
    mGBufferSplitterScreenQuad->SetTCoord(3, SEVector2f(0.0f, 1.0f));
    mGBufferSplitterScreenQuad->CreateDeviceResource(device);
	mGBufferSplitterScreenQuad->TileInfo = SEVector4f(float(mTileWidth), 
		float(mTileHeight), float(mTileM), float(mTileN));

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

    // Create outputs.
    AddFrameBufferTarget(RTGI_GBufferSplitter_Position_Name,
        desc.Width, desc.Height, 0, TT_Texture2D, desc.PositionFormat, 
        desc.PositionMipmap);

    AddFrameBufferTarget(RTGI_GBufferSplitter_Normal_Name,
        desc.Width, desc.Height, 0, TT_Texture2D, desc.NormalFormat,
        desc.NormalMipmap);

    CreateFrameBuffer(desc.Width, desc.Height, 0, TT_Texture2D);
}
//----------------------------------------------------------------------------
void GBufferSplitter::Render()
{
    SESubRenderer::RenderSingle(mGBufferSplitterScreenQuad, 0, 0,
        SRO_FrameBuffer, mPSB, 0);
}
//----------------------------------------------------------------------------