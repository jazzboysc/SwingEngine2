#include "VPLviaSVOGI.pch.h"
#include "RSMRenderer.h"

using namespace Swing;

//----------------------------------------------------------------------------
RSMRenderer::RSMRenderer(SEGPUDevice* device, SERenderSet* renderSet)
    :
    SESubRenderer(device, renderSet),
    mRSMInfoBufferBindingPoint(0)
{
    mPSB = new SEPipelineStateBlock();
    mPSB->PipelineStageFlag |= PB_OutputMerger;
    mPSB->OutputMerger.OutputMergerOpFlag |= OMB_Clear;
    mPSB->OutputMerger.ClearMask = OMCB_Color_Buffer | OMCB_Depth_Buffer;

    mPointLightFaceCount = 0;
    mRSMCount = 0;
	mRSMFluxMipmapMaxLevel = 0;
}
//----------------------------------------------------------------------------
RSMRenderer::~RSMRenderer()
{
    mPSB = 0;
    mLightManager = 0;
    mRSMInfoBuffer = 0;
}
//----------------------------------------------------------------------------
void RSMRenderer::Initialize(int width, int height, int pointLightFaceCount, 
	SEBufferFormat format, SELightManager* lightManager, bool fluxMipmap)
{
    assert(lightManager);
    mLightManager = lightManager;
    mPointLightFaceCount = pointLightFaceCount;

    // Compute how many RSMs are needed.
    int pointLightCount = mLightManager->GetPointLightCount();
    int spotLightCount = mLightManager->GetSpotLightCount();
    mRSMCount = pointLightCount*mPointLightFaceCount + spotLightCount;

    // Create RSM texture array.
    AddFrameBufferTarget(RTGI_RSMRenderer_RSMPosition_Name, width, height, 
        mRSMCount, TT_Texture2DArray, format);
    AddFrameBufferTarget(RTGI_RSMRenderer_RSMNormal_Name, width, height, 
        mRSMCount, TT_Texture2DArray, format);
    AddFrameBufferTarget(RTGI_RSMRenderer_RSMFlux_Name, width, height, 
		mRSMCount, TT_Texture2DArray, format, fluxMipmap);
    CreateFrameBuffer(width, height, mRSMCount, TT_Texture2DArray);

	unsigned int wLevel = (unsigned int)SEMathf::Log2((float)width);
    unsigned int hLevel = (unsigned int)SEMathf::Log2((float)height);
	mRSMFluxMipmapMaxLevel = SE_MIN(wLevel, hLevel);
	mRSMFluxMipmapMaxLevel -= 1;

    // Create RSM info uniform buffer.
    mRSMInfoBuffer = new SEUniformBuffer();
    size_t bufferSize = sizeof(RSMInfo);
    mRSMInfoBuffer->ReserveMutableDeviceResource(mGPUDevice, bufferSize,
        BU_Dynamic_Copy);
}
//----------------------------------------------------------------------------
void RSMRenderer::Render(int technique, int pass)
{
	if( mLightManager->GetPointLightCount() == 0 &&
		mLightManager->GetSpotLightCount() == 0 )
	{
		return;
	}

    PreRender(SRO_FrameBuffer, mPSB);

    int i = 0;
    int bufferData[2];
    size_t bufferSize = sizeof(bufferData);

    for( ; i < mLightManager->GetPointLightCount(); ++i )
    {
        // Update RSM info uniform buffer. Let the shader program know
        // which light is being used for RSM generation and where to output
        // the result.
        bufferData[0] = i;
        bufferData[1] = i*mPointLightFaceCount;
        mRSMInfoBuffer->UpdateSubData(mRSMInfoBufferBindingPoint, 0, 
            bufferSize, (void*)bufferData);

        // We don't need to pass in a camera object since we are using a light
        // uniform buffer updated by light manager.
        SESubRenderer::OnRender(technique, pass, 0);
    }

    bufferData[1] = i*mPointLightFaceCount;
    for( int j = 0; j < mLightManager->GetSpotLightCount(); ++j )
    {
        int curLightIndex = i + j;
        bufferData[0] = curLightIndex;
        mRSMInfoBuffer->UpdateSubData(mRSMInfoBufferBindingPoint, 0, 
            bufferSize, (void*)bufferData);

        // Assume spot light RSM pass = point light RSM pass + 1.
        SESubRenderer::OnRender(technique, pass + 1, 0);
    }

    PostRender(SRO_FrameBuffer, mPSB);
}
//----------------------------------------------------------------------------
