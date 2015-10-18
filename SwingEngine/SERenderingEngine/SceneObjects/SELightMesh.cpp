// Swing Engine Version 2 Source Code 
// Copyright (c) 2007-2015

#include "SERenderingEnginePCH.h"
#include "SELightMesh.h"

using namespace Swing;

//----------------------------------------------------------------------------
SELightMesh::SELightMesh(SEMaterial* material, SERTGICamera* camera)
    :
    SETriangleMesh(material, camera)
{
}
//----------------------------------------------------------------------------
SELightMesh::~SELightMesh()
{
    LightMeshTexture = 0;
}
//----------------------------------------------------------------------------
void SELightMesh::OnGetShaderConstants()
{
    SETriangleMesh::OnGetShaderConstants();

    SEShaderProgram* program = mMaterial->GetProgram(0, 0);
    program->GetUniformLocation(&mLightMeshTextureSamplerLoc, 
        "LightMeshTexture5");
}
//----------------------------------------------------------------------------
void SELightMesh::OnUpdateShaderConstants(int technique, int pass)
{
    SETriangleMesh::OnUpdateShaderConstants(technique, pass);

    SESamplerDesc sampler;
    sampler.MinFilter = FT_Nearest;
    sampler.MagFilter = FT_Nearest;
    sampler.WrapS = WT_Clamp;
    sampler.WrapT = WT_Clamp;

    mLightMeshTextureSamplerLoc.SetValue(0);
    LightMeshTexture->BindToSampler(0, &sampler);
}
//----------------------------------------------------------------------------
void SELightMesh::OnRender(SEPass* pass, SEPassInfo* passInfo)
{
    // TODO:
    //glDisable(GL_DEPTH_TEST);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    SETriangleMesh::OnRender(pass, passInfo);

    //glDisable(GL_BLEND);
}
//----------------------------------------------------------------------------