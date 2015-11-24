// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#ifndef Swing_LightMesh_H
#define Swing_LightMesh_H

#include "SERenderingEngineLIB.h"
#include "SETriangleMesh.h"
#include "SETexture2D.h"

namespace Swing
{

//----------------------------------------------------------------------------
// Author: Che Sun
// Date: 03/22/2015
//----------------------------------------------------------------------------
class SE_RENDERING_ENGINE_API SELightMesh : public SETriangleMesh
{
public:
    SELightMesh(SEMaterial* material, SERTGICamera* camera);
    virtual ~SELightMesh();

    // Implement base class interface.
    virtual void OnGetShaderConstants();
    virtual void OnUpdateShaderConstants(int technique, int pass);
    virtual void OnRender(SERenderPass* pass, SERenderPassInfo* renderPassInfo);

    SETexture2DPtr LightMeshTexture;

private:
    SEShaderUniform mLightMeshTextureSamplerLoc;
};

typedef SESmartPointer<SELightMesh> SELightMeshPtr;

}

#endif