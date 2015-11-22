#include "VPLviaSVOGI.pch.h"
#include "SplittedBufferMerger.h"

using namespace Swing;

//----------------------------------------------------------------------------
SplittedBufferMergerScreenQuad::SplittedBufferMergerScreenQuad(
	SEMaterial* material)
	:
	SEScreenQuad(material, 0)
{
}
//----------------------------------------------------------------------------
SplittedBufferMergerScreenQuad::~SplittedBufferMergerScreenQuad()
{
}
//----------------------------------------------------------------------------
void SplittedBufferMergerScreenQuad::OnUpdateShaderConstants(int, int)
{
	mSplittedBufferSamplerLoc.SetValue(0);
	mTileInfoLoc.SetValue(TileInfo);
}
//----------------------------------------------------------------------------
void SplittedBufferMergerScreenQuad::OnGetShaderConstants()
{
	SEShaderProgram* program = mMaterial->GetProgram(0, 0);

	program->GetUniformLocation(&mSplittedBufferSamplerLoc, "SplittedBufferSampler");
	program->GetUniformLocation(&mTileInfoLoc, "TileInfo");
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
SplittedBufferMerger::SplittedBufferMerger(SEGPUDevice* device, 
	SERenderSet* renderSet)
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
SplittedBufferMerger::~SplittedBufferMerger()
{
	mPSB = 0;
}
//----------------------------------------------------------------------------
void SplittedBufferMerger::Initialize(SEGPUDevice* device, SESubRenderer* source,
	const std::string& srcBufferName, const std::string& dstBufferName, 
	int m, int n)
{
	assert(device && source);

	SETexture2D* srcBuffer = (SETexture2D*)source->GetFrameBufferTextureByName(
		srcBufferName);
	assert(srcBuffer);

	mTileM = m;
	mTileN = n;
	mTileWidth = srcBuffer->Width / m;
	mTileHeight = srcBuffer->Height / n;

	SEShaderProgramInfo programInfo;
	programInfo.VShaderFileName = "VPLviaSVOGI/vSplittedBufferMerger.glsl";
	programInfo.FShaderFileName = "VPLviaSVOGI/fSplittedBufferMerger.glsl";
	programInfo.ShaderStageFlag = SEShaderStage::SS_Vertex |
		                          SEShaderStage::SS_Fragment;
    SERenderPassTargetsInfo targetsInfo;
    targetsInfo.ColorTargetCount = 1;
    targetsInfo.ColorTargetFormats[0] = srcBuffer->GetFormat();
	SERenderPass* passSplittedBufferMerger = new SERenderPass(programInfo, targetsInfo);

	SETechnique* techSplittedBufferMerger = new SETechnique();
	techSplittedBufferMerger->AddPass(passSplittedBufferMerger);
	SEMaterialTemplate* mtSplittedBufferMerger = new SEMaterialTemplate();
	mtSplittedBufferMerger->AddTechnique(techSplittedBufferMerger);

	SEMaterial* material = new SEMaterial(mtSplittedBufferMerger);
	mScreenQuad = new SplittedBufferMergerScreenQuad(material);
	mScreenQuad->LoadFromPLYFile("screenquad.ply");
	mScreenQuad->SetTCoord(0, SEVector2f(0.0f, 0.0f));
	mScreenQuad->SetTCoord(1, SEVector2f(1.0f, 0.0f));
	mScreenQuad->SetTCoord(2, SEVector2f(1.0f, 1.0f));
	mScreenQuad->SetTCoord(3, SEVector2f(0.0f, 1.0f));
	mScreenQuad->CreateDeviceResource(device);
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

	// Create output.

	AddFrameBufferTarget(dstBufferName, srcBuffer->Width, srcBuffer->Height, 
		0, TT_Texture2D, srcBuffer->GetFormat(), srcBuffer->HasMipMap);

	CreateFrameBuffer(srcBuffer->Width, srcBuffer->Height, 0, TT_Texture2D);
}
//----------------------------------------------------------------------------
void SplittedBufferMerger::Render()
{
	SESubRenderer::RenderSingle(mScreenQuad, 0, 0,
		SRO_FrameBuffer, mPSB, 0);
}
//----------------------------------------------------------------------------