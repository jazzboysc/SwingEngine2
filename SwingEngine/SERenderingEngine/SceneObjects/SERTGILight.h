// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2017

#ifndef Swing_RTGILight_H
#define Swing_RTGILight_H

#include "SERenderingEngineLIB.h"
#include "SEReferencable.h"
#include "SERTGICamera.h"
#include "SETriangleMesh.h"
#include "SEILight.h"

namespace Swing
{

// CPU memory footage of scene light used by shaders.
struct SE_RENDERING_ENGINE_API SESceneLight
{
    // type 1: point light, type 2: spot light
    SEVector4f WorldPositionAndType;
    SEVector4f Intensity;

    // SpotDirection, CosCutoff
    SEVector4f Params1;
    // SpotExponent, ConstantAttenuation, QuadraticAttenuation, 
    // SpotInnerCosCutoff
    SEVector4f Params2;

    SEMatrix4f LightProjectorView;
    SEMatrix4f LightProjectorProj;
    SEVector4f LightProjectorNearFar;
};

enum SERTGILightType
{
    LT_Point = 1,
    LT_Spot  = 2,
    LT_Rectangle = 3,
    LT_Sky = 4
};

class SELightManager;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/14/2013
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SERTGILight : public SEReferencable, public SEILight
{
public:
    SERTGILight(SERTGILightType type = LT_Point);
    virtual ~SERTGILight();

	SEVector3f Intensity;
    
    void SetProjector(SERTGICamera* projector);
    SERTGICamera* GetProjector() const;
    void SetLocation(const SEVector3f& location);

    void SetLightMesh(SETriangleMesh* lightMesh);
    SETriangleMesh* GetLightMesh() const;

    void RenderLightMesh(int technique, int pass, 
        SESubRenderer* subRenderer = 0);

    // Called by light manager to update shader uniform buffer for this light.
    virtual void OnUpdateLightBufferCache(SESceneLight* cache);

    // Implement common light interface.
    virtual SEMatrix3f GetRotation() const override;
    virtual SEVector3f GetLocation() const override;
    virtual SEColorRGB GetColor() const override;
    virtual float GetIntensity() const override;
    virtual SE_UInt32 GetSampleCount() const override;
    virtual bool CastShadow() const override;
    virtual bool IsVisible() const override;

    // Implement light rectangle interface.
    virtual float GetWidth() const override;
    virtual float GetHeight() const override;

    // Implement sky light interface.
    virtual bool IsSpherical() const override;
    virtual bool UseHDRFile() const override;
    virtual const char* GetHDRFilePath() const override;

    SEColorRGB Color;
    float ColorIntensity;
    SE_UInt32 SampleCount;
    bool CanCastShadow;
    bool Visible;
    float Width;
    float Height;
    bool Spherical;
    bool HDRFile;
    std::string HDRFilePath;
   
protected:
    friend class SELightManager;

    SERTGILightType mLightType;
    SEVector3f mLocation;
    SERTGICamera* mProjector;
    SETriangleMeshPtr mLightMesh;

    SEVector4f mParams1;
    SEVector4f mParams2;
};

typedef SESmartPointer<SERTGILight> SERTGILightPtr;

}

#endif
