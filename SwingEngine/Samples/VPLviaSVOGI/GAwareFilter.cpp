#include "GAwareFilter.h"
#include "SEGBufferRenderer.h"

using namespace Swing;

//----------------------------------------------------------------------------
GAwareFilterScreenQuad::GAwareFilterScreenQuad(SEMaterial* material)
	:
	SEScreenQuad(material, 0)
{
    Threshold[0] = 1.0f;
    Threshold[1] = 0.8f;
}
//----------------------------------------------------------------------------
GAwareFilterScreenQuad::~GAwareFilterScreenQuad()
{
}
//----------------------------------------------------------------------------
void GAwareFilterScreenQuad::OnUpdateShaderConstants(int, int)
{
	mSrcBufferSamplerLoc.SetValue(0);
	mGBufferPositionSamplerLoc.SetValue(1);
	mGBufferNormalSamplerLoc.SetValue(2);
    mThresholdLoc.SetValue(Threshold);
	mTileInfoLoc.SetValue(TileInfo);
}
//----------------------------------------------------------------------------
void GAwareFilterScreenQuad::OnGetShaderConstants()
{
	SEShaderProgram* program = mMaterial->GetProgram(0, 0);

	program->GetUniformLocation(&mSrcBufferSamplerLoc, "SrcBufferSampler");
    program->GetUniformLocation(&mThresholdLoc, "Threshold");
	program->GetUniformLocation(&mTileInfoLoc, "TileInfo");
	program->GetUniformLocation(&mGBufferPositionSamplerLoc,
		"GBufferPositionSampler");
	program->GetUniformLocation(&mGBufferNormalSamplerLoc,
		"GBufferNormalSampler");
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
GAwareFilter::GAwareFilter(SEGPUDevice* device, SERenderSet* renderSet)
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
GAwareFilter::~GAwareFilter()
{
	mPSB = 0;
}
//----------------------------------------------------------------------------
void GAwareFilter::Initialize(SEGPUDevice* device, SESubRenderer* gbufferSrc, 
	SESubRenderer* source, const std::string& srcBufferName, 
    const std::string& dstBufferName, int m, int n, float thresholdPostion, 
    float thresholdNormal)
{
	assert(device && gbufferSrc && source);

	SETexture2D* srcBuffer = (SETexture2D*)source->GetFrameBufferTextureByName(
		srcBufferName);
	assert(srcBuffer);

	mTileM = m;
	mTileN = n;
	mTileWidth = srcBuffer->Width / m;
	mTileHeight = srcBuffer->Height / n;

	SEShaderProgramInfo programInfo;
	programInfo.VShaderFileName = "VPLviaSVOGI/vGAwareFilter.glsl";
	programInfo.FShaderFileName = "VPLviaSVOGI/fGAwareFilter.glsl";
	programInfo.ShaderStageFlag = SEShaderStage::SS_Vertex |
		                          SEShaderStage::SS_Fragment;
	SEPass* passGAwareFilter = new SEPass(programInfo);

	SETechnique* techGAwareFilter = new SETechnique();
	techGAwareFilter->AddPass(passGAwareFilter);
	SEMaterialTemplate* mtGAwareFilter = new SEMaterialTemplate();
	mtGAwareFilter->AddTechnique(techGAwareFilter);

	SEMaterial* material = new SEMaterial(mtGAwareFilter);
	mScreenQuad = new GAwareFilterScreenQuad(material);
	mScreenQuad->LoadFromPLYFile("screenquad.ply");
	mScreenQuad->SetTCoord(0, SEVector2f(0.0f, 0.0f));
	mScreenQuad->SetTCoord(1, SEVector2f(1.0f, 0.0f));
	mScreenQuad->SetTCoord(2, SEVector2f(1.0f, 1.0f));
	mScreenQuad->SetTCoord(3, SEVector2f(0.0f, 1.0f));
	mScreenQuad->CreateDeviceResource(device);
    mScreenQuad->Threshold[0] = thresholdPostion;
    mScreenQuad->Threshold[1] = thresholdNormal;
	mScreenQuad->TileInfo = SEVector4f(float(mTileWidth), float(mTileHeight),
		float(mTileM), float(mTileN));

	// Setup input.

	SERendererInputDataView view;
	view.Type = RDT_Texture;
	view.BindingType = BF_BindIndex;
	view.Sampler.MinFilter = FT_Nearest;
	view.Sampler.MagFilter = FT_Nearest;
	view.Sampler.WrapS = WT_Clamp;
	view.Sampler.WrapT = WT_Clamp;

	ClearInputDependency();

	view.BindingSlot = 0;
	AddInputDependency(source, srcBufferName, &view);

	view.BindingSlot = 1;
	AddInputDependency(gbufferSrc, RTGI_GBuffer_Position_Name, &view);

	view.BindingSlot = 2;
	AddInputDependency(gbufferSrc, RTGI_GBuffer_Normal_Name, &view);

	// Create output.

	AddFrameBufferTarget(dstBufferName, srcBuffer->Width, srcBuffer->Height,
		0, TT_Texture2D, srcBuffer->GetFormat(), srcBuffer->HasMipMap);

	CreateFrameBuffer(srcBuffer->Width, srcBuffer->Height, 0, TT_Texture2D);
}
//----------------------------------------------------------------------------
void GAwareFilter::Render()
{
	SESubRenderer::RenderSingle(mScreenQuad, 0, 0,
		SRO_FrameBuffer, mPSB, 0);
}
//----------------------------------------------------------------------------