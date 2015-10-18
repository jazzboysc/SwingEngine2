#ifndef RTGI_VPLGenerator_H
#define RTGI_VPLGenerator_H

#include "SwingEngineRenderingEngine.h"
#include "RSMRenderer.h"
#include "SEComputeTask.h"
#include "SETexture1D.h"
#include "SETexture2DArray.h"
#include "SEStructuredBuffer.h"

namespace Swing
{

#define RTGI_VPLGenerator_VPLBuffer_Name "VPLBuffer"

struct VPLBufferHead
{
    SEVector2f prevLevelStartXY;
    SEVector2f curLevelStartXY;
    float curLevelWarpExtension;
    float padding1;
    SEVector2f curLevelWarpStartXY;
    float importance00;
    float importance10;
    float importance01;
    float importance11;
};

struct VPL
{
    SEVector4f WorldPosition;
    SEVector4f WorldNormal;
    SEVector4f Flux;
    SEMatrix4f View;
};

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/29/2014
//----------------------------------------------------------------------------
class SampleRSM : public SEComputeTask
{
public:
    SampleRSM(bool vertexCompute = true);
    ~SampleRSM();

    // Implement base class interface.
    virtual void OnGetShaderConstants();
    virtual void OnPreDispatch(unsigned int pass);
    virtual void OnPostDispatch(unsigned int pass);

    SETexture1DPtr VPLSamplePattern;
	int VPLCount;
	int RSMFluxMipmapMaxLevel;

private:
    bool mVertexCompute;
	SEShaderUniform mVPLCountLoc;
    SEShaderUniform mRSMPositionLoc;
    SEShaderUniform mRSMNormalLoc;
    SEShaderUniform mRSMFluxLoc;
	SEShaderUniform mVPLSamplePatternLoc;
	SEShaderUniform mRSMFluxMipmapMaxLevelLoc;
};

typedef SESmartPointer<SampleRSM> SampleRSMPtr;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 12/05/2014
//----------------------------------------------------------------------------
class VPLGenerator : public SESubRenderer
{
public:
    VPLGenerator(SEGPUDevice* device, SERenderSet* renderSet = 0);
    virtual ~VPLGenerator();

    RSMRenderer* GetRSMRenderer() const;

    void Run();
    virtual void OnRender(int technique, int pass, SERTGICamera* camera);

	void Initialize(SEGPUDevice* device, RSMRenderer* rsm, int vplCount, 
		bool vertexCompute = true);

private:
    RSMRendererPtr mRSM;

    // Sample RSM.
    bool mVertexCompute;
    SampleRSMPtr mSampleRSMTask;

    // VPL stuff.
    int mVPLCount;
    SETexture1DPtr mVPLSamplePattern;
	SEStructuredBuffer* mVPLBuffer;
};

typedef SESmartPointer<VPLGenerator> VPLGeneratorPtr;

}

#endif