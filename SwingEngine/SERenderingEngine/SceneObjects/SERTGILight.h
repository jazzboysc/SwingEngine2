// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_RTGILight_H
#define Swing_RTGILight_H

#include "SERenderingEngineLIB.h"
#include "SEReferencable.h"
#include "SERTGICamera.h"
#include "SETriangleMesh.h"

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
    LT_Spot  = 2
};

class SELightManager;

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 11/14/2013
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SERTGILight : public SEReferencable
{
public:
    SERTGILight(SERTGILightType type = LT_Point);
    virtual ~SERTGILight();

	SEVector3f Intensity;
    
    void SetProjector(SERTGICamera* projector);
    SERTGICamera* GetProjector() const;
    void SetLocation(const SEVector3f& location);
    SEVector3f GetLocation() const;

    void SetLightMesh(SETriangleMesh* lightMesh);
    SETriangleMesh* GetLightMesh() const;

    void RenderLightMesh(int technique, int pass, 
        SESubRenderer* subRenderer = 0);

    // Called by light manager to update shader uniform buffer for this light.
    virtual void OnUpdateLightBufferCache(SESceneLight* cache);
   
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
