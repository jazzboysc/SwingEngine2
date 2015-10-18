#ifndef RTGI_SplittedBufferMerger_H
#define RTGI_SplittedBufferMerger_H

#include "SwingEngineRenderingEngine.h"

namespace Swing
{

#define RTGI_SplittedBufferMerger_MergedBuffer_Name  "MergedBuffer"

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 07/08/2015
//----------------------------------------------------------------------------
class SplittedBufferMergerScreenQuad : public SEScreenQuad
{
public:
	SplittedBufferMergerScreenQuad(SEMaterial* material);
	virtual ~SplittedBufferMergerScreenQuad();

	// Implement base class interface.
	virtual void OnUpdateShaderConstants(int technique, int pass);
	virtual void OnGetShaderConstants();

	SEVector4f TileInfo;  // x:tile width y:tile height z:m w:n

private:
	SEShaderUniform mTileInfoLoc;
	SEShaderUniform mSplittedBufferSamplerLoc;
};

typedef SESmartPointer<SplittedBufferMergerScreenQuad> SplittedBufferMergerScreenQuadPtr;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 07/08/2015
//----------------------------------------------------------------------------
class SplittedBufferMerger : public SESubRenderer
{
public:
	SplittedBufferMerger(SEGPUDevice* device, SERenderSet* renderSet = 0);
	virtual ~SplittedBufferMerger();

	void Initialize(SEGPUDevice* device, SESubRenderer* source, 
		const std::string& srcBufferName, const std::string& dstBufferName, 
		int m, int n);

	void Render();

private:
	SEPipelineStateBlockPtr mPSB;
	SplittedBufferMergerScreenQuadPtr mScreenQuad;
	int mTileM, mTileN;
	int mTileWidth, mTileHeight;
};

typedef SESmartPointer<SplittedBufferMerger> SplittedBufferMergerPtr;

}

#endif