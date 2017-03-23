#include "VPLviaSVOGI.pch.h"
#include "VPLGenerator.h"

using namespace Swing;

//#define VPL_DEBUG

//----------------------------------------------------------------------------
SampleRSM::SampleRSM(bool vertexCompute)
    :
	VPLCount(0),
    mVertexCompute(vertexCompute)
{
}
//----------------------------------------------------------------------------
SampleRSM::~SampleRSM()
{
    VPLSamplePattern = 0;
	RSMFluxMipmapMaxLevel = 0;
}
//----------------------------------------------------------------------------
void SampleRSM::OnGetShaderConstants()
{
    SEComputePass* p = (SEComputePass*)GetPass(0);
    SEShaderProgram* program = p->GetShaderProgram();

	program->GetUniformLocation(&mVPLCountLoc, "VPLCount");
    program->GetUniformLocation(&mRSMPositionLoc, "RSMPosition");
    program->GetUniformLocation(&mRSMNormalLoc, "RSMNormal");
    program->GetUniformLocation(&mRSMFluxLoc, "RSMFlux");
	program->GetUniformLocation(&mVPLSamplePatternLoc, "VPLSamplePattern");
	program->GetUniformLocation(&mRSMFluxMipmapMaxLevelLoc, "RSMFluxMipmapMaxLevel");
}
//----------------------------------------------------------------------------
void SampleRSM::OnPreDispatch(unsigned int)
{
    // imageLoad function in vertex shader is broken since NVIDIA 350.12 driver.
    //VPLSamplePattern->BindToImageUnit(0, BA_Read_Only);
    SESamplerDesc sd;
    sd.MinFilter = FT_Nearest;
    sd.MagFilter = FT_Nearest;
    sd.WrapS = WT_Clamp;
    sd.WrapT = WT_Clamp;
    VPLSamplePattern->BindToSampler(3, &sd);

	mVPLCountLoc.SetValue(VPLCount);
    mRSMPositionLoc.SetValue(0);
    mRSMNormalLoc.SetValue(1);
    mRSMFluxLoc.SetValue(2);
	mVPLSamplePatternLoc.SetValue(3);
	mRSMFluxMipmapMaxLevelLoc.SetValue(RSMFluxMipmapMaxLevel);
}
//----------------------------------------------------------------------------
void SampleRSM::OnPostDispatch(unsigned int)
{
    mDevice->GPUMemoryBarrier(MBT_Structured);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
VPLGenerator::VPLGenerator(SEGPUDevice* device, SERenderSet* renderSet)
    :
    SESubRenderer(device, renderSet)
{
}
//----------------------------------------------------------------------------
VPLGenerator::~VPLGenerator()
{
}
//----------------------------------------------------------------------------
RSMRenderer* VPLGenerator::GetRSMRenderer() const
{
    return mRSM;
}
//----------------------------------------------------------------------------
void VPLGenerator::Run()
{
    SESubRenderer::Render(0, 0, SRO_GenericBuffer, 0);
}
//----------------------------------------------------------------------------
void VPLGenerator::OnRender(int, int, SERTGICamera*)
{
    if( mVertexCompute )
    {
        mSampleRSMTask->DispatchVertex(0, mVPLCount);

#ifdef VPL_DEBUG
		mVPLBuffer->Bind();
		void* bufferData = mVPLBuffer->Map(BA_Read_Only);
		VPLBufferHead* head = (VPLBufferHead*)bufferData;
		VPL* vpls = (VPL*)(((VPLBufferHead*)bufferData) + 1);
		mVPLBuffer->Unmap();
#endif
    }
    else
    {
        mSampleRSMTask->DispatchCompute(0, mVPLCount, 1, 1);
    }
}
//----------------------------------------------------------------------------
void VPLGenerator::Initialize(SEGPUDevice* device, RSMRenderer* rsm, 
	int vplCount, bool vertexCompute)
{
	assert(device && rsm);

    mVPLCount = vplCount;
    mVertexCompute = vertexCompute;
	mRSM = rsm;

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
	AddInputDependency(rsm, RTGI_RSMRenderer_RSMPosition_Name, &view);

	view.BindingSlot = 1;
	AddInputDependency(rsm, RTGI_RSMRenderer_RSMNormal_Name, &view);

	view.BindingSlot = 2;
	AddInputDependency(rsm, RTGI_RSMRenderer_RSMFlux_Name, &view);

    // Create VPL sample pattern.
    mVPLSamplePattern = new SETexture1D();
    //mVPLSamplePattern->CreateUniformRandomTexture(device, mVPLCount, 4);
    mVPLSamplePattern->CreateHaltonRandomTexture(device, mVPLCount, 4);

    // Create VPL buffer.
    unsigned int vplBufferSize = sizeof(VPLBufferHead) + sizeof(VPL)*mVPLCount;
    AddGenericBufferTarget(RTGI_VPLGenerator_VPLBuffer_Name, 
        RDT_StructuredBuffer, vplBufferSize, BU_Dynamic_Draw, BF_BindIndex, 0);
	mVPLBuffer = (SEStructuredBuffer*)GetGenericBufferByName(
		RTGI_VPLGenerator_VPLBuffer_Name);

    // Create VPL sample compute tasks.
    SEShaderProgramInfo sampleRSMProgramInfo;
    if( mVertexCompute )
    {
        sampleRSMProgramInfo.VShaderFileName = "VPLviaSVOGI/vSampleRSM.glsl";
        sampleRSMProgramInfo.ShaderStageFlag = SEShaderStage::SS_Vertex;
    }
    else
    {
        sampleRSMProgramInfo.CShaderFileName = "VPLviaSVOGI/cSampleRSM.glsl";
        sampleRSMProgramInfo.ShaderStageFlag = SEShaderStage::SS_Compute;
    }
    SEComputePass* passSampleRSM = new SEComputePass(sampleRSMProgramInfo);
    mSampleRSMTask = new SampleRSM(mVertexCompute);
    mSampleRSMTask->AddPass(passSampleRSM);
    mSampleRSMTask->CreateDeviceResource(device);
    mSampleRSMTask->VPLSamplePattern = mVPLSamplePattern;
	mSampleRSMTask->VPLCount = mVPLCount;
	mSampleRSMTask->RSMFluxMipmapMaxLevel = mRSM->GetRSMFluxMipmapMaxLevel();
}
//----------------------------------------------------------------------------