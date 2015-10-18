#include "VPLviaSVOGI.pch.h"
#include "ShadowMapsGenerator.h"

using namespace Swing;

//----------------------------------------------------------------------------
ShadowMapsGenerator::ShadowMapsGenerator(SEGPUDevice* device, SERenderSet* renderSet)
    :
    SESubRenderer(device, renderSet),
    mShadowMapInfoBufferBindingPoint(0)
{
    mPSB = new SEPipelineStateBlock();
    mPSB->PipelineStageFlag |= PB_OutputMerger;
    mPSB->OutputMerger.OutputMergerOpFlag |= OMB_Clear;
    mPSB->OutputMerger.ClearMask = OMCB_Color_Buffer | OMCB_Depth_Buffer;

    mShadowMapCount = 0;
}
//----------------------------------------------------------------------------
ShadowMapsGenerator::~ShadowMapsGenerator()
{
    mPSB = 0;
    mLightManager = 0;
    mShadowMapInfoBuffer = 0;
}
//----------------------------------------------------------------------------
void ShadowMapsGenerator::Initialize(int width, int height,
    SEBufferFormat format, SELightManager* lightManager)
{
    assert(lightManager);
    mLightManager = lightManager;

    // Compute how many shadow maps are needed.
    int pointLightCount = mLightManager->GetPointLightCount();
    int spotLightCount = mLightManager->GetSpotLightCount();
    mShadowMapCount = pointLightCount + spotLightCount;

    // Create shadow map array.
    AddFrameBufferTarget(RTGI_ShadowMapRenderer_ShadowMaps_Name, width, height,
        mShadowMapCount, TT_Texture2DArray, format);
    CreateFrameBuffer(width, height, mShadowMapCount, TT_Texture2DArray);

    // Create shadow map info uniform buffer.
    mShadowMapInfoBuffer = new SEUniformBuffer();
    size_t bufferSize = sizeof(ShadowMapInfo);
    mShadowMapInfoBuffer->ReserveMutableDeviceResource(mDevice, bufferSize,
        BU_Dynamic_Copy);
}
//----------------------------------------------------------------------------
void ShadowMapsGenerator::Render(int technique, int pass)
{
	if( mLightManager->GetPointLightCount() == 0 &&
		mLightManager->GetSpotLightCount() == 0 )
	{
		return;
	}

    PreRender(SRO_FrameBuffer, mPSB);

    int i = 0;
    for( ; i < mLightManager->GetPointLightCount(); ++i )
    {
        // Update shadow map info uniform buffer. Let the shader program know
        // which light is being used for shadow map generation.
        mShadowMapInfoBuffer->UpdateSubData(mShadowMapInfoBufferBindingPoint, 
            0, sizeof(unsigned int), (void*)&i);

        // We don't need to pass in a camera object since we are using a light
        // uniform buffer updated by light manager.
        SESubRenderer::OnRender(technique, pass, 0);
    }
    for( int j = 0; j < mLightManager->GetSpotLightCount(); ++j )
    {
        int curLightIndex = i + j;
        mShadowMapInfoBuffer->UpdateSubData(mShadowMapInfoBufferBindingPoint,
            0, sizeof(unsigned int), (void*)&curLightIndex);

        // Assume spot light shadow pass = point light shadow pass + 1.
        SESubRenderer::OnRender(technique, pass + 1, 0);
    }

    PostRender(SRO_FrameBuffer, mPSB);
}
//----------------------------------------------------------------------------