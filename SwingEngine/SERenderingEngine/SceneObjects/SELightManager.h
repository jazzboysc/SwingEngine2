// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_LightManager_H
#define Swing_LightManager_H

#include "SERenderingEngineLIB.h"
#include "SEReferencable.h"
#include "SEGPUDevice.h"
#include "SERTGILight.h"
#include "SEUniformBuffer.h"
#include "SESubRenderer.h"

namespace Swing
{

struct SE_RENDERING_ENGINE_API SELightBufferHead
{
    unsigned int PointLightCount;
    unsigned int SpotLightCount;
    unsigned int Reserved1;
    unsigned int Reserved2;
};

struct SE_RENDERING_ENGINE_API SELightProjectorDesc
{
    float UpFovDegrees;
    float AspectRatio;
    float NearPlane;
    float FarPlane;
    SEVector3f Location;
    SEVector3f LookAt;
    SEVector3f Up;
};

struct SE_RENDERING_ENGINE_API SEPointLightDesc
{
    SEVector3f Intensity;
};

struct SE_RENDERING_ENGINE_API SESpotLightDesc
{
    SEVector3f Intensity;
    float CosCutoff;
    float InnerCosCutoff;
    float SpotExponent;
    float ConstantAttenuation;
    float QuadraticAttenuation;
};

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 04/07/2015
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SELightManager : public SEReferencable
{
public:
    SELightManager(SEGPUDevice* device);
    ~SELightManager();

    // Create a point light and add it to light manager.
    SERTGILight* CreatePointLight(SELightProjectorDesc* projectorDesc, 
        SERTGICamera* lightMeshCamera, SEPointLightDesc* pointLightDesc);

    // Create a spot light and add it to light manager.
    SERTGILight* CreateSpotLight(SELightProjectorDesc* projectorDesc,
        SERTGICamera* lightMeshCamera, SESpotLightDesc* spotLightDesc);

    // Light access.
    void AddPointLight(SERTGILight* light);
    SERTGILight* GetPointLight(int i) const;
    int GetPointLightCount() const;
    void AddSpotLight(SERTGILight* light);
    SERTGILight* GetSpotLight(int i) const;
    int GetSpotLightCount() const;

    // Light buffer is a uniform buffer managed by light manager. It stores
    // lights' information that can be accessed by lighting shaders.
    void CreateLightBuffer(SEGPUDevice* device);
    void UpdateLightBuffer();

    void SetLightBufferBindingPoint(unsigned int bindingPoint);
    unsigned int GetLightBufferBindingPoint() const;

    // Show light meshes as scene objects.
    void RenderLightMesh(int technique, int pass, SESubRenderer* subRenderer = 0);

    enum { MAX_LIGHT_COUNT = 256 };

private:
    SEGPUDevice* mDevice;
    SEMaterialTemplatePtr mMtLightMesh;
    std::vector<SERTGILightPtr> mPointLights;
    std::vector<SERTGILightPtr> mSpotLights;
    unsigned int mLightBufferBindingPoint;
    SEUniformBufferPtr mLightBuffer;
    unsigned char* mLightBufferCache;
};

typedef SESmartPointer<SELightManager> SELightManagerPtr;

}

#endif
