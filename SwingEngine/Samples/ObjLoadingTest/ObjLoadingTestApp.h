#ifndef RTGI_ObjLoadingTestApp_H
#define RTGI_ObjLoadingTestApp_H

#include "ObjMesh.h"
#include "SEApplication.h"
#include "SwingEngineAssetUtility.h"
#include "SwingEngineRenderingEngine.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 06/13/2015
//----------------------------------------------------------------------------
class ObjLoadingTestApp : public SEApplication
{
public:
    ObjLoadingTestApp(int width = 1024, int height = 768);
    ~ObjLoadingTestApp();

	void Initialize(SEGPUDevice* device) override;
	void ProcessInput() override;
	void FrameFunc() override;
	void Terminate() override;

	int ShowMode;

private:
    std::vector<ObjMeshPtr> mMeshes;
    ObjMeshPtr mModel;
    SERenderSequencePtr mModel2Sequence;
	bool mIsWireframe;
    SEPipelineStateBlockPtr mWireframePSB;
    SEPipelineStateBlockPtr mSolidPSB;
};

}

#endif