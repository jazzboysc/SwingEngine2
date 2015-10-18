#ifndef RTGI_RSMRenderer_H
#define RTGI_RSMRenderer_H

#include "SwingEngineRenderingEngine.h"
#include "SELightManager.h"

namespace Swing
{

#define RTGI_RSMRenderer_RSMPosition_Name "RSMPosition"
#define RTGI_RSMRenderer_RSMNormal_Name "RSMNormal"
#define RTGI_RSMRenderer_RSMFlux_Name "RSMFlux"

struct RSMInfo
{
    unsigned int CurLightIndex;
    unsigned int CurRSMBaseIndex;
    unsigned int Reserved1;
    unsigned int Reserved2;
};

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 12/05/2014
//----------------------------------------------------------------------------
class RSMRenderer : public SESubRenderer
{
public:
    RSMRenderer(SEGPUDevice* device, SERenderSet* renderSet = 0);
    virtual ~RSMRenderer();

    void Initialize(int width, int height, int pointLightFaceCount, 
        SEBufferFormat format, SELightManager* lightManager, 
		bool fluxMipmap = false);
    void Render(int technique, int pass);

    inline int GetRSMCount() const { return mRSMCount; }
    inline void SetRSMInfoBufferBindingPoint(unsigned int bindingPoint)
    {
        mRSMInfoBufferBindingPoint = bindingPoint;
    }
    inline unsigned int GetRSMInfoBufferBindingPoint() const
    {
        return mRSMInfoBufferBindingPoint;
    }
	inline unsigned int GetRSMFluxMipmapMaxLevel() const
	{
		return mRSMFluxMipmapMaxLevel;
	}

protected:
    SEPipelineStateBlockPtr mPSB;
    SELightManagerPtr mLightManager;
    int mPointLightFaceCount;
    int mRSMCount;
	unsigned int mRSMFluxMipmapMaxLevel;
    SEUniformBufferPtr mRSMInfoBuffer;
    unsigned int mRSMInfoBufferBindingPoint;
};

typedef SESmartPointer<RSMRenderer> RSMRendererPtr;

}

#endif