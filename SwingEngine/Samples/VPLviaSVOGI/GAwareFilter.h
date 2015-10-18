#ifndef RTGI_GAwareFilter_H
#define RTGI_GAwareFilter_H

#include "SESubRenderer.h"
#include "SEVector4.h"
#include "SEScreenQuad.h"

namespace Swing
{

#define RTGI_GAwareFilter_FilteredBuffer_Name  "FilteredBuffer"

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 07/10/2015
//----------------------------------------------------------------------------
class GAwareFilterScreenQuad : public SEScreenQuad
{
public:
	GAwareFilterScreenQuad(SEMaterial* material);
	virtual ~GAwareFilterScreenQuad();

	// Implement base class interface.
	virtual void OnUpdateShaderConstants(int technique, int pass);
	virtual void OnGetShaderConstants();

    float Threshold[2];  // x:position y:normal
	SEVector4f TileInfo;  // x:tile width y:tile height z:m w:n

private:
    SEShaderUniform mThresholdLoc;
	SEShaderUniform mTileInfoLoc;
	SEShaderUniform mSrcBufferSamplerLoc;
	SEShaderUniform mGBufferPositionSamplerLoc;
	SEShaderUniform mGBufferNormalSamplerLoc;
};

typedef SESmartPointer<GAwareFilterScreenQuad> GAwareFilterScreenQuadPtr;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 07/10/2015
//----------------------------------------------------------------------------
class GAwareFilter : public SESubRenderer
{
public:
	GAwareFilter(SEGPUDevice* device, SERenderSet* renderSet = 0);
	virtual ~GAwareFilter();

	void Initialize(SEGPUDevice* device, SESubRenderer* gbufferSrc, 
		SESubRenderer* source, const std::string& srcBufferName, 
		const std::string& dstBufferName, int m, int n, 
        float thresholdPostion, float thresholdNormal);

	void Render();

private:
	SEPipelineStateBlockPtr mPSB;
	GAwareFilterScreenQuadPtr mScreenQuad;
	int mTileM, mTileN;
	int mTileWidth, mTileHeight;
};

typedef SESmartPointer<GAwareFilter> GAwareFilterPtr;

}

#endif