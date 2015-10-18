#ifndef RTGI_ShadowMapsGenerator_H
#define RTGI_ShadowMapsGenerator_H

#include "SESubRenderer.h"
#include "SELightManager.h"

namespace Swing
{

#define RTGI_ShadowMapRenderer_ShadowMaps_Name "ShadowMaps"

struct ShadowMapInfo
{
    unsigned int CurLightIndex;
    unsigned int Reserved1;
    unsigned int Reserved2;
    unsigned int Reserved3;
};

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 12/05/2014
//----------------------------------------------------------------------------
class ShadowMapsGenerator : public SESubRenderer
{
public:
    ShadowMapsGenerator(SEGPUDevice* device, SERenderSet* renderSet = 0);
    virtual ~ShadowMapsGenerator();

    void Initialize(int width, int height, SEBufferFormat format, 
        SELightManager* lightManager);
    void Render(int technique, int pass);

    inline int GetShadowMapCount() const { return mShadowMapCount; };
    inline void SetShadowMapInfoBufferBindingPoint(unsigned int bindingPoint)
    {
        mShadowMapInfoBufferBindingPoint = bindingPoint;
    };
    inline unsigned int GetShadowMapInfoBufferBindingPoint() const
    {
        return mShadowMapInfoBufferBindingPoint;
    };

protected:
    SEPipelineStateBlockPtr mPSB;
    SELightManagerPtr mLightManager;
    int mShadowMapCount;
    SEUniformBufferPtr mShadowMapInfoBuffer;
    unsigned int mShadowMapInfoBufferBindingPoint;
};

typedef SESmartPointer<ShadowMapsGenerator> ShadowMapsGeneratorPtr;

}

#endif